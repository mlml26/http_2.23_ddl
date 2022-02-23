#include "all.hpp"

int main(void){
    Server s("152.3.77.9", 12345);
    int serverSockfd = s.connectListenFromServer();
    int tId = 0;
    ofstream logfile("/home/ml607/http_2.21/proxy.log");
    //    Cache cache(1000);
     while(1){
        tId++;
        serverBundle sv = acceptRequestFromClient(serverSockfd);
        serverCacheBundle svc;
        svc.clientAddr = sv.clientAddr;
        svc.acceptFd = sv.acceptFd;
	// svc.s_cache = &cache;
        //svc.scb_lock = PTHREAD_MUTEX_INITIALIZER;
	//        pthread_t threadId;
	svc.tId = tId;
		handleFunction(&svc);
		//pthread_create(&threadId, NULL, handleFunction, &svc);
		// pthread_detach(threadId);
        
    }

    return EXIT_SUCCESS;
}
