/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Ann(@alirin222)
 */

#include "pico/stdlib.h"
#include <MPGS.h>
#include "BoardConfig.h"
#include "gamepad.h"
#include "storage.h"
#include "turbo.h"

#define TURBO_DEBOUNCE_MILLIS 5

void TURBOModule::setup(Gamepad *pcGamepad)
{
    // Setup TURBO Key
    gpio_init(PIN_BUTTON_TURBO);             // Initialize pin
    gpio_set_dir(PIN_BUTTON_TURBO, GPIO_IN); // Set as INPUT
    gpio_pull_up(PIN_BUTTON_TURBO);          // Set as PULLUP
    uDebTime = getMillis();
    bDebState = false;

    TurboOptions options = getTurboOptions();
    if ((options.shotCount < TURBO_SHOT_MIN) || (options.shotCount > TURBO_SHOT_MAX)) {
        state.uShotCount = DEFAULT_SHOT_PER_SEC;
        pcGamepad->uShotCount = DEFAULT_SHOT_PER_SEC;
    } else {
        state.uShotCount = options.shotCount;
        pcGamepad->uShotCount = options.shotCount;
    }
    pcGamepad->bTurboState = false;

    // for Debug
    gpio_init(TURBO_LED_PIN);
    gpio_set_dir(TURBO_LED_PIN, GPIO_OUT);
    gpio_put(TURBO_LED_PIN, 0);
}

bool TURBOModule::read()
{
    // Get TURBO Key State
    return(!gpio_get(PIN_BUTTON_TURBO));
}

void TURBOModule::debounce()
{
    uint32_t uNowTime = getMillis();

    if ((bDebState != state.bTurboState) && ((uNowTime - uDebTime) > TURBO_DEBOUNCE_MILLIS)) {
        bDebState ^= true;
        uDebTime = uNowTime;
   }

    state.bTurboState = bDebState;
}

void TURBOModule::process(Gamepad *pcGamepad)
{
    static uint16_t lastState = 0;
    static uint8_t lastDpad = 0; 
    uint16_t nowState = 0;
    uint8_t nowDpad = 0;
    TurboOptions options;

    // Get TURBO Button State
    state.bTurboState = read();
#if TURBO_DEBOUNCE_MILLIS > 0
   debounce();
#endif

    nowState = pcGamepad->state.buttons & TURBO_BUTTON_MASK;
    nowDpad = pcGamepad->state.dpad & GAMEPAD_MASK_DPAD;
 
    // Set TURBO Enable Buttons 
    if (state.bTurboState) {
        if (nowState && (lastState != nowState)) {
            // Set TURBO Button enable state
            pcGamepad->turboState.buttons ^= nowState;
            // Set Enable Button on -> off
            pcGamepad->state.buttons &= ~(TURBO_BUTTON_MASK);

        } else if (nowDpad && (lastDpad != nowDpad)) {
            // Continuous Shot interval adjustment
            switch (nowDpad)
            {
                case GAMEPAD_MASK_DOWN:
                    if (TURBO_SHOT_MIN < state.uShotCount) {
                        state.uShotCount--;
                        state.uIntervalMS = 1000 / state.uShotCount;
                        options.shotCount = state.uShotCount;
                        setTurboOptions(options);
                        GamepadStore.save();
                    }
                    pcGamepad->state.dpad = 0;
                    break;

                case GAMEPAD_MASK_UP:
                    if (TURBO_SHOT_MAX > state.uShotCount) {
                        state.uShotCount++;
                        state.uIntervalMS = 1000 / state.uShotCount;
                        options.shotCount = state.uShotCount;
                        setTurboOptions(options);
                        GamepadStore.save();
                    }
                    pcGamepad->state.dpad = 0;
                    break;
            }
            pcGamepad->uShotCount = state.uShotCount;
        }
//     	gpio_put(TURBO_LED_PIN, 1);     // for Debug
    } else {
//    	gpio_put(TURBO_LED_PIN, 0);     // for Debug
    }
    // Save previous state
    lastState = nowState;
    lastDpad = nowDpad;
    pcGamepad->bTurboState = state.bTurboState;
}

void TURBOModule::loop(Gamepad *pcGamepad)
{
    static uint32_t start_ms = 0;
    static const uint32_t intervalTB = 3;

 	if ((getMillis() - start_ms) < state.uIntervalMS) {
        if (state.bTurboOff) {
            // TURBO Enable Button State Off
            pcGamepad->state.buttons &= ~(pcGamepad->turboState.buttons);
        }
  		return;
    }

    // Button ON/OFF State Revers
    state.bTurboOff ^= true;
 
    if (state.bTurboOff) {
        // TURBO Enable Button State Off
        pcGamepad->state.buttons &= ~(pcGamepad->turboState.buttons);
        start_ms += (1000 / state.uShotCount - 2);
    } else {
        start_ms += intervalTB;
    }

    // Set TURBO LED
    if (pcGamepad->state.buttons & pcGamepad->turboState.buttons) {
     	gpio_put(TURBO_LED_PIN, 1);
    } else {
    	gpio_put(TURBO_LED_PIN, 0);
    }

    return;
}