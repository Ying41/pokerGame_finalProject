#include <windows.h>
#include <string>
#include <vector>
#include <array>
#include <random>      
#include <algorithm>   
#include <sstream>   

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
        int v31 = top / 2;
        int v32 = -v31;
        int v41 = top / 3;
        int v42 = -v41;
        int v51 = (top + v41) / 2;
        int v52 = -v51;
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
// 雙人計時賽模式管理器（保持原樣，不連 DLL）
//==================================================
class TwoPlayerMode {
private:
    bool active_ = false;
    int currentSuitIdx_ = 0;
    int currentValueIdx_ = 0;
    int currentSeqIdx_ = 0;
    size_t deckPos_ = 0;
    ULONGLONG shownTick_ = 0;

    std::vector<int> deckOrder_;
    std::mt19937& rng_;

public:
    explicit TwoPlayerMode(std::mt19937& rng) : rng_(rng) {}

    void Start() {
        active_ = true;
        deckPos_ = 0;
        currentSeqIdx_ = 0;

        deckOrder_.clear();
        deckOrder_.reserve(52);
        for (int i = 0; i < 52; ++i) deckOrder_.push_back(i);
        std::shuffle(deckOrder_.begin(), deckOrder_.end(), rng_);

        NextCard();
    }

    void Stop() { active_ = false; }
    bool IsActive() const { return active_; }

    bool NextCard() {
        if (deckPos_ >= deckOrder_.size()) return false;

        int cardId = deckOrder_[deckPos_++];
        currentSuitIdx_ = cardId / 13;
        currentValueIdx_ = cardId % 13;

        currentSeqIdx_ = (deckPos_ - 1) % 13;
        shownTick_ = GetTickCount64();
        return true;
    }

    bool IsMatched() const {
        std::wstring cardLabel = GetCardValues()[currentValueIdx_].label;
        std::wstring seqLabel = GetSequenceLabels()[currentSeqIdx_];
        if (cardLabel == L"A" && seqLabel == L"1") return true;
        return cardLabel == seqLabel;
    }

    void HandleKeyPress(HWND hwnd, wchar_t playerKey) {
        if (!active_) return;

        ULONGLONG elapsed = GetTickCount64() - shownTick_;
        bool matched = IsMatched();
        Stop();

        std::wstring playerStr = (playerKey == L'A') ? L"玩家 A" : L"玩家 L";
        std::wstring resultStr;

        if (matched) {
            resultStr = playerStr + L" 贏！";
        }
        else {
            resultStr = playerStr + L" 輸！(誤觸)";
        }

        std::wstringstream ss;
        ss << L"【雙人競速結果】\n"
            << L"撲克牌面：" << GetCardValues()[currentValueIdx_].label << L"\n"
            << L"當前數字：" << GetSequenceLabels()[currentSeqIdx_] << L"\n"
            << L"反應時間：" << elapsed << L" 毫秒\n"
            << L"判定：" << resultStr;

        MessageBoxW(hwnd, ss.str().c_str(), L"遊戲結束", MB_OK | (matched ? MB_ICONINFORMATION : MB_ICONWARNING));
        InvalidateRect(hwnd, nullptr, TRUE);
    }

    void Draw(HDC hdc) const {
        if (!active_) return;

        Card card(GetCardValues()[currentValueIdx_], GetSuits()[currentSuitIdx_]);
        card.Draw(hdc, 200, 150);

        std::wstring matchText = L"比對目標：" + std::wstring(GetSequenceLabels()[currentSeqIdx_]);
        TextRenderer::Draw(hdc, 400, 220, matchText, 32, RGB(255, 128, 0), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);

        TextRenderer::Draw(hdc, 400, 290, L"雙人鍵盤熱鍵競速中：", 18, RGB(50, 50, 50), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
        TextRenderer::Draw(hdc, 400, 320, L"左邊玩家 A 鍵 | 右邊玩家 L 鍵", 20, RGB(0, 150, 0), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
        TextRenderer::Draw(hdc, 400, 360, L"相同時看誰快！按錯則直接判輸！", 16, RGB(120, 120, 120), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
    }

private:
    static const std::array<Suit, 4>& GetSuits() {
        static const std::array<Suit, 4> suits = {
            Suit{ L"♠", L"spade",   RGB(0, 0, 0) }, Suit{ L"♥", L"heart",   RGB(220, 0, 0) },
            Suit{ L"♦", L"diamond", RGB(220, 0, 0) }, Suit{ L"♣", L"club",    RGB(0, 0, 0) }
        };
        return suits;
    }
    static const std::array<CardValue, 13>& GetCardValues() {
        static const std::array<CardValue, 13> values = {
            CardValue{ L"A", 1 }, CardValue{ L"2", 2 }, CardValue{ L"3", 3 }, CardValue{ L"4", 4 },
            CardValue{ L"5", 5 }, CardValue{ L"6", 6 }, CardValue{ L"7", 7 }, CardValue{ L"8", 8 },
            CardValue{ L"9", 9 }, CardValue{ L"10", 10 }, CardValue{ L"J", 11 }, CardValue{ L"Q", 12 }, CardValue{ L"K", 13 }
        };
        return values;
    }
    static const std::array<const wchar_t*, 13>& GetSequenceLabels() {
        static const std::array<const wchar_t*, 13> seq = {
            L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K"
        };
        return seq;
    }
};

//==================================================
// 主視窗 App 類別
//==================================================
class PokerWindowApp {
private:
    HINSTANCE hInstance_;
    HWND hwnd_;

    // ===== 🎯 單人連結外掛專用變數 =====
    typedef void(*CheckFunc)(HWND, int, int);
    CheckFunc myCheckPlugin = nullptr;
    HMODULE hPluginDLL = nullptr;
    // ===================================

    // 單人模式變數
    int currentSuitIndex_ = 0;
    int currentValueIndex_ = 0;
    int currentShownSeqIndex_ = 0;
    int nextSeqIndex_ = 0;
    bool running_ = false;
    bool hasCurrentItem_ = false;
    ULONGLONG shownTick_ = 0;
    std::vector<int> deckOrder_;
    size_t deckPos_ = 0;
    bool anyMatchOccurred_ = false;

    std::mt19937 rng_;
    TwoPlayerMode twoPlayerMode_;

    static constexpr const wchar_t* CLASS_NAME = L"PokerCardWin32MenuReaction";
    static constexpr UINT TIMER_ID = 1001;
    static constexpr UINT ID_MENU_START = 40001;
    static constexpr UINT ID_MENU_STOP = 40002;
    static constexpr UINT ID_MENU_EXIT = 40003;

    static constexpr int HOTKEY_ID_SPACE = 50001;
    static constexpr int HOTKEY_ID_ESC = 50002;
    static constexpr int HOTKEY_ID_R = 50003;
    static constexpr int HOTKEY_ID_A = 50004;
    static constexpr int HOTKEY_ID_L = 50005;

    static constexpr UINT ID_SINGLE_START = 40001;
    static constexpr UINT ID_DOUBLE_START = 10002;
    static constexpr UINT ID_MULTIPLE = 10003;
    static constexpr UINT ID_HELP_ABOUT = 10004;

public:
    explicit PokerWindowApp(HINSTANCE hInstance)
        : hInstance_(hInstance), hwnd_(nullptr), twoPlayerMode_(rng_) {
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
            0, CLASS_NAME, L"撲克牌心臟病(單人外掛版)",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700,
            nullptr, CreateAppMenu(), hInstance_, this
        );

        if (!hwnd_) return false;

        // ===== 自動載入外掛 DLL =====
        hPluginDLL = LoadLibrary(L"s1142006.dll");
        if (hPluginDLL) {
            myCheckPlugin = (CheckFunc)GetProcAddress(hPluginDLL, "check");
        }
        // ============================

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
        AppendMenuW(hMenuBar, MF_STRING, ID_DOUBLE_START, L" 雙人鍵盤競速 ");
        AppendMenuW(hMenuBar, MF_STRING, ID_MULTIPLE, L" 多人視覺辨識賽 ");
        AppendMenuW(hMenuBar, MF_STRING, ID_HELP_ABOUT, L" 說明 ");

        return hMenuBar;
    }

    int RandomIntervalMs() {
        std::uniform_int_distribution<int> dist(800, 1200);
        return dist(rng_);
    }

    void StartTwoPlayerGame() {
        StopAnyGame();
        twoPlayerMode_.Start();

        RegisterHotKey(hwnd_, HOTKEY_ID_A, 0, 'A');
        RegisterHotKey(hwnd_, HOTKEY_ID_L, 0, 'L');
        RegisterHotKey(hwnd_, HOTKEY_ID_ESC, 0, VK_ESCAPE);

        SetTimer(hwnd_, TIMER_ID, RandomIntervalMs(), nullptr);
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void StartSingleGame() {
        StopAnyGame();
        running_ = true;
        hasCurrentItem_ = false;
        nextSeqIndex_ = 0;
        currentShownSeqIndex_ = 0;
        anyMatchOccurred_ = false;

        deckOrder_.clear();
        deckOrder_.reserve(52);
        for (int i = 0; i < 52; ++i) deckOrder_.push_back(i);
        std::shuffle(deckOrder_.begin(), deckOrder_.end(), rng_);
        deckPos_ = 0;

        RegisterHotKey(hwnd_, HOTKEY_ID_SPACE, 0, VK_SPACE);
        RegisterHotKey(hwnd_, HOTKEY_ID_ESC, 0, VK_ESCAPE);

        ShowNextSingleItem();
        SetTimer(hwnd_, TIMER_ID, RandomIntervalMs(), nullptr);
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void StopAnyGame() {
        KillTimer(hwnd_, TIMER_ID);
        UnregisterHotKey(hwnd_, HOTKEY_ID_SPACE);
        UnregisterHotKey(hwnd_, HOTKEY_ID_ESC);
        UnregisterHotKey(hwnd_, HOTKEY_ID_A);
        UnregisterHotKey(hwnd_, HOTKEY_ID_L);
        running_ = false;
        twoPlayerMode_.Stop();
    }

    bool ShowNextSingleItem() {
        if (deckPos_ >= deckOrder_.size()) {
            StopAnyGame();
            MessageBoxW(hwnd_, L"牌組發完", L"遊戲結束", MB_OK | MB_ICONINFORMATION);
            return false;
        }
        int cardId = deckOrder_[deckPos_++];
        currentSuitIndex_ = cardId / 13;
        currentValueIndex_ = cardId % 13;
        currentShownSeqIndex_ = nextSeqIndex_;
        nextSeqIndex_ = (nextSeqIndex_ + 1) % 13;
        shownTick_ = GetTickCount64();
        hasCurrentItem_ = true;
        return true;
    }

    void HandleSingleSpace() {
        if (!running_ || !hasCurrentItem_) return;
        ULONGLONG elapsed = GetTickCount64() - shownTick_;

        std::wstring cardText = GetCardValues()[currentValueIndex_].label;
        std::wstring shownText = GetSequenceLabels()[currentShownSeqIndex_];
        bool correct = (cardText == L"A" && shownText == L"1") || (cardText == shownText);

        StopAnyGame();

        std::wstringstream ss;
        ss << L"結果：" << (correct ? L"答對！" : L"失敗！") << L"\n反應時間：" << elapsed << L" 毫秒";
        MessageBoxW(hwnd_, ss.str().c_str(), L"結算", MB_OK);
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void OnPaint() {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);
        RECT rc;
        GetClientRect(hwnd_, &rc);

        HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        if (twoPlayerMode_.IsActive()) {
            twoPlayerMode_.Draw(hdc);
        }
        else if (running_ && hasCurrentItem_) {
            Card currentCard(GetCardValues()[currentValueIndex_], GetSuits()[currentSuitIndex_]);
            currentCard.Draw(hdc, 200, 150);

            std::wstring matchText = L"比對目標：" + std::wstring(GetSequenceLabels()[currentShownSeqIndex_]);
            TextRenderer::Draw(hdc, 400, 220, matchText, 32, RGB(0, 0, 255), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
        }
        else {
            TextRenderer::Draw(hdc, 600, 260, L"【功能選單提示】", 24, RGB(255, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            TextRenderer::Draw(hdc, 600, 310, L"請點擊上方選單 [單人計時賽 -> 開始] 開始遊戲", 18, RGB(60, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
        }

        EndPaint(hwnd_, &ps);
    }

public:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case ID_MENU_START:
                StartSingleGame();
                return 0;
            case ID_DOUBLE_START:
                StartTwoPlayerGame();
                return 0;
            case ID_MENU_STOP:
                StopAnyGame();
                InvalidateRect(hwnd_, nullptr, TRUE);
                return 0;
            case ID_MENU_EXIT:
                DestroyWindow(hwnd_);
                return 0;
            case ID_HELP_ABOUT:
                MessageBoxW(hwnd_, L"單人模式：相同時按[空白鍵]\n外掛只會在單人計時賽自動觸發！", L"說明", MB_OK);
                return 0;
            }
            break;

        case WM_TIMER:
            if (wParam == TIMER_ID) {
                if (twoPlayerMode_.IsActive()) {
                    if (!twoPlayerMode_.NextCard()) {
                        StopAnyGame();
                        MessageBoxW(hwnd_, L"牌組發完", L"遊戲結束", MB_OK | MB_ICONINFORMATION);
                        return 0;
                    }
                }
                else if (running_) {
                    ShowNextSingleItem();
                }

                KillTimer(hwnd_, TIMER_ID);
                SetTimer(hwnd_, TIMER_ID, RandomIntervalMs(), nullptr);

                InvalidateRect(hwnd_, nullptr, TRUE);
                UpdateWindow(hwnd_);

                // ===== 🎯 修正：只在「單人模式」啟動時呼叫 DLL 外掛 =====
                if (myCheckPlugin && running_) {
                    int cVal = currentValueIndex_ + 1; // 撲克牌點數 (1~13)
                    int tVal = currentShownSeqIndex_ + 1; // 畫面目標數字 (1~13)
                    myCheckPlugin(hwnd_, cVal, tVal);
                }
            }
            return 0;

        case WM_HOTKEY:
            if (wParam == HOTKEY_ID_SPACE) {
                HandleSingleSpace();
            }
            else if (wParam == HOTKEY_ID_A) {
                twoPlayerMode_.HandleKeyPress(hwnd_, L'A');
            }
            else if (wParam == HOTKEY_ID_L) {
                twoPlayerMode_.HandleKeyPress(hwnd_, L'L');
            }
            else if (wParam == HOTKEY_ID_ESC) {
                StopAnyGame();
                InvalidateRect(hwnd_, nullptr, TRUE);
            }
            return 0;

        case WM_PAINT:
            OnPaint();
            return 0;

        case WM_DESTROY:
            StopAnyGame();
            if (hPluginDLL) {
                FreeLibrary(hPluginDLL);
            }
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
        if (pThis) return pThis->HandleMessage(msg, wParam, lParam);
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

private:
    static const std::array<Suit, 4>& GetSuits() {
        static const std::array<Suit, 4> suits = {
            Suit{ L"♠", L"spade",   RGB(0, 0, 0) }, Suit{ L"♥", L"heart",   RGB(220, 0, 0) },
            Suit{ L"♦", L"diamond", RGB(220, 0, 0) }, Suit{ L"♣", L"club",    RGB(0, 0, 0) }
        };
        return suits;
    }
    static const std::array<CardValue, 13>& GetCardValues() {
        static const std::array<CardValue, 13> values = {
            CardValue{ L"A", 1 }, CardValue{ L"2", 2 }, CardValue{ L"3", 3 }, CardValue{ L"4", 4 },
            CardValue{ L"5", 5 }, CardValue{ L"6", 6 }, CardValue{ L"7", 7 }, CardValue{ L"8", 8 },
            CardValue{ L"9", 9 }, CardValue{ L"10", 10 }, CardValue{ L"J", 11 }, CardValue{ L"Q", 12 }, CardValue{ L"K", 13 }
        };
        return values;
    }
    static const std::array<const wchar_t*, 13>& GetSequenceLabels() {
        static const std::array<const wchar_t*, 13> seq = {
            L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K"
        };
        return seq;
    }
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    PokerWindowApp app(hInstance);
    if (!app.Create(nCmdShow)) return 0;
    return app.Run();
}