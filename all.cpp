#include "all.hpp"
//#include "global.hpp"
std::ofstream logFile("/home/ml607/http_2.21/proxy.log");
Cache cache(1000); 
pthread_mutex_t cache_lock = PTHREAD_MUTEX_INITIALIZER;
string currentTime() {
    time_t now = time(0);
    char * dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    return string(dt);
    //cout << string(dt) << endl;
}

void * handleFunction(void * args){
    serverCacheBundle svcBundle = *((serverCacheBundle *)args);
    serverBundle svBundle;
    svBundle.acceptFd = svcBundle.acceptFd;
    svBundle.clientAddr = svcBundle.clientAddr;
    svBundle.tId = svcBundle.tId;
    //Cache * cache = (svcBundle.s_cache);
    //pthread_mutex_t cache_lock = svcBundle.scb_lock;
    
    std::vector<std::vector<char> > accInfo = acceptProcessFromServer(svBundle);
    int accServerfd = svBundle.acceptFd;
    std::vector<char> clientIpAddr = accInfo[0];
    std::vector<char> clientText = accInfo[1];
    std::vector<std::vector<char> > requestInfo = parseHttpRequest(clientText);
    int method = requestInfo[0][0];
    std::vector<char> requestLines = requestInfo[1];
    std::vector<char> serverIpAddr = requestInfo[2];
    std::cout << "!!!!!!! method is: " << method << "\n";
    int port;
    if(method==0 || method==1){ //GET, POST
        port = 80;
    }
    else{ //CONNECT
        port = 443;
    }

    Client c(serverIpAddr.data(), port);
    int clientSockfd = c.connectFromClient(); 
    c.sendFromClient(clientSockfd, requestLines);

    Request input(clientText);
    cout << "@@@@@@ request: " <<input.getRequestLines() << "@@@@@@@" << endl;
    if(method==0 || method==1){
      logFile << svBundle.tId << ": " << input.getRequestLine() << " from " << clientIpAddr.data() << " @ " << currentTime() << endl;
      
        if((cache).inCache(input)) {
            cout << "~~~~~~~~~~~in cache~~~~~~~~~~~~" << endl;
	    logFile << svBundle.tId << "in cache, ";
	    //            pthread_mutex_lock(&cache_lock);
            Response rsp = cache.get(input);
            //pthread_mutex_unlock(&cache_lock);
            cout << "----------------old response header:"<< rsp.getHeader()<< "\n------------"<< endl;
            if(rsp.needRevalidation()) {
	      rsp = cache.revalidation(input, rsp, clientSockfd, svBundle.tId);
            }
            else {
               //logfile
	      logFile << "valid" <<endl;
            }
            
            send(accServerfd, rsp.getResponse().data(), rsp.getResponse().size(), 0);
        }
        else{
            cout << "not in cache." << endl;
            std::vector<std::vector<char> > responseInfo = sendDataByGetPost(clientSockfd, accServerfd);
            std::vector<char> responseLines = responseInfo[0];
            std::vector<char> responseBody = responseInfo[1];
            //cout << "***responseLines: " << responseLines.data() << "*********" << endl;
            cout << "***response_fulltext: " << responseBody.data() << "**********" <<endl;
            Response newrsp(responseBody);
            //pthread_mutex_lock(&cache_lock);
            (cache).storeResponse(input, newrsp, svBundle.tId);
            //pthread_mutex_unlock(&cache_lock);
            cout << "store in cache" << endl;
        }
     
      //std::vector<std::vector<char> > responseInfo = sendDataByGetPost(clientSockfd, accServerfd);
      //std::vector<char> responseLines = responseInfo[0];
      //std::vector<char> responseBody = responseInfo[1];
    }
    else{
        //do something
    }
    
    close(accServerfd);
    close(clientSockfd);

    return NULL;
}
