#include <unistd.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using namespace std;
using namespace boost::asio;

#define USB "/dev/ttyACM0"

int main(void){
	
	boost::asio::io_service io_serial;
	boost::system::error_code error_serial;

	serial_port *sp;
	sp = new serial_port(io_serial);
	sp->open(USB, error_serial);
	if (error_serial){
		cout << "Error opening serial port" << endl;
		exit(-1);
	}
	sp->set_option(serial_port_base::baud_rate(9600), error_serial);
	if (error_serial) {
		cout << "Error opening serial port" << endl;
		exit(-1);
	}

	while(1){
		write( *sp, boost::asio::buffer("mandei cenas\n") );
		usleep( 1000000 ); //1 sec
		cout<< "enviei: mandei cenas" << endl<<endl;
	}
	

	sp->close();

}