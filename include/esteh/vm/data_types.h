

#ifndef ESTEH_VM_DATA_TYPES_H
#define ESTEH_VM_DATA_TYPES_H

#define TEA_NULL 	0
#define TEA_INTEGER	(1 << 0)
#define TEA_FLOAT	(1 << 1)
#define TEA_STRING	(1 << 2)
#define TEA_ARRAY	(1 << 3)

union tea_val {
	long long llval;
	double fval;
	struct {
		char *val;
		size_t len;
	} str;
};

enum tea_type {
	tea_null = TEA_NULL,
	tea_integer = TEA_INTEGER,
	tea_float = TEA_FLOAT,
	tea_string = TEA_STRING,
	tea_array = TEA_ARRAY
};

typedef struct _tea_data {
	union tea_val val;
	enum tea_type type;
} tea_data;

#endif
