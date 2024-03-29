#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#include "uniquer_lib.h"

pthread_mutex_t counter_mutex;

const unsigned int SOCKADDR_LEN = sizeof(struct sockaddr_in);

void * handle_request(void *arg) {
	uniquer_request req = *(uniquer_request *)arg;
	unsigned long long id;
	char resp[255];
	bzero(resp, sizeof(resp));
	if (strcmp(req.question, "n\n")==0) {
		pthread_mutex_lock(req.counter_mutex);
		uniquer_get_next_id(req.c_data, &id);
		pthread_mutex_unlock(req.counter_mutex);
		sprintf(resp, "%llu", id);
	} else {
		sprintf(resp, "Uknown Command");
	}
	sendto(*(req.sock), &resp, 255, 0, (struct sockaddr *)&req.cli_name,
		SOCKADDR_LEN);
	return arg;
}

int main(int argc, char **argv) {
	//constants
	const unsigned int DEFAULT_SAVE_EVERY = 10000;
	const char * DEFAULT_PATH = "counter.txt";
	const short int DEFAULT_PORT = 9999;

	//options
	char * opt_path = malloc(strlen(DEFAULT_PATH));
	opt_path = strdup(DEFAULT_PATH);
	short int opt_port = DEFAULT_PORT;
	unsigned int opt_save_every = DEFAULT_SAVE_EVERY;

	//parse options
	int ch;
	opterr = 0;
	while ((ch = getopt(argc, argv, "p:f:s:")) != -1) {
		switch (ch) {
			case 'p': {
				opt_port = atoi((char *)optarg);
				break;
			}
			case 'f': {
				opt_path = strdup((char *)optarg);
				break;
			}
			case 's': {
				opt_save_every = atoi((char *)optarg);
				break;
			}
		}
	}
	//one file descriptor
	FILE *fd;

	//socket/request stuff
	int sock;
	struct sockaddr_in serv_name;
	char question[255];

	//business logic vars
	unsigned long long counter = 0, last_save = 0;
	uniquer_counter_data c_data;
	//initialize counter_data
	c_data.counter = &counter;
	c_data.last_save = &last_save;
	c_data.path = strdup(opt_path);
	c_data.save_every = opt_save_every;
	c_data.fd = fd;
	uniquer_counter_init(&c_data);

	//initialize TEH SOCKET!
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf("FAIL!");
		exit(1);
	}
	bzero(&serv_name, sizeof(serv_name));
	serv_name.sin_family = AF_INET;
	serv_name.sin_port = htons(opt_port);
	if (bind(sock, (struct sockaddr *)&serv_name, SOCKADDR_LEN) < 0) {
		printf("Couln't bind :( \n");
		exit(1);
	}

	pthread_mutex_init(&counter_mutex, NULL);

	for (;;) {
		pthread_t thread;
		uniquer_request req;
		bzero(question, sizeof(question));
		recvfrom (sock, question, 255, 0, (struct sockaddr *)&req.cli_name,
			(unsigned int *)&SOCKADDR_LEN);
		//pass pointer to the counter to threads
		req.c_data = &c_data;

		//pass socket info to thread
		req.sock = &sock;

		//copy the request string in case I want to do something with it later
		req.question = strdup(question);

		//pass pointer to mutex to threads
		req.counter_mutex = &counter_mutex;

		pthread_create(&thread, NULL, handle_request, &req);
	}
	//here just in case I add signal handlers for graceful exits later
	fclose(fd);
	pthread_mutex_destroy(&counter_mutex);
	pthread_exit(NULL);
}
