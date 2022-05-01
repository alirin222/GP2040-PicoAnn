/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2022 Ann(@alirin222)
 */

#ifndef TURBO_H_
#define TURBO_H_

#include "pico/stdlib.h"
#include <MPGS.h>
#include "BoardConfig.h"
#include "gamepad.h"


#ifndef DEFAULT_SHOT_PER_SEC
#define DEFAULT_SHOT_PER_SEC 20
#endif  // DEFAULT_SHOT_PER_SEC

// TURBO Button Mask
#define TURBO_BUTTON_MASK (GAMEPAD_MASK_B1 | GAMEPAD_MASK_B2 | GAMEPAD_MASK_B3 | GAMEPAD_MASK_B4 | \
                            GAMEPAD_MASK_L1 | GAMEPAD_MASK_R1 | GAMEPAD_MASK_L2 | GAMEPAD_MASK_R2)

// TURBO LED
#define TURBO_LED_PIN   25

#define TURBO_SHOT_MAX  60
#define TURBO_SHOT_MIN  1

// Struct Turbo Status
struct turboStatus
{
 	uint16_t buttons {0};                               // TURBO Enable Buttons 
    bool bTurboState {false};                           // TURBO Buttons State
    bool bTurboOff {false};                             // TURBO Enable Buttons OFF Flag
    uint32_t uShotCount {DEFAULT_SHOT_PER_SEC};         // Rapid-Fire (Shot/Sec)
    uint32_t uIntervalMS {1000/DEFAULT_SHOT_PER_SEC};   // TURBO Interval
};

class TURBOModule
{
public:
   	turboStatus state;          // TURBO Status
    bool bDebState = false;     // Debounce TURBO Button State
    uint32_t uDebTime = 0;      // Debounce TURBO Button Time

    // TURBO Button Setup
	void setup(Gamepad *pcGamepad);
    // Get TURBO Button State
	bool read();
    // TURBO Button Debouncer
    void debounce();
    // TURBO Setting of buttons (Enable/Disable)
    void process(Gamepad *pcGamepad);
    // Rapid-Fire function
    void loop(Gamepad *pcGamepad);
};
#endif  // TURBO_H_
