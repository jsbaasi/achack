#include "common.h"
#include "pointer.h"
#include <thread>
#include <chrono>
#include <mutex>

void memory_read() {
	game_data local_gd{};
	local_gd._entity_count = _gd._entity_count;
	for (DWORD i{ 1 }; i < local_gd._entity_count; i++) {
		RPM<ent>(_da._entity_addr_list[i], local_gd._entity_list[i]);
		ent& curr_bot = local_gd._entity_list[i];
		_ed._entity_cuboids.push_back(cuboid{curr_bot});
	}
	_global_d.ready.set_value();
	while (true) {
		RPM<player_ent>(_da._player_addr, local_gd._player_ent);
		for (DWORD i{ 1 }; i < local_gd._entity_count; i++) {
			auto& curr_bot = local_gd._entity_list[i];
			RPM<ent>(_da._entity_addr_list[i], curr_bot);
		}
		{
			std::scoped_lock lock(_ed._cuboid_mex);
			for (DWORD i{ 1 }; i < local_gd._entity_count; i++) {
				auto& curr_bot = local_gd._entity_list[i];
				_ed._entity_cuboids[i-1].update(curr_bot);
			}
		}
		RPM<projection_matrix>(_da._projection_mat_addr, local_gd._proj_mat);
		{
			std::scoped_lock lock(_gd_mutex);
			_gd._player_ent = local_gd._player_ent;
			_gd._entity_list = local_gd._entity_list;
			_gd._proj_mat = local_gd._proj_mat;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}