/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef FLASHPROM_H_
#define FLASHPROM_H_

#include <stdint.h>
#include <string.h>
#include <pico/lock_core.h>
#include <pico/multicore.h>
#include <hardware/flash.h>
#include <hardware/timer.h>

#define EEPROM_SIZE_BYTES    0x5000           // Reserve 20k of flash memory (ensure this value is divisible by 256)
#define EEPROM_ADDRESS_START _u(0x101FB000) // Updated according to EEPROM_SIZE_BYTES

// Warning: If the write wait is too long it can stall other processes
#define EEPROM_WRITE_WAIT    50             // Amount of time in ms to wait before blocking core1 and committing to flash

class FlashPROM
{
	public:
		void start();
		void commit();
		void reset();

		static uint8_t writeCache[EEPROM_SIZE_BYTES];
};

inline FlashPROM EEPROM;

#endif
