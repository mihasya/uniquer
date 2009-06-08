int counter_init(long unsigned *counter) {
	//TODO: pull latest counter from a file
	*counter = 1;
	return 0;
}

int get_next_id(long unsigned *counter) {
	//TODO: keep track of counter since last save 
	//TODO: save counter if it's been 10000
	int id;
	if (*counter == -1) {
		counter_init(counter);
		id = *counter;
	} else {
		id = ++*counter;
	}
	return id;
}