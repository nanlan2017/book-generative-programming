#include "PtrList.h"
#include "list Generator.h"

// 定义一个Config: 它实际是指定一组具体类型的Component

struct TracedCopyMonoPersonLenListConfig
{
	using ElementType        = Person;

	using SetHeadElementType = const ElementType;
	using LengthType         = int;
	using Destoryer          = ElementDestoryer<ElementType>;
	using TypeChecker        = DynamicTypeChecker <ElementType>;
	using Copier             = MonomorphicCopier<ElementType>;

	/**
	1. 元素类型为Person， 且为单态的（Mono-）:只有这一种类型的元素
	2. 长度采用int 计量
	3. 持有元素的拷贝。故Destory采用ElementDestoryer
	4. 提供长度计数、tracing功能
	*/
	using ReturnType = TracedList<
							LengthList<
								 PtrList<TracedCopyMonoPersonLenListConfig>>>;
};

using MyList = TracedCopyMonoPersonLenListConfig::ReturnType;

//X-------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// 生成示范
	using Config1 = typename LIST_GENERATOR<Person, cp, poly, with_counter, with_tracing>::Config::ElementType;
	using TEST1 = typename LIST_GENERATOR<Person, cp, poly, with_counter, with_tracing>::RET;
}

