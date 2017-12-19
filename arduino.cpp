#include "arduino.h"

arduino::arduino(std::string ard_id)
: occupancy(true),
num_obs(0),
transient(1),
arduino_ID(ard_id),
l_buffer(3000),
d_buffer(3000),
time_buffer(3000)
{}

//bool arduino::restart = false;
extern std::vector<arduino*> arduino_list;

float arduino::inst_power_system = 0;
float arduino::acc_ener_system = 0;
float arduino::acc_comfort_error_system = 0;
float arduino::acc_comfort_var_system = 0;
float arduino::init_time = 0;
float arduino::elapsed_time = 0;
bool arduino::restart = 0;

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
	//std::cout << nSpan << "\n";
	return nSpan;
}


void arduino::parse_i2c(std::string i2c_msg) {
	//std::cout << "parsing i2c\n";
	
	//id lux pwm lower_bound ext_illuminance lux_ref
	//std::cout << "msg inicio:" << i2c_msg <<" -> " <<i2c_msg.length() << "\n";
	std::string treated_string;

	treated_string= i2c_msg.substr(0,3)+" "+i2c_msg.substr(3,3)+"."+i2c_msg.substr(6,2)+" "+
	i2c_msg.substr(8,3)+" "+i2c_msg.substr(11,3)+" "+
	i2c_msg.substr(18,3)+"."+i2c_msg.substr(21,2)+" "+i2c_msg.substr(23,3)+"."+i2c_msg.substr(26,2);

	i2c_msg = treated_string;
	//std::cout << "msg :" << i2c_msg << "\n";

	std::vector<std::string> strs;
	boost::split(strs,i2c_msg,boost::is_any_of(" "));

	if (restart == 1) {
		inst_power_system = 0;
		acc_ener_system = 0;
		acc_comfort_error_system = 0;
		acc_comfort_var_system = 0;
		init_time = 0;
		elapsed_time = 0;
		restart = 0;
	}

	if (this->num_obs == 0) {
		init_time = getMilliCount();
	}
	else {
		elapsed_time = getMilliSpan(init_time);
	}


	if (this->num_obs > 1){
		this->prev2_duty_cycle = this->prev_duty_cycle;
		this->prev_delta_time = this->delta_time;
	}
	
	if(this->num_obs > 0){
		this->prev_duty_cycle = this->duty_cycle;
		this->delta_time = getMilliSpan(this->time_1);
		//std::cout << this->delta_time << "\n";
	}
	//std::cout << "fora " << this->delta_time << "\n";
	std::string::size_type sz;

	this->illuminance = std::stof(strs[1], &sz);
	this->duty_cycle = std::stof(strs[2], &sz);
	this->lower_bound = std::stof(strs[4], &sz);
	this->ext_illuminance = std::stof(strs[5], &sz);
	float lux_ref = std::stof(strs[3], &sz);
	//std::cout << "ill :" << this->illuminance <<" pwm  "<< this->duty_cycle<<  "\n";
	if (this->illuminance_ref != lux_ref)
	{
		this->illuminance_ref = lux_ref;
		this->transient = 1;
		this->prev_time_transient = getMilliCount();
	}
	
	calc_inst_power_desk();
	update_acc_ener_desk();

	if (this->transient == 0)
	{
		this->num_obs += 1;
		update_acc_comfort_error_desk();
		update_acc_comfort_var_desk();
	}else{
		this->time_transient = getMilliSpan(this->prev_time_transient);
		//std::cout << "time_transient " << this->time_transient << "\n";
		if (this->time_transient > 100)
		{
			this->transient = 0;
		}
	}
	
	inst_power_system = calc_inst_power_system(::arduino_list);
	acc_ener_system = calc_acc_ener_system(::arduino_list);
	acc_comfort_error_system = calc_acc_comfort_error_system(::arduino_list);
	acc_comfort_var_system = calc_acc_comfort_var_system(::arduino_list);
	this->time_1 = getMilliCount();

	//this->l_buffer.push_back(this->illuminance);
	this->l_buffer.push_back(this->illuminance);
	this->d_buffer.push_back(this->duty_cycle);
	this->time_buffer.push_back(elapsed_time);

	std::cout << "l_buffer: " << this->l_buffer.back() << " d_buffer " << this->d_buffer.back() << " time_buffer " << this->time_buffer.back() << "\n"; 
	std::cout << "num_obs " << this->num_obs << " elapsed_time " << elapsed_time << "\n";
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
		this->acc_ener_desk += this->inst_power_desk*this->delta_time/1000;
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
	float aux =  this->illuminance_ref-this->illuminance;
	if (aux > 0)
	{
		this->acc_comfort_error_desk += aux;
	}else{
		this->acc_comfort_error_desk += 0;
	}
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
		float ts = 0.02*0.02;
		float sum = this->duty_cycle - 2*(this->prev_duty_cycle) + this->prev2_duty_cycle;
		if (sum < 0)
		{
			sum = sum * (-1);
		}
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