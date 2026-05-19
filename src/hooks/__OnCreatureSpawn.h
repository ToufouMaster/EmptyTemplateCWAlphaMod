#pragma once
#include "../../cube.h"

void* ASMOnWanderingCreatureSpawnData_jmpback;
void* ASMOnRuinsCreatureSpawnData_jmpback;
void* ASMOnCreatureSpawn_jmpback;
/*
uint32_t __stdcall OnCreatureSpawnData(uint32_t race) {
    //typedef uint32_t (__stdcall* get_random_gender_race_from_race)(uint32_t race);
    //get_random_gender_race_from_race GetRandomGenderRaceFromRace = (get_random_gender_race_from_race)(base + 0x1A0ED0);
    cube::GetGameController()->ChatWidget->Print(L"Race: " + std::to_wstring(race), Color::White());
    return 0;// CreatureRandomizer::GetRandomRaceFromID(race);//GetRandomGenderRaceFromRace(race));
}
*/

void __stdcall OnCreatureSpawnData(uint32_t* race) {
    //typedef uint32_t (__stdcall* get_random_gender_race_from_race)(uint32_t race);
    //get_random_gender_race_from_race GetRandomGenderRaceFromRace = (get_random_gender_race_from_race)(base + 0x1A0ED0);
    *race = CreatureRandomizer::GetRandomRaceFromID(*race);//GetRandomGenderRaceFromRace(race));
}

void __declspec(naked) ASMOnCreatureSpawn() {
    __asm {

        /*
        003fd32a  51                 push    ecx
        003fd32b  83c02c             add     eax, 0x2c {CreatureSpawnData::race}
        003fd32e  50                 push    eax
        */

        push ecx
        add eax, 0x2c // eax being the CreatureSpawnData adding 0x2c lead to the race address

        PUSH_ALL

        push eax;

        call OnCreatureSpawnData

        POP_ALL
        push eax;

        jmp[ASMOnCreatureSpawn_jmpback];
    }
}

void __declspec(naked) ASMOnWanderingCreatureSpawnData() {
    __asm {
        PUSH_ALL

        push eax

        call OnCreatureSpawnData

        mov [ebp - 0x1d4], eax;

        POP_ALL

        mov eax, [ebp - 0x1d4]

        jmp[ASMOnWanderingCreatureSpawnData_jmpback];
    }
}

void __declspec(naked) ASMOnRuinsCreatureSpawnData() {
    __asm {
        PUSH_ALL

        push eax

        call OnCreatureSpawnData

        mov[ebp + 0x18], eax;

        POP_ALL

        mov eax, [ebp + 0x18]

        jmp[ASMOnRuinsCreatureSpawnData_jmpback];
    }
}

void SetupOnCreatureSpawnData() {
    WriteJMP((void*)(base + 0x1ED32A), &ASMOnCreatureSpawn, 5U);
    ASMOnCreatureSpawn_jmpback = (void*)(base + 0x1ED32F);
    //WriteJMP((void*)(base + 0x1D702B), &ASMOnWanderingCreatureSpawnData, 6U);
    //WriteJMP((void*)(base + 0x1F3884), &ASMOnRuinsCreatureSpawnData, 5U);
    //ASMOnWanderingCreatureSpawnData_jmpback = (void*)(base + 0x1D7031);
    //ASMOnRuinsCreatureSpawnData_jmpback = (void*)(base + 0x1F3889);
    //WriteCALL((void*)(base + 0x1D70F6), &OnCreatureSpawnData);
}