#include "serial_send.h"

serial_send::serial_send(io_service& io_serial)
{
	start_serial(io_serial); // CONFIRMAR APONTADOR
}
	
void serial_send::start_serial(io_service& io_serial)
{
	//start_serial(io_serial);
	std::cout<< "START SERIAL\n" << std::endl;
	boost::system::error_code error_serial;
	

	sp = new serial_port(io_serial);
	sp->open(USB, error_serial);
	if (error_serial){
		std::cout << "Error opening serial port" << std::endl;
		//exit(-1);
	}

	sp->set_option(serial_port_base::baud_rate(9600), error_serial);
	if (error_serial) {
		std::cout << "Error opening serial port" << std::endl;
		//exit(-1);
	} 
	
	while(1){
		write(*sp, boost::asio::buffer("1234567890123456\n"));
		usleep( 1000000 ); //1 sec
		std::cout<< "enviei : serial" << std::endl << std::endl;
	}
	
	sp->close();
}

void serial_send::serial_write(std::string message)
{
	write(*sp, boost::asio::buffer("mandei cenas\n"));
}
