#include <string>
#include <iostream>
#include <future>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "18095"    // the port users will be connecting to
#define MAXPLAYERS 64
#define MAXBUFLEN 100

bool quit = 0;

struct player {
	//unsigned long id;
	unsigned long id;
	float x;
	float z;
	sockaddr* addr;
};

struct player players [MAXPLAYERS];
char* substr(const char* source, int from, int to){
	char* target;
	for(int i = 0; to <= from; ++to, ++i){
		target[i] = source[to];
	}
	return target;
}
void ParsePacket(const char* msg, struct sockaddr* their_addr){
	switch(msg[0]){
		case 'c': // connect header
			{
				/*int i = 0;
				for (; players[i].id != NULL; ++i);
				srand(time(NULL));
				players[i].id = rand()%4294967295;*/
				int i = 0;
				for (; players[i].addr != NULL; ++i);
				players[i].addr = their_addr;
				std::cout << "Player connected\n";
				break;
			}
		case 'p': //position change header
			{
				int i = 0;
				for (; players[i].addr != their_addr; ++i);
				std::string msgg(msg);
				size_t xLocsizet = msgg.find('x');
				size_t zLocsizet = msgg.find('z');
				if (xLocsizet == std::string::npos || zLocsizet == std::string::npos){
					std::cout << "Got an invalid packet\n";	
					break;
				}
				int xLoc = static_cast<int>(xLocsizet);
				int zLoc = static_cast<int>(zLocsizet);
				players[i].x = atof((msgg.substr(xLoc + 1, zLoc - xLoc - 1)).c_str());
				players[i].z = atof((msgg.substr(zLoc + 1, msgg.length() - zLoc - 1)).c_str());
				std::cout << "Got position change x " << players[i].x << " z " << players[i].z << "\n";
				break;
			}
		case 'q':
			{
				std::cout << "Remote quit signal received\n";
				quit = true;
			}
	}
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void listen_(int sockfd){
	struct sockaddr their_addr;
	socklen_t addr_len;
	addr_len = sizeof their_addr;
	int numbytes;
	char buf[MAXBUFLEN];
	char s[INET6_ADDRSTRLEN];
	while(!quit){
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
			&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			//exit(1);
		}
		ParsePacket(buf, &their_addr);
		printf("listener: got packet from %s\n",
			inet_ntop(their_addr.sa_family,
				get_in_addr(&their_addr),
				s, sizeof s));
		printf("listener: packet is %d bytes long\n", numbytes);
		buf[numbytes] = '\0';
		printf("listener: packet contains \"%s\"\n", buf);
	}
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");
	auto future = std::async(listen_,sockfd);
	while (!quit){
		//send commands to every player every 50 ms
		for (int i = 0; i < MAXPLAYERS; ++i){
			
		}
	}
    future.get(); //joins threead to main
    close(sockfd);

    return 0;
}