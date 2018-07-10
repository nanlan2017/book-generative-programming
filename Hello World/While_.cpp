#include "_Common.h"

//X --------------------------------------------------------------------------------------------------
/*MARK
//X WHILE的行为： 检测当前条件值，若为true,则对当前Env进行一次迭代；
//?								否则,结束当前迭代

// 类模板总是比函数模板有更强大的表达能力，所以都是用类模板 ???
//?    ▇▇▇▇▇1 ▇▇▇▇ 似“函数”模板  （有 RET）
//?				——> 返回一个数据：
//?							1. 整型数 enum RET 
//?							2. 类型实例（实例对象）  using RET     
//?	   ▇▇▇▇▇2 ▇▇▇▇ 似“对象” 模板  (1个模板实例相当于一个对象instance) 
//x 而条件值为true/false： 是根据Env里的状态值计算出来的。  所以“条件”表示为： Cond(Env)
//x 进行一次迭代也是更新一次Env，而且行为是固定的。         所以“进行迭代”表示为： Iter(Env)

//x IF<> 是选择类型的，用在计算using RET 里。而enum里的 RET值 应该是用 ?:

*/
//X --------------------------------------------------------------------------------------------------
//x While语句是 “似函数”的：返回一个对象:最终的迭代状态值。

//? While_<>
template<
		bool Condition,
		template<typename> class Con_,		
		template<typename > class Iter_,	
		typename Env_			
		>
struct While_
{
	// 执行完本次迭代后的Env_
	using NextEnv = typename Iter_<Env_>::RET ;

	// 判断下一个Env是否还要继续迭代
	enum{ next_condition = Con_<NextEnv>::RET };

	// 如果下一个Env继续迭代，则使用下一次迭代的结果； 否则返回当前Env的结果
	
	using RET = typename IF<Condition,
							typename While_<next_condition, Con_, Iter_, NextEnv>::RET,
							Env_
						 >::RET;
};
//? While_<>模板偏特化：当false时
template<
		template<typename> class Con_,		
		template<typename > class Iter_,	
		typename Env_						
		>
struct While_<false, Con_,Iter_,Env_>
{
	using RET = Env_	;
};

//? WHILE<> 模板
template<
		template<typename> class Con_,		// 第1个模板参数： 一个模板（其参数为：1个类型）——> 1个“似函数”模板
		template<typename > class Iter_,	// 第2个模板参数： 一个模板（其参数为：1个类型）——> 1个“似函数”模板
		typename Env_						// 第3个模板参数： 一个类型
		>
struct WHILE
{
	enum { go = Con_<Env_>::RET  };
	using RET = typename While_<go, Con_, Iter_, Env_>::RET;
};
//X --------------------------------------------------------------------------------------------------
/*
template<bool Condition,     // 循环的继续条件
		int n,				 // 循环的参数
		int i, int x, int y> // 循环过程中的状态: 当前处于的下标、当前结果
struct Fib_
{
	enum
	{
		RET = Fib_<(i+1<n), n, i+1, x+y, x>::RET	
	};
};
*/
//? 环境：一次迭代的栈的状态
template<int n_,int i_,int x_,int y_>
struct FibEnv
{
	enum
	{
		n = n_	,
		i = i_	,
		x = x_	,
		y = y_	,
	};
};

//? 一次迭代的计算
template<typename Env>
struct FibIter
{
	enum
	{
		new_i = Env::i+1	,
		new_x = Env::x +Env::y	,
		new_y = Env::x
	};
	using RET = FibEnv<Env::n, new_i, new_x, new_y>;
};

//? 迭代继续的条件: 返回该Env下是否继续的标志位
template<typename Env>
struct FibCond
{
	enum
	{
		RET = Env::n > Env::i	,
	};
};

//X --------------------------------------------------------------------------------------------------
//? --------------------------------------------------------------------------------------------------
template<int n>
struct Fib
{
	enum
	{
		RET = WHILE<FibCond,
					FibIter,
					FibEnv<n,1,1,0>
				 >::RET::x	,
	};
};