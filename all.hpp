#include "cache.hpp"
#include "socket.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
//#include "global.hpp"
//extern std::ofstream logFile;
extern Cache cache; 
struct serverCacheBundle{
    struct sockaddr_in clientAddr;
    int acceptFd;
  //    Cache * s_cache;
  //    pthread_mutex_t scb_lock;
    int tId;
};

string currentTime();
void * handleFunction(void * args);
