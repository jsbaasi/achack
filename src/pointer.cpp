#include <Windows.h>
#include "pointer.h"

template <typename T> void RPM(DWORD addr, T& out) {
	ReadProcessMemory(di.gameHandle, (LPCVOID)addr, (LPVOID)&out, sizeof(T), NULL);
}

pointer::operator DWORD() const {
	return addr;
}

pointer& pointer::level(DWORD offset = 0) { // offsets first, and then follows the resulting address, updates addr variable
	addr += offset;
	RPM<DWORD>(addr, addr);
	return *this;
}
