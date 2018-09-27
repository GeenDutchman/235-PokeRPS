//personal touch
#ifndef SET_H_
#define SET_H_

#include "BST.h"
#include "SetInterface.h"
#include "Pairs.h"

template<typename Xobject>
class Set : public SetInterface<Xobject>
{
private:
	size_t theSize;
	BST<Xobject> container;

public:
	/**Set constructor.*/
	Set() : theSize(0) {}

	//deconstructor
	~Set() 
	{
		this->clear();
	}

	/**Copy constructor*/
	Set(const Set& other) : theSize(other.theSize), container(other.container) {}

	/**Inserts item into the set, if the container doesn't
	already contain an element with an equivalent value.*/
	bool insert(const Xobject& item)
	{
		if(container.addNode(item))
		{
			theSize++;
			return true;
		}
		return false;
	}

	/** @return: the number of elements removed from the Set. */
	size_t erase(const Xobject& item)
	{
		if(container.removeNode(item))
		{
			theSize--;
			return 1;
		}
		return 0;
	}

	/** Removes all items from the set. */
	void clear()
	{
		if(container.clearTree())
		{
			theSize = 0;
		}
	}

	/** @return: the number of elements contained by the Set. */
	size_t size() const
	{
		return theSize;
	}

	/** @return: return 1 if contains element equivalent to item, else 0. */
	size_t count(const Xobject& item) const
	{
		if(container.contains(item))
		{
			return 1;
		}
		return 0;
	}

	/** @return: string representation of items in Set. */
	std::string toString() const
	{
		if(theSize)
		{
			return container.printInOrder();
		}
		return string("");
	}

	//required friend ostream operator<< function
	friend std::ostream& operator<< (std::ostream& theOut, const Set& theSet)
	{
		theOut << theSet.toString();
		return theOut;
	}
};

#endif
