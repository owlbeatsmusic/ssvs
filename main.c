#include <stdio.h>
#include "ssvs.h"

int main() {
	ssvs_initialize_file("output.hex", 0);
	ssvs_write_int("output.hex", 9284328, 0);
	ssvs_read_int("output.hex", 1);
	return 0;
}
