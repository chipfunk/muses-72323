#include "muses_72323.h"

/**
 * Mask relevant bits in MUSES 72323 commands
 */
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

// 2 bits chip-address
#define MUSES_72323_MAX_CHIP_ADDR 0x03

// 2 bits chip-address
#define MUSES_72323_MAX_SELECT_ADDR 0x03

#define MUSES_72323_MUTE 0x1FF

// Chip provides amplifications in 8 steps of 3dB
#define MUSES_72323_MAX_CHANNEL_GAIN 0x07

static muses_72323_command_t _muses_72323_prepare_command(const muses_72323_chip_address_t chip_address, const _muses_72323_select_address_t select_address) {
    muses_72323_command_t command = 0x00;

    command |= MUSES_72323_BITMASK_SELECT_ADDRESS & select_address << 2;
    command |= MUSES_72323_BITMASK_CHIP_ADDRESS & chip_address;

    return command;
}

const muses_72323_command_t muses_72323_configure(const muses_72323_chip_address_t chip_address, const muses_72323_zero_window_t zero_window, const muses_72323_soft_step_clock_divider_t clock_divider, const bool soft_step_clock) {
    muses_72323_command_t command = _muses_72323_prepare_command(chip_address, MUSES_72323_SELECT_ADDRESS_CONFIGURE);

    command |= MUSES_72323_BITMASK_ZERO_WINDOW & zero_window << 13;
    command |= MUSES_72323_BITMASK_CLK_DIV & clock_divider << 10;
    if (soft_step_clock) command |= MUSES_72323_BITMASK_SS_CLK & 0x01 << 9;

    return command;
}

const muses_72323_command_t muses_72323_set_gain(const muses_72323_chip_address_t chip_address, const muses_72323_channel_gain_t left, const muses_72323_channel_gain_t right, const bool l_r_control, const bool zero_cross) {
    muses_72323_command_t command = _muses_72323_prepare_command(chip_address, MUSES_72323_SELECT_ADDRESS_GAIN);

    if (l_r_control) command |= MUSES_72323_BITMASK_L_R_CONT & 0x01 << 15;

    if (left > MUSES_72323_MAX_CHANNEL_GAIN) {
        command |= MUSES_72323_BITMASK_LEFT_CHANNEL_GAIN & MUSES_72323_MAX_CHANNEL_GAIN << 12;
    } else {
        command |= MUSES_72323_BITMASK_LEFT_CHANNEL_GAIN & left << 12;
    }

    if (right > MUSES_72323_MAX_CHANNEL_GAIN) {
        command |= MUSES_72323_BITMASK_RIGHT_CHANNEL_GAIN & MUSES_72323_MAX_CHANNEL_GAIN << 9;
    } else {
        command |= MUSES_72323_BITMASK_RIGHT_CHANNEL_GAIN & right << 9;
    }

    if (zero_cross) command |= MUSES_72323_BITMASK_ZERO_CROSS & 0x01 << 8;

    return command;
}

const muses_72323_command_t muses_72323_set_volume(const muses_72323_chip_address_t chip_address, const muses_72323_channel_t channel, const muses_72323_attenuation_t attenuation, const bool soft_step) {
    muses_72323_command_t command = _muses_72323_prepare_command(chip_address, channel);

    if (attenuation < MUSES_72323_MIN_ATTENUATION) {
        command |= MUSES_72323_BITMASK_CHANNEL_VOLUME & MUSES_72323_MIN_ATTENUATION << 7;
    } else if (attenuation > MUSES_72323_MAX_ATTENUATION) {
        command |= MUSES_72323_BITMASK_CHANNEL_VOLUME & MUSES_72323_MAX_ATTENUATION << 7;
    } else {
        command |= MUSES_72323_BITMASK_CHANNEL_VOLUME & attenuation << 7;
    }

    if (soft_step) command |= MUSES_72323_BITMASK_SOFT_STEP & 0x01 << 4;

    return command;
}

const muses_72323_command_t muses_72323_mute(const muses_72323_chip_address_t chip_address, const muses_72323_channel_t channel, const bool soft_step) {
    muses_72323_command_t command = _muses_72323_prepare_command(chip_address, channel);

    command |= MUSES_72323_BITMASK_CHANNEL_VOLUME & MUSES_72323_MUTE << 7;

    if (soft_step) command |= MUSES_72323_BITMASK_SOFT_STEP & 0x01 << 4;

    return command;
}
