#ifndef DARK_MATH
#define DARK_MATH
#include <vector>
#include <chrono>
namespace dark
{
	namespace math
	{
		// return a **Real** random number between 0 and 1 with the power of microseconds.
		double random()
		{
			auto time = std::chrono::duration_cast<std::chrono::microseconds>
				(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			srand((unsigned int)time);
			return (double)(rand() / (double)RAND_MAX);
		}
		// return the number of digits in the integer x.
		int digits(long long x)
		{
			x < 0 ? x = -x : 0;
			return x < 10 ? 1 :
				x < 100 ? 2 :
				x < 1000 ? 3 :
				x < 10000 ? 4 :
				x < 100000 ? 5 :
				x < 1000000 ? 6 :
				x < 10000000 ? 7 :
				x < 100000000 ? 8 :
				x < 1000000000 ? 9 :
				x < 10000000000 ? 10 : 0;
		}
		// return the type of the integer `x` as a number.
		long long type(const long long x)
		{
			long long i = abs(x);
			for (long long t = 10; t < INT_MAX; t *= 10)
			{
				if (i < t)
					return t / 10;
			}
			return -1;
		}
		// return the first digit of integer `x`.
		int first(const long long x)
		{
			return (int)(x / type(x));
		}
		// return the last digit of integer `x`.
		int last(const long long x)
		{
			return x % 10;
		}

		// return integer `x` reversed from right to left.
		long long reverse(long long x)
		{
			auto negative = x < 0 ? 1, x = -x : 0;
			long long rev = 0; // reversed
			while (x > 0)
			{
				rev = rev * 10 + x % 10;
				x /= 10;
			}
			return negative ? -rev : rev;
		}
		// return sum of the average number of numeric elements in the vector `v`.
		template<typename t, typename a>
		t mean(std::vector<t, a> v)
		{
			auto sum = 0;
			for (int i = 0; i < v.size(); sum += v.at(i), ++i);
			return sum / v.size();
		}
		// return how far is `x` from zero.
		template<typename value>
		value abs(value x)
		{
			return x < 0 ? -x : x;
		}
	};
}
#endif