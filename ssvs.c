#include <stdlib.h>
#include <string.h>
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
		[ ] fix problem where char converts to wrong hex(reading error hex->char)
		[ ] write int and update index and size in lookup
		[ ] when writing check for empty spot with fitting size
*/



// tool funtions

void replace_string_at_indices(char *from, int from_index, char *to, int to_index) {

	int from_length = strlen(from);
	int to_length = strlen(to);
	int length = to_length;

	//printf("r_s_a_i: to_length=%d\n", to_length);

	if (from_length < to_length) length = from_length;

	for (int i = 0; i < length; i++) {
		to[to_index+i] = from[from_index+i];
		//printf("r_s_a_i: to(i=%d)=\"%s\"\n", i, to);
	}
}

void error_prefix_print() {
	printf("%s%serror%s: ", "\x1B[1m", "\x1B[31m", "\x1b[0m");
}





// Initialze the ssvs file
void ssvs_initialize_file(char file_path[], const unsigned char values) {
	FILE *hex_file = fopen(file_path, "wb");
	if (hex_file == NULL) {
		printf("could not create/open file (%s).\n", file_path);
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
		printf("could not create/open file (%s).\n", file_path);
		return;
	}
	
	fseek(hex_file, 0L, SEEK_END);
	size_t length = ftell(hex_file)+1;	
	fseek(hex_file, 0L, SEEK_SET);
	
	unsigned char buf[length];
	fgets(buf, length, hex_file);
	
	unsigned char size[3];
	sprintf(size, "%02x", (unsigned char)sizeof(var) * 2);

	replace_string_at_indices(size, 0, buf, 2+6*index);
		
	// TODO: check if there are ant empty addreses otherwise:
	unsigned char addr[5];
	sprintf(addr, "%04x", (unsigned short)length);
	replace_string_at_indices(addr, 0, buf, 4+6*index);


	fclose(hex_file);



	// write back to file
	hex_file = fopen(file_path, "wb");

	for (size_t i = 0; i < length; i += 2) {
		unsigned int byte_value;
		sscanf(&buf[i], "%02x", &byte_value);
		fprintf(hex_file, "%02x", byte_value);
	}

	fprintf(hex_file, "%08x", var);
	
	fclose(hex_file);
}

// clear data at index
void ssvs_erase_variable(char file_path[], unsigned char index) {

}

void ssvs_read_file(char file_path[]) {
	FILE *hex_file = fopen(file_path, "rb");
	if (hex_file == NULL) {
		error_prefix_print();
		printf("could not create/open file (%s).\n", file_path);
		return;
	}

	fseek(hex_file, 0L, SEEK_END);
	size_t length = ftell(hex_file)+1;	
	fseek(hex_file, 0L, SEEK_SET);
	
	unsigned char buf[length];
	fgets(buf, length, hex_file);
	printf("read_file: \"%s\"\n", buf);

 	
	fclose(hex_file);

}

int ssvs_read_int(char file_path[], unsigned char index) {
	FILE *hex_file = fopen(file_path, "rb");
	if (hex_file == NULL) {
		error_prefix_print();
		printf("could not create/open file (%s).\n", file_path);
		return -1;
	}

	fseek(hex_file, 0L, SEEK_END);
	size_t length = ftell(hex_file)+1;	
	fseek(hex_file, 0L, SEEK_SET);
	
	unsigned char buf[length];
	fgets(buf, length, hex_file);



	unsigned char size_hex[] = "  "; // 2 spaces
	replace_string_at_indices(buf, 2+6*index, size_hex, 0);
	int size_int;	
	//printf("read_int: size_hex=%s\n", size_hex);
	//printf("read_int: size_hex(length_2)=%ld\n", strlen(size_hex));
	if (sscanf(size_hex, "%x", &size_int) != 1) {		
		error_prefix_print();
		printf("read_int: could not convert hex(size) to integer\n");
	}

	

	unsigned char addr_hex[] = "    "; // 4 spaces 
	replace_string_at_indices(buf, 4+6*index, addr_hex, 0);	
	int addr_int;
	//printf("read_int: addr_hex=%s\n", addr_hex);
	//printf("read_int: addr_hex(length_2)=%ld\n", strlen(addr_hex));
	if (sscanf(addr_hex, "%x", &addr_int) != 1) {
		error_prefix_print();
		printf("read_int: could not convert hex(addr) to integer\n");
	}


	unsigned char variable_hex[size_int];
	strcat(variable_hex, "   "); // weird shit; we cant initialize without space
	replace_string_at_indices(buf, addr_int, variable_hex, 0);
	int variable_int;
	//printf("read_int: size_int=%d\n", size_int);
	//printf("read_int: variable_hex=%s\n", variable_hex);
	if (sscanf(variable_hex, "%x", &variable_int) != 1) {
		error_prefix_print();
		printf("read_int: could not convert hex(variable) to integer\n");
	}


	fclose(hex_file);

	return variable_int;

}

