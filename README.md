# MUSES 72323

This C-library generates commands to control a MUSES 72323.


## Integration

To integrate this library into your project:

- include "muses_72327.h"
- implement a function to send the generated commands via GPIO / SPI / ...
- generate the appropriate command to control the chip
- send the generated command by calling your send()-function


### Code-Example

    #include "muses_72327.h"

    /**
     * @TODO implement this for your platform
     *
     * Send @param command to send
     */
    void MUSES_SEND(muses_72327_command_t command) {
        MUSES_PIN_LATCH_SetLow();
        ...
        MUSES_PIN_DATA_OUT_send(high_byte(command));
        MUSES_PIN_DATA_OUT_send(low_byte(command));
        ...
        MUSES_PIN_LATCH_SetHigh();
        ...
    }
    
    ...

    muses_72323_chip_address_t chip_address = 0;
    muses_72323_channel_t channel = MUSES_72323_CHANEL_LEFT;
    muses_72323_attenuation_t volume = 32;
    bool zero_cross = false;

    muses_72327_command_t command;
    muses_72323_error_t error = muses_72323_set_volume(&command, chip_addr, channel, volume, zero_cross);
    if(error) {
        ...
    }

    MUSES_SEND(command);


## Something missing or wrong?

Tell me about it :)
