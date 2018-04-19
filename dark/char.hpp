#ifndef DARK_CHAR
#define DARK_CHAR
namespace dark
{
	namespace c
	{
		// return the char is an alphabetic char or not
		int isalpha(int c)
		{
			// a = 97, z = 122, A = 65, Z = 90
			return c > 96 && c < 123 || c > 64 && c < 91;
		}
		// return `c` is a char number between '0'-'9' or not
		int isdigit(int c)
		{
			// '0' = 48 - '9' = 57
			return c > 47 && c < 58;
		}
		// return the specfied letter in lowercase
		char tolower(int c)
		{
			// (int)a = 97, (int)A = 65
			// (a)97 - (A)65 = 32
			// therefore 32 + 65 = a
			return c > 64 && c < 91 ? c + 32 : c;
		}
		// return the specfied letter in uppercase
		char toupper(int c)
		{
			// (int)a = 97, (int)A = 65
			// (a)97 - (A)65 = 32
			// therefore 97 - 32 = A
			return c > 96 && c < 123 ? c - 32 : c;
		}
	}
}
#endif