

#include <ostream>
using namespace std;

//? Interface：Swappable
class Swappable
{
public:
	virtual Swappable& assign(const Swappable& rhs) = 0;	// 纯虚函数（相当于Java中的abstract方法）

	Swappable& operator =(const Swappable& rhs) { return assign(rhs);  }

	virtual Swappable* copy() const = 0;
	virtual void print(std::ostream& os) const = 0;
	virtual ~Swappable() = 0;

};

std::ostream& operator <<(ostream& os,const Swappable& rhs)
{
	rhs.print(os);
	return os;
}

//x------------------------------------------------------------------------
//? Int
class Int : public Swappable
{
public:
	Int(int n) :value(n) {}

	Int& operator =(const Int& rhs)
	{
		return assign(rhs);
	}

	Int& assign()(const Swappable& rhs)
	{
		if (this != &rhs) {
			value = dynamic_cast<const Int&>(rhs).value;	//x dynamic_cast: 用于基类型 向派生类型 转化。
			return *this;
		}
	}

	Swappable* copy() const
	{
		return new Int(value);
	}

	void print(ostream& os) const
	{
		os << value;
	}


private:
	int value;
};
