#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include <msp430.h>
#include <stdint.h>

struct i2c_data
{
    const void *tx_buf;
    unsigned tx_len;
    void *rx_buf;
    unsigned rx_len;
};

struct i2c_device
{
    uint8_t address;
};

int i2c_init(void);
int i2c_transfer(const struct i2c_device *dev, struct i2c_data *data);


#endif
