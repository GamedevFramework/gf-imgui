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
#include <gf/Vertex.h>


namespace {

  const char *GetClipboardText([[maybe_unused]] void *user_data) {
    static std::string g_clipboard;
    g_clipboard = gf::Clipboard::getString();
    return g_clipboard.c_str();
  }

  void SetClipboardText([[maybe_unused]] void *user_data, const char *text) {
    gf::Clipboard::setString(text);
  }

  void UpdateKey(ImGuiIO& io, gf::Keycode keycode, gf::Flags<gf::Mod> modifiers, bool pressed) {
    switch (keycode) {
      case gf::Keycode::Tab:
        io.AddKeyEvent(ImGuiKey_Tab, pressed);
        break;
      case gf::Keycode::Left:
        io.AddKeyEvent(ImGuiKey_LeftArrow, pressed);
        break;
      case gf::Keycode::Right:
        io.AddKeyEvent(ImGuiKey_RightArrow, pressed);
        break;
      case gf::Keycode::Up:
        io.AddKeyEvent(ImGuiKey_UpArrow, pressed);
        break;
      case gf::Keycode::Down:
        io.AddKeyEvent(ImGuiKey_DownArrow, pressed);
        break;
      case gf::Keycode::PageUp:
        io.AddKeyEvent(ImGuiKey_PageUp, pressed);
        break;
      case gf::Keycode::PageDown:
        io.AddKeyEvent(ImGuiKey_PageDown, pressed);
        break;
      case gf::Keycode::Home:
        io.AddKeyEvent(ImGuiKey_Home, pressed);
        break;
      case gf::Keycode::End:
        io.AddKeyEvent(ImGuiKey_End, pressed);
        break;
      case gf::Keycode::Insert:
        io.AddKeyEvent(ImGuiKey_Insert, pressed);
        break;
      case gf::Keycode::Delete:
        io.AddKeyEvent(ImGuiKey_Delete, pressed);
        break;
      case gf::Keycode::Backspace:
        io.AddKeyEvent(ImGuiKey_Backspace, pressed);
        break;
      case gf::Keycode::Space:
        io.AddKeyEvent(ImGuiKey_Space, pressed);
        break;
      case gf::Keycode::Return:
        io.AddKeyEvent(ImGuiKey_Enter, pressed);
        break;
      case gf::Keycode::Escape:
        io.AddKeyEvent(ImGuiKey_Escape, pressed);
        break;
      case gf::Keycode::NumpadEnter:
        io.AddKeyEvent(ImGuiKey_KeypadEnter, pressed);
        break;
      case gf::Keycode::A:
        io.AddKeyEvent(ImGuiKey_A, pressed);
        break;
      case gf::Keycode::C:
        io.AddKeyEvent(ImGuiKey_C, pressed);
        break;
      case gf::Keycode::V:
        io.AddKeyEvent(ImGuiKey_V, pressed);
        break;
      case gf::Keycode::X:
        io.AddKeyEvent(ImGuiKey_X, pressed);
        break;
      case gf::Keycode::Y:
        io.AddKeyEvent(ImGuiKey_Y, pressed);
        break;
      case gf::Keycode::Z:
        io.AddKeyEvent(ImGuiKey_Z, pressed);
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
        io.AddMouseButtonEvent(0, pressed);
        break;
      case gf::MouseButton::Middle:
        io.AddMouseButtonEvent(2, pressed);
        break;
      case gf::MouseButton::Right:
        io.AddMouseButtonEvent(1, pressed);
        break;
      default:
        // nothing to do
        break;
    }
  }

  void UpdateGamepadButton(ImGuiIO& io, gf::GamepadButton button, bool pressed, float value) {
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) {
      return;
    }

    switch (button) {
      case gf::GamepadButton::A:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceDown, pressed, value);
        break;
      case gf::GamepadButton::B:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceRight, pressed, value);
        break;
      case gf::GamepadButton::X:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceLeft, pressed, value);
        break;
      case gf::GamepadButton::Y:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadFaceUp, pressed, value);
        break;
      case gf::GamepadButton::DPadLeft:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadLeft, pressed, value);
        break;
      case gf::GamepadButton::DPadRight:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadRight, pressed, value);
        break;
      case gf::GamepadButton::DPadUp:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadUp, pressed, value);
        break;
      case gf::GamepadButton::DPadDown:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadDpadDown, pressed, value);
        break;
      case gf::GamepadButton::LeftBumper:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadL1, pressed, value);
        break;
      case gf::GamepadButton::RightBumper:
        io.AddKeyAnalogEvent(ImGuiKey_GamepadR1, pressed, value);
        break;
      default:
        // nothing to do
        break;
    }

  }

  void UpdateGamepadAxis(ImGuiIO& io, gf::GamepadAxis axis, int16_t raw) {
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) {
      return;
    }

    static constexpr int16_t Threshold = 8000;

    switch (axis) {
      case gf::GamepadAxis::LeftX:
        if (raw > Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, true, 1.0f);
        } else if (raw < -Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft, true, 1.0f);
        } else {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickRight, false, 0.0f);
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickLeft, false, 0.0f);
        }
        break;
      case gf::GamepadAxis::LeftY:
        if (raw > Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown, true, 1.0f);
        } else if (raw < -Threshold) {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp, true, 1.0f);
        } else {
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickDown, false, 0.0f);
          io.AddKeyAnalogEvent(ImGuiKey_GamepadLStickUp, false, 0.0f);
        }
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

  io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
//       io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

  io.FontGlobalScale = 2.0f;

  io.BackendPlatformName = "imgui_impl_gf";

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
      io.AddMousePosEvent(pos.x, pos.y);
      return io.WantCaptureMouse;
    }

    case gf::EventType::GamepadButtonPressed:
      UpdateGamepadButton(io, event.gamepadButton.button, true, 1.0f);
      break;

    case gf::EventType::GamepadButtonReleased:
      UpdateGamepadButton(io, event.gamepadButton.button, false, 0.0f);
      break;

    case gf::EventType::GamepadAxisMoved:
      UpdateGamepadAxis(io, event.gamepadAxis.axis, event.gamepadAxis.value);
      break;

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

  static constexpr gf::RenderAttributeInfo CustomAttributes[] = {
    { "a_position",   2,  gf::RenderAttributeType::Float, false,  offsetof(ImDrawVert, pos) },
    { "a_texCoords",  2,  gf::RenderAttributeType::Float, false,  offsetof(ImDrawVert, uv)  },
    { "a_color",      4,  gf::RenderAttributeType::UByte, true,   offsetof(ImDrawVert, col) },
  };

  ImGuiIO& io = ImGui::GetIO();
  auto target = static_cast<gf::RenderTarget*>(io.BackendRendererUserData);

  auto originalScissor = target->getCanonicalScissorBox();

  for (int i = 0; i < data->CmdListsCount; ++i) {
    const ImDrawList *list = data->CmdLists[i];
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
        states.texture[0] = texture;

        target->customDraw(list->VtxBuffer.Data, sizeof(ImDrawVert), indices, command->ElemCount, gf::PrimitiveType::Triangles, CustomAttributes, states);
      }

      indices += command->ElemCount;
    }
  }

  target->setCanonicalScissorBox(originalScissor);
}

void ImGui_ImplGF_Shutdown() {
  ImGuiIO& io = ImGui::GetIO();
  auto texture = static_cast<gf::Texture *>(io.Fonts->TexID);
  delete texture;
  io.Fonts->TexID = nullptr;
}
