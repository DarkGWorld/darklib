#include "char.h"
#include "string.h"
#include <stdlib.h>
namespace dark
{
	int str::length(const char * cstr)
	{
		if (!cstr)
			return 0;
		int i = 0;
		while (*cstr && ++i && *cstr++);
		return i;
	}
	int str::calpha(const char* cstr)
	{
		if (!cstr || *cstr == 0)
			return 0;

		int len = length(cstr);
		int i = -1;
		int n = 0;
		while (i++ < len)
			if (c::isalpha(cstr[i]))
				++n;
		return n;
	}
	int str::calphawords(const char* cstr)
	{
		if (!cstr || *cstr == 0)
			return 0;
		int n = 0;
		for (int i = length(cstr); i > 0; --i)
			if (c::isalpha(cstr[i]) && !c::isalpha(cstr[i + 1]))
				++n;
		return n;
	}

	cstr str::cpy(const cstr from)
	{
		if(!from)
			return 0;
		size_t len = length(from);
		cstr to = new char[len];
		for (int i = len; i > -1; --i)
			to[i] = from[i];
		return to;
	}

	cstrarr str::split(const cstr string, cstrarr to, char delimiter)
	{
		size_t len = length(string);
		int n = 0;
		if (string && to)
		{
			int n = 0;
			int c = 0;
			for (int i = 0; string[c] != '\0'; i++, c++)
			{
				to[n][i] = string[c];
				if (string[c] == delimiter)
				{
					to[n][i] = '\0';
					i = -1;
					++n;
				}
			}
		}
		return to;
	}

	cstr str::allocate(int bytes)
	{
		return (cstr)calloc(1, bytes);
	}

	cstrarr str::allocate(int slots, int bytes)
	{
		const char ** arr = (const char **)calloc(1, slots);
		for (int i = 0; i <= slots; ++i)
		{
			arr[i] = (cstr)calloc(1, bytes);
		}
		return (cstrarr)arr;
	}
}