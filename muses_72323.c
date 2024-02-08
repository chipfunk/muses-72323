#include "muses_72323.h"

/** List of possible command identifiers */
typedef enum
{
  MUSES_72323_SELECT_ADDRESS_CHANNEL_LEFT = 0x00,
  MUSES_72323_SELECT_ADDRESS_CHANNEL_RIGHT = 0x01,
  MUSES_72323_SELECT_ADDRESS_GAIN = 0x02,
  MUSES_72323_SELECT_ADDRESS_CONFIGURE = 0x03,
} _muses_72323_select_address_t;

/** Bitmasks */
#define MUSES_72323_BITMASK_SELECT_ADDRESS 0x000C
#define MUSES_72323_BITMASK_CHIP_ADDRESS 0x0003

#define MUSES_72323_BITMASK_CHANNEL_VOLUME 0xFF80
#define MUSES_72323_BITMASK_SOFT_STEP 0x0010

#define MUSES_72323_BITMASK_L_R_CONT 0x8000
#define MUSES_72323_BITMASK_LEFT_CHANNEL_GAIN 0x7000
#define MUSES_72323_BITMASK_RIGHT_CHANNEL_GAIN 0x0E00
#define MUSES_72323_BITMASK_ZERO_CROSS 0x0080

#define MUSES_72323_BITMASK_ZERO_WINDOW 0x6000
#define MUSES_72323_BITMASK_CLK_DIV 0x01C0
#define MUSES_72323_BITMASK_SS_CLK 0x0200

/** Set this value to mute a channel */
#define MUSES_72323_MUTE 0x1FF

/**
 * Prepare command to be sent.
 *
 * \param chip_address select the chip this command should be sent to
 * \param select_address the command to be sent
 *
 * \return a command with selected chip- and select-address
 */
static muses_72323_error_t
_muses_72323_prepare_command (
    muses_72323_command_t *command,
    const muses_72323_chip_address_t chip_address,
    const _muses_72323_select_address_t select_address)
{
  if (chip_address > MUSES_72323_MAX_CHIP_ADDR)
    return MUSES_72323_ERROR_CHIP_ADDRESS_GREATER_THAN_MAX;

  *command = 0x00;

  *command |= MUSES_72323_BITMASK_SELECT_ADDRESS & select_address << 2;
  *command |= MUSES_72323_BITMASK_CHIP_ADDRESS & chip_address;

  return MUSES_72323_ERROR_NONE;
}

muses_72323_error_t
muses_72323_configure (
    muses_72323_command_t *command,
    const muses_72323_chip_address_t chip_address,
    const muses_72323_zero_window_t zero_window,
    const muses_72323_soft_step_clock_divider_t clock_divider,
    const muses_72323_soft_step_clock_source_t soft_step_clock)
{
  muses_72323_error_t error;
  if ((error = _muses_72323_prepare_command (
      command, chip_address, MUSES_72323_SELECT_ADDRESS_CONFIGURE)))
    return error;

  if (zero_window > MUSES_72323_MAX_ZERO_WINDOW)
    return MUSES_72323_ERROR_ZERO_WINDOW_GREATER_THAN_MAX;

  if (clock_divider > MUSES_72323_MAX_CLOCK_DIVIDER)
    return MUSES_72323_ERROR_CLOCK_DIVIDER_GREATER_THAN_MAX;

  *command |= MUSES_72323_BITMASK_ZERO_WINDOW & zero_window << 13;
  *command |= MUSES_72323_BITMASK_CLK_DIV & clock_divider << 10;

  *command |= MUSES_72323_BITMASK_SS_CLK & soft_step_clock << 9;

  return MUSES_72323_ERROR_NONE;
}

muses_72323_error_t
muses_72323_set_gain (muses_72323_command_t *command,
		      const muses_72323_chip_address_t chip_address,
		      const muses_72323_channel_gain_t left,
		      const muses_72323_channel_gain_t right,
		      const bool link_channels, const bool zero_cross)
{
  muses_72323_error_t error;
  if ((error = _muses_72323_prepare_command (command, chip_address,
					     MUSES_72323_SELECT_ADDRESS_GAIN)))
    return error;

  if (left > MUSES_72323_MAX_CHANNEL_GAIN)
    return MUSES_72323_ERROR_CHANNEL_GAIN_GREATER_THAN_MAX;

  if (right > MUSES_72323_MAX_CHANNEL_GAIN)
    return MUSES_72323_ERROR_CHANNEL_GAIN_GREATER_THAN_MAX;

  if (link_channels)
    *command |= MUSES_72323_BITMASK_L_R_CONT & 0x01 << 15;

  *command |= MUSES_72323_BITMASK_LEFT_CHANNEL_GAIN & left << 12;
  *command |= MUSES_72323_BITMASK_RIGHT_CHANNEL_GAIN & right << 9;

  if (zero_cross)
    *command |= MUSES_72323_BITMASK_ZERO_CROSS & 0x01 << 8;

  return MUSES_72323_ERROR_NONE;
}

muses_72323_error_t
muses_72323_set_volume (muses_72323_command_t *command,
			const muses_72323_chip_address_t chip_address,
			const muses_72323_channel_t channel,
			const muses_72323_attenuation_t attenuation,
			const bool use_soft_step)
{
  if (attenuation > MUSES_72323_MAX_ATTENUATION)
    return MUSES_72323_ERROR_ATTENUTION_GREATER_THAN_MAX;

  _muses_72323_select_address_t select_addr;
  if (channel == MUSES_72323_CHANNEL_LEFT)
    select_addr = MUSES_72323_SELECT_ADDRESS_CHANNEL_LEFT;
  else
    select_addr = MUSES_72323_SELECT_ADDRESS_CHANNEL_RIGHT;

  muses_72323_error_t error;
  if ((error = _muses_72323_prepare_command (command, chip_address, select_addr)))
    return error;

  *command |= MUSES_72323_BITMASK_CHANNEL_VOLUME & (0x20 + attenuation) << 7;

  if (use_soft_step)
    *command |= MUSES_72323_BITMASK_SOFT_STEP & 0x01 << 4;

  return MUSES_72323_ERROR_NONE;
}

muses_72323_error_t
muses_72323_mute (muses_72323_command_t *command,
		  const muses_72323_chip_address_t chip_address,
		  const muses_72323_channel_t channel, const bool use_soft_step)
{
  _muses_72323_select_address_t select_addr;
  if (channel == MUSES_72323_CHANNEL_LEFT)
    select_addr = MUSES_72323_SELECT_ADDRESS_CHANNEL_LEFT;
  else
    select_addr = MUSES_72323_SELECT_ADDRESS_CHANNEL_RIGHT;

  muses_72323_error_t error;
  if ((error = _muses_72323_prepare_command (command, chip_address, select_addr)))
    return error;

  *command |= MUSES_72323_BITMASK_CHANNEL_VOLUME & MUSES_72323_MUTE << 7;

  if (use_soft_step)
    *command |= MUSES_72323_BITMASK_SOFT_STEP & 0x01 << 4;

  return MUSES_72323_ERROR_NONE;
}
