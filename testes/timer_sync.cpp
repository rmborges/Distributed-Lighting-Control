//TIMER_SYNC.CPP
#include	<iostream>
#include	<chrono>
#include	<boost/asio/steady_timer.hpp>

int main()	{
	boost::asio::io_service io;
	boost::asio::steady_timer tim(io);
	int count=0;
	for(;;)	{
		tim.expires_from_now(milliseconds(1000));
		tim.wait();		//blocks	here
		std::cout<<count++<<std::endl;
	}	
}