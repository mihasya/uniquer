#include "uniquer_lib.h"

int counter_init(counter_data *c_data) {
	*(c_data->counter) = 1;
	return 0;
}

int get_next_id(counter_data *c_data, long unsigned *id) {
	//TODO: keep track of counter since last save 
	//TODO: save counter if it's been 10000
	if (*(c_data->counter) == 0) {
		counter_init(c_data);
		*id = *(c_data->counter);
	} else {
		*id = ++(*(c_data->counter));
	}
	return 0;
}