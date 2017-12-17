#include "arduino.h"

arduino::arduino(std::string ard_id)
: occupancy(true),
num_obs(0),
arduino_ID(ard_id)
{}

//bool arduino::restart = false;

void arduino::parse_i2c(std::string i2c_msg) {
	std::cout << "oi\n";
}

void arduino::calc_inst_power_desk() {
	inst_power_desk = duty_cycle/255*1;		// %PWM * nominal power (1W)
}

float arduino::calc_inst_power_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it :arduino_list) {
		result += it->inst_power_desk;	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_ener_desk(float energy_desk) {
	acc_ener_desk += energy_desk;
}

float arduino::calc_acc_ener_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it : arduino_list) {
		result += it->acc_ener_desk;	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_comfort_error_desk(float comf_desk) {
	acc_comfort_error_desk += comf_desk;
}

float arduino::calc_acc_comfort_error_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it : arduino_list) {
		result += it->acc_comfort_error_desk;	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_comfort_var_desk(float comf_desk_var) {
	acc_comfort_error_desk += comf_desk_var;
}

float arduino::calc_acc_comfort_var_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it : arduino_list) {
		result += it->acc_comfort_var_desk;	// soma para os varios arduinos
	}
	return result;
}
