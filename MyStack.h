//personal touch
#ifndef MYSTACK_H_
#define MYSTACK_H_

#include "MyDeque.h"


//that's a wrap!
template <typename T>
class MyStack
{
private:
	MyDeque<T> stackContainer;

public:
	//de/constructors
	MyStack() {}
	~MyStack() {}

	//copy constructor
	MyStack(const MyStack & rhs) : stackContainer(rhs.stackContainer) {}

	//delegated functions

	/** Insert item at top of stack */
	void push(const T& value)
	{
		stackContainer.push_back(value);
	}

	/** Remove the top item */
	void pop(void)
	{
		stackContainer.pop_back();
	}

	/** Return the top item of the stack */
	T& top(void)
	{
		return stackContainer.back();
	}

	/** Return the number of items in the stack */
	size_t size(void) const
	{
		return stackContainer.size();
	}

	/** Return true if stack is empty */
	bool empty(void) const
	{
		return stackContainer.empty();
	}

	/** Return item in stack at index (0 based) */
	T& at(size_t index)
	{
		return stackContainer.at(index);
	}

	/**Search through stack to find item value.
	@throws if stack is empty
	@throws if not found
	@return index of the value
	*/
	size_t search(const T value) const
	{
		return stackContainer.search(value);
	}

	/** Return the stack items */
	string toString() const
	{
		return stackContainer.toString();
	}

	/** Friend Ostream operator<<, required. Returns a representation of the container.*/
	friend std::ostream & operator<< (std::ostream & outy, const MyStack<T> & theStack)
	{
		outy << theStack.toString();
		return outy;
	}

};


#endif
