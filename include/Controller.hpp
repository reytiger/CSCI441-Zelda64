#pragma once
#include "Utils.hpp"

#include "PrettyGLUT.hpp"

#ifdef USE_XINPUT

#include <XInput.h>

// Writes into 'pressed' for each controller state pressed.
// This effectively translates XInput to ASCII keyboar inputs.
static inline void checkControllerInput(bool pressed[256]) {
    XINPUT_STATE input;
    ZeroMemory(&input, sizeof(input));
    XInputGetState(0, &input);

    if (XInputGetState(0, &input) != ERROR_SUCCESS) {
        // No controller or there was an error connecting.
        // Bail.
        return;
    }

    // The thumb sticks can be nonbinary, but we don't care. Just make
    // sure they're out of their deadzones.

    // Left sick moves around.

    // Forward
    pressed['w'] = input.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    // Backward
    pressed['s'] = input.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

    // Left
    pressed['a'] = input.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    // Right
    pressed['d'] = input.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

    // Up
    pressed['q'] = input.Gamepad.bLeftTrigger != 0;
    // Down
    pressed['e'] = input.Gamepad.bRightTrigger != 0;

    // Trigger Navi
    pressed[' '] = (input.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;

    // Switch Cam
    static bool pressOnCoolDown;
    if (!pressOnCoolDown
        && (input.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0) {
        pressOnCoolDown = true;
        glutTimerFunc(500, [](int) { pressOnCoolDown = false; }, 0);
        // This is declared in PrettyGLUT.cpp. Trust me. I wouldn't lie to you,
        // would I?
        void normalKeysDown(unsigned char key, int, int);
        normalKeysDown('c', 0, 0);
    }

    if ((input.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0) {
        info("Exiting.");
        exit(0);
    }

    // Right stick looks around.
    int dx = -input.Gamepad.sThumbRX;

    // Wrap the deadzones to 0.
    if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < dx && dx < 0) {
        dx = 0;
    } else if (0 < dx && dx < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
        dx = 0;
    }

    int dy = input.Gamepad.sThumbRY;
    if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < dy && dy < 0) {
        dy = 0;
    } else if (0 < dy && dy < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
        dy = 0;
    }

    float fudge;
    if (dynamic_cast<ArcBallCamera *>(activeCam)) {
        fudge = 0.0000025f;
    } else {
        fudge = 0.000002f;
    }

    // Adjust the radius of the active cam. Moves by a constant factor of
    // the idstance of the mouse moved.
    if ((input.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) {
        // Different controls, different fudge factor.
        fudge *= 3.0f;

        Vec dist = Vec(dx, dy);
        info("%s", dist);
        auto radius = activeCam->radius();
        if (dy > 0) {
            radius = radius - fudge * dist.norm();
        } else {
            radius = radius + fudge * dist.norm();
        }
        radius = clamp(radius, 3.0f, 1000.0f);
        activeCam->radius(radius);
        return;
    }

    // Arcball feels more natural with the y inverted and more fudge.
    if (dynamic_cast<ArcBallCamera *>(activeCam)) {
        activeCam->rotate(fudge * dx, -fudge * dy);
    } else {
        activeCam->rotate(fudge * dx, fudge * dy);
    }
}

#else
// On non-Windows platforms, this function should be a no-op.
static inline void checkControllerInput(bool pressed[256]) {}
#endif
