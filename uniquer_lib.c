#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uniquer_lib.h"

int counter_init(counter_data *c_data) {
	if (*(c_data->counter) == 0) {
		char counter_str[40];
		if ((c_data->fd = fopen(c_data->path, "r+")) == NULL) {
			c_data->fd = fopen(c_data->path, "w+");
		}
		fgets(counter_str, 40, c_data->fd);
		*(c_data->counter) = *(c_data->last_save) = atoi(counter_str);
		if (1 == strlen(counter_str)) {
			;
		} else {
			*(c_data->counter) = *(c_data->last_save) = *(c_data->counter) 
				+ c_data->save_every;
		}
		save_counter(c_data);
		return 0;
	} else {
		return 1;
	}
}
int save_counter(counter_data *c_data) {
	char counter_str[40];
	sprintf(counter_str, "%llu\n", *(c_data->counter));
	fseek(c_data->fd, 0, SEEK_SET);
	fputs(counter_str, c_data->fd);
	*(c_data->last_save) = *(c_data->counter);
	fflush(c_data->fd);
}
int get_next_id(counter_data *c_data, unsigned long long *id) {
	*id = ++(*(c_data->counter));
	if (*id >= (*(c_data->last_save)+c_data->save_every)) {
		save_counter(c_data);
	}
	return 0;
}
