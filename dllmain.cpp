// dllmain.cpp : Defines the entry point for the application.
#include "dllmain.h"
#include "string"
#include "src/APUtils.h"

DWORD lastChatBoxUpdate = 0;

void SetupHooks() {
    SetupOnLevelUp();
    SetupOnMessageSent();
;}

void onItemCleared() {

}

void onItemReceived(int itemId, bool notify) {
    if (!notify) return;
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    chatWidget->Print(L"item Received: " + std::to_wstring(itemId) + L" Notify(" + ((notify) ? L"True" : L"False") + L")\n", Color::White());
}

void onLocationChecked(int locationId) {
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    chatWidget->Print(L"Location Checked: " +std::to_wstring(locationId) + L"\n", Color::White());
}

void PrintArchipelago(cube::ChatWidget* chatWidget) {
    chatWidget->Print(L"[AP] ", Color(0.1f, 0.1f, 0.5f, 1.f));
}

void PrintItemName(cube::ChatWidget* chatWidget, std::wstring itemName) {
    Color color = (std::find(APUtils::items.begin(), APUtils::items.end(), itemName) != APUtils::items.end())
        ? APUtils::progressiveItemColor : APUtils::itemColor;
    chatWidget->Print(itemName, color);
}

void PrintCountdown(AP_CountdownMessage* msg) {
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    std::wstring timer = std::to_wstring(msg->timer);

    PrintArchipelago(chatWidget);
    chatWidget->Print(L"Countdown: ", Color::White());
    chatWidget->Print((msg->timer == 0) ? L"GO" : timer, Color::White());
    chatWidget->Print(L"\n", Color::White());
}

void PrintHint(AP_HintMessage* msg) {
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    std::wstring senderName = std::wstring(msg->sendPlayer.begin(), msg->sendPlayer.end());
    std::wstring recverName = std::wstring(msg->recvPlayer.begin(), msg->recvPlayer.end());
    std::wstring locationName = std::wstring(msg->location.begin(), msg->location.end());
    std::wstring itemName = std::wstring(msg->item.begin(), msg->item.end());

    PrintArchipelago(chatWidget);
    chatWidget->Print(recverName, APUtils::playerColor);
    chatWidget->Print(L" ", (msg->checked) ? Color::Green() : Color::White());
    PrintItemName(chatWidget, itemName);
    chatWidget->Print(L" is in ", (msg->checked) ? Color::Green() : Color::White());
    chatWidget->Print(senderName, APUtils::playerColor);
    chatWidget->Print(L" world at ", (msg->checked) ? Color::Green() : Color::White());
    chatWidget->Print(locationName, APUtils::locationColor);
    if (msg->checked) {
        chatWidget->Print(L" and has already been found!", Color::Green());
    }
    chatWidget->Print(L"\n", Color::White());
}

void PrintItemRecv(AP_ItemRecvMessage* msg) {
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    std::wstring senderName = std::wstring(msg->sendPlayer.begin(), msg->sendPlayer.end());
    std::wstring itemName = std::wstring(msg->item.begin(), msg->item.end());

    PrintArchipelago(chatWidget);
    chatWidget->Print(senderName, APUtils::playerColor);
    chatWidget->Print(L" just received ", Color::White());
    PrintItemName(chatWidget, itemName);
    chatWidget->Print(L"\n", Color::White());
}

void PrintItemSend(AP_ItemSendMessage* msg) {
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    std::wstring recverName = std::wstring(msg->recvPlayer.begin(), msg->recvPlayer.end());
    std::wstring itemName = std::wstring(msg->item.begin(), msg->item.end());

    PrintArchipelago(chatWidget);
    PrintItemName(chatWidget, itemName);
    chatWidget->Print(L" was sent to ", Color::White());
    chatWidget->Print(recverName, APUtils::playerColor);
    chatWidget->Print(L"\n", Color::White());
}

void PrintMessage(std::wstring message, Color color) {
    cube::ChatWidget* chatWidget = cube::GetGameController()->ChatWidget;
    chatWidget->Print(message, color);
    chatWidget->Print(L"\n", color);

}

// This function is called whenever the mod is loaded
DWORD WINAPI MainLoop() {
    cube::InitAPI();
    SetupHooks();

    cube::GameController* gc = cube::GetGameController();
    cube::ChatWidget* chatWidget = gc->ChatWidget;

    // APCpp
    const char* AP_ip = "127.0.0.1:25565";
    const char* AP_gameName = "CubeWorld_Alpha";
    const char* AP_slotName = "CubeWorld";
    const char* AP_password = "";

    std::string AP_ipString = std::string(AP_ip);

    AP_Init(AP_ip, AP_gameName, AP_slotName, AP_password); // ip, gameName, slotName, password
    AP_SetItemClearCallback(onItemCleared);
    AP_SetItemRecvCallback(onItemReceived);
    AP_SetLocationCheckedCallback(onLocationChecked);
    AP_Start();

    int l = 0;

    while (true) {
        // DEBUG
        if (GetAsyncKeyState((int)'H') & 0x8000) {
            AP_SendItem(LevelUpItemId + l);
            l++;
            Sleep(250);
        }
        // DEBUG

        if (AP_IsMessagePending()) {
            AP_Message* message = AP_GetLatestMessage();
            switch (message->type) {
            case AP_MessageType::Plaintext:
                PrintMessage(std::wstring(message->text.begin(), message->text.end()), Color::White());
                break;
            case AP_MessageType::ItemSend:
                PrintItemSend((AP_ItemSendMessage*)message);
                break;
            case AP_MessageType::ItemRecv:
                PrintItemRecv((AP_ItemRecvMessage*)message);
                break;
            case AP_MessageType::Hint:
                PrintHint((AP_HintMessage*)message);
                break;
            case AP_MessageType::Countdown:
                PrintCountdown((AP_CountdownMessage*)message);
                break;
            }
            AP_ClearLatestMessage();
            lastChatBoxUpdate = gc->world.Time;
            chatWidget->plasma_D3D9RenderSurface->alpha_filter_multiplier = 1.0;
        }

        // Chat box update
        
        if (chatWidget->is_typing_message) {
            lastChatBoxUpdate = gc->world.Time;
            chatWidget->plasma_D3D9RenderSurface->alpha_filter_multiplier = 1.0;
        }

        // Should prevent chat from reseting when a new day begin.
        if (gc->world.Time < lastChatBoxUpdate) {
            lastChatBoxUpdate = gc->world.Time + 100000;
        }

        DWORD time_since_last_update = gc->world.Time - lastChatBoxUpdate-100000;
        if (time_since_last_update < 10000 && time_since_last_update > 0) {
            double animation_progress = (double)time_since_last_update / 10000.0;
            chatWidget->plasma_D3D9RenderSurface->alpha_filter_multiplier = 1.0 - (animation_progress * 0.9);
        }

        chatWidget->pos_x = 0;
        chatWidget->pos_y = 650;
        chatWidget->width = 750;
        chatWidget->height = 850;
    }

    return 0;
}

// This function is the entry function called whenever the dll is loaded
extern "C" BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Call the MainLoop Function on an other Thread.
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainLoop, 0, 0, NULL);
        break;
    }
    return TRUE;
}