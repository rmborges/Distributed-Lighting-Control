#ifndef CLIENT_MSG_H_
#define CLIENT_MSG_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include "arduino.h"
#include "serial_send.h"
#include <algorithm>

#define MSG_RESET "RESTART"

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class client_msg {

public:
	client_msg();

	std::string print_message(std::string msg_from_client, std::string msg_to_client);


};

#endif