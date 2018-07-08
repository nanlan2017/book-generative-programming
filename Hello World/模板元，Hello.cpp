#include <iostream>

using namespace std;

/*
//x RET 就是该元函数的 返回值:
		—————— 变量都是 符号，其代表两种数据：一个整型值/ 一种类型
		如果这个模板的作用是生成类型：则RETT是 typedef 出的“类型性质的” 符号
		如果这个模板的作用是计算值：  则RETV是 enum 中定义的“值性质的”  符号

//x 一个template类：可当做一个“函数”
		一个计算表达式 enumulator RET = f(x,y,z)；
//x 一个template类：也可当做一个“Class” 。它的一个实例 Temp<x,y> 就相当于一个绑定了status value的 对象。
*/
/***************************************************************/
//? if-else
template <bool Condition,typename Then, typename Else>
struct  IF
{
	typedef Then RET;
};

template <typename Then, typename Else>
struct  IF<false,Then,Else>
{
	typedef Else RET;
};

/***************************************************************/
//? switch-case
template <int default_Case> 
struct CaseActions
{
	enum
	{
		RET = 0, // 该种Case的action
	};
};

template <> 
struct CaseActions<1>
{
	enum
	{
		RET = 111, // 该种Case的action
	};
};

template <> 
struct CaseActions<2>
{
	enum
	{
		RET = 222, // 该种Case的action
	};
};

template <int Condition_val>
struct SWITCH
{
	enum
	{
		RET = CaseActions<Condition_val>::RET
	};
};
/***************************************************************/
//? 计算 Factorial<n>::RET   =  n!
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

/***************************************************************/
//x template class既然是可以包含值状态的，那它自己的每个实例都可以当成instance啊！
//x 而且定义了函数调用运算符后，这个模板实例就成了一个函数对象


//? 浮点数
template <int mantissa_, int exp_>
struct Float
{
	enum
	{
		mantissa = mantissa_,
		exp = exp_
	};
	operator const float() const {
		return mantissa_ * pow(10, exp_);
	}

	static const float v = Float<mantissa_, exp_>();
};

/***************************************************************/
//? Sqrt()
template<typename Float>
struct Sqrt
{
	enum
	{
		val = sqrtf(Float()),
	};

	typedef Float<val,0> RET;
};
/***************************************************************/

//x***************************************************************

int main(int argc, char* argv[])
{
	cout << Factorial<10>::RET << endl;

	IF<(1 + 2 > 4), short, int>::RET i;

	cout << Combinations<3,10>::RET << endl;

	cout << SWITCH<555>::RET << endl;
	cout << SWITCH<1>::RET << endl;
	cout << SWITCH<2>::RET << endl;

	typedef Float<25, -1> F2p5;
	float v3 = 8.0;
	static const float v0 = v3 + F2p5();
	typedef typename Sqrt<F2p5>::RET RESULT;
	static const int v1 = 8 + RESULT();
	cout << v0 << v1;

}

const int v5 = 8.0 + Float<25, -1>::v;
