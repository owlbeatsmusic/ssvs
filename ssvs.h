#ifndef SSVS_H_
#define SSVS_H_

void ssvs_initialize_file(char file_path[], const unsigned char variables);
void ssvs_write_int(char file_path[], int var, unsigned char index);
void ssvs_print_file(char file_path[]);
int ssvs_read_int(char file_path[], unsigned char index);

#endif
