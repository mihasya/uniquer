#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>


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
    size_t *len;
    struct sockaddr_in *serv_name;
    char *question;
    pthread_mutex_t *counter_mutex;
} request;

int counter_init(counter_data *c_data);
int get_next_id(counter_data *c_data, unsigned long long *id);
int save_counter(counter_data *c_data);
