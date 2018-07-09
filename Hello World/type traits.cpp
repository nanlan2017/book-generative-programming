#include "_Common.h"
#include <vcruntime.h>
/***************************************************************/
template<typename ElemType_, int size_>
class Vector
{
public:
	typedef ElemType_ element_type;
	enum { size = size_ };

private:
	element_type elems[size];
};

//? 枚举：矩阵的形状
enum MatrixShapes
{
	rectangle,
	square,
	diagonal,
	lower_triang,
	upper_triang
};

class Diagonal_10x10_MatrixOf_Doubles
{
public:
	//? traits class
	struct Config
	{
		typedef double ElemType;
		enum
		{
			rows = 10,
			cols = 10,
			shape = diagonal
		};
	};

	double at(int x, int y);
};

//? 类型信息的模板，及类型的default信息值
template <typename T>
class numeric_limits
{
public:
	static const bool is_specialized = false; // 是否已特化
	static const bool has_denorm = false; // 是否有非正常表示
	static const bool has_infinity = false; // 是否可取无限
	static const bool is_integer = false; // 是否为整型

	static const int digit = 0; // 十进制的最大值
	static const int digit10 = 0; // 十进制的最大值

	static T min() throw();

};

//? 特化：一种特定类型的类型信息
template<>
class numeric_limits<float>
{
	static const bool is_specialized = true; // 是否已特化
	static const bool has_denorm = false; // 是否有非正常表示
	static const bool has_infinity = false; // 是否可取无限
	static const bool is_integer = false; // 是否为整型

	static const int digit10 = 6; // 十进制的最大值

	static float min() throw() { return 1.175435E-38F; }
};

//? 类型提升：返回两个类型中精度较高的类型（比较类型的特性exponent10和digits）
template<typename A, typename B>
struct Promotion<>
{
	enum
	{
		max_exp_A = numeric_limits<A>::max_exponent10,
		max_exp_B = numeric_limits<B>::max_exponent10,
		digit_A = numeric_limits<A>::digits,
		digit_B = numeric_limits<B>::digits,
	};
	typedef typename IF<(max_exp_A < max_exp_B) || (max_exp_A== max_exp_B && digit_A < digit_B)  , 
				B, A>::RET RET;
};