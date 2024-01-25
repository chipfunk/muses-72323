#ifndef MUSES_72323_H
#define	MUSES_72323_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdint.h>
    #include <stdbool.h>

  // Volume-boundaries
  #define MUSES_72323_MIN_ATTENUATION 0x20
  #define MUSES_72323_MAX_ATTENUATION 0x1DF

    typedef uint16_t muses_72323_command_t; // A command to be sent via SPI
    typedef uint8_t muses_72323_chip_address_t; // Address one of the [0..3] chips on same SPI line
    typedef uint8_t muses_72323_channel_gain_t; // [0..7] times 3dB gain per channel
    typedef uint8_t muses_72323_zero_window_t;
    typedef uint8_t muses_72323_soft_step_clock_divider_t;
    typedef int16_t muses_72323_attenuation_t; // volume-attenuation in 0.25dB steps [32..512]

    // List possible commands to send
    typedef enum MUSES_72323_SELECT_ADDRESS {
        MUSES_72323_SELECT_ADDRESS_CHANNEL_LEFT = 0x00,
        MUSES_72323_SELECT_ADDRESS_CHANNEL_RIGHT = 0x01,
        MUSES_72323_SELECT_ADDRESS_GAIN = 0x02,
        MUSES_72323_SELECT_ADDRESS_CONFIGURE = 0x03,
    } _muses_72323_select_address_t;

    typedef enum MUSES_72323_CHANNEL {
        MUSES_72323_CHANNEL_LEFT = 0x00,
        MUSES_72323_CHANNEL_RIGHT = 0x01,
    } muses_72323_channel_t;

    /**
    * Configure the MUSES 72323.
    *
    * @param chip_address The chip-address to send command to
    * @param zero_window
    * @param clock_divider
    * @param soft_step_clock
    *
    * @return 16-bit command to be sent to MUSES 72323.
    */
    muses_72323_command_t muses_72323_configure(const muses_72323_chip_address_t chip_address, const muses_72323_zero_window_t zero_window, const muses_72323_soft_step_clock_divider_t clock_divider, const bool soft_step_clock);

    /**
    * Set channel-gain.
    *
    * @param chip_address The chip-address to send command to
    * @param left The left channel gain
    * @param right The right channel gain
    * @param l_r_control
    * @param zero_cross Use zero-cross
    *
    * @return 16-bit command to be sent to MUSES 72323.
    */
    muses_72323_command_t muses_72323_set_gain(const muses_72323_chip_address_t chip_address, const muses_72323_channel_gain_t left, const muses_72323_channel_gain_t right, const bool l_r_control, const bool zero_cross);

    /**
    * Set volume per channel.
    *
    * @param chip_address The chip-address to send command to
    * @param channel The channel-number [0..1] to set volume for
    * @param attenuation The volume-attenuation in -0.25db steps in range [32...512].
    * @param soft_step Use soft-step
    *
    * @return 16-bit command to be sent to MUSES 72323.
    */
    muses_72323_command_t muses_72323_set_volume(const muses_72323_chip_address_t chip_address, const muses_72323_channel_t channel, const muses_72323_attenuation_t attenuation, const bool soft_step);

    /**
    * Mute a channel. Un-mute it by setting a volume.
    *
    * @param chip_address The chip-address to send command to
    * @param channel The channel-number [0..1] to set volume for
    * @param soft_step Use soft-step
    *
    * @return 16-bit command to be sent to MUSES 72323.
    */
    muses_72323_command_t muses_72323_mute(const muses_72323_chip_address_t chip_address, const muses_72323_channel_t channel, const bool soft_step);

#ifdef	__cplusplus
}
#endif

#endif	/* MUSES_72323_H */

