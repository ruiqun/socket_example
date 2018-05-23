#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include "constants.h"

using namespace std;

void error(string err)
{
    cerr << err << endl;
    exit(1);
}

int main()
{
    struct hostent* server;
    server = gethostbyname(SERVER_IP);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    struct sockaddr_in serv_addr = {};
    serv_addr.sin_family = AF_INET;
    memcpy(server->h_addr_list[0],
           &serv_addr.sin_addr.s_addr,
           server->h_length);
    serv_addr.sin_port = htons(SERVER_PORT);
    
    int n;
    char rec_buf[255];
    while (true)
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd < 0)
        {
            error("ERROR opening socket");
        }
        
        int connected = connect(sockfd,
                                (struct sockaddr*) &serv_addr,
                                sizeof(serv_addr));
        if (connected < 0)
        {
            error("ERROR connecting");
        }
        
        string send_str;
        send_str.clear ();

        printf("Please enter the message: \n");
        cin >> send_str;
        n = write(sockfd, send_str.c_str(), send_str.length()+1);
        cout << "sent int:" << send_str << ", " << n << " bytes"<<endl << endl;
        if (n < 0)
        {
            error("ERROR writing to socket");
        }

        if (send_str == "") break;
        // When done with connection, close it
        close(sockfd);
    }
    
    return 0;
}
