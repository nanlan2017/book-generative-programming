#include <iostream>

using namespace std;
/*****************************************************************************/
// square(n) = n*n*n;
template <int n>
struct Square
{
	enum
	{
		RET = n*n*n
	};
};

//? "template template"    模板参数可以为 另一个模板！！
template<int n, template<int> class F>
struct Accumulate
{
	// return f(n)+ accumulate(n-1, f)
	enum{ RET = F<n>::RET + Accumulate<n-1,F>::RET };
};

// Accumulate(0, f) = f(0)
template<template<int> class F>
struct Accumulate<0,F>
{
	// return f(0)
	enum{ RET = F<0>::RET };
};
/*****************************************************************************/
//x 若编译器尚不支持template template， 则使用“内置模板”（把这个要传的模板放在一个struct中传递）
struct Sqa
{
	template <int n>
	struct Apply
	{
		enum
		{
			RET = n*n
		};
	};
};

template <int n, typename F>
struct Accum
{
	enum
	{
		RET = F::template Apply<n>::RET + Accum<n-1, F>::RET
	};
};

template<typename F>
struct Accum<0,F>
{
	enum
	{
		RET = F::template Apply<0>::RET
	};
};
/*****************************************************************************/

void run()
{
	cout << Accumulate<15, Square>::RET;

	cout << Accum<13, Sqa>::RET;
}