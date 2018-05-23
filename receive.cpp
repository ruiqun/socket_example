//
// Created by h45yang on 7/16/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include "constants.h"
#include <pthread.h>
#include <signal.h>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;

int  listen_sock_fd;
int  client_sock_fd;

static const float EPSILON = 0.001;

void error(string err)
{
    cerr << err << endl;
    exit(1);
}

// Communicating with IPC by socket
void *receive(void *pVoid)
{

    char buffer[256];
    /*************** Receive and Process Client Data *************/

    struct sockaddr_in cli_addr = {};
    socklen_t clilen = sizeof(cli_addr);
    int n;
    while (true)
    {
        client_sock_fd = accept(listen_sock_fd,
                                (struct sockaddr*) &cli_addr,
                                &clilen);
        if (client_sock_fd < 0)
            error("ERROR on accept");

        *buffer = {};
        
        n = read(client_sock_fd, buffer, 255);
        if (n < 0) error("ERROR reading from socket");

        if (string(buffer) == "")
            break;
            
        cout << "Here is the message : " << buffer << endl;
        usleep(1000*100);
        close(client_sock_fd);
    }
}

void myHandler(int signal) {
    printf("\nThis program will be closed! signal = %d\n",signal);
    close(listen_sock_fd);
    close(client_sock_fd);
    exit(1);
}


int main(void)
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = myHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    struct sockaddr_in serv_addr = {};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERVER_PORT);

    listen_sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock_fd < 0)
        error("ERROR opening socket");
    int  tmp;
    setsockopt(listen_sock_fd,SOL_SOCKET, SO_REUSEADDR, (char *)&tmp, sizeof(int));

    int bound = bind(listen_sock_fd,
                     (struct sockaddr *) &serv_addr,
                     sizeof(serv_addr));
    if (bound < 0)
    {
        error("ERROR on binding");
    }
    cout << "listen: " << listen(listen_sock_fd, 5) << endl;

    pthread_t  rec_thread;
    
    int ret = pthread_create (&rec_thread, NULL, receive ,NULL);
    if(ret != 0)
    {
        std::cout<<"pthread_create error: error_code="<<ret<<std::endl;
    }
    if(ret != 0)
    {
        std::cout<<"pthread_create error: error_code="<<ret<<std::endl;
    }
    pthread_exit (NULL);
}
