#include "client_msg.h"
#include "arduino.h"

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
	int flag = -1;

	if (cmd.compare("r") == 0) 
	{
		msg_to_client = "ack";
		// RESTART - remover objetos arduino no RPi
		arduino reset("restart");
		reset.restart = 1; // mudar boolean para reiniciar variaveis static da classe arduino
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
		arduino* pt_ard;
		for (auto ard : arduino_list) {
			if (ard->arduino_ID.compare(strs[2])==0){
				std::cout << "sou o arduino " << ard->arduino_ID << "---" << strs[2] << std::endl;
				count +=1;
				pt_ard = ard;
				break;
			}
		}

		if (number && count > 0) //get data from one arduino
		{

			switch(strs[1].at(0)){
				case 'l': 		
				msg_to_client = "l "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->illuminance);
				break;
				case 'd': 
				msg_to_client = "d "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->duty_cycle);
				break;
				case 'o': 
				if (pt_ard->occupancy == true)
				{
					flag = 1;
				}else if (pt_ard->occupancy == false)
				{
					flag = 0;
				}
				msg_to_client = "o "+pt_ard->arduino_ID+" "+std::to_string(flag);
				break;
				case 'L': 
				msg_to_client = "L "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->lower_bound);
				break;
				case 'O': 
				msg_to_client = "O "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->ext_illuminance);
				break;
				case 'r': 
				msg_to_client = "r "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->illuminance_ref);
				break;
				case 'p': 
				msg_to_client = "p "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->inst_power_desk);
				break;
				case 'e': 
				msg_to_client = "e "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->acc_ener_desk);
				break;
				case 'c': 
				msg_to_client = "c "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->acc_comfort_error_desk);
				break;
				case 'v': 
				msg_to_client = "v "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->acc_comfort_var_desk);
				break;
				case 't':
				msg_to_client = "t "+pt_ard->arduino_ID+" "+std::to_string(pt_ard->elapsed_time);
				break;
			}

		}
		else //get data from all arduinos
		{
			switch(strs[1].at(0)){
				case 'p': 
				msg_to_client = "p T "+std::to_string(pt_ard->inst_power_system);
				break;
				case 'e':
				msg_to_client = "e T "+std::to_string(pt_ard->acc_ener_system);
				break;
				case 'c': 
				msg_to_client = "c T "+std::to_string(pt_ard->acc_comfort_error_system);
				break;
				case 'v':
				msg_to_client = "v T "+std::to_string(pt_ard->acc_comfort_var_system); 
				break;
			}
		}
	}
	else if (cmd.compare("s") == 0)
	{
		arduino* pt_ard_s;
		for (auto ard : arduino_list) {
			if (ard->arduino_ID.compare(strs[2])==0){
				pt_ard_s = ard;
				break;
			}
		}
		if (strs[2].compare("0") == 0)
		{
			pt_ard_s->occupancy = false;
		}else{
			pt_ard_s->occupancy = true;
		}
		
		msg_to_client = "ack";


	}
	else if (cmd.compare("b") == 0)
	{
		int count = 0;
		arduino* pt_ard;
		for (auto ard : arduino_list) {
			if (ard->arduino_ID.compare(strs[2])==0){
				std::cout << "sou o arduino " << ard->arduino_ID << "---" << strs[2] << std::endl;
				count +=1;
				pt_ard = ard;
				break;
			}
		}

		if (strs[1].compare("l") == 0) {
			msg_to_client = "b l "+strs[2]+" ";
			auto it = pt_ard->l_buffer[0];
			int pos = 0;
			while (it != pt_ard->l_buffer.back()) {
				msg_to_client += std::to_string(it);
				msg_to_client += ", ";
				pos++;
				it = pt_ard->l_buffer[pos];
			}
			/*for (auto it = pt_ard->l_buffer[0]; it != pt_ard->l_buffer.back(); ) {
				msg_to_client += std::to_string(*it);
				msg_to_client += ", ";
				it = pt_ard->l_buffer[0]
			}*/
			msg_to_client.erase(msg_to_client.end()-2, msg_to_client.end()); // elimina ", " a mais no fim
			//+ std::to_string(pt_ard->acc_comfort_var_system);
		}
	}
	else if (cmd.compare("c") == 0)
	{
		//para continuar----------------------------
	}
	else if (cmd.compare("d") == 0)
	{
		//para continuar----------------------------
		msg_to_client = "ack";
	}



	//msg_to_client = "oiiii galera, tudo bem?\n";

	return msg_to_client;
}
