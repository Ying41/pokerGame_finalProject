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
        HFONT hFontOld = (HFONT)SelectObject(hdc, hFont);

        COLORREF oldColor = SetTextColor(hdc, color);
        int oldBkMode = SetBkMode(hdc, TRANSPARENT);
        UINT oldAlign = SetTextAlign(hdc, textAlign);

        TextOutW(hdc, x, y, text.c_str(), static_cast<int>(text.length()));

        SetTextAlign(hdc, oldAlign);
        SetBkMode(hdc, oldBkMode);
        SetTextColor(hdc, oldColor);
        SelectObject(hdc, hFontOld);
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
// 雙人競速賽管理器類別 (OOP)
//==================================================
class DoublePlayerMatch {
public:
    enum class MatchResult { NONE, A_WIN, L_WIN, A_LOSE_WRONG, L_LOSE_WRONG };

private:
    int scoreA_ = 0;
    int scoreL_ = 0;
    ULONGLONG lastElapsedA_ = 0;
    ULONGLONG lastElapsedL_ = 0;
    MatchResult lastResult_ = MatchResult::NONE;
    bool roundActive_ = true;

public:
    void Reset() {
        scoreA_ = 0;
        scoreL_ = 0;
        lastElapsedA_ = 0;
        lastElapsedL_ = 0;
        lastResult_ = MatchResult::NONE;
        roundActive_ = true;
    }

    void ProcessKeyPress(wchar_t player, bool isMatched, ULONGLONG elapsedTime) {
        if (!roundActive_) return;

        if (player == L'A') {
            lastElapsedA_ = elapsedTime;
            if (isMatched) {
                scoreA_++;
                lastResult_ = MatchResult::A_WIN;
            }
            else {
                scoreL_++;
                lastResult_ = MatchResult::A_LOSE_WRONG;
            }
            roundActive_ = false;
        }
        else if (player == L'L') {
            lastElapsedL_ = elapsedTime;
            if (isMatched) {
                scoreL_++;
                lastResult_ = MatchResult::L_WIN;
            }
            else {
                scoreA_++;
                lastResult_ = MatchResult::L_LOSE_WRONG;
            }
            roundActive_ = false;
        }
    }

    void NewRound() {
        roundActive_ = true;
    }

    int GetScoreA() const { return scoreA_; }
    int GetScoreL() const { return scoreL_; }
    ULONGLONG GetElapsedA() const { return lastElapsedA_; }
    ULONGLONG GetElapsedL() const { return lastElapsedL_; }
    MatchResult GetLastResult() const { return lastResult_; }
};

//==================================================
// 主視窗 App 類別
//==================================================
class PokerWindowApp {
private:
    HINSTANCE hInstance_;
    HWND hwnd_;

    enum class GameMode { NONE, SINGLE_PLAY, DOUBLE_PLAY };
    GameMode currentMode_ = GameMode::NONE;

    DoublePlayerMatch doubleMatch_;

    int currentSuitIndex_ = 0;
    int currentValueIndex_ = 0;

    int currentShownSeqIndex_ = 0;
    int nextSeqIndex_ = 0;

    bool running_ = false;
    bool hasCurrentItem_ = false;

    ULONGLONG shownTick_ = 0;
    std::mt19937 rng_;

    std::vector<int> deckOrder_;
    size_t deckPos_ = 0;
    bool anyMatchOccurred_ = false;

    Deck deck_;

    static constexpr const wchar_t* CLASS_NAME = L"PokerCardWin32MenuReaction";
    static constexpr UINT TIMER_ID = 1001;
    static constexpr UINT ID_MENU_START = 40001;
    static constexpr UINT ID_MENU_STOP = 40002;
    static constexpr UINT ID_MENU_EXIT = 40003;
    static constexpr int HOTKEY_ID_SPACE = 50001;
    static constexpr int HOTKEY_ID_ESC = 50002;
    static constexpr int HOTKEY_ID_R = 50003;
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
            L"1112060 1142006 1142016 1142056 1142059",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            1200, 700,
            nullptr,
            CreateAppMenu(),
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
        std::uniform_int_distribution<int> dist(800, 1200);
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
        currentMode_ = GameMode::SINGLE_PLAY;
        running_ = true;
        hasCurrentItem_ = false;
        nextSeqIndex_ = 0;
        currentShownSeqIndex_ = 0;
        anyMatchOccurred_ = false;
        BuildShuffledDeck();

        if (!RegisterHotKey(hwnd_, HOTKEY_ID_SPACE, 0, VK_SPACE)) {
            MessageBoxW(hwnd_, L"無法註冊空白鍵 HotKey", L"錯誤", MB_OK | MB_ICONERROR);
        }
        RegisterHotKey(hwnd_, HOTKEY_ID_ESC, 0, VK_ESCAPE);
        RegisterHotKey(hwnd_, HOTKEY_ID_R, 0, 'R');

        ShowNextItem();
        ScheduleNextTimer();
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void StartDoubleMatch() {
        StopSequence();

        currentMode_ = GameMode::DOUBLE_PLAY;
        running_ = true;
        hasCurrentItem_ = false;
        nextSeqIndex_ = 0; // 修正：原先漏掉底線
        currentShownSeqIndex_ = 0;
        anyMatchOccurred_ = false;

        doubleMatch_.Reset();
        BuildShuffledDeck();

        RegisterHotKey(hwnd_, HOTKEY_ID_ESC, 0, VK_ESCAPE);
        RegisterHotKey(hwnd_, HOTKEY_ID_R, 0, 'R');

        ShowNextItem();
        ScheduleNextTimer();
        InvalidateRect(hwnd_, nullptr, TRUE);
    }

    void StopSequence() {
        KillTimer(hwnd_, TIMER_ID);
        UnregisterHotKey(hwnd_, HOTKEY_ID_SPACE);
        UnregisterHotKey(hwnd_, HOTKEY_ID_ESC);
        UnregisterHotKey(hwnd_, HOTKEY_ID_R);
        running_ = false;
    }

    void ScheduleNextTimer() {
        KillTimer(hwnd_, TIMER_ID);
        SetTimer(hwnd_, TIMER_ID, RandomIntervalMs(), nullptr);
    }

    bool ShowNextItem() {
        if (deckPos_ >= deckOrder_.size()) {
            ShowNoMatchFailureIfNeeded();
            return false;
        }

        int cardId = deckOrder_[deckPos_++];
        currentSuitIndex_ = cardId / 13;
        currentValueIndex_ = cardId % 13;

        currentShownSeqIndex_ = nextSeqIndex_;
        nextSeqIndex_ = (nextSeqIndex_ + 1) % 13;

        shownTick_ = GetTickCount64();
        hasCurrentItem_ = true;

        // 修正：原先誤寫成賦值 "=" 運算，改為比較運算 "=="
        if (currentMode_ == GameMode::DOUBLE_PLAY) {
            doubleMatch_.NewRound();
        }

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
            if (currentMode_ == GameMode::SINGLE_PLAY) {
                MessageBoxW(hwnd_, L"牌組播放完畢！您漏掉了所有相符的牌面。", L"遊戲結束", MB_OK | MB_ICONINFORMATION);
            }
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
    }

    void OnPaint() {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd_, &ps);
        RECT rc;
        GetClientRect(hwnd_, &rc);

        HBRUSH bg = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        if (running_ && hasCurrentItem_) {
            const auto& suits = GetSuits();
            const auto& values = GetCardValues();
            Card currentCard(values[currentValueIndex_], suits[currentSuitIndex_]);
            currentCard.Draw(hdc, 150, 150);

            const auto& seq = GetSequenceLabels();
            std::wstring matchText = L"比對目標：" + std::wstring(seq[currentShownSeqIndex_]);
            TextRenderer::Draw(hdc, 350, 200, matchText, 32, RGB(0, 0, 255), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);

            if (currentMode_ == GameMode::SINGLE_PLAY) {
                TextRenderer::Draw(hdc, 350, 260, L"當兩者相符時，請立刻按下 [空白鍵]！", 18, RGB(60, 60, 60), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
            }
            else if (currentMode_ == GameMode::DOUBLE_PLAY) {
                TextRenderer::Draw(hdc, 350, 250, L"【雙人競速模式】", 20, RGB(0, 128, 64), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
                TextRenderer::Draw(hdc, 350, 280, L"玩家 A 請按 [ A ] 鍵 ｜ 玩家 L 請按 [ L ] 鍵", 18, RGB(60, 60, 60), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);

                std::wstringstream ssA, ssL;
                ssA << L"玩家 A 分數：" << doubleMatch_.GetScoreA() << L"  (反應：" << doubleMatch_.GetElapsedA() << L" ms)";
                ssL << L"玩家 L 分數：" << doubleMatch_.GetScoreL() << L"  (反應：" << doubleMatch_.GetElapsedL() << L" ms)";

                TextRenderer::Draw(hdc, 350, 340, ssA.str(), 18, RGB(220, 0, 0), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
                TextRenderer::Draw(hdc, 350, 370, ssL.str(), 18, RGB(0, 0, 220), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);

                std::wstring resultStr = L"等待搶答...";
                COLORREF resColor = RGB(100, 100, 100);
                auto res = doubleMatch_.GetLastResult();
                if (res == DoublePlayerMatch::MatchResult::A_WIN) { resultStr = L"✦ 這一發 A 贏了！(按對)"; resColor = RGB(220, 0, 0); }
                else if (res == DoublePlayerMatch::MatchResult::L_WIN) { resultStr = L"✦ 這一發 L 贏了！(按對)"; resColor = RGB(0, 0, 220); }
                else if (res == DoublePlayerMatch::MatchResult::A_LOSE_WRONG) { resultStr = L"✕ A 按錯處罰！分數給 L"; resColor = RGB(255, 100, 0); }
                else if (res == DoublePlayerMatch::MatchResult::L_LOSE_WRONG) { resultStr = L"✕ L 按錯處罰！分數給 A"; resColor = RGB(255, 100, 0); }

                TextRenderer::Draw(hdc, 350, 420, L"本輪結果：" + resultStr, 22, resColor, TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            }

            TextRenderer::Draw(hdc, 350, 480, L"按下 [ESC] 回到主選單 ｜ 按下 [ R ] 重新開始", 16, RGB(120, 120, 120), TA_LEFT | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
        }
        else {
            TextRenderer::Draw(hdc, 600, 260, L"【光刺激誘發癲癇症警告】", 24, RGB(255, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            TextRenderer::Draw(hdc, 600, 300, L"有極少數的人在觀看一些視覺影像時可能會突然癲癇發作，", 20, RGB(255, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            TextRenderer::Draw(hdc, 600, 340, L"這些影像包括視訊遊戲中出現的閃光或圖形。", 20, RGB(255, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            TextRenderer::Draw(hdc, 600, 380, L"若有不適請立即停止該遊戲並送醫診斷。", 20, RGB(255, 60, 60), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_BOLD);
            TextRenderer::Draw(hdc, 600, 440, L"請點選上方選單選擇「單人賽」或「雙人鍵盤競速」開始遊戲", 16, RGB(100, 100, 100), TA_CENTER | TA_BASELINE, 0, L"Segoe UI", FW_NORMAL);
        }

        EndPaint(hwnd_, &ps);
    }

public:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case ID_MENU_START:
                StartSequence();
                return 0;
            case ID_DOUBLE:
                StartDoubleMatch();
                return 0;
            case ID_MENU_STOP:
                StopSequence();
                currentMode_ = GameMode::NONE;
                InvalidateRect(hwnd_, nullptr, TRUE);
                return 0;
            case ID_MENU_EXIT:
                DestroyWindow(hwnd_);
                return 0;
            case ID_HELP_ABOUT:
                MessageBoxW(hwnd_, L"【單人模式】：相符時按空白鍵。\n【雙人模式】：相符時玩家A按'A'、玩家L按'L'，看誰反應快！", L"關於本遊戲", MB_OK | MB_ICONINFORMATION);
                return 0;
            }
            break;

        case WM_KEYDOWN:
            if (running_ && hasCurrentItem_ && currentMode_ == GameMode::DOUBLE_PLAY) {
                ULONGLONG now = GetTickCount64();
                ULONGLONG elapsed = now - shownTick_;
                bool matched = IsCardAndTextMatched();

                if (wParam == 'A' || wParam == 'a') {
                    doubleMatch_.ProcessKeyPress(L'A', matched, elapsed);
                    InvalidateRect(hwnd_, nullptr, TRUE);
                }
                else if (wParam == 'L' || wParam == 'l') {
                    doubleMatch_.ProcessKeyPress(L'L', matched, elapsed);
                    InvalidateRect(hwnd_, nullptr, TRUE);
                }
            }
            break;

        case WM_TIMER:
            if (wParam == TIMER_ID) {
                if (!ShowNextItem()) {
                    KillTimer(hwnd_, TIMER_ID);
                    if (currentMode_ == GameMode::DOUBLE_PLAY) {
                        std::wstringstream ss;
                        ss << L"52張撲克牌已全數發完！\n\n"
                            << L"【最終戰果】\n"
                            << L"玩家 A 總分：" << doubleMatch_.GetScoreA() << L" 分\n"
                            << L"玩家 L 總分：" << doubleMatch_.GetScoreL() << L" 分\n\n";
                        if (doubleMatch_.GetScoreA() > doubleMatch_.GetScoreL()) ss << L"🎉 恭喜 玩家 A 獲得最終勝利！";
                        else if (doubleMatch_.GetScoreL() > doubleMatch_.GetScoreA()) ss << L"🎉 恭喜 玩家 L 獲得最終勝利！";
                        else ss << L"🤝 雙方平手！不分軒輊！";

                        MessageBoxW(hwnd_, ss.str().c_str(), L"雙人賽結束", MB_OK | MB_ICONINFORMATION);
                        currentMode_ = GameMode::NONE;
                    }
                }
                else {
                    ScheduleNextTimer();
                }
            }
            return 0;

        case WM_HOTKEY:
            if (wParam == HOTKEY_ID_SPACE) {
                if (currentMode_ == GameMode::SINGLE_PLAY) {
                    HandleSpacePressed();
                }
            }
            else if (wParam == HOTKEY_ID_ESC) {
                StopSequence();
                currentMode_ = GameMode::NONE;
                InvalidateRect(hwnd_, nullptr, TRUE);
            }
            else if (wParam == HOTKEY_ID_R) {
                if (currentMode_ == GameMode::SINGLE_PLAY) StartSequence();
                else if (currentMode_ == GameMode::DOUBLE_PLAY) StartDoubleMatch();
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