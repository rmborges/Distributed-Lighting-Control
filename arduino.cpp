#include "arduino.h"

arduino::arduino()
: occupancy(true)
{}

bool arduino::restart = false;

void arduino::calc_inst_power_desk() {
	inst_power_desk = duty_cycle/255*1;		// %PWM * nominal power (1W)
}

float arduino::calc_inst_power_system(std::vector<arduino> arduino_list) {
	float result = 0;
	for(auto it = arduino_list.begin(); it != arduino_list.end(); ++it) {
		result += it->get_inst_power_desk();	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_ener_desk(float energy_desk) {
	acc_ener_desk += energy_desk;
}

float arduino::calc_acc_ener_system(std::vector<arduino> arduino_list) {
	float result = 0;
	for(auto it = arduino_list.begin(); it != arduino_list.end(); ++it) {
		result += it->get_acc_ener_desk();	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_comfort_error_desk(float comf_desk) {
	acc_comfort_error_desk += comf_desk;
}

float arduino::calc_acc_comfort_error_system(std::vector<arduino> arduino_list) {
	float result = 0;
	for(auto it = arduino_list.begin(); it != arduino_list.end(); ++it) {
		result += it->get_acc_comfort_error_desk();	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_comfort_var_desk(float comf_desk_var) {
	acc_comfort_error_desk += comf_desk_var;
}

float arduino::calc_acc_comfort_var_system(std::vector<arduino> arduino_list) {
	float result = 0;
	for(auto it = arduino_list.begin(); it != arduino_list.end(); ++it) {
		result += it->get_acc_comfort_var_desk();	// soma para os varios arduinos
	}
	return result;
}