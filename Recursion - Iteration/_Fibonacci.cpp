
//X 递归算法

int fib(int n)
{
	if (n == 0) return 0;
	if (n == 1) return 1;
	return fib(n - 1) + fib(n - 2);
}
//?  这是一般的递归。  需要先转化成

//X 迭代算法
int fib_(int n)  //? ★★
{
	int i = 1;
	int x = 1, y = 0; // x，y分别用来保存 fi,  f i-1

	while (i<n) {
		i = i + 1;
		x = x + y; // fk+1 = fk + fk-1
		y = x - y;
	}
	return x;
}
//x----------------------------------------------------------------------------------------
//x 迭代算法：借助元函数的尾递归实现。
//? 把上一次递归的状态都当作参数传递给下一条递归，从而不需要一直保持上一个递归的栈
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
//x 此递归的终止方式 采用 特化参数方案
template< int n,
		  int i, int x, int y>
struct Fib_<false,n,i,x,y>
{
	enum { RET = x };
};
//************************ 以上为Fib计算的辅助元函数。 
template<int n>
struct Fib
{
	enum
	{
		RET = Fib_<(n>1), n, 1,1,0 >::RET
	};
};
