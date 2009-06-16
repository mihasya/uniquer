#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
    unsigned long long *counter;
    unsigned long long *last_save;
    short int save_every;
    char *path;
    FILE *fd;
} uniquer_counter_data;

typedef struct {
    uniquer_counter_data *c_data;
    int *sock;
    struct sockaddr_in cli_name;
    char *question;
    pthread_mutex_t *counter_mutex;
} uniquer_request;

int uniquer_counter_init(uniquer_counter_data *c_data);
int uniquer_get_next_id(uniquer_counter_data *c_data, unsigned long long *id);
int uniquer_save_counter(uniquer_counter_data *c_data);
