all: async_tcp_client server

async_tcp_client: async_tcp_client.cpp 
	g++ -std=c++11 async_tcp_client.cpp -o async_tcp_client -lpthread -lboost_system -lboost_thread

server: server.cpp
	g++ -std=c++11 server.cpp client_msg.cpp arduino.cpp serial_send.cpp i2c_receive.cpp -o server -pthread -lpthread -lboost_system -lboost_thread -lpigpio -lrt -lstdc++
