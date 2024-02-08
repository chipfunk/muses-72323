#ifndef MUSES_72323_H
#define	MUSES_72323_H

#include <stdint.h>
#include <stdbool.h>

/** \define Lower attenuation boundary */
#define MUSES_72323_MAX_ATTENUATION 0x1BF

/** \define Upper clock-divider boundary */
#define MUSES_72323_MAX_CHANNEL_GAIN 7

/** \define Upper chip-address boundary */
#define MUSES_72323_MAX_CHIP_ADDR 3

/** \define Upper clock-divider boundary */
#define MUSES_72323_MAX_CLOCK_DIVIDER 7

/** \define Upper zero-window boundary */
#define MUSES_72323_MAX_ZERO_WINDOW 3

#ifdef	__cplusplus
extern "C"
{
#endif

  /** Possible errors */
  typedef enum
  {
    MUSES_72323_ERROR_NONE = 0,
    MUSES_72323_ERROR_CHIP_ADDRESS_GREATER_THAN_MAX,
    MUSES_72323_ERROR_ATTENUTION_GREATER_THAN_MAX,
    MUSES_72323_ERROR_CHANNEL_GAIN_GREATER_THAN_MAX,
    MUSES_72323_ERROR_CLOCK_DIVIDER_GREATER_THAN_MAX,
    MUSES_72323_ERROR_ZERO_WINDOW_GREATER_THAN_MAX,
  } muses_72323_error_t;

  typedef enum
  {
    MUSES_72323_SOFT_STEP_CLOCK_EXTERNAL = 0x00,
    MUSES_72323_SOFT_STEP_CLOCK_INTERNAL = 0x01,
  } muses_72323_soft_step_clock_source_t;

  /** \typedef A command to be sent via SPI */
  typedef uint16_t muses_72323_command_t;

  /** \typedef Address one of the [0..3] chips on same SPI line */
  typedef uint8_t muses_72323_chip_address_t;

  /** \typedef  [0..7] times 3dB gain per channel */
  typedef uint8_t muses_72323_channel_gain_t;

  /** \typedef The zero-window */
  typedef uint8_t muses_72323_zero_window_t;

  /** \typedef The clock-divider */
  typedef uint8_t muses_72323_soft_step_clock_divider_t;

  /** \typedef volume-attenuation in 0.25dB steps [0..480] */
  typedef uint16_t muses_72323_attenuation_t;

  /**
   * \enum Select channel for commands
   */
  typedef enum
  {
    MUSES_72323_CHANNEL_LEFT = 0x00, MUSES_72323_CHANNEL_RIGHT = 0x01,
  } muses_72323_channel_t;

  /**
   * \func Configure the MUSES 72323.
   *
   * \param command The memory-address to write generated command to
   * \param chip_address The chip-address to send command to
   * \param zero_window
   * \param clock_divider
   * \param soft_step_clock
   *
   * \return 16-bit command to be sent to MUSES 72323.
   */
  muses_72323_error_t
  muses_72323_configure (
      muses_72323_command_t *command,
      const muses_72323_chip_address_t chip_address,
      const muses_72323_zero_window_t zero_window,
      const muses_72323_soft_step_clock_divider_t clock_divider,
      const muses_72323_soft_step_clock_source_t soft_step_clock);

  /**
   * \func Set channel-gain.
   *
   * When \param l_r_control is true the value for right channel is ignored.
   *
   * \param command The memory-address to write generated command to
   * \param chip_address The chip-address to send command to
   * \param left The left channel gain
   * \param right The right channel gain
   * \param link_channels Link L/R channels
   * \param zero_cross Use zero-cross
   *
   * \return 16-bit command to be sent to MUSES 72323.
   */
  muses_72323_error_t
  muses_72323_set_gain (muses_72323_command_t *command,
			const muses_72323_chip_address_t chip_address,
			const muses_72323_channel_gain_t left,
			const muses_72323_channel_gain_t right,
			const bool link_channels, const bool use_zero_cross);

  /**
   * Set volume per channel.
   *
   * \param command The memory-address to write generated command to
   * \param chip_address The chip-address to send command to
   * \param channel The channel-number [0..1] to set volume for
   * \param attenuation The volume-attenuation in -0.25db steps in range [0...480].
   * \param use_soft_step Use soft-step
   *
   * \return zero or error-code on error
   */
  muses_72323_error_t
  muses_72323_set_volume (muses_72323_command_t *command,
			  const muses_72323_chip_address_t chip_address,
			  const muses_72323_channel_t channel,
			  const muses_72323_attenuation_t attenuation,
			  const bool use_soft_step);

  /**
   * Mute a channel. Un-mute it by setting a volume.
   *
   * \param command The memory-address to write generated command to
   * \param chip_address The chip-address to send command to
   * \param channel The channel-number [0..1] to set volume for
   * \param use_soft_step Use soft-step
   *
   * \return 16-bit command to be sent to MUSES 72323.
   */
  muses_72323_error_t
  muses_72323_mute (muses_72323_command_t *command,
		    const muses_72323_chip_address_t chip_address,
		    const muses_72323_channel_t channel,
		    const bool use_soft_step);

#ifdef	__cplusplus
}
#endif

#endif	/* MUSES_72323_H */

