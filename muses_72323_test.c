/*
 * muses_72323_test.c
 *
 *  Created on: Jan 19, 2024
 *      Author: d00d3
 */
#include <stdio.h>
#include <assert.h>

#include "muses_72323.h"

/**
 * List of bitmasks for command-bits
 *
 * All commands
 * 0b0000000000001100 - select-address
 * 0b0000000000000011 - chip-address
 *
 * Set volume command
 * 0b1111111110000000 - volume
 * 0b0000000000010000 - use soft-step
 *
 * Set gain command
 * 0b1000000000000000 - l/r control
 * 0b0111000000000000 - left
 * 0b0000111000000000 - right
 * 0b0000000100000000 - zero-cross
 *
 * Configure command
 * 0b0110000000000000 - zero-window
 * 0b0001110000000000 - clock-divider
 * 0b0000001000000000 - soft-step clock
 */

void
test_configure ()
{
  muses_72323_zero_window_t zero_window = 0;
  muses_72323_soft_step_clock_divider_t clock_divider = 0;
  bool soft_step_clock = true;

  muses_72323_command_t command;
  muses_72323_error_t error;

  muses_72323_chip_address_t chip_address = 0x00;

  error = muses_72323_configure (&command, chip_address,
						     zero_window, clock_divider,
						     soft_step_clock);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0000001000001100);

  zero_window = 3;
  error = muses_72323_configure (&command, chip_address, zero_window,
				   clock_divider, soft_step_clock);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0110001000001100);
}

void
test_set_gain ()
{
  bool link_channels = false;
  bool use_zero_crossing = true;

  muses_72323_command_t command;
  muses_72323_error_t error;

  muses_72323_chip_address_t chip_address = 0x00;

  muses_72323_attenuation_t left, right;

  left = right = 0;
  error = muses_72323_set_gain (&command, chip_address, left, right,
				  link_channels, use_zero_crossing);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0000000000001000);

  left = right = 1;
  error = muses_72323_set_gain (&command, chip_address, left, right,
				  link_channels, use_zero_crossing);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0001001000001000);

  left = right = 0b111;
  error = muses_72323_set_gain (&command, chip_address, left, right,
				  link_channels, use_zero_crossing);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0111111000001000);

  link_channels = true;
  error = muses_72323_set_gain (&command, chip_address, left, right,
				  link_channels, use_zero_crossing);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1111111000001000);
}

void
test_set_volume ()
{
  muses_72323_chip_address_t chip_address = 0x00;
  muses_72323_channel_t channel = MUSES_72323_CHANNEL_LEFT;
  muses_72323_attenuation_t attenuation = 0;
  bool soft_step = false;

  muses_72323_command_t command;
  muses_72323_error_t error;

  attenuation = 0;
  error = muses_72323_set_volume (&command, chip_address, channel,
				    attenuation, soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0001000000000000);

  attenuation = - 1;
  error = muses_72323_set_volume (&command, chip_address, channel,
				    attenuation, soft_step);
  assert(error == MUSES_72323_ERROR_ATTENUTION_GREATER_THAN_MAX);
  assert(command == 0b0001000000000000);

  attenuation = MUSES_72323_MAX_ATTENUATION;
  error = muses_72323_set_volume (&command, chip_address, channel,
				    attenuation, soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1110111110000000);

  attenuation = MUSES_72323_MAX_ATTENUATION + 1;
  error = muses_72323_set_volume (&command, chip_address, channel,
				    attenuation, soft_step);
  assert(error == MUSES_72323_ERROR_ATTENUTION_GREATER_THAN_MAX);
  assert(command == 0b1110111110000000);

  attenuation = 447;
  soft_step = true;
  error = muses_72323_set_volume (&command, chip_address, channel,
				    attenuation, soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1110111110010000);

  chip_address = 0x02;
  error = muses_72323_set_volume (&command, chip_address, channel,
				    attenuation, soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1110111110010010);
}

void
test_mute (void)
{
  muses_72323_chip_address_t chip_address = 0x00;
  muses_72323_channel_t channel = MUSES_72323_CHANNEL_LEFT;
  bool soft_step = false;

  muses_72323_command_t command;
  muses_72323_error_t error;

  error = muses_72323_mute (&command, chip_address, channel, soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1111111110000000);
}

int
main (void)
{
  fprintf (stderr, "Starting tests ...\n");

  test_configure ();
  test_set_gain ();
  test_set_volume ();
  test_mute ();

  fprintf (stderr, "... all good\n");
}
