#pragma once
#include "../../cube.h"

const int LevelUpItemId = 0xAC00000;

void* ASMOnLevelUp_jmpback;

void __fastcall OnLevelUp(int level, cube::Creature* creature) {
    cube::GameController* gc = cube::GetGameController();
    cube::Creature* local_player = cube::GetGameController()->local_player;
    if (creature->GUID != local_player->GUID) return;

    int toLevel = level + 1;
    AP_SendItem(LevelUpItemId + level-1); // Level 2 is the first check at id 0
}

void __declspec(naked) ASMOnLevelUp() {
    __asm {

        mov ecx, [esi + 0x190]

        PUSH_ALL
        mov edx, esi

        call OnLevelUp

        POP_ALL

        jmp[ASMOnLevelUp_jmpback];
    }
}

void SetupOnLevelUp() {
    //WriteJMP((void*)(base + 0x47B70), (void*)&ASMOnLevelUp, 6U);
    ASMOnLevelUp_jmpback = (void*)(base + 0x47B76);
}