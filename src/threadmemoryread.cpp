#include "common.h"
#include "pointer.h"
#include <thread>
#include <chrono>

void acMemoryReading() {
	while (true) {
		for (DWORD i{ 1 }; i < gi.entityCount; i++) {
			gi.entityList[i] = RPM<botent>(gi.entityAddrList[i]);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}