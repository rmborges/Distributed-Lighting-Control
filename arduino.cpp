#include "arduino.h"

arduino::arduino()
: restart(false),
occupancy(true)
{

}


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
	double[BUFFER_SIZE] illuminance_min;
	double[BUFFER_SIZE] duty_cycle_min;