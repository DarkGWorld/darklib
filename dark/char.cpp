#include "char.h"

int dark::c::isalpha(int c)
{
	// a = 97, z = 122, A = 65, Z = 90
	return c > 96 && c < 123 || c > 64 && c < 91;
}

int dark::c::isdigit(int c)
{
	// '0' = 48 - '9' = 57
	return c > 47 && c < 58;
}
