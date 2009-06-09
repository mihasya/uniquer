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
#define FILENAME 'counter.txt'

pthread_mutex_t counter_mutex;

void * handle_request(void *arg) {
	request *req = (request *)arg;
	unsigned long id;
	char resp[255];
	bzero(resp, sizeof(resp));
	if (strcmp(req->question, "n\n")==0) {
		pthread_mutex_lock(req->counter_mutex);
		get_next_id(req->c_data, &id);
		pthread_mutex_unlock(req->counter_mutex);
		sprintf(resp, "%d", id);

	} else {
		sprintf(resp, "Uknown Command");
	}
	sendto(*(req->sock), &resp, 255, 0, (struct sockaddr *)(req->serv_name), *(req->len));
}

int main() {
	int sock;
	size_t len;
	struct sockaddr_in serv_name;
	char question[255];
	unsigned long counter, last_save;

	counter_data c_data;

	//initialize TEH SOCKET!
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("FAIL!");
		exit(1);
	}
	bzero(&serv_name, sizeof(serv_name));
	serv_name.sin_family = AF_INET;
	serv_name.sin_port = htons(PORT);
	len = sizeof(serv_name);
	if (bind(sock, (struct sockaddr *)&serv_name, len) < 0) {
		printf("Couln't bind :( \n");
		exit(1);
	}
	len = sizeof(serv_name);

	pthread_mutex_init(&counter_mutex, NULL);

	for (;;) {
		pthread_t thread;
		request req;

		bzero(question, sizeof(question));
		recvfrom (sock, &question, 255, 0, (struct sockaddr *)&serv_name, (unsigned int *)&len);

		//initialize counter_data
		c_data.counter = &counter;
		c_data.last_save = &last_save;

		//pass pointer to the counter to threads
		req.c_data = &c_data;

		//pass socket info to thread
		req.sock = &sock;
		req.len = &len;
		req.serv_name = &serv_name;

		//copy the request string in case I want to do something with it later
		req.question = strdup(question);

		//pass pointer to mutex to threads
		req.counter_mutex = &counter_mutex;

		pthread_create(&thread, NULL, handle_request, &req);
	}
	pthread_mutex_destroy(&counter_mutex);
	pthread_exit(NULL);
}