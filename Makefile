server: socket.hpp global.hpp all.hpp all.cpp request.cpp rsp.cpp cache.cpp socket.cpp main.cpp
	g++ -Wall -Wextra global.hpp all.hpp all.cpp socket.hpp request.cpp rsp.cpp cache.cpp socket.cpp main.cpp -o server -g -lpthread
