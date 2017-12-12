FLAGS=-std=c++11 -lpthread -lboost_system -lboost_thread -lpigpio -lrt -lstdc++

all: serial_send.o i2c_receive.o arduino.o messages.o server_prof_multi

serial_send.o: serial_send.cpp
	g++ serial_send.cpp $(FLAGS)

server_prof_multi: serial_send.o i2c_receive.o arduino.o messages.o server_prof_multi.cpp
	g++ server_prof_multi.cpp serial_send.o i2c_receive.o arduino.o messages.o -o server_prof_multi $(FLAGS)
	
i2c_receive.o: i2c_receive.cpp
	g++ i2c_receive.cpp $(FLAGS)
	
arduino.o: arduino.cpp
	g++ arduino.cpp $(FLAGS)
	
messages.o: messages.cpp
	g++ messages.cpp $(FLAGS)