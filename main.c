#include <stdio.h>
#include "ssvs.h"

int main() {
	printf("\n[ ssvs ]\n");
	ssvs_initialize_file("output.hex", 2);
	ssvs_write_int("output.hex",369003283, 0);
	ssvs_read_file("output.hex");
	printf("main: int at index=0: %d\n", ssvs_read_int("output.hex", 0));
	return 0;
}
