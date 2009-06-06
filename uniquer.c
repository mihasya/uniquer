#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#include "uniquer_lib.h"

#define PORT 9999

void * handle_request(void *arg) {
	request *req = (request*)arg;
	int id;
	char resp[255];
	bzero(resp, sizeof(resp));
	id = get_next_id(req->counter);
	sprintf(resp, "%d", id);
	sendto(*(req->sock), &resp, 255, 0, (struct sockaddr *)(req->serv_name), *(req->len));
}

int main() {
	int sock;
	size_t len;
	struct sockaddr_in serv_name;
	char question[255];

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
		int id;
		pthread_t thread;
		request req;

		bzero(question, sizeof(question));

		recvfrom (sock, &question, 255, 0, (struct sockaddr *)&serv_name, (unsigned int *)&len);

		req.counter = &counter;
		req.sock = &sock;
		req.len = &len;
		req.serv_name = &serv_name;
		strcpy(req.question, question);
		pthread_create(&thread, NULL, handle_request, &req);
	}
}