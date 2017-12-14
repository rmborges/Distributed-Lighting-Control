#include "arduino.h"

arduino::arduino()
: occupancy(true)
{}

bool arduino::restart = false;

void arduino::calc_inst_power_desk() {
	inst_power_desk = duty_cycle/255*1;		// %PWM * nominal power (1W)
}

void arduino::calc_inst_power_system() {
	
}

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}