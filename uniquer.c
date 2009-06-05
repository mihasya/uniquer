#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


#define PORT 9999

int main() {
	int sock;
	size_t len;
	struct sockaddr_in serv_name;
	char ques[255], resp[255];
	
	unsigned long counter = 0;
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("FAIL!");
		exit(1);
	}
	//initialize TEH SOCKET!
	bzero(&serv_name, sizeof(serv_name));
	serv_name.sin_family = AF_INET;
	serv_name.sin_port = htons(PORT);
	len = sizeof(serv_name);
	if (bind(sock, (struct sockaddr *)&serv_name, len) < 0) {
		printf("Couln't bind :( \n");
		exit(1);
	}
	len = sizeof(serv_name);
	for (;;) {
		//TODO: make this threaded
		//TODO: use libev?
		int id;
		bzero(ques, sizeof(ques));
		bzero(resp, sizeof(resp));
		recvfrom (sock, &ques, 255, 0, (struct sockaddr *)&serv_name, (unsigned int *)&len);
		id = get_next_id(&counter);
		sprintf(resp, "%d", id);
		sendto(sock, &resp, 255, 0, (struct sockaddr *)&serv_name, len);
	}
}