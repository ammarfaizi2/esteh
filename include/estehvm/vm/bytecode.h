
#ifndef estehvm__vm__bytecode_h
#define estehvm__vm__bytecode_h

typedef struct _keyword {
	void *handler;
} bt_keyword;

typedef struct _constant {
	
} bt_constant;

typedef struct _variable {

} bt_variable;

typedef enum _esteh_bytecode_type {
	tb_keyword = 1,
	tb_constant = 2,
	tb_variable = 3
} esteh_bytecode_type;

typedef union _esteh_bytecode_data {
	struct _keyword keyword;
	struct _constant constant;
	struct _variable variable;
} bytecode_data;

typedef struct _esteh_bytecode {
	bytecode_data data;
	esteh_bytecode_type type;
} esteh_bytecode;

#endif
