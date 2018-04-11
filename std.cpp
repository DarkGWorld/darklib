#include "std.hpp"
#ifdef _DARK_
using namespace dark;
strarr dark::String::Split(const str string, strarr to, char delimiter)
{
	size_t length = strlen(string);
	int n = 0;
	if (string && to)
	{
		int n = 0;
		int c = 0;
		for (int i = 0; string[c] != '\0'; i++, c++)
		{
			try {
				to[n][i] = string[c];
				if (string[c] == delimiter)
				{
					to[n][i] = '\0';
					i = -1;
					++n;
				}
			}
			catch (std::exception &e)
			{

			}
		}
	}
	return strarr();
}
str dark::String::Allocate(int bytes)
{
	return (str)calloc(1, bytes);
}
strarr dark::String::Allocate(int slots, int bytes)
{
	const char ** arr = (const char **)calloc(1, slots);
	for (int i = 0; i <= slots; ++i)
	{
		arr[i] = (str)calloc(1, bytes);
	}
	return (strarr)arr;
}
#endif