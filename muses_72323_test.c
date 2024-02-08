#include <stdio.h>
#include <assert.h>

#include "muses_72323.h"

void
test_configure ()
{
  muses_72323_zero_window_t zero_window = 0;
  muses_72323_soft_step_clock_divider_t clock_divider = 0;
  muses_72323_soft_step_clock_source_t soft_step_clock =
      MUSES_72323_SOFT_STEP_CLOCK_EXTERNAL;

  muses_72323_command_t command;
  muses_72323_error_t error;

  muses_72323_chip_address_t chip_address = 0x00;

  error = muses_72323_configure (&command, chip_address, zero_window,
				 clock_divider, soft_step_clock);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0000000000001100);

  soft_step_clock = MUSES_72323_SOFT_STEP_CLOCK_INTERNAL;
  error = muses_72323_configure (&command, chip_address, zero_window,
				 clock_divider, soft_step_clock);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0000001000001100);

  zero_window = 3;
  error = muses_72323_configure (&command, chip_address, zero_window,
				 clock_divider, soft_step_clock);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0110001000001100);

  error = muses_72323_configure (&command, chip_address,
				 MUSES_72323_MAX_ZERO_WINDOW + 1, clock_divider,
				 soft_step_clock);
  assert(error == MUSES_72323_ERROR_ZERO_WINDOW_GREATER_THAN_MAX);

  error = muses_72323_configure (&command, chip_address, zero_window,
  MUSES_72323_MAX_CLOCK_DIVIDER + 1,
				 soft_step_clock);
  assert(error == MUSES_72323_ERROR_CLOCK_DIVIDER_GREATER_THAN_MAX);

  error = muses_72323_configure (&command, MUSES_72323_MAX_CHIP_ADDR + 1,
				 zero_window, clock_divider, soft_step_clock);
  assert(error == MUSES_72323_ERROR_CHIP_ADDRESS_GREATER_THAN_MAX);
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

  error = muses_72323_set_gain (&command, chip_address,
				MUSES_72323_MAX_CHANNEL_GAIN + 1, right,
				link_channels, use_zero_crossing);
  assert(error == MUSES_72323_ERROR_CHANNEL_GAIN_GREATER_THAN_MAX);

  error = muses_72323_set_gain (&command, chip_address, left,
				MUSES_72323_MAX_CHANNEL_GAIN + 1, link_channels,
				use_zero_crossing);
  assert(error == MUSES_72323_ERROR_CHANNEL_GAIN_GREATER_THAN_MAX);

  error = muses_72323_set_gain (&command, MUSES_72323_MAX_CHIP_ADDR + 1, left,
				right, link_channels, use_zero_crossing);
  assert(error == MUSES_72323_ERROR_CHIP_ADDRESS_GREATER_THAN_MAX);
}

void
test_set_volume ()
{
  muses_72323_chip_address_t chip_address = 0x00;
  muses_72323_channel_t channel = MUSES_72323_CHANNEL_LEFT;
  muses_72323_attenuation_t attenuation = 0;
  bool use_soft_step = false;

  muses_72323_command_t command;
  muses_72323_error_t error;

  attenuation = 0;
  error = muses_72323_set_volume (&command, chip_address, channel, attenuation,
				  use_soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b0001000000000000);

  attenuation = MUSES_72323_MAX_ATTENUATION;
  error = muses_72323_set_volume (&command, chip_address, channel, attenuation,
				  use_soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1110111110000000);

  use_soft_step = true;
  error = muses_72323_set_volume (&command, chip_address, channel, attenuation,
				  use_soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1110111110010000);

  chip_address = 0x02;
  error = muses_72323_set_volume (&command, chip_address, channel, attenuation,
				  use_soft_step);
  assert(error == MUSES_72323_ERROR_NONE);
  assert(command == 0b1110111110010010);

  error = muses_72323_set_volume (&command, chip_address, channel,
				  MUSES_72323_MAX_ATTENUATION + 1,
				  use_soft_step);
  assert(error == MUSES_72323_ERROR_ATTENUTION_GREATER_THAN_MAX);

  error = muses_72323_set_volume (&command, MUSES_72323_MAX_CHIP_ADDR + 1,
				  channel, attenuation, use_soft_step);
  assert(error == MUSES_72323_ERROR_CHIP_ADDRESS_GREATER_THAN_MAX);
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

  error = muses_72323_mute (&command, MUSES_72323_MAX_CHIP_ADDR + 1, channel,
			    soft_step);
  assert(error == MUSES_72323_ERROR_CHIP_ADDRESS_GREATER_THAN_MAX);
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
