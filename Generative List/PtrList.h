#ifndef PRTLIST_H
#define PRTLIST_H

#include <cassert>
#include <iostream>

struct Person;

/*
这个组件依赖于多个组件。
若不用模板，则对于一个组件只能持有一个具体的实现。
当然，你可以用Interface? 来实现多态？？？
或者，把被依赖的组件当作模板的类型参数？
*/
template<typename Generator>
class PtrList
{
public:
	// 使Config_成为member type
	using Config = typename Generator::Config;
private:
	// retrieve needed types from config repository
	using ElementType = typename Config::ElementType;
	using SetHeadElementType = typename Config::SetHeadElementType;
	using ReturnType = typename Config::ReturnType;
	using Destoryer = typename Config::Destoryer;
	using TypeChecker = typename Config::TypeChecker;
	using Copier = typename Config::Copier;

public:
	PtrList(SetHeadElementType& h, ReturnType* t= nullptr):
		head_(nullptr),tail_(t)
	{ setHead(h); }

	~PtrList() { Destoryer::destory(head_); }

	void setHead(SetHeadElementType& h)
	{
		TypeChecker::check(h);
		head_ = Copier::copy(h);
	}

	void setTail(ReturnType* t) { tail_ = t; }

	ElementType& head(){ return *head_; }

	ReturnType* tail(){ return tail_; }

private:
	ElementType* head_;
	ReturnType* tail_;
};

//X ============================================================================
//? 析构组件：
// 若链表保存的是元素的拷贝/自身引用，则用此析构。
template<typename ElementType>
struct ElementDestoryer
{
	static void destory(ElementType* e) { delete e; }
};

// 若对Elements的持有是“external reference”
template<typename ElementType>
struct EmptyDestoryer
{
	static void destory(ElementType* e) { }
};
//X ============================================================================
//? TypeChecker
template<typename ElementType>
struct DynamicTypeChecker
{
	static void check(ElementType& e)
	{
		assert(	typeid(e) == typeid(ElementType) );
	}
};

template<typename ElementType>
struct EmptyTypeChecker
{
	static void check(ElementType& e){ }
};
//X ============================================================================
//? 拷贝工具Copier
template<typename ElementType>
struct PolymorphicCopier
{
	static ElementType* copy(ElementType& e) { return e->clone(); }
};

template<typename ElementType>
struct MonomorphicCopier
{
	static ElementType* copy(ElementType& e) { return new ElementType(e); }
};

template<typename ElementType>
struct EmptyCopier
{
	static ElementType* copy(ElementType& e) { return &e; }
};
//X ============================================================================
template<typename BaseList>
class LengthList: public BaseList
{
public:
	using Config = typename BaseList::Config;

private:
	using ElementType = typename Config::ElementType;
	using SetHeadElementType = typename Config::SetHeadElementType;
	using ReturnType = typename Config::ReturnType;
	using LengthType = typename Config::LengthType;

public:
	LengthList(SetHeadElementType& h, ReturnType* t=nullptr):
		BaseList(h,t),length_(computedLength())
	{ }

	void setTail(ReturnType* t)
	{
		BaseList::setTail(t);
		length_ = computedLength();
	}

	const LengthType& length() const
	{
		return length_;
	}

private:
	LengthType computedLength() const
	{
		return BaseList::tail() ? BaseList::tail()->Length + 1 : 1;
	}

	LengthType length_;
};

//X ============================================================================
template<typename BaseList>
class TracedList: public BaseList
{
public:
	using Config = typename BaseList::Config;
private:
	using ElementType = typename Config::ElementType;
	using SetHeadElementType = typename Config::SetHeadElementType;
	using ReturnType = typename Config::ReturnType;

public:
	TracedList(SetHeadElementType& h, ReturnType* t = nullptr):
		BaseList(h,t)
	{ }

	void setHead(SetHeadElementType& h)
	{
		std::cout << "setHead(" << h << ")" << std::endl;
		BaseList::setHead(h);
	}

	ElementType& head()
	{
		std::cout << "head()" << std::endl;
		return BaseList::head();
	}

	void setTail(ReturnType* t)
	{
		std::cout << "setTail(t)" << std::endl;
		BaseList::setTail(t);
	}
		
};

#endif

