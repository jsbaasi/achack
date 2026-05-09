#include <algorithm>

#include "common.h"
#include <cmath>
#include <thread>
#include <chrono>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"
#include "pointer.h"

using MagYawPitch = std::pair<float, yaw_pitch>;

void cuboid::init(ent& entity) {
    float& x{entity.camera_x}, y{entity.camera_y}, z{entity.camera_z};

    glm::mat4 rot(1.0f);
    rot = glm::translate(rot,{entity.camera_x, entity.camera_y, entity.camera_z});
    rot = glm::rotate(rot, glm::radians(entity.yaw), glm::vec3(0.0f,0.0f,1.0f));
    rot = glm::translate(rot, {-entity.camera_x, -entity.camera_y, -entity.camera_z});

    float left   = x + constants::gap_between_right;
    float right  = x - constants::gap_between_right;
    float front  = y - constants::gap_between_front;
    float back   = y + constants::gap_between_front;
    float top    = z + constants::gap_between_top;
    float bottom = z - constants::gap_between_bottom;

    points_3d[TOP_BACK_LEFT]   = {left,  back,  top,    1};
    points_3d[TOP_BACK_RIGHT]  = {right, back,  top,    1};
    points_3d[TOP_FRONT_LEFT]  = {left,  front, top,    1};
    points_3d[TOP_FRONT_RIGHT] = {right, front, top,    1};
    points_3d[BOT_BACK_LEFT]   = {left,  back,  bottom, 1};
    points_3d[BOT_BACK_RIGHT]  = {right, back,  bottom, 1};
    points_3d[BOT_FRONT_LEFT]  = {left,  front, bottom, 1};
    points_3d[BOT_FRONT_RIGHT] = {right, front, bottom, 1};

    for (auto& point : points_3d) {
        point = rot * point;
    }

    is_dead = entity.is_dead;
}

void cuboid::draw(ImDrawList* dl, const ImU32 colour_prmry, const ImU32 colour_scnd, const float thickness) const {
    if (is_dead) return;
    auto line = [&](const Corner a, const Corner b, const ImU32 col) {
        if (points_2d[a] && points_2d[b]) {
            dl->AddLine(points_2d[a].value(), points_2d[b].value(), col, thickness);
        }
    };
    line(TOP_BACK_LEFT,   TOP_BACK_RIGHT, colour_prmry);
    line(TOP_BACK_LEFT,   TOP_FRONT_LEFT, colour_prmry);
    line(TOP_BACK_LEFT,   BOT_BACK_LEFT, colour_prmry);

    line(TOP_FRONT_RIGHT, TOP_BACK_RIGHT, colour_prmry);
    line(TOP_FRONT_RIGHT, TOP_FRONT_LEFT, colour_scnd);
    line(TOP_FRONT_RIGHT, BOT_FRONT_RIGHT, colour_scnd);

    line(BOT_FRONT_LEFT,  BOT_FRONT_RIGHT, colour_scnd);
    line(BOT_FRONT_LEFT,  TOP_FRONT_LEFT, colour_scnd);
    line(BOT_FRONT_LEFT,  BOT_BACK_LEFT, colour_prmry);

    line(BOT_BACK_RIGHT,  BOT_BACK_LEFT, colour_prmry);
    line(BOT_BACK_RIGHT,  BOT_FRONT_RIGHT, colour_prmry);
    line(BOT_BACK_RIGHT, TOP_BACK_RIGHT, colour_prmry);
}

void cuboid::world_to_screen(const glm::mat4& view, const glm::mat4& proj, const ImVec2& window_size, const ImVec2 &default_position) {
    for (int i{}; i<points_3d.size(); ++i) {
        points_2d[i] = mvp_transform_to_vec2(points_3d[i], view, proj, window_size, default_position);
    }
}

void pack_view_matrix(glm::mat4 &view, game_data &_gd) {
    const glm::vec3 camera_position{_gd._player_ent.camerax, _gd._player_ent.cameray, _gd._player_ent.cameraz};
    const float &roll{_gd._player_ent.roll}, pitch{_gd._player_ent.pitch}, yaw{_gd._player_ent.yaw};

    view = glm::mat4(1.0f);

    // view = glm::rotate(view, glm::radians(roll),  glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::rotate(view, glm::radians(pitch), glm::vec3(-1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

    // glRotatef(-90, 1, 0, 0)  -- world Z-up to view Y-up
    view = glm::rotate(view, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // glScalef(1, -1, 1)
    view = glm::scale(view, glm::vec3(1.0f, -1.0f, 1.0f));

    // glTranslatef(-x, -y, -z)
    view = glm::translate(view, -camera_position);
}

void pack_proj_matrix(const projection_matrix &pm, glm::mat4 &proj) {
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

bool is_clipped(glm::vec4 &pos) {
    return (
        pos.x > pos.w or pos.x < -pos.w or
        pos.y > pos.w or pos.y < -pos.w or
        pos.z > pos.w or pos.z < -pos.w
    );
}

glm::vec4 dehomogenize(const glm::vec4 &pos) {
    glm::vec4 res{};
    res.x = pos.x / pos.w;
    res.y = pos.y / pos.w;
    res.z = pos.z / pos.w;
    res.w = 1.0f;
    return res;
}

void pack_viewport_matrix(const ImVec2 window_size, const float z_far, const float z_near, glm::mat4 &viewport_mat) {
    // far is 875
    // near is 0.15
    viewport_mat[0].x = window_size.x / 2;
    viewport_mat[0].y = 0;
    viewport_mat[0].z = 0;
    viewport_mat[0].w = 0;

    viewport_mat[1].x = 0;
    viewport_mat[1].y = window_size.y / 2;
    viewport_mat[1].z = 0;
    viewport_mat[1].w = 0;

    viewport_mat[2].x = 0;
    viewport_mat[2].y = 0;
    viewport_mat[2].z = (z_far - z_near) / 2;
    viewport_mat[2].w = 0;

    viewport_mat[3].x = window_size.x / 2;
    viewport_mat[3].y = window_size.y / 2;
    viewport_mat[3].z = (z_far + z_near) / 2;
    viewport_mat[3].w = 1;
}

std::optional<ImVec2> mvp_transform_to_vec2(const glm::vec4 world_pos, const glm::mat4& view, const glm::mat4& proj, const ImVec2& window_size, const ImVec2 &default_position) {
    glm::vec4 res{};
    res = proj * view * world_pos;
    if (is_clipped(res)) {
        return std::nullopt;
    } else {
        res = dehomogenize(res);
        return ImVec2{
            (res.x * 0.5f + 0.5f) * window_size.x,
            (1.0f - (res.y * 0.5f + 0.5f)) * window_size.y
        };
    }
}

glm::vec4 mvp_transform_to_vec4(const glm::vec4 world_pos, const glm::mat4 &view, const glm::mat4 &proj,
                        const ImVec2 &window_size, const ImVec2 &origin) {
    glm::vec4 res{};
    res = proj * view * world_pos;
    if (is_clipped(res)) {
        return {origin.x, window_size.y, 0, 0};
    } else {
        res = dehomogenize(res);
        return {
            (res.x * 0.5f + 0.5f) * window_size.x,
            (1.0f - (res.y * 0.5f + 0.5f)) * window_size.y,
            0,
            0
        };
    }
}

float get_yaw(float x, float y) {
    // opposite goes first then adjacent
    auto at = std::atan2(x, -y);
    return at>=0 ? at : (2*std::numbers::pi) - (at*-1);
}

void esp_data() {
    game_data local_gd{};
    projection_matrix &pm{local_gd._proj_mat};
    glm::mat4 view{}, proj{}, vp{}, viewport{};
    glm::vec4 world_space_pos{}, clip_space_pos{}, ndc_pos{};
    _global_d.future.wait();
    while (true) {
        {
            std::scoped_lock lock(_gd_mutex);
            local_gd = _gd;
        }
        ImVec2 origin{_gd._window_size.x / 2, _gd._window_size.y / 2};
        pack_view_matrix(view, local_gd);
        pack_proj_matrix(pm, proj);
        pack_viewport_matrix(_gd._window_size, constants::z_far, constants::z_near, viewport);
        vp = proj * view;

        for (int i{0}; i < local_gd._entity_count-1; ++i) {
            _ed._entity_cuboids[i].world_to_screen(view, proj, local_gd._window_size, origin);
        }

        {
            std::scoped_lock lock(_gd_mutex);
            _gd._screen_entity_vec2 = local_gd._screen_entity_vec2;
        }
        if (!_gd._toggles._aimbot) continue;
        std::vector<MagYawPitch> magnitude_euler_pairs{};
        for (int i{1}; i < local_gd._entity_count; ++i) {
            /*
             * loop through all of the entities, multiply each of their positions by view matrix, get their magnitude. map magnitude to (yaw, pitch) to aim there
             * sort magnitudes, look up for yaw, pitch and set my yaw and pitch to that.
             */
            auto& curr_ent = local_gd._entity_list[i];
            if (curr_ent.is_dead) continue;
            glm::vec4 my_world_pos = {local_gd._player_ent.camerax, local_gd._player_ent.cameray, local_gd._player_ent.cameraz, 1};
            glm::vec4 target = {curr_ent.camera_x, curr_ent.camera_y, curr_ent.camera_z, 1};
            glm::vec4 look_at = target - my_world_pos;
            auto distance_away = glm::length(look_at);
            look_at = glm::normalize(look_at);
            auto magnitude = glm::length(look_at);
            float yaw = glm::degrees(get_yaw(look_at.x, look_at.y));
            float pitch = glm::degrees(asin(look_at.z));
            magnitude_euler_pairs.push_back({distance_away, {yaw, pitch}});
        }
        std::sort(magnitude_euler_pairs.begin(), magnitude_euler_pairs.end(), [](MagYawPitch a, MagYawPitch b) {
            return a.first < b.first;
        });
        auto& closest = magnitude_euler_pairs[0];
        WPM(pointer(_da._player_addr).add(0x34), closest.second);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}