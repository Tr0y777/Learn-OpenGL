#pragma once

#include "Keys.h"

namespace Bunny {

    class Input {
    public:
        static bool isKeyPressed(const KeyCode key_code);
        static void PressKey(const KeyCode key_code);
        static void ReleaseKey(const KeyCode key_code);

        //static bool IsMouseButtonPressed(const MouseButton mouse_button);
        //static void PressMouseButton(const MouseButton mouse_button);
        //static void ReleaseMouseButton(const MouseButton mouse_button);

    private:
        static bool m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)];
    };

}