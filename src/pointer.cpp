#include <Windows.h>
#include "pointer.h"
#include "common.h"

pointer::operator DWORD() const {
	return addr;
}

pointer& pointer::level(DWORD offset) { // offsets first, and then follows the resulting address, updates addr variable
	addr += offset;
	RPM<DWORD>(addr, addr);
	return *this;
}

pointer& pointer::add(DWORD offset) { // offsets first, and then follows the resulting address, updates addr variable
	addr += offset;
	return *this;
}

pointer& pointer::deref() {
	RPM<DWORD>(addr, addr);
	return *this;
}
