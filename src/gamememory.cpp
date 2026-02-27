#include "gamememory.h"
#include <Windows.h>
#include "tlhelp32.h"
#include "common.h"
#include "pointer.h"
#include <array>

bool initDynamicInfo(const char* pName, dynamicInfo& di) {
    DWORD pID = 0;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
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
        baseAddr = (DWORD)me32.modBaseAddr;
    }

    di.baseAddr = baseAddr;
    di.processId = pID;

    CloseHandle(hModuleSnap);
    HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (h == NULL) {
        return 1;
    }
    di.gameHandle = h;
    return 0; // Success
}

void initGameInfo(dynamicInfo& di, gameInfo& gi) {
    auto entityListAddr = pointer(di.baseAddr).level(0x18AC04);
    auto entityCountAddr = pointer(di.baseAddr + 0x18AC0C);
    RPM<DWORD>(entityCountAddr, gi.entityCount);
    RPM<std::array<DWORD, 32>>(entityListAddr, gi.entityAddrList);
}