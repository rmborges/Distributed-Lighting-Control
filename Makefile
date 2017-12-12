all: client_prof server_prof_multi i2c_final

client_prof: client_prof.cpp 
	g++ -std=c++11 client_prof.cpp -o client_prof -lpthread -lboost_system -lboost_thread

server_prof_multi: server_prof_multi.cpp
	g++ -std=c++11 server_prof_multi.cpp serial_send.cpp -o server_prof_multi -lpthread -lboost_system -lboost_thread

i2c_final: i2c_final.cpp
	gcc -std=c++11 -o i2c_final i2c_final.cpp -lpigpio -lrt -lstdc++
