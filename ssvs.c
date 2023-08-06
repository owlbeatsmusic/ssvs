#include <stdio.h>
#include "ssvs.h"

/*
	First byte will define how many values are stored (0-255).

	Then there will be two bytes(a short) for as many values that are stored
	so every single value gets a index (like a lookup table)

	0x0a : end of variable definition.
	
	Will not support pointers.

	Supported types (for now):
		- int ([])
		- char ([])
		- float ([])
		- double ([])

	TODO:
		[ ] write int and update index and size in lookup
		[ ] when writing check for empty spot with fitting size
*/



// Initialze the ssvs file
void ssvs_initialize_file(char file_path[], const unsigned char values) {
	FILE *hex_file = fopen(file_path, "wb");
	if (hex_file == NULL) {
		printf("error: could not create/open file (%s).\n", file_path);
		return;
	}

	// ammount of values (0-255)
	fprintf(hex_file, "%02x", values);
	


	
	// the size of the "lookup table" of the index of the values.
	size_t lookup_size = values * sizeof(unsigned char);

	for (size_t i = 0; i < lookup_size; i++) {
		// 		              index              size
		fprintf(hex_file, "%04x%02x", (unsigned short)0, (unsigned char)0);
	}
	
	
	fclose(hex_file);
	
}

void ssvs_write_int(char file_path[], int var, unsigned char index) {
	FILE *hex_file = fopen(file_path, "rb");
	if (hex_file == NULL) {
		printf("error: could not create/open file (%s).\n", file_path);
		return;
	}
	
	fseek(hex_file, 0L, SEEK_END);
	int length = ftell(hex_file);	
	fseek(hex_file, 0L, SEEK_SET);
	
	unsigned char buf[length];
	fgets(buf, length, hex_file);
	
	unsigned char size = (unsigned int)sizeof(var) * 2;
	

	
	buf[2+6*index]

	fprintf(hex_file, "%08x", var);
	
	fclose(hex_file);
}

// clear data at index
void ssvs_erase_variable(char file_path[], unsigned char index) {

}

int ssvs_read_int(char file_path[], unsigned char index) {
	FILE *hex_file = fopen(file_path, "rb");
	if (hex_file == NULL) {
		printf("error: could not create/open file (%s).\n", file_path);
		return 1;
	}


	unsigned char variables_buf[3];
	
	fgets(variables_buf, 3, hex_file);
	printf("\"%s\"\n", variables_buf);
 	
	fclose(hex_file);

	return 0;
}


