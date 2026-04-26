#include "common.h"
#include <thread>
#include <chrono>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"

void pack_view_matrix(glm::mat4& view, game_data& _gd) {
    const glm::vec3 camera_position{_gd._player_ent.camerax, _gd._player_ent.cameray, _gd._player_ent.cameraz};
    const float& roll{_gd._player_ent.roll}, pitch{_gd._player_ent.pitch}, yaw{_gd._player_ent.yaw};

    view = glm::mat4(1.0f);

    // view = glm::rotate(view, glm::radians(roll),  glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::rotate(view, glm::radians(pitch), glm::vec3(-1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(yaw),   glm::vec3(0.0f, 1.0f, 0.0f));

    // glRotatef(-90, 1, 0, 0)  -- world Z-up to view Y-up
    view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // glScalef(1, -1, 1)
    view = glm::scale(view, glm::vec3(1.0f, -1.0f, 1.0f));

    // glTranslatef(-x, -y, -z)
    view = glm::translate(view, -camera_position);
}

void pack_proj_matrix(const projection_matrix& pm, glm::mat4& proj) {
    proj[0].x = pm.x0;
    proj[0].y = pm.y0;
    proj[0].z = pm.z0;
    proj[0].w = pm.w0;

    proj[1].x = pm.x1;
    proj[1].y = pm.y1;
    proj[1].z = pm.z1;
    proj[1].w = pm.w1;

    proj[2].x = pm.x2;
    proj[2].y = pm.y2;
    proj[2].z = pm.z2;
    proj[2].w = pm.w2;

    proj[3].x = pm.x3;
    proj[3].y = pm.y3;
    proj[3].z = pm.z3;
    proj[3].w = pm.w3;
}

bool is_clipped(glm::vec4& pos) {
    return (
        pos.x > pos.w or pos.x < -pos.w or
        pos.y > pos.w or pos.y < -pos.w or
        pos.z > pos.w or pos.z < -pos.w
    );
}

void dehomogenize(glm::vec4& pos) {
    pos.x /= pos.w;
    pos.y /= pos.w;
    pos.z /= pos.w;
    pos.w = 1.0f;
}

void pack_viewport_matrix(const ImVec2 window_size, const float z_far, const float z_near, glm::mat4& viewport_mat) {
    // far is 875
    // near is 0.15
    viewport_mat[0].x = window_size.x/2;
    viewport_mat[0].y = 0;
    viewport_mat[0].z = 0;
    viewport_mat[0].w = 0;

    viewport_mat[1].x = 0;
    viewport_mat[1].y = window_size.y/2;
    viewport_mat[1].z = 0;
    viewport_mat[1].w = 0;

    viewport_mat[2].x = 0;
    viewport_mat[2].y = 0;
    viewport_mat[2].z = (z_far-z_near)/2;
    viewport_mat[2].w = 0;

    viewport_mat[3].x = window_size.x/2;
    viewport_mat[3].y = window_size.y/2;
    viewport_mat[3].z = (z_far+z_near)/2;
    viewport_mat[3].w = 1;

    // static Mat4 viewport = {
    //     W/2,0,0,X+W/2,
    //     0,H/2,0,Y+H/2,
    //     0,0,(F-N)/2,(F+N)/2,
    //     0,0,0,1,
    // }
}



void esp_data() {
    game_data local_gd{};
    projection_matrix& pm{local_gd._proj_mat};
    glm::mat4 view{}, proj{}, vp{}, viewport{};
    glm::vec4 world_space_pos{}, clip_space_pos{}, ndc_pos{};

    while (true) {
        {
            std::scoped_lock lock(_gd_mutex);
            local_gd = _gd;
        }
        ImVec2 origin {_gd._window_size.x/2, _gd._window_size.y/2};
        pack_view_matrix(view, local_gd);
        pack_proj_matrix(pm, proj);
        pack_viewport_matrix(_gd._window_size, constants::z_far, constants::z_near, viewport);
        vp = proj * view;
        _td.view_pos = view * _td.world_pos;
        _td.clip_pos = proj * _td.view_pos;
        _td.view = view;
        _td.proj = proj;


        for (int i{}; i<local_gd._entity_count; ++i) {
            world_space_pos.x = local_gd._entity_list[i].x;
            world_space_pos.y = local_gd._entity_list[i].y;
            world_space_pos.z = local_gd._entity_list[i].z;
            world_space_pos.w = 1;
            clip_space_pos = vp * world_space_pos;
            if (is_clipped(clip_space_pos)) {
                local_gd._ndc_entity_vec2[i].x = origin.x;
                local_gd._ndc_entity_vec2[i].y = origin.y;
                continue;
            }
            dehomogenize(clip_space_pos);
            // now clip_space_pos is ndc_pos

            local_gd._ndc_entity_vec2[i].x = (clip_space_pos.x * 0.5f + 0.5f) * _gd._window_size.x;
            local_gd._ndc_entity_vec2[i].y = (1.0f - (clip_space_pos.y * 0.5f + 0.5f)) * _gd._window_size.y;
        }

        {
            std::scoped_lock lock(_gd_mutex);
            _gd._ndc_entity_vec2 = local_gd._ndc_entity_vec2;
        }

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
