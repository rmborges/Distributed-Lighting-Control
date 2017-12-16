#ifndef CLIENT_MSG_H_
#define CLIENT_MSG_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class client_msg {

public:
	client_msg();

	void print_message(std::string message);


};

#endif