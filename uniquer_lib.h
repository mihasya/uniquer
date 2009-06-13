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
} counter_data;

typedef struct {
    counter_data *c_data;
    int *sock;
    struct sockaddr_in cli_name;
    size_t cli_name_len;
    char *question;
    pthread_mutex_t *counter_mutex;
} request;

int counter_init(counter_data *c_data);
int get_next_id(counter_data *c_data, unsigned long long *id);
int save_counter(counter_data *c_data);
