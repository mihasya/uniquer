#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uniquer_lib.h"

/**
 * initialize the counter struct
 */
int uniquer_counter_init(uniquer_counter_data *c_data) {
	if (*(c_data->counter) == 0) {
		char counter_str[40];
		if ((c_data->fd = fopen(c_data->path, "r+")) == NULL) {
			c_data->fd = fopen(c_data->path, "w+");
		}
		fgets(counter_str, 40, c_data->fd);
		*(c_data->counter) = *(c_data->last_save) = atoi(counter_str);
		// if the only thing in the file is the nul byte, do nothing
		if (1 == strlen(counter_str)) {
			;
		} else {
			// if there was something in the file, add the step to it to make
			// sure we don't end up with duplicates
			*(c_data->counter) = *(c_data->last_save) = *(c_data->counter) 
				+ c_data->save_every;
		}
		uniquer_save_counter(c_data);
		return 0;
	} else {
		// if the counter struct has already been initialized, return 1
		// not sure if this is useful
		return 1;
	}
}

/**
 * write the counter to a file
 */
int uniquer_save_counter(uniquer_counter_data *c_data) {
	char counter_str[40];
	sprintf(counter_str, "%llu\n", *(c_data->counter));
	fseek(c_data->fd, 0, SEEK_SET);
	fputs(counter_str, c_data->fd);
	*(c_data->last_save) = *(c_data->counter);
	fflush(c_data->fd);
	return 0;
}

/**
 * increment the counter; set the id parameter; save if needed
 */
int uniquer_get_next_id(uniquer_counter_data *c_data, unsigned long long *id) {
	*id = ++(*(c_data->counter));
	if (*id >= (*(c_data->last_save)+c_data->save_every)) {
		uniquer_save_counter(c_data);
	}
	return 0;
}
