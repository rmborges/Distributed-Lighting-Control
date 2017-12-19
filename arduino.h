#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <boost/circular_buffer.hpp>
#include <boost/algorithm/string.hpp>

#include <cstdlib> //tempo
#include <sys/timeb.h>


using namespace boost::asio;
using ip::tcp;
using boost::asio::deadline_timer;



class arduino {

public:
	arduino(std::string ard_id);

	//setters
	//void set_restart(bool res) {restart = res;}
	//void set_occupancy(bool occup) {occupancy = occup;}

	void parse_i2c(std::string i2c_msg);

	//calculations
	void calc_inst_power_desk();
	float calc_inst_power_system(std::vector<arduino*> arduino_list);
	void update_acc_ener_desk();
	float calc_acc_ener_system(std::vector<arduino*> arduino_list);
	void update_acc_comfort_error_desk();
	float calc_acc_comfort_error_system(std::vector<arduino*> arduino_list);
	void update_acc_comfort_var_desk();
	float calc_acc_comfort_var_system(std::vector<arduino*> arduino_list);

//private:
	// common to all arduinos
	static bool restart; // necessário? ou enviar so mensagem?
	static float inst_power_system;
	static float acc_ener_system;
	static float acc_comfort_error_system;
	static float acc_comfort_var_system;

	static float init_time;
	static float elapsed_time;


	//for each arduino
	std::string arduino_ID; // ID do arduino   trocar para inteiro?----------------------
	int num_obs; // numero de observaçoes recolhidas
	int delta_time;
	int time_1;
	int prev_delta_time;
	int prev_duty_cycle;
	int prev2_duty_cycle;

	int transient;
	int time_transient;
	int prev_time_transient;

	float illuminance; //LDR
	float duty_cycle; //PWM
	bool occupancy;
	float lower_bound; // lux_ref - delta ?
	float ext_illuminance; // LDR com luzes desligadas
	float illuminance_ref; // lux_ref
	float inst_power_desk;
	float acc_ener_desk;
	float acc_comfort_error_desk;
	float acc_comfort_var_desk;
	std::vector<float> illuminance_min;
	std::vector<float> duty_cycle_min;
	/*std::vector<float> l_buffer;
	std::vector<float> d_buffer;*/
	boost::circular_buffer<float> l_buffer;
	boost::circular_buffer<float> d_buffer;
	boost::circular_buffer<float> time_buffer;
	//boost::circular_buffer<float> l_buffer(3000);
	//boost::circular_buffer<float> d_buffer(3000);
	//boost::circular_buffer<float> time_buffer(3000); // keeps arduino time instants

};

#endif

//CODIGO ANTIGO
// getters
	/*bool get_restart() {return restart;}
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
	std::vector<float> get_d_buffer() {return d_buffer;}*/
