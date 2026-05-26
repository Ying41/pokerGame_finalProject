#include <windows.h>
#include <tchar.h>
#define ID_SINGLE  1001
#define ID_DOUBLE 1002
#define ID_MUTIPLE 1003
#define ID_HELP_ABOUT 1004

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_SINGLE:
            break;
        case  ID_DOUBLE:
            break;
        case ID_MUTIPLE:
            break;
        case ID_HELP_ABOUT:
            MessageBox(hwnd, _T("第2組:1112060 1142006 1142016 1142056 1142059"), _T("About"), MB_OK | MB_ICONINFORMATION);
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetGraphicsMode(hdc, GM_ADVANCED);

        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
        SelectObject(hdc, hPen);
        SetBkMode(hdc, TRANSPARENT);

        HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI Symbol");
        HFONT hFontBig = CreateFont(60, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Times New Roman");

        int cardWidth = 140;
        int cardHeight = 210;
        int gap = 5; // 牌與牌之間的微小間距
        int offset = 7;//旋轉後偏移量
        const TCHAR* ranks[] = { L"A", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K" };

        for (int i = 0; i < 13; i++) {
            // 公式化座標：不重疊，所以間距必須大於等於 cardWidth
            int cardX = 10 + (i * (cardWidth + gap));
            int cardY = 10;

            Rectangle(hdc, cardX, cardY, cardX + cardWidth, cardY + cardHeight);

            // --- 繪製中間花色與字母 ---
            int cx = cardX + (cardWidth / 2);
            int cy = cardY + (cardHeight / 2);
            int dx = cardWidth / 5;  // 水平間距比例
            int dy = cardHeight / 5; // 垂直間距比例
            

            SelectObject(hdc, hFont);
            if (i < 10) { // A ~ 10 的花色分佈
                // --- 【上半部：正立花色】 ---
                if (i == 0) { // A
                    TextOut(hdc, cx - 10, cy - 15, L"♠", 1);
                }
                else if (i == 1) { // 2
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♠", 1);
                }
                else if (i == 2) { // 3
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♠", 1); // 中間那顆
                }
                else if (i == 3) { // 4
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                }
                else if (i == 4) { // 5
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♠", 1); // 正中央
                }
                else if (i == 5) { // 6
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♠", 1); // 中左
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♠", 1); // 中右
                }
                else if (i == 6) { // 7
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♠", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♠", 1); // 中間偏上
                }
                else if (i == 7) { // 8
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♠", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♠", 1); // 中上
                }
                else if (i == 8) { // 9
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 4) - 15, L"♠", 1); // 稍微往中間靠
                    TextOut(hdc, cx + dx - 10, cy - (dy / 4) - 15, L"♠", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♠", 1);
                }
                else if (i == 9) { // 10
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♠", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 3) - 15, L"♠", 1);
                    TextOut(hdc, cx + dx - 10, cy - (dy / 3) - 15, L"♠", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♠", 1); // 中間兩顆的上顆
                }
            }
            else { // J, Q, K
                SelectObject(hdc, hFontBig);
                TextOut(hdc, cx - 25, cy - 35, ranks[i], 1);
                SelectObject(hdc, hFont);
            }

            // --- 繪製正立與倒立角標 + 倒立花色 ---
            for (int side = 0; side < 2; side++) {
                if (side == 1) { // 倒立設定
                    XFORM xf = { -1.0f, 0, 0, -1.0f, (float)(2 * cardX + cardWidth), (float)(2 * cardY + cardHeight) };
                    SetWorldTransform(hdc, &xf);

                    // 【關鍵修改】：當 side == 1 (倒立) 時，補畫下半部的花色
                    if (i == 1) TextOut(hdc, cx - 10+offset, cy - dy - 15, L"♠", 1); // 2 的下半
                    else if (i == 2) TextOut(hdc, cx - 10+offset, cy - dy - 15, L"♠", 1); // 3 的下半
                    else if (i == 3) { // 4 的下半
                        TextOut(hdc, cx - dx - 10+offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♠", 1);
                    }
                    else if (i == 4) { // 5 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♠", 1);
                    }
                    else if (i == 5) { // 6 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♠", 1);
                    }
                    else if (i == 6) { // 7 的下半 (注意 7 的中間那顆不用倒畫)
                        TextOut(hdc, cx - dx - 10+offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10+offset, cy - dy - 15, L"♠", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♠", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♠", 1);
                    }
                    else if (i == 7) { // 8 的下半
                        TextOut(hdc, cx - dx - 10+offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10+offset, cy - dy - 15, L"♠", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♠", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♠", 1);
                        TextOut(hdc, cx - 10+offset, cy - (dy / 2) - 15, L"♠", 1); // 對稱中下
                    }
                    else if (i == 8) { // 9 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 4) - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 4) - 15, L"♠", 1);
                    }
                    else if (i == 9) { // 10 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♠", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 3) - 15, L"♠", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 3) - 15, L"♠", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy * 2 / 3) - 15, L"♠", 1);
                    }
                }

                // 繪製角標（A, 2, 3... 與 ♠）
                TextOut(hdc, cardX + 10, cardY + 5, ranks[i], (int)_tcslen(ranks[i]));
                TextOut(hdc, cardX + 10, cardY + 30, L"♠", 1);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
            }
        }
        //愛心
        for (int i = 0; i < 13; i++) {
            // 公式化座標：不重疊，所以間距必須大於等於 cardWidth
            int cardX = 10 + (i * (cardWidth + gap));
            int cardY = 230;

            Rectangle(hdc, cardX, cardY, cardX + cardWidth, cardY + cardHeight);

            // --- 繪製中間花色與字母 ---
            int cx = cardX + (cardWidth / 2);
            int cy = cardY + (cardHeight / 2);
            int dx = cardWidth / 5;  // 水平間距比例
            int dy = cardHeight / 5; // 垂直間距比例


            SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(255, 0, 0));
            if (i < 10) { // A ~ 10 的花色分佈
                // --- 【上半部：正立花色】 ---
                if (i == 0) { // A
                    TextOut(hdc, cx - 10, cy - 15, L"♥️", 1);
                }
                else if (i == 1) { // 2
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♥️", 1);
                }
                else if (i == 2) { // 3
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♥️", 1); // 中間那顆
                }
                else if (i == 3) { // 4
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                }
                else if (i == 4) { // 5
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♥️", 1); // 正中央
                }
                else if (i == 5) { // 6
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♥️", 1); // 中左
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♥️", 1); // 中右
                }
                else if (i == 6) { // 7
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♥️", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♥️", 1); // 中間偏上
                }
                else if (i == 7) { // 8
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♥️", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♥️", 1); // 中上
                }
                else if (i == 8) { // 9
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 4) - 15, L"♥️", 1); // 稍微往中間靠
                    TextOut(hdc, cx + dx - 10, cy - (dy / 4) - 15, L"♥️", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♥️", 1);
                }
                else if (i == 9) { // 10
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♥️", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 3) - 15, L"♥️", 1);
                    TextOut(hdc, cx + dx - 10, cy - (dy / 3) - 15, L"♥️", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♥️", 1); // 中間兩顆的上顆
                }
            }
            else { // J, Q, K
                SelectObject(hdc, hFontBig);
                TextOut(hdc, cx - 25, cy - 35, ranks[i], 1);
                SelectObject(hdc, hFont);
            }

            // --- 繪製正立與倒立角標 + 倒立花色 ---
            for (int side = 0; side < 2; side++) {
                if (side == 1) { // 倒立設定
                    XFORM xf = { -1.0f, 0, 0, -1.0f, (float)(2 * cardX + cardWidth), (float)(2 * cardY + cardHeight) };
                    SetWorldTransform(hdc, &xf);

                    // 【關鍵修改】：當 side == 1 (倒立) 時，補畫下半部的花色
                    if (i == 1) TextOut(hdc, cx - 10 + offset, cy - dy - 15, L"♥️", 1); // 2 的下半
                    else if (i == 2) TextOut(hdc, cx - 10 + offset, cy - dy - 15, L"♥️", 1); // 3 的下半
                    else if (i == 3) { // 4 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                    }
                    else if (i == 4) { // 5 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                    }
                    else if (i == 5) { // 6 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                    }
                    else if (i == 6) { // 7 的下半 (注意 7 的中間那顆不用倒畫)
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♥️", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♥️", 1);
                    }
                    else if (i == 7) { // 8 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♥️", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♥️", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy / 2) - 15, L"♥️", 1); // 對稱中下
                    }
                    else if (i == 8) { // 9 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 4) - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 4) - 15, L"♥️", 1);
                    }
                    else if (i == 9) { // 10 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♥️", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 3) - 15, L"♥️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 3) - 15, L"♥️", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy * 2 / 3) - 15, L"♥️", 1);
                    }
                }

                // 繪製角標（A, 2, 3... 與 ♥️）
                TextOut(hdc, cardX + 10, cardY + 5, ranks[i], (int)_tcslen(ranks[i]));
                TextOut(hdc, cardX + 10, cardY + 30, L"♥️", 1);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
            }
        }
        //菱形
        for (int i = 0; i < 13; i++) {
            // 公式化座標：不重疊，所以間距必須大於等於 cardWidth
            int cardX = 10 + (i * (cardWidth + gap));
            int cardY = 460;

            Rectangle(hdc, cardX, cardY, cardX + cardWidth, cardY + cardHeight);

            // --- 繪製中間花色與字母 ---
            int cx = cardX + (cardWidth / 2);
            int cy = cardY + (cardHeight / 2);
            int dx = cardWidth / 5;  // 水平間距比例
            int dy = cardHeight / 5; // 垂直間距比例


            SelectObject(hdc, hFont);
            if (i < 10) { // A ~ 10 的花色分佈
                // --- 【上半部：正立花色】 ---
                if (i == 0) { // A
                    TextOut(hdc, cx - 10, cy - 15, L"♦️", 1);
                }
                else if (i == 1) { // 2
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♦️", 1);
                }
                else if (i == 2) { // 3
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♦️", 1); // 中間那顆
                }
                else if (i == 3) { // 4
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                }
                else if (i == 4) { // 5
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♦️", 1); // 正中央
                }
                else if (i == 5) { // 6
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♦️", 1); // 中左
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♦️", 1); // 中右
                }
                else if (i == 6) { // 7
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♦️", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♦️", 1); // 中間偏上
                }
                else if (i == 7) { // 8
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♦️", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♦️", 1); // 中上
                }
                else if (i == 8) { // 9
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 4) - 15, L"♦️", 1); // 稍微往中間靠
                    TextOut(hdc, cx + dx - 10, cy - (dy / 4) - 15, L"♦️", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♦️", 1);
                }
                else if (i == 9) { // 10
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♦️", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 3) - 15, L"♦️", 1);
                    TextOut(hdc, cx + dx - 10, cy - (dy / 3) - 15, L"♦️", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♦️", 1); // 中間兩顆的上顆
                }
            }
            else { // J, Q, K
                SelectObject(hdc, hFontBig);
                TextOut(hdc, cx - 25, cy - 35, ranks[i], 1);
                SelectObject(hdc, hFont);
            }

            // --- 繪製正立與倒立角標 + 倒立花色 ---
            for (int side = 0; side < 2; side++) {
                if (side == 1) { // 倒立設定
                    XFORM xf = { -1.0f, 0, 0, -1.0f, (float)(2 * cardX + cardWidth), (float)(2 * cardY + cardHeight) };
                    SetWorldTransform(hdc, &xf);

                    // 【關鍵修改】：當 side == 1 (倒立) 時，補畫下半部的花色
                    if (i == 1) TextOut(hdc, cx - 10 + offset, cy - dy - 15, L"♦️", 1); // 2 的下半
                    else if (i == 2) TextOut(hdc, cx - 10 + offset, cy - dy - 15, L"♦️", 1); // 3 的下半
                    else if (i == 3) { // 4 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                    }
                    else if (i == 4) { // 5 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                    }
                    else if (i == 5) { // 6 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                    }
                    else if (i == 6) { // 7 的下半 (注意 7 的中間那顆不用倒畫)
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♦️", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♦️", 1);
                    }
                    else if (i == 7) { // 8 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♦️", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♦️", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy / 2) - 15, L"♦️", 1); // 對稱中下
                    }
                    else if (i == 8) { // 9 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 4) - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 4) - 15, L"♦️", 1);
                    }
                    else if (i == 9) { // 10 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♦️", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 3) - 15, L"♦️", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 3) - 15, L"♦️", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy * 2 / 3) - 15, L"♦️", 1);
                    }
                }

                // 繪製角標（A, 2, 3... 與 ♦️）
                TextOut(hdc, cardX + 10, cardY + 5, ranks[i], (int)_tcslen(ranks[i]));
                TextOut(hdc, cardX + 10, cardY + 30, L"♦️", 1);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
            }
        }
        //梅花
        for (int i = 0; i < 13; i++) {
            // 公式化座標：不重疊，所以間距必須大於等於 cardWidth
            int cardX = 10 + (i * (cardWidth + gap));
            int cardY = 690;

            Rectangle(hdc, cardX, cardY, cardX + cardWidth, cardY + cardHeight);

            // --- 繪製中間花色與字母 ---
            int cx = cardX + (cardWidth / 2);
            int cy = cardY + (cardHeight / 2);
            int dx = cardWidth / 5;  // 水平間距比例
            int dy = cardHeight / 5; // 垂直間距比例


            SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(0, 0, 0));
            if (i < 10) { // A ~ 10 的花色分佈
                // --- 【上半部：正立花色】 ---
                if (i == 0) { // A
                    TextOut(hdc, cx - 10, cy - 15, L"♣", 1);
                }
                else if (i == 1) { // 2
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♣", 1);
                }
                else if (i == 2) { // 3
                    TextOut(hdc, cx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♣", 1); // 中間那顆
                }
                else if (i == 3) { // 4
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                }
                else if (i == 4) { // 5
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - 10, cy - 15, L"♣", 1); // 正中央
                }
                else if (i == 5) { // 6
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♣", 1); // 中左
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♣", 1); // 中右
                }
                else if (i == 6) { // 7
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♣", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♣", 1); // 中間偏上
                }
                else if (i == 7) { // 8
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - dx - 10, cy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - 15, L"♣", 1);
                    TextOut(hdc, cx - 10, cy - (dy / 2) - 15, L"♣", 1); // 中上
                }
                else if (i == 8) { // 9
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 4) - 15, L"♣", 1); // 稍微往中間靠
                    TextOut(hdc, cx + dx - 10, cy - (dy / 4) - 15, L"♣", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♣", 1);
                }
                else if (i == 9) { // 10
                    TextOut(hdc, cx - dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - dy - 15, L"♣", 1);
                    TextOut(hdc, cx - dx - 10, cy - (dy / 3) - 15, L"♣", 1);
                    TextOut(hdc, cx + dx - 10, cy - (dy / 3) - 15, L"♣", 1);
                    TextOut(hdc, cx - 10, cy - (dy * 2 / 3) - 15, L"♣", 1); // 中間兩顆的上顆
                }
            }
            else { // J, Q, K
                SelectObject(hdc, hFontBig);
                TextOut(hdc, cx - 25, cy - 35, ranks[i], 1);
                SelectObject(hdc, hFont);
            }

            // --- 繪製正立與倒立角標 + 倒立花色 ---
            for (int side = 0; side < 2; side++) {
                if (side == 1) { // 倒立設定
                    XFORM xf = { -1.0f, 0, 0, -1.0f, (float)(2 * cardX + cardWidth), (float)(2 * cardY + cardHeight) };
                    SetWorldTransform(hdc, &xf);

                    // 【關鍵修改】：當 side == 1 (倒立) 時，補畫下半部的花色
                    if (i == 1) TextOut(hdc, cx - 10 + offset, cy - dy - 15, L"♣", 1); // 2 的下半
                    else if (i == 2) TextOut(hdc, cx - 10 + offset, cy - dy - 15, L"♣", 1); // 3 的下半
                    else if (i == 3) { // 4 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                    }
                    else if (i == 4) { // 5 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                    }
                    else if (i == 5) { // 6 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                    }
                    else if (i == 6) { // 7 的下半 (注意 7 的中間那顆不用倒畫)
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♣", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♣", 1);
                    }
                    else if (i == 7) { // 8 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        //TextOut(hdc, cx - dx - 10, cy - 15, L"♣", 1);
                        //TextOut(hdc, cx + dx - 10, cy - 15, L"♣", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy / 2) - 15, L"♣", 1); // 對稱中下
                    }
                    else if (i == 8) { // 9 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 4) - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 4) - 15, L"♣", 1);
                    }
                    else if (i == 9) { // 10 的下半
                        TextOut(hdc, cx - dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - dy - 15, L"♣", 1);
                        TextOut(hdc, cx - dx - 10 + offset, cy - (dy / 3) - 15, L"♣", 1);
                        TextOut(hdc, cx + dx - 10 + offset, cy - (dy / 3) - 15, L"♣", 1);
                        TextOut(hdc, cx - 10 + offset, cy - (dy * 2 / 3) - 15, L"♣", 1);
                    }
                }

                // 繪製角標（A, 2, 3... 與 ♣）
                TextOut(hdc, cardX + 10, cardY + 5, ranks[i], (int)_tcslen(ranks[i]));
                TextOut(hdc, cardX + 10, cardY + 30, L"♣", 1);

                ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
            }
        }


        DeleteObject(hPen); DeleteObject(hFont); DeleteObject(hFontBig);
        EndPaint(hwnd, &ps);
    }
                 break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

HMENU CreateAppMenu() {
    HMENU hMenuBar = CreateMenu();

    AppendMenu(hMenuBar, MF_STRING, ID_SINGLE, _T("單人計時賽"));
    AppendMenu(hMenuBar, MF_STRING, ID_DOUBLE, _T("雙人鍵盤競速"));
    AppendMenu(hMenuBar, MF_STRING, ID_MUTIPLE, _T("多人視覺辨識賽"));
    AppendMenu(hMenuBar, MF_STRING, ID_HELP_ABOUT, _T("說明"));
    return hMenuBar;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow)
{
    const TCHAR CLASS_NAME[] = _T("Win32MenuExample");
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, _T("Window Registration Failed!"), _T("Error"), MB_ICONERROR);
        return 0;
    }
    HWND hwnd = CreateWindow(CLASS_NAME, _T("心臟病(第2組:1112060 1142006 1142016 1142056 1142059"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 2500, 1000,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error"), MB_ICONERROR);
        return 0;
    }
    SetMenu(hwnd, CreateAppMenu());
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}