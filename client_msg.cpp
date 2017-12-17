#include "client_msg.h"

client_msg::client_msg()
{}


void client_msg::print_message(std::string message, char * msg_to_client){

	std::cout << "\nno messafes: "<< message << std::endl;

	msc_to_client = "oii, tudo bem?\n";
}
