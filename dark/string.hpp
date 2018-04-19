#ifndef DARK_STRING
#define DARK_STRING
#include "char.hpp"
#include <memory>
namespace dark
{
	namespace cstr
	{
		// return the length of `cstr`
		int length(const char * cstr)
		{
			if (!cstr)
				return 0;
			int i = 0;
			while (*cstr && ++i && *cstr++);
			return i;
		}
		// return the alphabetic letters count
		int count_alpha(const char* cstr)
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
		// return the alphabetic words count
		int count_alphawords(const char* cstr)
		{
			if (!cstr || *cstr == 0)
				return 0;
			int n = 0;
			for (int i = length(cstr); i > 0; --i)
				if (c::isalpha(cstr[i]) && !c::isalpha(cstr[i + 1]))
					++n;
			return n;
		}
		char * cpy(const char * from)
		{
			if (!from)
				return 0;
			size_t len = length(from);
			char * to = new char[len];
			for (int i = len; i > -1; --i)
				to[i] = from[i];
			return to;
		}
		char ** split(const char * string, char ** to, char delimiter)
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
		char * allocate(int bytes)
		{
			return (char *)calloc(1, bytes);
		}
		char ** allocate(int slots, int bytes)
		{
			char ** arr = (char **)calloc(1, slots);
			for (int i = 0; i <= slots; ++i)
			{
				arr[i] = (char *)calloc(1, bytes);
			}
			return arr;
		}
		int endsWith(const char * cstring, const char * end)
		{
			if (cstring && end)
			{
				int len = strlen(cstring);
				int end_len = strlen(end);
				for (int i = 0; i <= end_len; ++i)
					if (cstring[len - end_len + i] != end[i])
						return 0;
				return 1;
			}
			return 0;
		}
	}
	namespace str
	{
		int endsWith(std::string str, std::string end)
		{
				for (int i = 0; i <= end.size(); ++i)
					if (str[str.size() - end.size() + i] != end[i])
						return 0;
				return 1;
		}
	}
}
#endif