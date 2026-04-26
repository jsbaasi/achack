#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <vector>
#include <array>
#include <mutex>
#include <Windows.h>
#include "imgui.h"

struct bot_ent
{
    char pad_0000[4]; //0x0000
    float x; //0x0004
    float y; //0x000C
    float z; //0x0008
    char pad_0010[220]; //0x0010
    int32_t health; //0x00EC
    char pad_00F0[277]; //0x00F0
    char name[32]; //0x0205
}; //Size: 0x0225

struct player_ent
{
	char pad_0000[4]; //0x0000
	float camerax; //0x0004
	float cameray; //0x000C
	float cameraz; //0x0008
	float xspeed; //0x0010
	float yspeed; //0x0018
	float zspeed; //0x0014
	float x_velo; //0x001C
	float y_velo; //0x0024
	float z_velo; //0x0020
	float player_x; //0x0028
	float player_y; //0x0030
	float player_z; //0x002C
	float yaw; //0x0034
	float pitch; //0x0038
	float roll; //0x003C
	float recoil_punch; //0x0040
	char pad_0044[4]; //0x0044
	float jump_speed; //0x0048
	char pad_004C[16]; //0x004C
	uint32_t ladder_midair_bitmask; //0x005C
	char pad_0060[4]; //0x0060
	uint32_t player_state_bitmask; //0x0064
	char pad_0068[12]; //0x0068
	uint32_t input_bitmask; //0x0074
	char pad_0078[8]; //0x0078
	uint32_t movement_bitmask; //0x0080
	char pad_0084[104]; //0x0084
	int32_t health; //0x00EC
	int32_t armour; //0x00F0
	char pad_00F4[68]; //0x00F4
	int32_t reserve; //0x0138
	int32_t magazine; //0x013C
	char pad_0140[4]; //0x0140
	int32_t grenade; //0x0144
	char pad_0148[189]; //0x0148
	char name[32]; //0x0205
}; //Size: 0x0225

struct projection_matrix
{
	float x0; //0x0000
	float y0; //0x0004
	float z0; //0x0008
	float w0; //0x000C
	float x1; //0x0010
	float y1; //0x0014
	float z1; //0x0018
	float w1; //0x001C
	float x2; //0x0020
	float y2; //0x0024
	float z2; //0x0028
	float w2; //0x002C
	float x3; //0x0030
	float y3; //0x0034
	float z3; //0x0038
	float w3; //0x003C
}; //Size: 0x0040


struct camera
{
	char pad_0000[16]; //0x0000
	float i_hat_x; //0x0010
	float j_hat_x; //0x0014
	float k_hat_x; //0x0018
	float i_hat_y; //0x001C
	float j_hat_y; //0x0020
	float k_hat_y; //0x0024
	float i_hat_z; //0x0028
	float j_hat_z; //0x002C
	float k_hat_z; //0x0030


	// float i_hat_x; //0x0010
	// float i_hat_y; //0x0014
	// float i_hat_z; //0x0018
	// float j_hat_x; //0x001C
	// float j_hat_y; //0x0020
	// float j_hat_z; //0x0024
	// float k_hat_x; //0x0028
	// float k_hat_y; //0x002C
	// float k_hat_z; //0x0030


	char pad_0034[12]; //0x0034
	float camera_x; //0x0040
	float camera_y; //0x0048
	float camera_z; //0x0044
}; //Size: 0x004C

struct dynamic_addresses {
	DWORD base_addr{};
	DWORD processId{};
	HANDLE gameHandle{};
	std::array<DWORD, 32> _entity_addr_list{};
	DWORD _player_addr{};
	DWORD _camera_addr{};
	DWORD _projection_mat_addr{};
};

struct game_data {
	DWORD _entity_count{};
	std::array<bot_ent, 32> _entity_list{};
	std::array<ImVec2, 32> _screen_entity_vec2{};
	player_ent _player_ent{};
	projection_matrix _proj_mat{};
	ImVec2 _window_size{};
};

struct test_data {
	glm::vec4 world_pos{65,4,120,1};
	glm::vec4 view_pos{};
	glm::vec4 clip_pos{};
	glm::vec4 ndc_pos{};
	glm::vec4 screen_pos{};
	glm::mat4 view{};
	glm::mat4 proj{};
};

inline std::mutex _gd_mutex{};
inline dynamic_addresses _da;
inline game_data _gd;
inline test_data _td;
void memory_read();
void esp_data();

#endif