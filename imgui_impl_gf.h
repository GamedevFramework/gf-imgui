/*
 * gf-imgui
 * Copyright (C) 2020 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_IMGUI_H
#define GF_IMGUI_H

#include <gf/Event.h>
#include <gf/RenderTarget.h>
#include <gf/Time.h>
#include <gf/Window.h>

struct ImDrawData;

void ImGui_ImplGF_Init(gf::Window& window, gf::RenderTarget& renderer);

// returns true if the event should not be passed to the game
bool ImGui_ImplGF_ProcessEvent(const gf::Event& event);

void ImGui_ImplGF_Update(gf::Time time);

void ImGui_ImplGF_RenderDrawData(ImDrawData *data);

void ImGui_ImplGF_Shutdown();

#endif // GF_IMGUI_H
