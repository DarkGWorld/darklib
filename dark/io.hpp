#include <filesystem>

#include <stdio.h> // FILENAME_MAX
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define DarkGetCurrentDir _getcwd
#else
#include <unistd.h>
#define DarkGetCurrentDir getcwd
#endif
#pragma warning(disable:4996)
#pragma warning(disable:4018)
namespace dark
{
	namespace io
	{
		using namespace std;
#if __cplusplus <= 201700L
		namespace fs = std::experimental::filesystem;
#else
		namespace fs = std::filesystem;
#endif
		const char * get_currentpath()
		{
			char path[FILENAME_MAX];
			DarkGetCurrentDir(path, sizeof(path));
			char* ret = new char[FILENAME_MAX];
			strcpy(ret, path);
			return ret;
		}
		vector<std::string> get_allfiles(std::string path)
		{
			vector<std::string> s;
			for (auto p : fs::directory_iterator(path))
			{
				ostringstream oss;
				oss << p;
				s.push_back(oss.str());
			}
			return s;
		}
		vector<std::string> get_allsubfiles(std::string path)
		{
			vector<std::string> s;
			for (auto p : fs::recursive_directory_iterator(path))
			{
				ostringstream oss;
				oss << p;
				s.push_back(oss.str());
			}
			return s;
		}
		// return the number of (files) deleted.
		template <typename Path>
		int remove(Path path)
		{
			return fs::remove_all(path);
		}
		// clear an file.
		template <typename Path>
		void clear(Path path)
		{
			std::ifstream ofs(path, std::ofstream::out | std::ofstream::trunc);
		}
		template <typename Path>
		int exists(Path path)
		{
			struct stat Stat;
			return stat(path, &Stat) == 0;
		}
	}
}