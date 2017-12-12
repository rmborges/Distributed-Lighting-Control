#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class messages {

public:
	messages();
private:
	void print_message(std::string message);


};

#endif