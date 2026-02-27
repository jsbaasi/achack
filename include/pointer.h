#ifndef POINTER_H
#define POINTER_H

#include <Windows.h>
#include "common.h"

template <typename T> void RPM(DWORD addr, T& out) {
	ReadProcessMemory(di.gameHandle, reinterpret_cast<LPCVOID>(addr), reinterpret_cast<LPVOID>(&out), sizeof(T), NULL);
}

class pointer {
public:
	pointer(DWORD startingAddr) :
		addr{ startingAddr }
	{
	}
	operator DWORD() const;
	pointer& level(DWORD offset = 0);
private:
	DWORD addr{};
};

#endif