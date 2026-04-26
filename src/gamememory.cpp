#include "gamememory.h"
#include <Windows.h>
#include "tlhelp32.h"
#include "common.h"
#include "pointer.h"
#include <array>

bool init_dynamic_info(const char* pName, dynamic_addresses& di) {
    DWORD pID = 0;
    const HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pInfo{ sizeof(PROCESSENTRY32) };

    if (Process32First(hProcessSnap, &pInfo)) {
        do {
            if (strcmp(pName, pInfo.szExeFile) == 0) {
                pID = pInfo.th32ProcessID;
                break;
            }
        } while (Process32Next(hProcessSnap, &pInfo));
    }
    CloseHandle(hProcessSnap);

    if (pID == 0) return 1; // Process not found

    // Now create a MODULE snapshot for that specific PID
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
    MODULEENTRY32 me32{ sizeof(MODULEENTRY32) };
    DWORD baseAddr = 0;

    if (Module32First(hModuleSnap, &me32)) {
        baseAddr = (DWORD)(DWORD_PTR)me32.modBaseAddr;
    }

    di.base_addr = baseAddr;
    di.processId = pID;

    CloseHandle(hModuleSnap);
    HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (h == NULL) {
        return 1;
    }
    di.gameHandle = h;
    return 0; // Success
}

void init_game_info() {
    _da._player_addr = pointer(_da.base_addr).add(0x18AC00).deref();
    _da._camera_addr = pointer(_da.base_addr).add(0x192060);
    _da._projection_mat_addr = pointer(_da.base_addr).add(0x17E0B0);
    const auto entity_list_addr = pointer(_da.base_addr).add(0x18AC04).deref();
    const auto entity_count_addr = pointer(_da.base_addr).add(0x18AC0C);
    RPM<std::array<DWORD, 32>>(entity_list_addr, _da._entity_addr_list);
    RPM<DWORD>(entity_count_addr, _gd._entity_count);
}