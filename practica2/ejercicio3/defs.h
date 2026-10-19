#ifndef DEFS_H
#define DEFS_H

/**
 * Available actions supported by the program
 */
typedef enum {
	NONE_ACT,
	READ_TEXT_ACT,
	WRITE_TEXT_ACT,
	READ_BINARY_ACT,
	WRITE_BINARY_ACT
} action_t;

/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
struct options {
	char* input_file;
	char* output_file;
	action_t action_read;
	action_t action_write;
};

#endif