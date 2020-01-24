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
#include "imgui_impl_gf.h"

#include <memory>

#include <imgui.h>

#include <gf/Clipboard.h>
#include <gf/Keyboard.h>

namespace ImGui {

  namespace gf {

    namespace {

      const char *GetClipboardText(void *user_data) {
        gf::unused(user_data);
        static std::string clipboard;
        clipboard = gf::Clipboard::getString();
        return clipboard.c_str();
      }

      void SetClipboardText(void *user_data, const char *text) {
        gf::unused(user_data);
        gf::Clipboard::setString(text);
      }

      void updateKey(ImGuiIO& io, gf::Keycode keycode, gf::Modifiers modifiers, bool pressed) {
        switch (keycode) {
          case gf::Keycode::Tab:
            io.KeysDown[ImGuiKey_Tab] = pressed;
            break;
          case gf::Keycode::Left:
            io.KeysDown[ImGuiKey_LeftArrow] = pressed;
            break;
          case gf::Keycode::Right:
            io.KeysDown[ImGuiKey_RightArrow] = pressed;
            break;
          case gf::Keycode::Up:
            io.KeysDown[ImGuiKey_UpArrow] = pressed;
            break;
          case gf::Keycode::Down:
            io.KeysDown[ImGuiKey_DownArrow] = pressed;
            break;
          case gf::Keycode::PageUp:
            io.KeysDown[ImGuiKey_PageUp] = pressed;
            break;
          case gf::Keycode::PageDown:
            io.KeysDown[ImGuiKey_PageDown] = pressed;
            break;
          case gf::Keycode::Home:
            io.KeysDown[ImGuiKey_Home] = pressed;
            break;
          case gf::Keycode::End:
            io.KeysDown[ImGuiKey_End] = pressed;
            break;
          case gf::Keycode::Insert:
            io.KeysDown[ImGuiKey_Insert] = pressed;
            break;
          case gf::Keycode::Delete:
            io.KeysDown[ImGuiKey_Delete] = pressed;
            break;
          case gf::Keycode::Backspace:
            io.KeysDown[ImGuiKey_Backspace] = pressed;
            break;
          case gf::Keycode::Space:
            io.KeysDown[ImGuiKey_Space] = pressed;
            break;
          case gf::Keycode::Return:
            io.KeysDown[ImGuiKey_Enter] = pressed;
            break;
          case gf::Keycode::Escape:
            io.KeysDown[ImGuiKey_Escape] = pressed;
            break;
          case gf::Keycode::NumpadEnter:
            io.KeysDown[ImGuiKey_KeyPadEnter] = pressed;
            break;
          case gf::Keycode::A:
            io.KeysDown[ImGuiKey_A] = pressed;
            break;
          case gf::Keycode::C:
            io.KeysDown[ImGuiKey_C] = pressed;
            break;
          case gf::Keycode::V:
            io.KeysDown[ImGuiKey_V] = pressed;
            break;
          case gf::Keycode::X:
            io.KeysDown[ImGuiKey_X] = pressed;
            break;
          case gf::Keycode::Y:
            io.KeysDown[ImGuiKey_Y] = pressed;
            break;
          case gf::Keycode::Z:
            io.KeysDown[ImGuiKey_Z] = pressed;
            break;
        }

        io.KeyCtrl = modifiers.test(gf::Mod::Control);
        io.KeyShift = modifiers.test(gf::Mod::Shift);
        io.KeyAlt = modifiers.test(gf::Mod::Alt);
        io.KeySuper = modifiers.test(gf::Mod::Super);
      }

      void updateMouseButton(ImGuiIO& io, gf::MouseButton button, bool pressed) {
        switch (button) {
          case gf::MouseButton::Left:
            io.MouseDown[0] = pressed;
            break;
          case gf::MouseButton::Middle:
            io.MouseDown[2] = pressed;
            break;
          case gf::MouseButton::Right:
            io.MouseDown[1] = pressed;
            break;
        }
      }

    }


    void Init(gf::Window& window) {
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();

//       io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
//       io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
//       io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

      io.BackendPlatformName = "imgui_impl_gf";

      for (int i = 0; i < ImGuiKey_COUNT; ++i) {
        io.KeyMap[i] = i;
      }

      io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
      io.GetClipboardTextFn = SetClipboardText;
      io.ClipboardUserData = nullptr;

      io.BackendPlatformUserData = static_cast<void*>(&window);

      io.BackendRendererName = "imgui_impl_gf";

      gf::Vector2i size;
      unsigned char *pixels = nullptr;
      io.Fonts->GetTexDataAsRGBA32(&pixels, &size.width, &size.height);
      auto texture = std::make_unique<gf::Texture>(size);
      texture->update(pixels);
      io.Fonts->TexID = static_cast<void*>(texture.release());
    }

    bool ProcessEvent(const Event& event) {
      ImGuiIO& io = ImGui::GetIO();


      switch (event.type) {
        case gf::EventType::KeyPressed:
          updateKey(io, event.key.keycode, event.key.modifiers, true);
          return io.WantCaptureKeyboard;

        case gf::EventType::KeyReleased:
          updateKey(event.key.keycode, event.key.modifiers, false);
          return io.WantCaptureKeyboard;

        case gf::EventType::MouseWheelScrolled: {
          gf::Vector2i offset = event.mouseWheel.offset;

          if (offset.x > 0) { io.MouseWheelH += 1; }
          if (offset.x < 0) { io.MouseWheelH -= 1; }
          if (offset.y > 0) { io.MouseWheel += 1; }
          if (offset.y < 0) { io.MouseWheel -= 1; }

          return io.WantCaptureMouse;
        }

        case gf::EventType::MouseButtonPressed:
          updateMouseButton(io, event.mouseButton.button, true);
          return io.WantCaptureMouse;

        case gf::EventType::MouseButtonReleased:
          updateMouseButton(io, event.mouseButton.button, false);
          return io.WantCaptureMouse;

        case gf::EventType::MouseMoved: {
          gf::Vector2f pos = event.mouseCursor.coords;
          io.MousePos = ImVec2(pos.x, pos.y);
          return io.WantCaptureMouse;
        }

        case gf::EventType::TextEntered:
          io.AddInputCharactersUTF8(event.text.rune.data);
          return io.WantCaptureKeyboard;
      }

      return false;
    }

    void Update(gf::Time time) {
      ImGuiIO& io = ImGui::GetIO();
      auto window = static_cast<gf::Window*>(io.BackendPlatformUserData);

      gf::Vector2f size = window->getSize();
      io.DisplaySize = ImVec2(size.x, size.y);

      if (size.x > 0 && size.y > 0) {
        gf::Vector2f scale = window->getFramebufferSize() / size;
        io.DisplayFramebufferScale = ImVec2(scale.x, scale.y);
      }

      io.DeltaTime = time.asSeconds();

      ImGui::NewFrame();
    }

    void Render(RenderTarget& target) {
      ImGui::EndFrame();
      ImGui::Render();
      auto data = ImGui::GetDrawData();

      // TODO: set viewport

      for (int i = 0; i < data->CmdListsCount; ++i) {
        const ImDrawList *list = data->CmdLists[i];
        const ImDrawVert *vertices = list->VtxBuffer.Data;

        // TODO: find something better than this
        std::vector<gf::Vertex> vertices;

        for (int k = 0; k < list->VtxBuffer.Size; ++k) {
          const ImDrawVert& vertex = list->VtxBuffer.Data[k];
          gf::Vector2f position = { vertex.pos.x, vertex.pos.y };
          gf::Color4f color = gf::Color::fromRgba32(vertex.col);
          gf::Vector2f texCoords = { vertex.uv.x, vertex.uv.y };
          vertices.push_back({ position, color, texCoords });
        }

        const ImDrawIdx *indices = list->IdxBuffer.Data;

        for (int j = 0; j < list->CmdBuffer.Size; ++j) {
          const ImDrawCmd *command = &list->CmdBuffer[j];

          if (command->UserCallback) {
            command->UserCallback(list, command);
          } else {
            auto texture = static_cast<const gf::Texture *>(command->TextureId);
            // set scissor
            ImVec2 position = draw_data->DisplayPos;
            gf::Vector2i min(command->ClipRect.x - position.x, command->ClipRect.y - position.y);
            gf::Vector2i max(command->ClipRect.z - position.x, command->ClipRect.w - position.y);
            gf::RectI scissor = gf::RectI::fromMinMax(min, max);
            target.setScissorBox(scissor);

            target.draw(vertices.data(), indices, command->ElemCount, gf::PrimitiveType::Triangle);
          }

          indices += pcmd->ElemCount;
        }
      }

    }

    void Shutdown() {
      ImGui::DestroyContext();
    }

  }

}
