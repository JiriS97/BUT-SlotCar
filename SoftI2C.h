#ifndef SoftwareWire_h
#define SoftwareWire_h

#include <stdint.h>
#include <string.h>
#include <msp430.h>

class SoftI2C
{
public:
	void Init(uint32_t freq = 10000000);
	void SetClock(uint32_t clock);

	bool StartWrite(uint8_t address);
	bool StartRead(uint8_t address);
	bool RestartRead(uint8_t address);
	void EndTransmission(bool sendStop = true);
	void Write(const uint8_t*, int size);
	void ReadBuffer(uint8_t *data, int size, bool endAck = false);

	__attribute__((always_inline))
	bool Write(uint8_t data) {
		for (uint8_t i = 0; i < 8; i++)
		{
			i2c_writebit(data & 0x80);             // highest bit first
			data <<= 1;
		}

		return i2c_readbit() == 0; //true on ACK
	}

	__attribute__((always_inline))
	void Read(uint8_t *data, bool sendAck = true) {
		uint8_t res = 0;

		for (uint8_t i = 0; i < 8; i++)
		{
			res <<= 1;
			res |= i2c_readbit();
		}

		if (sendAck) i2c_writebit(0);
		else i2c_writebit(1);

		delayMicroseconds(_i2cdelay);

		*data = res;
	}

private:
	int _i2cdelay;          // delay in micro seconds for sda and scl bits.

	//helpers
	__attribute__((always_inline))
	void delayMicroseconds(uint32_t x)
	{
		for(uint32_t i=0;i<2*x;i++){
			_NOP();
		}

		//DELAY_microseconds(x);
		//while (x--) {
		//	__NOP();
		//}
	}
	
	__attribute__((always_inline))
	void i2c_sda_lo() {
	    P3DIR |= BIT1;
	}
	
	__attribute__((always_inline))
	void i2c_scl_lo() {
	    P3DIR |= BIT2;
	}
	
	__attribute__((always_inline))
	void i2c_sda_hi() {
	    P3DIR &= ~BIT1;
	}
	
	__attribute__((always_inline))
	void i2c_scl_hi() {
	    P3DIR &= ~BIT2;
	}
	
	__attribute__((always_inline))
	bool i2c_sda_read() {
		return P3IN & BIT1;
	}
	
	__attribute__((always_inline))
	bool i2c_scl_read()   {
        return P3IN & BIT2;
	}

	// private methods
	__attribute__((always_inline))
	void i2c_start() {
		i2c_sda_hi();
		i2c_scl_hi();

		delayMicroseconds(_i2cdelay);
    
		i2c_sda_lo();
		delayMicroseconds(_i2cdelay);
		i2c_scl_lo();
		delayMicroseconds(_i2cdelay);
	}
	
	__attribute__((always_inline))
	void i2c_repstart() {
		i2c_sda_hi();
		i2c_scl_lo();                           // force SCL low
		delayMicroseconds(_i2cdelay);
		i2c_sda_hi();                          // release SDA
		delayMicroseconds(_i2cdelay);
		i2c_scl_hi();                          // release SCL
		delayMicroseconds(_i2cdelay);
		i2c_sda_lo();                          // force SDA low
		delayMicroseconds(_i2cdelay);
	}
	
	__attribute__((always_inline))
	void i2c_stop() {
		i2c_scl_lo();
		i2c_sda_lo();

		delayMicroseconds(_i2cdelay);    // ADDED1

		// For a stop, make SCL high wile SDA is still low
		i2c_scl_hi();
		delayMicroseconds(_i2cdelay);

		// complete the STOP by setting SDA high
		i2c_sda_hi();

		// A delay after the STOP for safety.
		// It is not known how fast the next START will happen.
		delayMicroseconds(_i2cdelay);
	}
	
	__attribute__((always_inline))
	bool i2c_readbit() {
		i2c_sda_hi();             // 'hi' is the same as releasing the line
		i2c_scl_hi();

		bool bit = i2c_sda_read();
		delayMicroseconds(_i2cdelay);
		i2c_scl_lo();
		delayMicroseconds(_i2cdelay);

		return bit;
		/*i2c_sda_hi();             // 'hi' is the same as releasing the line
		i2c_scl_hi();

		delayMicroseconds(_i2cdelay);
		bool bit = i2c_sda_read();
		i2c_scl_lo();
		delayMicroseconds(_i2cdelay);

		return bit;*/
	}

	__attribute__((always_inline))
	void i2c_writebit(bool bit) {
		if (bit) i2c_sda_hi();
		else i2c_sda_lo();

		delayMicroseconds(_i2cdelay);    // This delay is not needed, but it makes it safer
		i2c_scl_hi();                      // clock high: the Slave will read the sda signal
		delayMicroseconds(_i2cdelay);
		i2c_scl_lo();

		delayMicroseconds(_i2cdelay);
		/*if (bit) i2c_sda_hi();
		else i2c_sda_lo();

		delayMicroseconds(_i2cdelay);    // This delay is not needed, but it makes it safer
		i2c_scl_hi();                      // clock high: the Slave will read the sda signal
		delayMicroseconds(_i2cdelay);
		i2c_scl_lo();*/
	}
};

#endif // SoftwareWire_h
