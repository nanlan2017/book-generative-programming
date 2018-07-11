#include "_Common.h"

//? 递归方式：结合模板特化 （指定结束时的参数）
template<int n>
struct Factorial
{
	enum
	{
		RET = Factorial<n-1>::RET*n
	};
};

template<>
struct Factorial<0>
{
	enum
	{
		RET = 1
	};
};
//x----------------------------------------------------------------------------------------

//? 递归方式：结合Stop函数
struct stop
{
	enum { RET = 1 };
};

template<int n>
struct Fact
{
	using PreviousFactorial = IF<n == 0, stop, Fact<n - 1>>;
	enum
	{
		RET = (n==0)
				? PreviousFactorial::RET 
				: PreviousFactorial::RET*n	,
	};
};

//x------------------------------------------------------------------------------------

//? C(k,n) = n! / k!(n-k)!  （n必须大于k，否则编译器推导时异常直接卡死！！！）
template<int k,int n>
class Combinations
{
private:
	enum
	{
		num = Factorial<n>::RET,
		denum = Factorial<k>::RET * Factorial<n-k>::RET
	};
public:
	enum
	{
		RET = num/ denum
	};
};