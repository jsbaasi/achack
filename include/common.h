#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <vector>
#include <array>

struct botent
{
    char pad_0000[4]; //0x0000
    float x; //0x0004
    float z; //0x0008
    float y; //0x000C
    char pad_0010[220]; //0x0010
    int32_t health; //0x00EC
    char pad_00F0[277]; //0x00F0
    char name[32]; //0x0205
}; //Size: 0x0225

//struct xyz {
//    float x, z, y;
//};

inline botent gEntityArray[32];

struct dynamicInfo {
    DWORD baseAddr{};
    DWORD processId{};
    HANDLE gameHandle{};
};
struct gameInfo {
    std::array<DWORD, 32> entityAddrList{};
    DWORD entityCount{};
    std::array<botent, 32> entityList{};
};

inline dynamicInfo di{};
inline gameInfo gi{};
void acMemoryReading();

#endif