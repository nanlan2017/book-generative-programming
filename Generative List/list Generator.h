#ifndef LIST_GEN_H
#define LIST_GEN_H

#include "PtrList.h"
#include "../Hello World/_Common.h"


//? “配置DSL”
enum Ownership { ext_ref,own_ref, cp};
enum Morphology { mono, poly };

enum CounterFlag { with_counter, no_counter };
enum TracingFlag { with_tracing, no_tracing };

/*
//x 未指定时的默认值：
Ownership = cp
Morphology = mono
CounterFlag = no_counter
TracingFlag = no_tracing
LengthType = int

*/

/*
//? 生成示范
LIST_GENERATOR<Person,cp,poly,with_counter,with_tracing>::RET  MyList1;
*/

template<
	typename ElementType_,
	Ownership   ownership   = cp,
	Morphology  morphology  = mono,
	CounterFlag counterFlag = no_counter,
	TracingFlag tracingFlag = no_tracing,
	typename    LengthType_ = int
>
class LIST_GENERATOR
{
public:
	using Generator = LIST_GENERATOR<ElementType_,ownership,morphology,counterFlag,tracingFlag,LengthType_>;

private:
	enum
	{
		isCopy     =		ownership     ==cp,
		isOwnRef   =		ownership    ==own_ref,
		isMono     =		morphology    ==mono,
		hasCounter =		counterFlag   == with_counter,
		doesTracing =		tracingFlag   ==with_tracing
	};

	using Destoryer_ = typename IF<isCopy || isOwnRef,
		ElementDestoryer<ElementType_>,
		EmptyDestoryer<ElementType_>>::RET;

	using TypeChecker_ = typename IF<isMono,
		DynamicTypeChecker<ElementType_>,
		EmptyTypeChecker<ElementType_>>::RET;

	using Copier_ = typename IF<isCopy,
		typename IF<isMono, MonomorphicCopier<ElementType_>, PolymorphicCopier<ElementType_>>::RET,
		EmptyCopier<ElementType_>>::RET;

	using SetHeadElementType_ = typename IF<isCopy,
		const ElementType_,
		ElementType_>::RET;

	using List = PtrList<Generator>;

	using List_with_counter_or_not = typename IF<hasCounter,
		LengthList<List>,
		List>::RET;

	using List_with_tracing_or_not = typename IF<doesTracing,
		TracedList<List_with_counter_or_not>,
		List_with_counter_or_not>::RET;

public:
	using RET = List_with_tracing_or_not;

	struct Config
	{
		using ElementType        = ElementType_;
		using SetHeadElementType = SetHeadElementType_;
		using Destoryer          = Destoryer_;
		using TypeChecker        = TypeChecker_;
		using Copier             = Copier_;
		using LengthType         = LengthType_;
		using ReturnType         = RET;
	};
};

#endif

