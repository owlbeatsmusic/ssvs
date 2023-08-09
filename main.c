#include <stdio.h>
#include "ssvs.h"

int main() {
	ssvs_initialize_file("output.hex", 2);
	ssvs_write_int("output.hex",369003283, 0);
	ssvs_read_file("output.hex");
	ssvs_read_int("output.hex", 0);
	return 0;
}
