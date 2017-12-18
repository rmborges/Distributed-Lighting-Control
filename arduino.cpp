#include "arduino.h"

arduino::arduino(std::string ard_id)
: occupancy(true),
num_obs(0),
arduino_ID(ard_id)
{}

//bool arduino::restart = false;
extern std::vector<arduino*> arduino_list;

float arduino::inst_power_system = 0;
float arduino::acc_ener_system = 0;
float arduino::acc_comfort_error_system = 0;
float arduino::acc_comfort_var_system = 0;

//codigo de http://www.cplusplus.com/forum/general/43203/
int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}
int getMilliSpan(int nTimeStart){
	int nSpan = getMilliCount() - nTimeStart;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}


void arduino::parse_i2c(std::string i2c_msg) {
	std::cout << "parsing i2c\n";
	std::vector<std::string> strs;
	boost::split(strs,i2c_msg,boost::is_any_of(" "));
	//id lux pwm lower_bound ext_illuminance lux_ref

	if (this->num_obs > 1){
		this->prev2_duty_cycle = this->prev_duty_cycle;
		this->prev_delta_time = this->delta_time;
	}
	
	if(this->num_obs > 0){
		this->prev_duty_cycle = this->duty_cycle;
		this->delta_time = getMilliSpan(this->time_1);
	}

	std::string::size_type sz;

	this->illuminance = std::stof(strs[1], &sz);
	this->duty_cycle = std::stof(strs[2], &sz);
	this->lower_bound = std::stof(strs[3], &sz);
	this->ext_illuminance = std::stof(strs[4], &sz);
	float lux_ref = std::stof(strs[5], &sz);
	if (this->illuminance_ref != lux_ref)
	{
		this->illuminance_ref = lux_ref;
		this->transient = 1;
		this->prev_time_transient = getMilliCount();
	}
	
	calc_inst_power_desk();
	update_acc_ener_desk();

	if (!(this->transient))
	{
		this->num_obs += 1;
		update_acc_comfort_error_desk();
		update_acc_comfort_var_desk();
	}else{
		this->time_transient = getMilliSpan(this->prev_time_transient);
		if (this->time_transient > 100)
		{
			this->transient = 1;
		}
	}
	
	inst_power_system = calc_inst_power_system(::arduino_list);
	acc_ener_system = calc_acc_ener_system(::arduino_list);
	acc_comfort_error_system = calc_acc_comfort_error_system(::arduino_list);
	acc_comfort_var_system = calc_acc_comfort_var_system(::arduino_list);
	this->time_1 = getMilliCount();

	std::cout << "lux + inst_power_system " << this->illuminance << " " << inst_power_system << "\n"; 
}

void arduino::calc_inst_power_desk() {
	this->inst_power_desk = (this->duty_cycle)/255*1;		// %PWM * nominal power (1W)
}

float arduino::calc_inst_power_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it :arduino_list) {
		result += it->inst_power_desk;	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_ener_desk() {
	if(this->num_obs <= 1){
		this->acc_ener_desk += 0;
	}else{
		this->acc_ener_desk += this->inst_power_desk*this->delta_time;
	}
}

float arduino::calc_acc_ener_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it : arduino_list) {
		result += it->acc_ener_desk;	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_comfort_error_desk() {
	this->acc_comfort_error_desk = this->acc_comfort_error_desk*(this->num_obs-1);
	this->acc_comfort_error_desk += this->illuminance_ref-this->illuminance;
	this->acc_comfort_error_desk = (this->acc_comfort_error_desk)/(this->num_obs);
}

float arduino::calc_acc_comfort_error_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it : arduino_list) {
		result += it->acc_comfort_error_desk;	// soma para os varios arduinos
	}
	return result;
}

void arduino::update_acc_comfort_var_desk() {
	if (this->num_obs > 1)
	{
		this->acc_comfort_var_desk = this->acc_comfort_var_desk*(this->num_obs-2);
		float ts = this->delta_time * this->prev_delta_time;
		float sum = this->duty_cycle - 2*(this->prev_duty_cycle) + this->prev2_duty_cycle;
		this->acc_comfort_var_desk += sum/ts;
		this->acc_comfort_var_desk = this->acc_comfort_var_desk/(this->num_obs-1);
	}
}

float arduino::calc_acc_comfort_var_system(std::vector<arduino*> arduino_list) {
	float result = 0;
	for(auto it : arduino_list) {
		result += it->acc_comfort_var_desk;	// soma para os varios arduinos
	}
	return result;
}
