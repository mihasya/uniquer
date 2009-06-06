typedef struct {
    unsigned long *counter;
    int *sock;
    size_t *len;
    struct sockaddr_in *serv_name;
    char *question;
} request;

int counter_init(unsigned long *counter);
int get_next_id(unsigned long *counter);