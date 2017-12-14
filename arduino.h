#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;

class arduino {

public:
	arduino();

	// getters
	bool get_restart() {return restart;}
	float get_inst_power_system() {return inst_power_system;}
	float get_acc_comfort_error_system() {return acc_comfort_error_system;}
	float get_acc_comfort_var_system() {return acc_comfort_var_system;}
	float get_illuminance() {return illuminance;}
	float get_duty_cycle() {return duty_cycle;}
	bool get_occupancy() {return occupancy;}
	float get_lower_bound() {return lower_bound;}
	float get_ext_illuminance() {return ext_illuminance;}
	float get_illuminance_ref() {return illuminance_ref;}
	float get_inst_power_desk() {return inst_power_desk;}
	float get_acc_ener_desk() {return acc_ener_desk;}
	float get_acc_ener_system() {return acc_ener_system;}
	float get_acc_comfort_error_desk() {return acc_comfort_error_desk;}
	float get_acc_comfort_var_desk() {return acc_comfort_var_desk;}
	std::vector<float> get_illuminance_min() {return illuminance_min;}
	std::vector<float> get_duty_cycle_min() {return duty_cycle_min;}
	std::vector<float> get_l_buffer() {return l_buffer;}
	std::vector<float> get_d_buffer() {return d_buffer;}

	//setters
	void set_restart(bool res) {restart = res;}
	void set_occupancy(bool occup) {occupancy = occup;}

	//calculations
	void calc_inst_power_desk();
	void calc_inst_power_system();

private:
	// common to all arduinos
	static bool restart; // necessário? ou enviar so mensagem?
	static float inst_power_system;
	static float acc_comfort_error_system;
	static float acc_comfort_var_system;

	//for each arduino
	float illuminance; //LDR
	float duty_cycle; //PWM
	bool occupancy;
	float lower_bound; // lux_ref - delta ?
	float ext_illuminance; // LDR com luzes desligadas
	float illuminance_ref; // lux_ref
	float inst_power_desk;
	float acc_ener_desk;
	float acc_ener_system;
	float acc_comfort_error_desk;
	float acc_comfort_var_desk;
	std::vector<float> illuminance_min;
	std::vector<float> duty_cycle_min;
	std::vector<float> l_buffer;
	std::vector<float> d_buffer;

};

#endif