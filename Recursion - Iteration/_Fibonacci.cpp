
// 动态代码：计算Fibonacci数
//X 缺点：对于一项 fib(k)进行了多次调用计算

int fib(int n)
{
	if (n == 0) return 0;
	if (n == 1) return 1;
	return fib(n - 1) + fib(n - 2);
}


//x----------------------------------------------------------------------------------------
//x----------------------------------------------------------------------------------------
//x----------------------------------------------------------------------------------------
// 具有线性时间复杂度的递归实现
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

//? 这TM就是 尾递归优化啊！ (tail recursion optimization)
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
//x Fib_<>的偏特化： Condition为false时
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
