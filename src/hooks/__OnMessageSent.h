#pragma once
#include "../../cube.h"

void* ASMOnMessageSent_jmpback;

void __stdcall OnMessageSent(wchar_t* message) {
    cube::GameController* gc = cube::GetGameController();
    cube::ChatWidget* chatWidget = gc->ChatWidget;
    std::wstring strMessage = std::wstring(chatWidget->current_typed_message.c_str());//chatWidget->current_typed_message;
    if (AP_GetConnectionStatus() == AP_ConnectionStatus::Authenticated) {
        AP_Say(std::string(strMessage.begin(), strMessage.end()));
    }
    else {
        // Also print player's name
        std::string name = std::string(gc->local_player->entity_data.name);
        std::wstring playerName = std::wstring(name.begin(), name.end());
        chatWidget->Print(playerName +L": ", Color(0.0f, 1.0f, 1.0f, 1.0f));
        Color color = Color::White();
        chatWidget->Print(strMessage+L"\n", color);
    }
    chatWidget->current_typed_message.set(L"");
}

void __declspec(naked) ASMOnMessageSent() {
    __asm {

        PUSH_ALL

        push esi // push message

        call OnMessageSent

        POP_ALL

        jmp[ASMOnMessageSent_jmpback];
    }
}

void SetupOnMessageSent() {
    WriteJMP((void*)(base + 0x8CCE6), (void*)(base + 0x8CCED), 7U); // Don't print player name
    WriteJMP((void*)(base + 0x7E6C5), (void*)(base + 0x7E6CA)); // Don't empty the current message string
    WriteJMP((void*)(base + 0x7E6D0), (void*)(base + 0x7E6D5)); // Don't empty the current message string
    WriteJMP((void*)(base + 0x8CD70), (void*)&ASMOnMessageSent, 7U); // Replace sent message with custom logic
    ASMOnMessageSent_jmpback = (void*)(base + 0x8CD77);
}