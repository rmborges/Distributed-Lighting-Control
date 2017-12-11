#include	<iostream>
#include	<chrono>
#include <boost/asio.hpp>
#include	<boost/asio/steady_timer.hpp>
using namespace boost::system;
using namespace boost::asio;

boost::asio::io_service io;
boost::asio::steady_timer tim(io);
int count	=	0;

void	deadline_handler(const boost::system::error_code &	ec)	
{	
	std::cout <<	count++	<<	std::endl;
	tim.expires_from_now(std::chrono::seconds(1));
	tim.async_wait(&deadline_handler);
}	

int main()	
{
	tim.expires_from_now();
	tim.async_wait(&deadline_handler);	
	io.run();
}