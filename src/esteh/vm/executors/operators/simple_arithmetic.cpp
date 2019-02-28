
#include <esteh/vm/executors/operators/simple_arithmetic.hpp>

long long icetea_add(void *op1, void *op2) {
	return (*((long long *)op1)) + (*((long long *)op2));
}

long long icetea_min(void *op1, void *op2) {
	return (*((long long *)op1)) - (*((long long *)op2));
}

long long icetea_mul(void *op1, void *op2) {
	return (*((long long *)op1)) * (*((long long *)op2));
}

long long icetea_div(void *op1, void *op2) {
	return (*((long long *)op1)) / (*((long long *)op2));
}

long long icetea_mod(void *op1, void *op2) {
	return (*((long long *)op1)) % (*((long long *)op2));
}
