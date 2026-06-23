#include "pch.h"
#include <windows.h>

//======================================================================
// 📦 封裝設計：撲克牌自動反應機器人 (PokerBot)
// 負責核心的按鍵模擬、邏輯比對以及自動化操作
//======================================================================
class PokerBot {
private:
    // 封裝私有成員：執行物理按鍵與訊息佇列的發送
    void SendSpaceNotification(HWND hwnd) const {
        if (hwnd == nullptr) return;

        // 方法 1：硬體級物理按鍵發送（最快、最精準）
        keybd_event(VK_SPACE, 0, 0, 0);
        keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);

        // 方法 2：訊息佇列轟炸（雙重保險）
        PostMessageW(hwnd, WM_KEYDOWN, VK_SPACE, 0);
        PostMessageW(hwnd, WM_KEYUP, VK_SPACE, 0);
    }

public:
    // 建構子
    PokerBot() = default;

    // 解構子
    ~PokerBot() = default;

    // 核心業務邏輯方法：檢查卡片點數是否相符，若相符則觸發按鍵
    void InspectAndReact(HWND hwnd, int cardValue, int targetValue) const {
        // 比對點數：如果撲克牌點數與右側目標值符合
        if (cardValue == targetValue) {
            // 如果需要防止太過誇張的 0 毫秒反應，可以在這裡加上 Sleep(10); 微調延遲
            SendSpaceNotification(hwnd);
        }
    }
};

//======================================================================
// 🌐 全域物件與標準 C 匯出介面（保持與主程式低耦合對接）
//======================================================================
#define DLL_EXPORT extern "C" __declspec(dllexport)

// 在內部維護一個全域的機器人執行個體指標
static PokerBot* g_pokerBot = nullptr;

// 🎯 修改這裡：名稱叫 "check"，參數收 HWND 與兩個點數數值
DLL_EXPORT void check(HWND hwnd, int cardValue, int targetValue) {
    // 延遲初始化 (Lazy Initialization)
    if (g_pokerBot == nullptr) {
        g_pokerBot = new PokerBot();
    }

    // 將職責轉交給專門的 PokerBot 物件處理
    g_pokerBot->InspectAndReact(hwnd, cardValue, targetValue);
}

//======================================================================
// 🔄 DLL 進入點
//======================================================================
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        if (g_pokerBot != nullptr) {
            delete g_pokerBot;
            g_pokerBot = nullptr;
        }
        break;
    }
    return TRUE;
}