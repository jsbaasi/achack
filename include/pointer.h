#ifndef POINTER_H
#define POINTER_H

#include <iostream>
#include <Windows.h>
#include "common.h"

template <typename T> void RPM(const DWORD addr, T& out) {
	ReadProcessMemory(_da.gameHandle, reinterpret_cast<LPCVOID>(addr), reinterpret_cast<LPVOID>(&out), sizeof(T), nullptr);
}

// e.g. WPM<2>(my_addr, std::array<std::uint8_t, 2>{0x01, 0x02});
template <size_t N> void WPM(const DWORD addr, const std::array<std::uint8_t, N>& in) {
	DWORD old_protect;
	VirtualProtectEx(_da.gameHandle, reinterpret_cast<LPVOID>(addr), N, PAGE_EXECUTE_READWRITE, &old_protect);
	if (!WriteProcessMemory(_da.gameHandle, reinterpret_cast<LPVOID>(addr), reinterpret_cast<LPCVOID>(in.data()), N, nullptr)) {
		std::cout << "Write memory failed because of: " << GetLastError() << std::endl;
	}
	VirtualProtectEx(_da.gameHandle, reinterpret_cast<LPVOID>(addr), N, old_protect, &old_protect);
}

template <typename T> void WPM(const DWORD addr, const T& in) {
	DWORD old_protect;
	VirtualProtectEx(_da.gameHandle, reinterpret_cast<LPVOID>(addr), sizeof(T), PAGE_EXECUTE_READWRITE, &old_protect);
	if (!WriteProcessMemory(_da.gameHandle, reinterpret_cast<LPVOID>(addr), reinterpret_cast<LPCVOID>(&in), sizeof(T), nullptr)) {
		std::cout << "Write memory failed because of: " << GetLastError() << std::endl;
	}
	VirtualProtectEx(_da.gameHandle, reinterpret_cast<LPVOID>(addr), sizeof(T), old_protect, &old_protect);
}

class pointer {
public:
	explicit pointer(const DWORD startingAddr) :
		addr{ startingAddr }
	{
	}
	operator DWORD() const;
	pointer& level(DWORD offset = 0);
	pointer& add(DWORD offset = 0);
	pointer& deref();
private:
	DWORD addr{};
};

#endif