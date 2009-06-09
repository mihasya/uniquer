#include <sys/types.h>
#include <sys/socket.h>


typedef struct {
    unsigned long *counter;
    unsigned long *last_save;
    char *path;
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
int get_next_id(counter_data *c_data, unsigned long *id);
