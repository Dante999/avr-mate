#ifndef TRANSCEIVER_HPP
#define TRANSCEIVER_HPP

#include <cstdint>

class transceiver_handheld_c {
public:
	virtual void    send_byte(uint8_t byte) = 0;
	virtual uint8_t receive_byte()          = 0;
	virtual ~transceiver_handheld_c() {}
};

class transceiver_cartridge_c {
public:
	virtual void    send_byte(uint8_t byte) = 0;
	virtual uint8_t receive_byte()          = 0;
	virtual ~transceiver_cartridge_c() {}
};

#endif // TRANSCEIVER_HPP
