#include "_Common.h"
#include <iostream>
using namespace std;

//? 一种 While<>实现
//?								每次进入while都包含一个bool标志：是否执行本次（为false有特殊处理）
template<bool _continue,	//x 有这个标志值，递归时就不用IF了 ！（这才是尾递归）  >  判断是下一帧的事情,而且自动匹配bool了
		typename Judger,
		typename Env_>
struct Whi_
{
	using NewEnv = typename Env_::Next;
	enum { judged_res = Judger::template Judge<NewEnv>::RET };

	using RET = typename Whi_<judged_res, Judger, NewEnv>::RET;
};

template<typename Judger,
		typename Env_>
struct Whi_<false,Judger,Env_>
{
	using RET = Env_;
};

//x WHILE不是递归的。				 Whi_是一个递归，而且是尾递归。
template<typename Judger, typename _InitStat>
struct WHILE
{
	enum { whether = Judger::template Judge<_InitStat>::RET };
	using RET = typename Whi_<whether, Judger, _InitStat>::RET;
};
//x***********************************************************************************
//? 更加简化的While<>实现
template<typename Stat>
struct Stop
{
	using RET = Stat;
};
template<typename Judger, typename _Stat>
struct Whi2
{
	using RET = typename IF<Judger::template Judge<_Stat>::RET,
								Whi2<Judger, typename _Stat::Next>,
								Stop<_Stat>
							>::RET::RET;
};

//x***********************************************************************************
template<int i_,int x_, int y_>
struct FibStat
{
	enum
	{
		i = i_,
		x = x_,
		y = y_
	};
	using Next = FibStat<i+1, x+y,x>;
};

template<int n>
struct FibCond
{
	template<typename _FibStat>
	struct Judge
	{
		enum { RET = _FibStat::i < n };
	};
};

template<int n_>
struct Fibonacci
{
	enum
	{
		RET = WHILE<FibCond<n_>, FibStat<1,1,0>>::RET::x	,
		RET2 =  Whi2<FibCond<n_>, FibStat<1,1,0>>::RET::x	
	};
};

void main_x()
{
	cout << Fibonacci<10>::RET;
	cout << Fibonacci<20>::RET;

	cout << Fibonacci<10>::RET2;
	cout << Fibonacci<20>::RET2;
}

