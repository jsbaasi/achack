#ifndef POINTER_H
#define POINTER_H

#include <Windows.h>

template <typename T> void RPM(DWORD addr, T& out);

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