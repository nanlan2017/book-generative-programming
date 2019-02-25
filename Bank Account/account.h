#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <ostream>
#include <cassert>
using namespace std;

template<typename Config_>
class PersonalAccount
{
public:
	using Config = Config_;
	using Owner = typename Config::Owner;
	using Currency = typename Config::Currency;
	using Date = typename Config::Date;
	using NumericType = typename Config::NumericType;
	using CounterType = typename Config::CounterType;
	using AccountNoType = typename Config::AccountNoType;

	PersonalAccount(Owner& own):
		owner_(own),balance_(0),transactions_(0)
	{ }

	// 对5个fields数据的访问
	const Owner& owner() { return owner_; }
	const AccountNoType& number() { return accountNo_; }
	const NumericType& balance() { return balance_; }
	const CounterType& transactions() { return transactions_; }
	const Date& openedOn() { return openedOn_; }

	const char* currency() const { return Currency::symbol(); }

	// 存款(余额加)
	void credit(const NumericType& amount)
	{
		assert(amount > 0);
		book(amount);
	}

	// 用款（余额减）
	void debit(const NumericType& amount)
	{
		assert(amount > 0);
		book(-amount);
	}

protected:
	NumericType balance_;	//x 余额；结余
	CounterType transactions_;

private:
	Owner owner_;
	const AccountNoType accountNo_;
	const Date openedOn_;

	// 记账
	void book(const NumericType& amount)
	{
		balance_ += amount;
		++transactions_;
	}

	// 禁止暴露以下接口
	PersonalAccount(const PersonalAccount&) {}; // 禁止账户拷贝：都是独一无二的
	PersonalAccount& operator =(const PersonalAccount&) { return *this; }
};

//x--------------------------------------------------------------------------
//? 货币Currency
struct DEM
{
	static const char* symbol() { return "DEM"; }
};

struct EUR
{
	static const char* symbol() { return "EUR"; }
};

struct USD
{
	static const char* symbol() { return "USD"; }
};
//x--------------------------------------------------------------------------
//? 账户号种类AccountNoType
class StaticNo
{
public:
	StaticNo():number_(counter_++){ }
	void print(std::ostream& os) const { os << number_; }
private:
	long int number_;
	static long int counter_;

	// 设为private，禁止调用
	StaticNo(const StaticNo&){ }
	StaticNo& operator =(const StaticNo&) { return *this; }
};
long int StaticNo::counter_ = 0;

inline
ostream& operator <<(ostream& os,const StaticNo& staNo)
{
	staNo.print(os);
	return os;
}

//x--------------------------------------------------------------------------

#endif

