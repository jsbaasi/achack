![Demo](https://github.com/jsbaasi/achack/blob/main/gif.gif)
- found entity list
- reversed the entity structure
- i've found the projection matrix
- i think i found the camera rotation matrix but the translation bit isn't there to make a whole view matrix.
- i_hat = right
j_hat = forward
k_hat = up

~~game view matrix i think
0.991 0.063 -0.119 27
0.119 0.008 0.993 95
-0.064 0.998 0 12
0 0 0 1~~

mine
-0.119 0.993 0 -9.068
0.063 0.008 0.998 -97.411
-0.991 -0.119 0.064 23.101
0 0 0 1

```

DebugMatrix("view", _td.view);
DebugMatrix("proj", _td.proj);
ImGui::Text("viewport size x=%.2f, y=%.2f", _gd._window_size.x, _gd._window_size.y);
ImGui::Text("test world pos: (x=%.2f, y=%.2f, z=%.2f, w=%.2f)",IMVEC4(_td.world_pos));
ImGui::Text("test view pos: (x=%.2f, y=%.2f, z=%.2f, w=%.2f)",IMVEC4(_td.view_pos));
ImGui::Text("test clip pos: (x=%.2f, y=%.2f, z=%.2f, w=%.2f)",IMVEC4(_td.clip_pos));
ImGui::Text("test ndc pos: (x=%.2f, y=%.2f, z=%.2f, w=%.2f)",IMVEC4(_td.ndc_pos));
ImGui::Text("test screen pos: (x=%.2f, y=%.2f, z=%.2f, w=%.2f)",IMVEC4(_td.screen_pos));
ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
draw_list->AddLine({460, 587}, {_td.screen_pos.x, _td.screen_pos.y}, IM_COL32(255, 120, 0, 255), 1);

auto res = mvp_transform_to_vec4({
     local_gd._entity_list[i].x,
     local_gd._entity_list[i].y,
     local_gd._entity_list[i].z,
     1
 }, view, proj, local_gd._window_size, origin);
local_gd._screen_entity_vec2[i] = {res.x, res.y};

_td.world_pos = {local_gd._player_ent.camerax+5, local_gd._player_ent.cameray, local_gd._player_ent.cameraz, 1.0f};
_td.world_pos = {
    local_gd._entity_list[1].x,
    local_gd._entity_list[1].y,
    local_gd._entity_list[1].z, 1.0f
};
_td.view_pos = view * _td.world_pos;
_td.clip_pos = proj * _td.view_pos;
if (is_clipped(_td.clip_pos)) {
    _td.ndc_pos.x = -1111111;
    _td.ndc_pos.y = -1111111;
    _td.screen_pos = {origin.x, origin.y, 0, 0};
} else {
    _td.ndc_pos = dehomogenize(_td.clip_pos);
    _td.screen_pos = {(_td.ndc_pos.x * 0.5f + 0.5f) * local_gd._window_size.x, (1.0f - (_td.ndc_pos.y * 0.5f + 0.5f)) * local_gd._window_size.y, 0, 0};
}
_td.screen_pos = mvp_transform_to_vec4(_td.world_pos, view, proj, local_gd._window_size, origin);

_td.view = view;
_td.proj = proj;
```
