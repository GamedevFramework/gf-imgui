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
#include <gf/Color.h>
#include <gf/Cursor.h>
#include <gf/Keyboard.h>
#include <gf/Texture.h>
#include <gf/Unused.h>
#include <gf/Vertex.h>


namespace {

  const char *GetClipboardText(void *user_data) {
    gf::unused(user_data);
    static std::string g_clipboard;
    g_clipboard = gf::Clipboard::getString();
    return g_clipboard.c_str();
  }

  void SetClipboardText(void *user_data, const char *text) {
    gf::unused(user_data);
    gf::Clipboard::setString(text);
  }

  void UpdateKey(ImGuiIO& io, gf::Keycode keycode, gf::Modifiers modifiers, bool pressed) {
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
      default:
        // nothing to do
        break;
    }

    io.KeyCtrl = modifiers.test(gf::Mod::Control);
    io.KeyShift = modifiers.test(gf::Mod::Shift);
    io.KeyAlt = modifiers.test(gf::Mod::Alt);
    io.KeySuper = modifiers.test(gf::Mod::Super);
  }

  void UpdateMouseButton(ImGuiIO& io, gf::MouseButton button, bool pressed) {
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
      default:
        // nothing to do
        break;
    }
  }


  void UpdateMouseCursor(ImGuiIO& io, gf::Window& window) {
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
      return;
    }

    ImGuiMouseCursor cursor = ImGui::GetMouseCursor();

    if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
      window.setMouseCursorVisible(false);
    } else {
      static ImGuiMouseCursor g_previous = ImGuiMouseCursor_None;
      static gf::Cursor g_cursor;

      if (cursor != g_previous) {
        switch (cursor) {
          case ImGuiMouseCursor_Arrow:
            g_cursor = gf::Cursor(gf::Cursor::Arrow);
            break;
          case ImGuiMouseCursor_TextInput:
            g_cursor = gf::Cursor(gf::Cursor::Text);
            break;
          case ImGuiMouseCursor_ResizeAll:
            g_cursor = gf::Cursor(gf::Cursor::SizeAll);
            break;
          case ImGuiMouseCursor_ResizeNS:
            g_cursor = gf::Cursor(gf::Cursor::SizeVertical);
            break;
          case ImGuiMouseCursor_ResizeEW:
            g_cursor = gf::Cursor(gf::Cursor::SizeHorizontal);
            break;
          case ImGuiMouseCursor_ResizeNESW:
            g_cursor = gf::Cursor(gf::Cursor::SizeBottomLeftTopRight);
            break;
          case ImGuiMouseCursor_ResizeNWSE:
            g_cursor = gf::Cursor(gf::Cursor::SizeTopLeftBottomRight);
            break;
          case ImGuiMouseCursor_Hand:
            g_cursor = gf::Cursor(gf::Cursor::Hand);
            break;
          case ImGuiMouseCursor_NotAllowed:
            g_cursor = gf::Cursor(gf::Cursor::NotAllowed);
            break;
          default:
            g_cursor = gf::Cursor(gf::Cursor::Arrow);
            break;
        }

        window.setMouseCursorVisible(true);
        window.setMouseCursor(g_cursor);

        g_previous = cursor;
      }
    }
  }

}


void ImGui_ImplGF_Init(gf::Window& window, gf::RenderTarget& renderer) {
  ImGuiIO& io = ImGui::GetIO();

//       io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
//       io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  io.BackendPlatformName = "imgui_impl_gf";

  for (int i = 0; i < ImGuiKey_COUNT; ++i) {
    io.KeyMap[i] = i;
  }

  io.SetClipboardTextFn = SetClipboardText;
  io.GetClipboardTextFn = GetClipboardText;
  io.ClipboardUserData = nullptr;

  io.BackendPlatformUserData = static_cast<void*>(&window);

  io.BackendRendererName = "imgui_impl_gf";

  gf::Vector2i size;
  unsigned char *pixels = nullptr;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &size.width, &size.height);
  std::unique_ptr<gf::BareTexture> texture = std::make_unique<gf::Texture>(size);
  texture->update(pixels);
  io.Fonts->TexID = static_cast<void*>(texture.release());

  io.BackendRendererUserData = static_cast<void*>(&renderer);
}

bool ImGui_ImplGF_ProcessEvent(const gf::Event& event) {
  ImGuiIO& io = ImGui::GetIO();


  switch (event.type) {
    case gf::EventType::KeyPressed:
      UpdateKey(io, event.key.keycode, event.key.modifiers, true);
      return io.WantCaptureKeyboard;

    case gf::EventType::KeyReleased:
      UpdateKey(io, event.key.keycode, event.key.modifiers, false);
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
      UpdateMouseButton(io, event.mouseButton.button, true);
      return io.WantCaptureMouse;

    case gf::EventType::MouseButtonReleased:
      UpdateMouseButton(io, event.mouseButton.button, false);
      return io.WantCaptureMouse;

    case gf::EventType::MouseMoved: {
      gf::Vector2f pos = event.mouseCursor.coords;
      io.MousePos = ImVec2(pos.x, pos.y);
      return io.WantCaptureMouse;
    }

    case gf::EventType::TextEntered:
      io.AddInputCharactersUTF8(event.text.rune.data);
      return io.WantCaptureKeyboard;

    default:
      // nothing to do
      break;
  }

  return false;
}

void ImGui_ImplGF_Update(gf::Time time) {
  ImGuiIO& io = ImGui::GetIO();
  auto window = static_cast<gf::Window*>(io.BackendPlatformUserData);

  gf::Vector2f size = window->getSize();
  io.DisplaySize = ImVec2(size.x, size.y);

  if (size.x > 0 && size.y > 0) {
    gf::Vector2f scale = window->getFramebufferSize() / size;
    io.DisplayFramebufferScale = ImVec2(scale.x, scale.y);
  }

  io.DeltaTime = time.asSeconds();

  UpdateMouseCursor(io, *window);
}

void ImGui_ImplGF_RenderDrawData(ImDrawData *data) {
  ImGuiIO& io = ImGui::GetIO();
  auto target = static_cast<gf::RenderTarget*>(io.BackendRendererUserData);

  for (int i = 0; i < data->CmdListsCount; ++i) {
    const ImDrawList *list = data->CmdLists[i];

    // TODO: find something better than this
    std::vector<gf::Vertex> vertices;

    for (int k = 0; k < list->VtxBuffer.Size; ++k) {
      const ImDrawVert& vertex = list->VtxBuffer.Data[k];
      gf::Vector2f position = { vertex.pos.x, vertex.pos.y };
      gf::Color4f color = gf::Color::fromRgba32(vertex.col >> IM_COL32_R_SHIFT, vertex.col >> IM_COL32_G_SHIFT, vertex.col >> IM_COL32_B_SHIFT, vertex.col >> IM_COL32_A_SHIFT);
      gf::Vector2f texCoords = { vertex.uv.x, vertex.uv.y };
      vertices.push_back({ position, color, texCoords });
    }

    const ImDrawIdx *indices = list->IdxBuffer.Data;

    for (int j = 0; j < list->CmdBuffer.Size; ++j) {
      const ImDrawCmd *command = &list->CmdBuffer[j];

      if (command->UserCallback) {
        command->UserCallback(list, command);
      } else {
        auto texture = static_cast<const gf::BareTexture *>(command->TextureId);
        // set scissor
        ImVec2 position = data->DisplayPos;
        gf::Vector2i min(command->ClipRect.x - position.x, command->ClipRect.y - position.y);
        gf::Vector2i max(command->ClipRect.z - position.x, command->ClipRect.w - position.y);
        gf::RectI scissor = gf::RectI::fromMinMax(min, max);
        target->setScissorBox(scissor);

        gf::RenderStates states;
        states.texture = texture;

        target->draw(vertices.data(), indices, command->ElemCount, gf::PrimitiveType::Triangles, states);
      }

      indices += command->ElemCount;
    }
  }

}

