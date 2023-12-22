# MUSES 72323

This C-library generates commands to control a MUSES 72323 via SPI.


## Integration

To integrate this library into your project:

- include "muses_72327.h"
- implement a function to send the generated commands via SPI
- generate the appropriate command to control the chip
- send the generated command via SPI by calling your provided function


### Code-Example

    // include library
    #include "muses_72327.h"

    /**
     * @TODO implement
     *
     * Send @param command via SPI.
     */
    void MY_SPI_SEND(muses_72327_command_t command) {
        ...
        spi_send(command);
        ...
    }

    // Configure command
    muses_72323_chip_address_t chip_address = 0;
    muses_72323_channel_t channel = MUSES_72323_CHANEL_LEFT;
    muses_72323_attenuation_t volume = 0;
    bool zero_cross = false;

    // Generate command
    muses_72327_command_t command = muses_72323_set_volume(chip_addr, channel, volume, zero_cross);

    // Send command
    MY_SPI_SEND(command);


## Something missing or wrong?

Tell me about it :)
