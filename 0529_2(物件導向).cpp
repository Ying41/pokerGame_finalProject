#include <windows.h>
#include <string>
#include <vector>
#include <array>
#include <random>      // 修正：mt19937 需要此標頭檔
#include <algorithm>   // 修正：std::shuffle 需要此標頭檔
#include <sstream>     // 修正：std::wstringstream 需要此標頭檔

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

//==================================================
// 基本資料結構
//==================================================
struct Suit {
    const wchar_t* symbol;
    const wchar_t* name;
    COLORREF color;
};

struct CardValue {
    const wchar_t* label;
    int value;
};

struct PipPos {
    int dx;
    int dy;
};

//==================================================
// 文字繪製工具類別
//==================================================
class TextRenderer {
public:
    static void Draw(
        HDC hdc,
        int x, int y,
        const std::wstring& text,
        int fontSize,
        COLORREF color,
        UINT textAlign,
        int angle10 = 0,
        const wchar_t* fontName = L"Segoe UI Symbol",
        int fontWeight = FW_NORMAL)
    {
        LOGFONTW lf = {};
        lf.lfHeight = -fontSize;
        lf.lfEscapement = angle10;
        lf.lfOrientation = angle10;
        lf.lfWeight = fontWeight;
        lf.lfCharSet = DEFAULT_CHARSET;
        lstrcpyW(lf.lfFaceName, fontName);

        HFONT hFont = CreateFontIndirectW(&lf);
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        COLORREF oldColor = SetTextColor(hdc, color);
        int oldBkMode = SetBkMode(hdc, TRANSPARENT);
        UINT oldAlign = SetTextAlign(hdc, textAlign);

        TextOutW(hdc, x, y, text.c_str(), static_cast<int>(text.length()));

        SetTextAlign(hdc, oldAlign);
        SetBkMode(hdc, oldBkMode);
        SetTextColor(hdc, oldColor);
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }
};

//==================================================
// 點數牌花色位置配置
//==================================================
class PipLayout {
public:
    static std::vector<PipPos> GetPositions(int count) {
        int top = -55;
        int bottom = 55;
        int v31 = top / 2;          // -27
        int v32 = -v31;             // 27
        int v41 = top / 3;          // -18
        int v42 = -v41;             // 18
        int v51 = (top + v41) / 2;  // -36
        int v52 = -v51;             // 36
        int mid = 0;
        int cen = 0;
        int left = -25;
        int right = 25;

        std::vector<std::vector<PipPos>> positions(11);
        positions[1] = { {cen, mid} };
        positions[2] = { {cen, top}, {cen, bottom} };
        positions[3] = { {cen, top}, {cen, mid}, {cen, bottom} };
        positions[4] = { {left, top}, {right, top}, {left, bottom}, {right, bottom} };
        positions[5] = { {left, top}, {right, top}, {cen, mid}, {left, bottom}, {right, bottom} };
        positions[6] = { {left, top}, {right, top}, {left, mid}, {right, mid}, {left, bottom}, {right, bottom} };
        positions[7] = { {left, top}, {right, top}, {cen, v31}, {left, mid}, {right, mid}, {left, bottom}, {right, bottom} };
        positions[8] = { {left, top}, {right, top}, {cen, v31}, {left, mid}, {right, mid}, {cen, v32}, {left, bottom}, {right, bottom} };
        positions[9] = { {left, top}, {right, top}, {cen, v51}, {left, v41}, {right, v41}, {left, v42}, {right, v42}, {left, bottom}, {right, bottom} };
        positions[10] = { {left, top}, {right, top}, {cen, v51}, {left, v41}, {right, v41}, {left, v42}, {right, v42}, {cen, v52}, {left, bottom}, {right, bottom} };

        if (count < 1 || count > 10) return {};
        return positions[count];
    }
};

//==================================================
// 單張牌類別
//==================================================
class Card {
private:
    Suit suit_;
    CardValue value_;
    static constexpr int WIDTH = 120;
    static constexpr int HEIGHT = 180;
    static constexpr int PADDING = 10;

public:
    Card(const CardValue& value, const Suit& suit)
        : suit_(suit), value_(value) {
    }

    void Draw(HDC hdc, int left, int top) const {
        DrawCardFrame(hdc, left, top);

        int innerLeft = left + PADDING;
        int innerTop = top + PADDING;
        int innerW = WIDTH - 2 * PADDING;
        int innerH = HEIGHT - 2 * PADDING;
        int innerCx = innerLeft + innerW / 2;
        int innerCy = innerTop + innerH / 2;

        DrawTopLeftCorner(hdc, innerLeft, innerTop);
        DrawBottomRightCorner(hdc, left, top);

        if (value_.value <= 10) {
            DrawPips(hdc, value_.value, innerCx, innerCy);
        }
        else {
            DrawFaceCard(hdc, innerCx, innerCy);
        }
    }

    static int GetWidth() { return WIDTH; }
    static int GetHeight() { return HEIGHT; }

private:
    void DrawCardFrame(HDC hdc, int left, int top) const {
        HBRUSH hWhite = CreateSolidBrush(RGB(255, 255, 255));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hWhite);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        RoundRect(hdc, left, top, left + WIDTH, top + HEIGHT, 10, 10);

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
        DeleteObject(hWhite);
        DeleteObject(hPen);
    }

    void DrawTopLeftCorner(HDC hdc, int innerLeft, int innerTop) const {
        TextRenderer::Draw(hdc, innerLeft, innerTop + 16, value_.label, 16, suit_.color, TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
        TextRenderer::Draw(hdc, innerLeft, innerTop + 32, suit_.symbol, 16, suit_.color, TA_LEFT | TA_BASELINE, 0, L"Segoe UI Symbol", FW_NORMAL);
    }

    void DrawBottomRightCorner(HDC hdc, int left, int top) const {
        TextRenderer::Draw(hdc, left + WIDTH - PADDING * 2, top + HEIGHT - PADDING * 2 - 16, suit_.symbol, 16, suit_.color, TA_RIGHT | TA_BASELINE, 1800, L"Segoe UI Symbol", FW_NORMAL);
        TextRenderer::Draw(hdc, left + WIDTH - PADDING * 2, top + HEIGHT - PADDING * 2, value_.label, 16, suit_.color, TA_RIGHT | TA_BASELINE, 1800, L"Segoe UI", FW_NORMAL);
    }

    void DrawPips(HDC hdc, int count, int cx, int cy) const {
        auto positions = PipLayout::GetPositions(count);
        for (const auto& p : positions) {
            int x = cx + p.dx;
            int y = cy + p.dy;
            if (p.dy < 5) {
                TextRenderer::Draw(hdc, x, y, suit_.symbol, 24, suit_.color, TA_CENTER | TA_BASELINE, 0, L"Segoe UI Symbol", FW_NORMAL);
            }
            else {
                TextRenderer::Draw(hdc, x, y, suit_.symbol, 24, suit_.color, TA_CENTER | TA_BASELINE, 1800, L"Segoe UI Symbol", FW_NORMAL);
            }
        }
    }

    void DrawFaceCard(HDC hdc, int cx, int cy) const {
        TextRenderer::Draw(hdc, cx, cy + 14, value_.label, 40, suit_.color, TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
    }
};

//==================================================
// 牌組類別：管理 52 張撲克牌
//==================================================
class Deck {
private:
    std::vector<Card> cards_;
public:
    Deck() {
        Initialize();
    }

    void Draw(HDC hdc, int startX, int startY, int gapX, int gapY) const {
        const int cardW = Card::GetWidth();
        const int cardH = Card::GetHeight();
        for (size_t i = 0; i < cards_.size(); ++i) {
            int row = static_cast<int>(i / 13);
            int col = static_cast<int>(i % 13);
            int x = startX + col * (cardW + gapX);
            int y = startY + row * (cardH + gapY);
            cards_[i].Draw(hdc, x, y);
        }
    }

private:
    void Initialize() {
        static const std::array<Suit, 4> suits = {
            Suit{ L"♠", L"spade",   RGB(0, 0, 0) },
            Suit{ L"♥", L"heart",   RGB(220, 0, 0) },
            Suit{ L"♦", L"diamond", RGB(220, 0, 0) },
            Suit{ L"♣", L"club",    RGB(0, 0, 0) }
        };

        static const std::array<CardValue, 13> values = {
            CardValue{ L"A",  1  }, CardValue{ L"2",  2  }, CardValue{ L"3",  3  },
            CardValue{ L"4",  4  }, CardValue{ L"5",  5  }, CardValue{ L"6",  6  },
            CardValue{ L"7",  7  }, CardValue{ L"8",  8  }, CardValue{ L"9",  9  },
            CardValue{ L"10", 10 }, CardValue{ L"J",  11 }, CardValue{ L"Q",  12 },
            CardValue{ L"K",  13 }
        };

        cards_.clear();
        cards_.reserve(52);
        for (const auto& suit : suits) {
            for (const auto& value : values) {
                cards_.emplace_back(value, suit);
            }
        }
    }
};

//==================================================
// 主視窗 App 類別
//==================================================
class PokerWindowApp {
private:
    HINSTANCE hInstance_;
    HWND hwnd_;

    int currentSuitIndex_ = 0;
    int currentValueIndex_ = 0;

    int currentShownSeqIndex_ = 0; // 右側目前正在顯示的序列索引
    int nextSeqIndex_ = 0;         // 下一張要顯示的序列索引

    bool running_ = false;
    bool hasCurrentItem_ = false;

    ULONGLONG shownTick_ = 0;      // 修正：將 ULONGONG 改為 ULONGLONG
    std::mt19937 rng_;

    std::vector<int> deckOrder_;   // 洗牌後的 52 張順序，每個元素 0~51
    size_t deckPos_ = 0;
    bool anyMatchOccurred_ = false; // 是否曾出現過任何一次「牌面 == 右側文字」

    Deck deck_;                     // 修正：補上原本遺漏的 Deck 成員變數

    static constexpr const wchar_t* CLASS_NAME = L"PokerCardWin32MenuReaction";
    static constexpr UINT TIMER_ID = 1001;
    static constexpr UINT ID_MENU_START = 40001;
    static constexpr UINT ID_MENU_STOP = 40002;
    static constexpr UINT ID_MENU_EXIT = 40003;
    static constexpr int HOTKEY_ID_SPACE = 50001;
    static constexpr UINT ID_SINGLE = 10001;
    static constexpr UINT ID_DOUBLE = 10002;
    static constexpr UINT ID_MULTIPLE = 10003;
    static constexpr UINT ID_HELP_ABOUT = 10004;

public:
    explicit PokerWindowApp(HINSTANCE hInstance)
        : hInstance_(hInstance), hwnd_(nullptr) {
        std::random_device rd;
        rng_.seed(rd());
    }

    bool Create(int nCmdShow) {
        WNDCLASSW wc = {};
        wc.lpfnWndProc = PokerWindowApp::WndProc;
        wc.hInstance = hInstance_;
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        RegisterClassW(&wc);

        hwnd_ = CreateWindowExW(
            0,
            CLASS_NAME,
            L"Win32 API 撲克牌快速反應遊戲",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            1200, 700,
            nullptr,
            CreateAppMenu(), // 修正：將產生的選單傳入視窗中，使其順利顯示
            hInstance_,
            this
        );

        if (!hwnd_) return false;
        ShowWindow(hwnd_, nCmdShow);
        UpdateWindow(hwnd_);
        return true;
    }

    int Run() {
        MSG msg = {};
        while (GetMessageW(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        return static_cast<int>(msg.wParam);
    }

private:
    HMENU CreateAppMenu() {
        HMENU hMenuBar = CreateMenu();
        HMENU hGameMenu = CreatePopupMenu();

        AppendMenuW(hGameMenu, MF_STRING, ID_MENU_START, L" 開始 ");
        AppendMenuW(hGameMenu, MF_STRING, ID_MENU_STOP, L" 停止 ");
        AppendMenuW(hGameMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(hGameMenu, MF_STRING, ID_MENU_EXIT, L" 結束 ");

        AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hGameMenu, L" 單人計時賽 ");
        AppendMenuW(hMenuBar, MF_STRING, ID_DOUBLE, L" 雙人鍵盤競速 ");
        AppendMenuW(hMenuBar, MF_STRING, ID_MULTIPLE, L" 多人視覺辨識賽 ");
        AppendMenuW(hMenuBar, MF_STRING, ID_HELP_ABOUT, L" 說明 ");

        return hMenuBar;
    }

    static const std::array<Suit, 4>& GetSuits() {
        static const std::array<Suit, 4> suits = {
            Suit{ L"♠", L"spade",   RGB(0, 0, 0) },
            Suit{ L"♥", L"heart",   RGB(220, 0, 0) },
            Suit{ L"♦", L"diamond", RGB(220, 0, 0) },
            Suit{ L"♣", L"club",    RGB(0, 0, 0) }
        };
        return suits;
    }

    static const std::array<CardValue, 13>& GetCardValues() {
        static const std::array<CardValue, 13> values = {
            CardValue{ L"A",  1 }, CardValue{ L"2",  2 }, CardValue{ L"3",  3 },
            CardValue{ L"4",  4 }, CardValue{ L"5",  5 }, CardValue{ L"6",  6 },
            CardValue{ L"7",  7 }, CardValue{ L"8",  8 }, CardValue{ L"9",  9 },
            CardValue{ L"10", 10 }, CardValue{ L"J",  11 }, CardValue{ L"Q",  12 },
            CardValue{ L"K",  13 }
        };
        return values;
    }

    static const std::array<const wchar_t*, 13>& GetSequenceLabels() {
        static const std::array<const wchar_t*, 13> seq = {
            L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K"
        };
        return seq;
    }

    int RandomIntervalMs() {
        std::uniform_int_distribution<int> dist(800, 1200); // 稍微拉長下限讓人類好反應
        return dist(rng_);
    }

    void BuildShuffledDeck() {
        deckOrder_.clear();
        deckOrder_.reserve(52);
        for (int i = 0; i < 52; ++i) {
            deckOrder_.push_back(i);
        }
        std::shuffle(deckOrder_.begin(), deckOrder_.end(), rng_);
        deckPos_ = 0;
    }

    bool IsCardAndTextMatched() const {
        const auto& values = GetCardValues();
        const auto& seq = GetSequenceLabels();

        std::wstring cardText = values[currentValueIndex_].label;
        std::wstring shownText = seq[currentShownSeqIndex_];

        if (cardText == L"A" && shownText == L"1") {
            return true;
        }
        return cardText == shownText;
    }

    void StartSequence() {
        StopSequence();

        running_ = true;
        hasCurrentItem_ = false;
        nextSeqIndex_ = 0;
        currentShownSeqIndex_ = 0;
        anyMatchOccurred_ = false;

        BuildShuffledDeck();

        if (!RegisterHotKey(hwnd_, HOTKEY_ID_SPACE, 0, VK_SPACE)) {
            MessageBoxW(hwnd_, L"無法註冊空白鍵 HotKey", L"錯誤", MB_OK | MB_ICONERROR);
        }

        ShowNextItem();
        ScheduleNextTimer();
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void StopSequence() {
        KillTimer(hwnd_, TIMER_ID);
        UnregisterHotKey(hwnd_, HOTKEY_ID_SPACE);
        running_ = false;
    }

    void ScheduleNextTimer() {
        KillTimer(hwnd_, TIMER_ID);
        SetTimer(hwnd_, TIMER_ID, RandomIntervalMs(), nullptr);
    }

    bool ShowNextItem() {
        if (deckPos_ >= deckOrder_.size()) {
            ShowNoMatchFailureIfNeeded(); // 52張播完如果都沒中就失敗
            return false;
        }

        int cardId = deckOrder_[deckPos_++];
        currentSuitIndex_ = cardId / 13;
        currentValueIndex_ = cardId % 13;

        currentShownSeqIndex_ = nextSeqIndex_;
        nextSeqIndex_ = (nextSeqIndex_ + 1) % 13;

        shownTick_ = GetTickCount64();
        hasCurrentItem_ = true;

        if (IsCardAndTextMatched()) {
            anyMatchOccurred_ = true;
        }

        InvalidateRect(hwnd_, nullptr, TRUE);
        return true;
    }

    void ShowNoMatchFailureIfNeeded() {
        StopSequence();
        if (!anyMatchOccurred_) {
            MessageBoxW(hwnd_, L"52 張牌全部顯示完畢，沒有任何一張與右側數字 / JQK 相符。結果：失敗", L"失敗", MB_OK | MB_ICONWARNING);
        }
        else {
            MessageBoxW(hwnd_, L"牌組播放完畢！您漏掉了所有相符的牌面。", L"遊戲結束", MB_OK | MB_ICONINFORMATION);
        }
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void HandleSpacePressed() {
        if (!running_ || !hasCurrentItem_) return;

        ULONGLONG now = GetTickCount64();
        ULONGLONG elapsed = now - shownTick_;

        bool correct = IsCardAndTextMatched();
        StopSequence();

        const auto& seq = GetSequenceLabels();
        const auto& values = GetCardValues();

        std::wstring shownText = seq[currentShownSeqIndex_];
        std::wstring cardText = values[currentValueIndex_].label;

        std::wstringstream ss;
        ss << L"撲克牌：" << cardText << L" "
            << L"\n右邊數字：" << shownText << L" "
            << L"\n結果：" << (correct ? L"答對！" : L"失敗！") << L" "
            << L"\n反應時間：" << elapsed << L" 毫秒";

        MessageBoxW(
            hwnd_,
            ss.str().c_str(),
            correct ? L"答對" : L"失敗",
            MB_OK | (correct ? MB_ICONINFORMATION : MB_ICONWARNING)
        );
        InvalidateRect(hwnd_, nullptr, TRUE);
    } // 修正：移除原本下方多出來的多餘右大括號

    void OnPaint() {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);
        RECT rc;
        GetClientRect(hwnd_, &rc);

        // 畫白色背景
        HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        if (running_ && hasCurrentItem_) {
            // 1. 繪製當前中間顯示的卡牌
            const auto& suits = GetSuits();
            const auto& values = GetCardValues();
            Card currentCard(values[currentValueIndex_], suits[currentSuitIndex_]);
            currentCard.Draw(hdc, 200, 150);

            // 2. 繪製右側對應的比對文字序列
            const auto& seq = GetSequenceLabels();
            std::wstring matchText = L"比對目標：" + std::wstring(seq[currentShownSeqIndex_]);
            TextRenderer::Draw(hdc, 400, 220, matchText, 32, RGB(0, 0, 255), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);

            TextRenderer::Draw(hdc, 400, 280, L"當兩者相符時，請立刻按下 [空白鍵]！", 18, RGB(60, 60, 60), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
        }
        else {
            // 繪製提示文字條
            RECT bannerRect = { 100, 250, 1100, 400 };
            HBRUSH hMuted = CreateSolidBrush(RGB(0, 0, 0));
            // 繪製半透明文字效果提示
            TextRenderer::Draw(hdc, 600, 320, L"請點選上方選單 [單人計時賽] -> [開始] 來啟動反應遊戲", 28, RGB(255, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            DeleteObject(hMuted);
        }

        EndPaint(hwnd_, &ps);
    }

public:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_COMMAND: // 修正：處理選單點擊事件
            switch (LOWORD(wParam)) {
            case ID_MENU_START:
                StartSequence();
                return 0;
            case ID_MENU_STOP:
                StopSequence();
                InvalidateRect(hwnd_, nullptr, TRUE);
                return 0;
            case ID_MENU_EXIT:
                DestroyWindow(hwnd_);
                return 0;
            case ID_HELP_ABOUT:
                MessageBoxW(hwnd_, L"快速反應遊戲！\n當中央撲克牌點數與右側數字相符（如 A 對應 1）時，請迅速按下空白鍵！", L"關於本遊戲", MB_OK | MB_ICONINFORMATION);
                return 0;
            }
            break;

        case WM_TIMER: // 修正：處理計時器更新（切換下一張牌）
            if (wParam == TIMER_ID) {
                if (!ShowNextItem()) {
                    KillTimer(hwnd_, TIMER_ID);
                }
                else {
                    ScheduleNextTimer(); // 隨機下一次更換的時間
                }
            }
            return 0;

        case WM_HOTKEY: // 修正：處理空白鍵熱鍵點擊
            if (wParam == HOTKEY_ID_SPACE) {
                HandleSpacePressed();
            }
            return 0;

        case WM_PAINT:
            OnPaint();
            return 0;

        case WM_DESTROY:
            StopSequence();
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProcW(hwnd_, msg, wParam, lParam);
    }

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        PokerWindowApp* pThis = nullptr;
        if (msg == WM_NCCREATE) {
            CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
            pThis = static_cast<PokerWindowApp*>(cs->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            pThis->hwnd_ = hwnd;
        }
        else {
            pThis = reinterpret_cast<PokerWindowApp*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        }
        if (pThis) {
            return pThis->HandleMessage(msg, wParam, lParam);
        }
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
};

//==================================================
// WinMain
//==================================================
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    PokerWindowApp app(hInstance);
    if (!app.Create(nCmdShow)) {
        return 0;
    }
    return app.Run();
}