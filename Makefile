all: client_prof server_prof_multi

client_prof: client_prof.cpp 
	g++ -std=c++11 client_prof.cpp -o client_prof -lpthread -lboost_system -lboost_thread

server_prof_multi: server_prof_multi.cpp
	g++ -std=c++11 server_prof_multi.cpp serial_send.cpp i2c_receive.cpp arduino.cpp client_msg.cpp -o server_prof_multi -pthread -lpthread -lboost_system -lboost_thread -lpigpio -lrt -lstdc++
