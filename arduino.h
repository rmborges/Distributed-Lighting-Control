#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>

#define BUFFER_SIZE 10000 // CORRIGIR VALOR!

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class arduino {

public:
	arduino();

private:	
	//void start_serial(io_service& io_serial, serial_port *sp);

	// common to all arduinos
	static bool restart;
	static double inst_power_system;
	static double acc_comfort_error_system;
	static double acc_comfort_var_system;

	//for each arduino
	double illuminance;
	double duty_cycle;
	bool occupancy;
	double lower_bound;
	double ext_illuminance;
	double illuminance_ref;
	double inst_power_desk;
	double acc_ener_desk;
	double acc_ener_system;
	double acc_comfort_error_desk;
	double acc_comfort_var_desk;
	std::vector<double> illuminance_min;
	std::vector<double> duty_cycle_min;



};

#endif