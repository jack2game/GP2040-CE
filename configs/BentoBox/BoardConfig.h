/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef PICO_BOARD_CONFIG_H_
#define PICO_BOARD_CONFIG_H_

#include "enums.pb.h"

#define BOARD_CONFIG_LABEL "BentoBox"

// Main pin mapping Configuration
//                          // GP2040 | Xinput | Switch  | PS3/4/5  | Dinput | Arcade |
#define PIN_DPAD_UP     2   // UP     | UP     | UP      | UP       | UP     | UP     |
#define PIN_DPAD_DOWN   12  // DOWN   | DOWN   | DOWN    | DOWN     | DOWN   | DOWN   | 
#define PIN_DPAD_RIGHT  11  // RIGHT  | RIGHT  | RIGHT   | RIGHT    | RIGHT  | RIGHT  | 
#define PIN_DPAD_LEFT   13  // LEFT   | LEFT   | LEFT    | LEFT     | LEFT   | LEFT   | 
#define PIN_BUTTON_B1   3   // B1     | A      | B       | Cross    | 2      | K1     |
#define PIN_BUTTON_B2   4   // B2     | B      | A       | Circle   | 3      | K2     |
#define PIN_BUTTON_R2   5   // R2     | RT     | ZR      | R2       | 8      | K3     |
#define PIN_BUTTON_L2   6   // L2     | LT     | ZL      | L2       | 7      | K4     |
#define PIN_BUTTON_B3   10  // B3     | X      | Y       | Square   | 1      | P1     |
#define PIN_BUTTON_B4   9   // B4     | Y      | X       | Triangle | 4      | P2     |
#define PIN_BUTTON_R1   8   // R1     | RB     | R       | R1       | 6      | P3     |
#define PIN_BUTTON_L1   7   // L1     | LB     | L       | L1       | 5      | P4     |
#define PIN_BUTTON_S1   21  // S1     | Back   | Minus   | Select   | 9      | Coin   |
#define PIN_BUTTON_S2   22  // S2     | Start  | Plus    | Start    | 10     | Start  |
#define PIN_BUTTON_L3   18  // L3     | LS     | LS      | L3       | 11     | LS     |
#define PIN_BUTTON_R3   17  // R3     | RS     | RS      | R3       | 12     | RS     |
#define PIN_BUTTON_A1   20  // A1     | Guide  | Home    | PS       | 13     | ~      |
#define PIN_BUTTON_A2   19  // A2     | ~      | Capture | ~        | 14     | ~      |
#define PIN_BUTTON_FN   -1  // Hotkey Function                                        |

// Keyboard Mapping Configuration
//                                            // GP2040 | Xinput | Switch  | PS3/4/5  | Dinput | Arcade |
#define KEY_DPAD_UP     HID_KEY_ARROW_UP      // UP     | UP     | UP      | UP       | UP     | UP     |
#define KEY_DPAD_DOWN   HID_KEY_ARROW_DOWN    // DOWN   | DOWN   | DOWN    | DOWN     | DOWN   | DOWN   | 
#define KEY_DPAD_RIGHT  HID_KEY_ARROW_RIGHT   // RIGHT  | RIGHT  | RIGHT   | RIGHT    | RIGHT  | RIGHT  | 
#define KEY_DPAD_LEFT   HID_KEY_ARROW_LEFT    // LEFT   | LEFT   | LEFT    | LEFT     | LEFT   | LEFT   | 
#define KEY_BUTTON_B1   HID_KEY_SHIFT_LEFT    // B1     | A      | B       | Cross    | 2      | K1     |
#define KEY_BUTTON_B2   HID_KEY_Z             // B2     | B      | A       | Circle   | 3      | K2     |
#define KEY_BUTTON_R2   HID_KEY_X             // R2     | RT     | ZR      | R2       | 8      | K3     |
#define KEY_BUTTON_L2   HID_KEY_V             // L2     | LT     | ZL      | L2       | 7      | K4     |
#define KEY_BUTTON_B3   HID_KEY_CONTROL_LEFT  // B3     | X      | Y       | Square   | 1      | P1     |
#define KEY_BUTTON_B4   HID_KEY_ALT_LEFT      // B4     | Y      | X       | Triangle | 4      | P2     |
#define KEY_BUTTON_R1   HID_KEY_SPACE         // R1     | RB     | R       | R1       | 6      | P3     |
#define KEY_BUTTON_L1   HID_KEY_C             // L1     | LB     | L       | L1       | 5      | P4     |
#define KEY_BUTTON_S1   HID_KEY_5             // S1     | Back   | Minus   | Select   | 9      | Coin   |
#define KEY_BUTTON_S2   HID_KEY_1             // S2     | Start  | Plus    | Start    | 10     | Start  |
#define KEY_BUTTON_L3   HID_KEY_EQUAL         // L3     | LS     | LS      | L3       | 11     | LS     |
#define KEY_BUTTON_R3   HID_KEY_MINUS         // R3     | RS     | RS      | R3       | 12     | RS     |
#define KEY_BUTTON_A1   HID_KEY_9             // A1     | Guide  | Home    | PS       | 13     | ~      |
#define KEY_BUTTON_A2   HID_KEY_F2            // A2     | ~      | Capture | ~        | 14     | ~      |
#define KEY_BUTTON_FN   -1                    // Hotkey Function                                        |

#define BOARD_LEDS_PIN 15

#define LED_BRIGHTNESS_MAXIMUM 255
#define LED_BRIGHTNESS_STEPS 10
#define LED_FORMAT LED_FORMAT_GRB
#define LEDS_PER_PIXEL 2

#define LEDS_DPAD_LEFT   0
#define LEDS_DPAD_DOWN   1
#define LEDS_DPAD_RIGHT  2
#define LEDS_DPAD_UP     11
#define LEDS_BUTTON_B3   3
#define LEDS_BUTTON_B4   4
#define LEDS_BUTTON_R1   5
#define LEDS_BUTTON_L1   6
#define LEDS_BUTTON_B1   10
#define LEDS_BUTTON_B2   9
#define LEDS_BUTTON_R2   8
#define LEDS_BUTTON_L2   7

#define HAS_I2C_DISPLAY 1
#define I2C_SDA_PIN 26
#define I2C_SCL_PIN 27
#define I2C_BLOCK i2c1
#define I2C_SPEED 800000
#define DISPLAY_FLIP 0
#define DISPLAY_INVERT 0

#define BUTTON_LAYOUT BUTTON_LAYOUT_STICKLESS
#define BUTTON_LAYOUT_RIGHT BUTTON_LAYOUT_STICKLESSB
#define SPLASH_MODE SPLASH_MODE_STATIC
#define SPLASH_CHOICE SPLASH_CHOICE_MAIN
#define SPLASH_DURATION 7500 

#define DEFAULT_SPLASH \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x04,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x04,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x04,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0x80,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0x80,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x03,0xE0,0x00,0x00,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x01,0xFF,0xFF,0xFE,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x80,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xC0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0xFE,0x00,0x00,0x70,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0xFF,0x00,0x00,0xF0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x3F,0xFF,0x80,0x03,0xE0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xC0,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0x80,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFE,0x00,0xFF,0xFF,0x8F,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x3F,0xFF,0x80,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x07,0xFE,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x03,0xC0,0x07,0x80,0x3C,0x00,0x82,0x00,0xF8,0x03,0xF8,0x04,0x01,0xF0,0x07,0x80, \
  0x04,0x00,0x08,0x00,0x22,0x00,0x82,0x00,0x84,0x00,0x40,0x04,0x01,0x00,0x04,0x40, \
  0x04,0x00,0x10,0x00,0x21,0x00,0x82,0x00,0x84,0x00,0x40,0x04,0x01,0x00,0x04,0x20, \
  0x06,0x00,0x10,0x00,0x21,0x00,0x82,0x00,0x84,0x00,0x40,0x04,0x01,0x00,0x04,0x20, \
  0x01,0x00,0x10,0x00,0x22,0x00,0x82,0x00,0xF8,0x00,0x40,0x04,0x01,0xF0,0x04,0x40, \
  0x00,0xC0,0x10,0x00,0x3C,0x00,0x82,0x00,0x84,0x00,0x40,0x04,0x01,0x00,0x07,0x80, \
  0x00,0x40,0x10,0x00,0x24,0x00,0x82,0x00,0x84,0x00,0x40,0x04,0x01,0x00,0x04,0x80, \
  0x00,0x40,0x08,0x00,0x22,0x00,0xC6,0x00,0x84,0x00,0x40,0x04,0x01,0x00,0x04,0x40, \
  0x07,0x80,0x07,0x80,0x21,0x00,0x7C,0x00,0xF8,0x00,0x40,0x04,0x01,0xF0,0x04,0x20, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, \
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 

#endif