
//? 可以计算pow = m^n的 EWHILE
//X 传递一个Env，代表参数状态         > exec(Env&) ： 这样就能实现丰富的迭代的内容


#include "../Hello World/_Common.h"

struct PowEnv
{
	int m, r;
	PowEnv(const int& m_,const int& r_):m(m_),r(r_){}
};


//? Bean: Statement : 一次递归时的运算数据 
//X 放一次递归体内的 状态信息、并用Next定义本次递归的计算.
template<int n_, int i_>
struct PowStat
{
	enum
	{
		n = n_,
		i = i_
	};
	using Next = PowStat<n, i + 1>;

	static void exec(PowEnv& env) { env.r *= env.m; };
};

struct PowCond
{
	template<typename Statement>
	struct Judge
	{
		enum { RET = Statement::i < Statement::n };
	};
};
//x------------------------------------------------------------------------------
namespace intimate 
{
	struct STOP
	{
		template<typename Env>
		static void exec(Env&) {};
	};
}

template<typename Condition,typename Statement>
struct EWHILE1
{
	// 判断：当前状态是否要exec()
	enum { judged = Condition::template Judge<Statement>::RET };

	template<typename Env>
	static void exec(Env& env)
	{
		// 完成当前Statement: 若要exec()，则exec()； 否则stop()
		IF<judged,
			Statement,
			intimate::STOP>	::RET::exec(env);
		// 进入下一次递归
		IF<judged,
			EWHILE1<Condition, typename Statement::Next>,
			intimate::STOP>	::RET::exec(env);
	}
};


//x------------------------------------------------------------------------------
//x 实现Pow
template<int n>
struct PowerD1
{
	static int exec(const int& m)
	{
		PowEnv env(m, 1);
		EWHILE1<PowCond, PowStat<n, 0>>	::exec(env);  //X 启动迭代！！！（入口值:n）
		return env.r;
	}
};

//? ------------------------------------------------------------------------------
void test_ewhile1()
{
	// 计算 2^3
	PowerD1<3>::exec(2);
}