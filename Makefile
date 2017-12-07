all: chat_client chat_server client_prof server_prof_multi

chat_client: chat_client.cpp chat_message.hpp
	g++ -std=c++11 chat_client.cpp -o chat_client -lpthread -lboost_system -lboost_thread

chat_server: chat_server.cpp chat_message.hpp
	g++ -std=c++11 chat_server.cpp -o chat_server -lpthread -lboost_system -lboost_thread

client_prof: client_prof.cpp
	g++ -std=c++11 client_prof.cpp -o client_prof -lpthread -lboost_system -lboost_thread

server_prof_multi: server_prof_multi.cpp
	g++ -std=c++11 server_prof_multi.cpp -o server_prof_multi -lpthread -lboost_system -lboost_thread