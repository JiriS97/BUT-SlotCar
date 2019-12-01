#include "SoftI2C.h"

void SoftI2C::Init(uint32_t freq)
{
	SetClock(freq);
	P3OUT &= ~0x6; //P3.1, P3.2
    //PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
	i2c_scl_hi();
	delayMicroseconds(_i2cdelay);
	i2c_sda_hi();

	delayMicroseconds(4 * _i2cdelay);
}

//
// beginTransmission starts the I2C transmission immediate.
//
bool SoftI2C::StartWrite(uint8_t address)
{
	i2c_start();
	return Write((address << 1) | 0); // The r/w bit is zero for write
}

//
void SoftI2C::EndTransmission(bool sendStop)
{
	if (sendStop) i2c_stop();
	else i2c_repstart();
}

//
// The requestFrom() read the data from the I2C bus and stores it in a buffer.
//
bool SoftI2C::StartRead(uint8_t address)
{
	i2c_start();
    return Write((address << 1) | 1);             // The r/w bit is '1' to read
}

bool SoftI2C::RestartRead(uint8_t address)
{
	i2c_repstart();
	return Write((address << 1) | 1);             // The r/w bit is '1' to read
}

//
void SoftI2C::Write(const uint8_t* data, int quantity)
{
	for (int i = 0; i < quantity; i++) 
	{
		Write(data[i]);
	}
}

void SoftI2C::ReadBuffer(uint8_t* buf, int size, bool endAck)
{
	for (int i = 0; i < size-1; i++) Read(&(buf[i]));
	Read(&(buf[size-1]), endAck);
}


void SoftI2C::SetClock(uint32_t clock)
{
	_i2cdelay = (1E6 / clock) / 2;
}
