#ifndef SERIAL_H_
#define SERIAL_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include "serial_send.h"

#define USB "/dev/ttyACM0"
//#define USB "/dev/ttyUSB0"

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class serial_send {

public:
	serial_send(io_service&	io_serial);

	
	void start_serial(io_service& io_serial);

	void serial_write(std::string message);
	serial_port *sp;
};

#endif
