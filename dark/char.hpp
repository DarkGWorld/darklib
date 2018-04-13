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
	}
}