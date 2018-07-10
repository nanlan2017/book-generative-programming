#include "_Common.h"

//X --------------------------------------------------------------------------------------------------
/*MARK
//X WHILE����Ϊ�� ��⵱ǰ����ֵ����Ϊtrue,��Ե�ǰEnv����һ�ε�����
//?								����,������ǰ����

// ��ģ�����ǱȺ���ģ���и�ǿ��ı������������Զ�������ģ�� ???
//?    �~�~�~�~�~1 �~�~�~�~ �ơ�������ģ��  ���� RET��
//?				����> ����һ�����ݣ�
//?							1. ������ enum RET 
//?							2. ����ʵ����ʵ������  using RET     
//?	   �~�~�~�~�~2 �~�~�~�~ �ơ����� ģ��  (1��ģ��ʵ���൱��һ������instance) 
//x ������ֵΪtrue/false�� �Ǹ���Env���״ֵ̬��������ġ�  ���ԡ���������ʾΪ�� Cond(Env)
//x ����һ�ε���Ҳ�Ǹ���һ��Env��������Ϊ�ǹ̶��ġ�         ���ԡ����е�������ʾΪ�� Iter(Env)

//x IF<> ��ѡ�����͵ģ����ڼ���using RET ���enum��� RETֵ Ӧ������ ?:

*/
//X --------------------------------------------------------------------------------------------------
//x While����� ���ƺ������ģ�����һ������:���յĵ���״ֵ̬��

//? While_<>
template<
		bool Condition,
		template<typename> class Con_,		
		template<typename > class Iter_,	
		typename Env_			
		>
struct While_
{
	// ִ���걾�ε������Env_
	using NextEnv = typename Iter_<Env_>::RET ;

	// �ж���һ��Env�Ƿ�Ҫ��������
	enum{ next_condition = Con_<NextEnv>::RET };

	// �����һ��Env������������ʹ����һ�ε����Ľ���� ���򷵻ص�ǰEnv�Ľ��
	
	using RET = typename IF<Condition,
							typename While_<next_condition, Con_, Iter_, NextEnv>::RET,
							Env_
						 >::RET;
};
//? While_<>ģ��ƫ�ػ�����falseʱ
template<
		template<typename> class Con_,		
		template<typename > class Iter_,	
		typename Env_						
		>
struct While_<false, Con_,Iter_,Env_>
{
	using RET = Env_	;
};

//? WHILE<> ģ��
template<
		template<typename> class Con_,		// ��1��ģ������� һ��ģ�壨�����Ϊ��1�����ͣ�����> 1�����ƺ�����ģ��
		template<typename > class Iter_,	// ��2��ģ������� һ��ģ�壨�����Ϊ��1�����ͣ�����> 1�����ƺ�����ģ��
		typename Env_						// ��3��ģ������� һ������
		>
struct WHILE
{
	enum { go = Con_<Env_>::RET  };
	using RET = typename While_<go, Con_, Iter_, Env_>::RET;
};
//X --------------------------------------------------------------------------------------------------
/*
template<bool Condition,     // ѭ���ļ�������
		int n,				 // ѭ���Ĳ���
		int i, int x, int y> // ѭ�������е�״̬: ��ǰ���ڵ��±ꡢ��ǰ���
struct Fib_
{
	enum
	{
		RET = Fib_<(i+1<n), n, i+1, x+y, x>::RET	
	};
};
*/
//? ������һ�ε�����ջ��״̬
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

//? һ�ε����ļ���
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

//? ��������������: ���ظ�Env���Ƿ�����ı�־λ
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