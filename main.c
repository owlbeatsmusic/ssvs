#include <stdio.h>
#include "ssvs.h"

int main() {
	
	ssvs_initialize_file("output.hex", 5);
	ssvs_write_int("output.hex",12345678, 1);
	printf("main: int at index=1: %d\n", ssvs_read_int("output.hex", 1));
	
	return 0;
}
