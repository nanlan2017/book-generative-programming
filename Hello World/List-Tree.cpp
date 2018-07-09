#include "_Common.h"

#include <iostream>
using namespace std;

#define EB enum{
#define EN };

//?  统一用using 定义类型变量， 放在前面很统一！！！

/*
//X 如果这个模板的功能相当于函数，
//x             那么它有返回值RET: 可能是返回一个对象（类型），也可能是返回一个值（enumerator）
//x               在使用这个模板时，必须加上 ::RET  !(如果返回的是类型，则还要加typenmae)
*/
/***************************************************************/
/*
//? 链表：  1-> 2 ->3
函数式写法：  cons(1, cons(2, cons(3 nail) ) )   
模板元写法：  Cons<1, Cons<2, Cons<3, End> > >

每个节点时一个实例（类型）： head是值，tail指向下一个实例（类型）
*/

const int endValue = ~(~0u >> 1); // int的最小值（位操作实现）

// 值head,下一个类型实例tail
struct End
{
	enum
	{
		head = endValue
	};
	typedef End tail;
};

template<int head_, typename tail_ = End >
struct Cons
{
	enum
	{
		head = head_
	};
	typedef tail_ tail;
};
/***************************************************************/
//? Length()
template<typename List>
struct Length
{
	enum
	{
		RET = 1 + Length<typename List::tail>::RET
	};
};

template<>
struct Length<End>
{
	enum { RET = 0};
};
/***************************************************************/
//? Append() ： 一个元素节点
template<typename List, int n>
struct AppendL
{
	using RET = Cons<List::head, 
					typename AppendL<typename List::tail, n>::RET
					>	;
};

template<int n>
struct AppendL<End, n>
{
	using RET = Cons<n>;
};
/***************************************************************/
//? Aped():  一个list
template<typename list1, typename list2>
struct Aped
{
	using RET = Cons<list1::head,
					typename Aped<typename list1::tail, list2>::RET
						>;
};

//x							 template <typename list1, typename list2>
template<typename list2> //x 特化一个模板时，被指定的模板参数就不能再写了！因为已经没有这个可变的模板参数了！！！！
struct Aped<End, list2>
{
	using RET = list2;
};

/***************************************************************/
//? isEmpty()
template<typename list>
struct IsEmpty
{
	enum
	{
		RET = list::head == endValue	,
	};
};

/***************************************************************/
//? Last()
template<typename list>
struct Last
{
	enum
	{
		RET = IsEmpty<typename list::tail>::RET 
							? list::head  
							: Last<typename list::tail>::RET,
	};
	using next = typename list::tail; //? 最后一个节点:End节点,其 RET0为Unknown
	using RET0 = typename IFI <IsEmpty<next>::RET, list, typename Last<next>::RET0>::RET;
};

template<>
struct Last<End>
{
	enum { RET = 666 };
	using RET0 = End;
};

/***************************************************************/
void main_3()
{
	using list1 = Cons<1, Cons<2, Cons<3>>>;
	cout << Length<list1>::RET;
	cout << Length< AppendL<list1, 4>::RET >::RET;

	using list2 = Cons<7, Cons<8, Cons<9>>>;
	cout << Length< Aped<list1, list2>::RET >::RET;

	cout <<IsEmpty<End>::RET;
	cout <<IsEmpty<list1>::RET;

	cout << Last<list1>::RET;   // 取最后一个节点的值  //TODO
	using last_of_list1 = Last<list1>::RET0;	// 取最后一个节点

}
/***************************************************************/
template<int n>
struct Int
{
	enum{ value = n};
};
struct Endp
{
	using head = Int<endValue>;
	using Tail = Endp;
};
template <typename head_, typename tail_= Endp>
struct Conp
{
	using head = head_;
	using Tail = tail_;
};

void main_4()
{
	//?  二叉树
	using list1 = Conp<Int<1>,  Conp<     Conp<Int<21>,Conp<Int<31>>>  ,  Conp<Int<22>>       >>;
}



