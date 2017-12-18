#include "client_msg.h"

client_msg::client_msg()
{}

extern std::vector<arduino*> arduino_list;

extern io_service io2;

std::string client_msg::print_message(std::string msg_from_client, std::string msg_to_client){

	char str_aux[100];
	memset(str_aux,0,sizeof(str_aux));

	std::cout << "\nmsg_from_client: "<< msg_from_client << std::endl;

	std::vector<std::string> strs;
	boost::split(strs,msg_from_client,boost::is_any_of(" "));
	std::string cmd = strs[0];
	if (cmd.compare("r") == 0) 
	{
		msg_to_client = "ack";
		//arduino.restart = true;
		// RESTART - remover objetos arduino no RPi
		for (auto it = arduino_list.begin(); it != arduino_list.end(); ) {
			delete * it;  
			it = arduino_list.erase(it);
		}
		// enviar comando pelo serial para RESTART
		serial_send msg_(io2);
		msg_.serial_write(MSG_RESET); // msg definida em client_msg.h
	}


	else if (cmd.compare("g") == 0)
	{
		//finds if strs[2] is a number
		bool number = std::all_of(strs[2].begin(),strs[2].end(), ::isdigit);
		int count = 0;
		int pt_ard = -1;
		for (auto ard : arduino_list) {
			if (ard->arduino_ID.compare(strs[2])){
				std::cout << "sou o arduino " << ard->arduino_ID << std::endl;
				count +=1;
				pt_ard = std::stoi(strs[2]);
				break;
			}
		}

		if (number && count > 0) //get data from one arduino
		{

			switch(strs[1].at(0)){
				case 'l': 
				//float carlos = arduino_list.at(pt_ard)->illuminance;
				//sprintf(str_aux, "l %s %f",strs[2], arduino_list.at(pt_ard)->illuminance);
				break;
				case 'd': 
				//sprintf(str_aux, "d %f %f",strs[2], pt_ard->duty_cycle);
				break;
				case 'o': 
				/*int flag = -1;
				if (pt_ard->occupancy == true)
				{
					flag = 1;
				}else if (pt_ard->occupancy == false)
				{
					flag = 0;
				}*/
				//sprintf(str_aux, "o %f %f",strs[1], flag);
					break;
					case 'L': 
				//sprintf(str_aux, "L %f %f",strs[1], pt_ard->lower_bound);
					break;
					case 'O': 
				//sprintf(str_aux, "O %f %f",strs[1], pt_ard->ext_illuminance);
					break;
					case 'r': 
				//sprintf(str_aux, "r %f %f",strs[1], pt_ard->illuminance_ref);
					break;
					case 'p': 
				//sprintf(str_aux, "p %f %f",strs[1], pt_ard->inst_power_desk);
					break;
					case 'e': 
				//sprintf(str_aux, "e %f %f",strs[1], pt_ard->acc_ener_desk);
					break;
					case 'c': 
				//sprintf(str_aux, "c %f %f",strs[1], pt_ard->acc_comfort_error_desk);
					break;
					case 'v': 
				//sprintf(str_aux, "v %f %f",strs[1], pt_ard->acc_comfort_var_desk);
					break;


				}

			}
		else //get data from all arduinos
		{
			switch(strs[1].at(0)){
				case 'p': 
				//sprintf(str_aux, "p T %f", arduno.calc_inst_power_system(arduino_list))
				break;
				case 'e':
				//sprintf(str_aux, "e T %f", arduno.calc_acc_ener_system(arduino_list)) 
				break;
				case 'c': 
				//sprintf(str_aux, "e T %f", arduno.calc_acc_comfort_error_system(arduino_list)) 
				break;
				case 'v': 
				//sprintf(str_aux, "v T %f", arduno.calc_acc_comfort_var_system(arduino_list)) 
				break;
			}
		}
	}
	else if (cmd.compare("s") == 0)
	{
		/*auto pt_ard;
		for (auto ard : arduino_list) {
			if (ard->arduino_ID == strs[2]){
				count +=1;
				pt_ard = ard;
				break;
			}
		}
		if (strs[2] == '0')
		{
			pt_ard->occupancy = false;
		}else{
			pt_ard->occupancy = true;
		}
		*/
		msg_to_client = "ack";


	}else if (cmd.compare("b") == 0)
	{
		//para continuar----------------------------
	}



/*
	if ( str_aux != 0 )
	{
		strcpy(msg_to_client, str_aux);
	}else{
		strcpy(msg_to_client, "oii, tudo bem?\n");
	}
*/
	msg_to_client = "oiiii galera, tudo bem?\n";

	return msg_to_client;
}
