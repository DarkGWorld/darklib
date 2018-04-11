namespace dark
{
	typedef char * cstr;
	typedef char ** cstrarr;
	namespace str
	{
		// return the length of `cstr`
		int length(const char * cstr);
		// return the alphabetic letters count
		int calpha(const char * cstr);
		// return the alphabetic words count
		int calphawords(const char * cstr);
		cstr cpy(const cstr from);
		cstrarr split(const cstr string, cstrarr to, char delimiter);
		cstr allocate(int bytes);
		cstrarr allocate(int slots, int bytes);
	}
}