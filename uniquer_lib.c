#include <stdio.h>
#include <string.h>
#include "uniquer_lib.h"

int counter_init(counter_data *c_data) {
	if (*(c_data->counter) == 0) {
		FILE *fd;
		char counter_str[40];
		if ((fd = fopen(c_data->path, "r+")) == NULL) {
			fd = fopen(c_data->path, "w+");
		}
		fgets(counter_str, 45, fd);
		if (strcmp(counter_str, "") == 0) {
			sprintf(counter_str, "%llu\n", (unsigned long long)1);
			*(c_data->counter) = 1;
			save_counter(c_data, fd);
		} else {
			*(c_data->counter) = *(c_data->last_save) = (atoi(counter_str) + c_data->save_every);
			++*(c_data->counter);
		}
		fclose(fd);
		return 0;
	} else {
		return 1;
	}
	return 0;
}
int save_counter(counter_data *c_data, FILE *fd) {
	//TODO: something funky happens here
	char counter_str[40];
	sprintf(counter_str, "%llu\n", *(c_data->counter));
	fputs(counter_str, fd);
	*(c_data->last_save) = *(c_data->counter);
}
int get_next_id(counter_data *c_data, unsigned long long *id) {
	if (*(c_data->counter) == 0) {
		counter_init(c_data);
		*id = *(c_data->counter);
	} else {
		*id = ++(*(c_data->counter));
	}
	if (*id >= (*(c_data->last_save)+c_data->save_every)) {
		FILE *fd = fopen(c_data->path, "w+");
		save_counter(c_data, fd);
		fclose(fd);
	}
	return 0;
}
