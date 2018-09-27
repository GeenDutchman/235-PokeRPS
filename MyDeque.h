//personal touch
#ifndef MYDEQUE_H_
#define MYDEQUE_H_

using std::string;

//#include "DequeInterface.h"

using std::stringstream;
using std::ostream;

template<typename T>
class MyDeque //: DequeInterface<T>
{
private:

	static const size_t DEFAULT_CAPACITY = 4;

	int startIndex;
	int endIndex;
	T * myContainer;
	size_t currentCapacity;
	size_t currentSize;

	/**Used to find an appropriate location howMany locations fromWhere, either forward or backward.
	@return zero based index of the "next" location.
	*/
	size_t rollIt(const size_t fromWhere, const size_t howMany, const bool forwardNotBackward) const
	{
		//const size_t ZERO_BASE_CORRECTION = 1;
		size_t nextLocation;

		if(forwardNotBackward)
		{
			nextLocation = fromWhere + howMany;
			if(nextLocation >= currentCapacity)
			{
				nextLocation %= currentCapacity;
			}
		}
		else
		{
			if(howMany > fromWhere) //fromWhere is zero-based, so if greater, *must* wrap around
			{
				size_t overShootBy = howMany - fromWhere;
				overShootBy %= currentCapacity;
				nextLocation = currentCapacity - overShootBy;
			}
			else
			{
				nextLocation = fromWhere - howMany;
			}
		}

		return nextLocation;
	}

	/**Doubles the size of myContainer*/
	void reallocate()
	{
		const size_t TWICE_THE_SIZE = 2;

		T * oldContainer = myContainer;

		myContainer = new T[currentCapacity * TWICE_THE_SIZE];

		size_t realIndex = startIndex;

		for(size_t i = 0; i < currentSize; i++)
		{
			if(realIndex >= currentCapacity) //this way it doesn't need to modulo it *every* time.
			{
				realIndex %= currentCapacity;
			}

			myContainer[i] = oldContainer[realIndex];

			realIndex++;
		}
		
		currentCapacity *= TWICE_THE_SIZE;
		startIndex = 0;
		endIndex = currentSize - 1;

		delete[] oldContainer;

	}

public:


	//de/constructors

	/**Constructor for MyDeque.*/
	MyDeque() : startIndex(0), endIndex(0), myContainer(new T[DEFAULT_CAPACITY]),
		currentCapacity(DEFAULT_CAPACITY), currentSize(0)
	{}

	/**Deconstructor for MyDeque.*/
	~MyDeque()
	{
		delete[] myContainer;
		myContainer = NULL;
	}

	/**Copy constructor for MyDeque. */
	MyDeque(const MyDeque & rhs) : startIndex(rhs.startIndex), endIndex(rhs.endIndex), myContainer(new T[rhs.DEFAULT_CAPACITY]), currentCapacity(rhs.currentCapacity), currentSize(rhs.currentSize)
	{
		size_t realIndex = startIndex;

		for(size_t i = 0; i < currentSize; i++)
		{
			if(realIndex >= currentCapacity) //this way it doesn't need to modulo it *every* time.
			{
				realIndex %= currentCapacity;
			}

			myContainer[i] = rhs.myContainer[realIndex];

			realIndex++;
		}

		//currentCapacity *= TWICE_THE_SIZE;
		startIndex = 0;
		endIndex = currentSize - 1;

	}

	/** Insert item at front of deque 
	@param T value to add.*/
	void push_front(const T& value)
	{
		const size_t ONE_MORE = 1;

		if(currentSize + ONE_MORE > currentCapacity)
		{
			this->reallocate();
		}


		if(currentSize) //if size is non-zero
		{
			startIndex = rollIt(startIndex, ONE_MORE, false); //to push_front, startIndex moves back
		}
		//otherwise just put value at startIndex

		myContainer[startIndex] = value;

		currentSize++;
	}

	/** Insert item at rear of deque.
	@param T value to add.*/
	void push_back(const T& value)
	{
		const size_t ONE_MORE = 1;

		if(currentSize + ONE_MORE > currentCapacity)
		{
			this->reallocate();
		}


		if(currentSize) //if size is non-zero
		{
			endIndex = rollIt(endIndex, ONE_MORE, true); //to push_back, endIndex moves forward
		}

		myContainer[endIndex] = value;

		currentSize++;
	}

	/** Remove the front item of the deque 
	@throws out_of_range if deque is empty.*/
	void pop_front(void)
	{
		const size_t ONE_MORE = 1;
		//not going to bother removing it, can just leave it there
		if(currentSize > ONE_MORE)
		{
			startIndex = rollIt(startIndex, ONE_MORE, true); //to pop the front, startIndex moves forward
			currentSize--; 
		}
		else if(currentSize == ONE_MORE)
		{

			currentSize--;//start is already pointing there
			/*Quick explaination: when accessing( like front()) it checks the size first.
			If the size is zero, it acts as if nothing is there.*/
		}
		else
		{
			throw(std::out_of_range("Pop_front: nothing to remove."));
		}
	}

	/** Remove the rear item of the deque.
	@throws if deque is empty.*/
	void pop_back(void)
	{
		const size_t ONE_MORE = 1;
		//not going to bother removing it, can just leave it there
		if(currentSize > ONE_MORE)
		{
			endIndex = rollIt(endIndex, ONE_MORE, false); //to pop the back, endIndex moves back
			currentSize--;
		}
		else if(currentSize == ONE_MORE)
		{
			currentSize--; //no need to rollIt
			//see note in pop_front
		}
		else
		{
			throw(std::out_of_range("Pop_back: nothing to remove."));
		}
	}

	/** Return the front item of the deque (Do not remove) 
	@throws out_of_range if deque is empty.
	@returns front item.*/
	T& front(void)
	{
		if(currentSize) //if size is not zero
		{
			return myContainer[startIndex];
		}
		else
		{
			throw(std::out_of_range("Front: Empty!"));
		}
	}

	/** Return the rear item of the deque (Do not remove) 
	@throws out_of_range if deque is empty.
	@returns rear item in deque.*/
	T& back(void)
	{
		if(currentSize) //if size is non-zero
		{
			return myContainer[endIndex];
		}
		else
		{
			throw(std::out_of_range("Back: Empty!"));
		}
	}

	/** Return the number of items in the deque
	@return size_t size of deque.*/
	size_t size(void) const
	{
		return this->currentSize;
	}

	/** Return true if deque is empty 
	@return true if empty, false if occupied.*/
	bool empty(void) const
	{
		if(currentSize) //if size is non-zero
		{
			return false;
		}
		return true;
	}

	/** Return item in deque at index (0 based) 
	@throws out_of_range if index is bigger than size.
	@returns object at the index.*/
	T& at(size_t index)
	{
		if(index < currentSize)
		{
			size_t realIndex = (startIndex + index) % currentCapacity;
			return myContainer[realIndex];
		}
		else
		{
			throw(std::out_of_range("Index not in range."));
		}
	}

	/**Searches through container for value.
	@param T value to search for.
	@throws exception if deque is empty.
	@throws exception if value not found.
	@returns index of the value.*/
	size_t search(const T value) const
	{
		if(!currentSize)
		{
			throw(std::exception::exception("Empty!"));
		}

		size_t realIndex = startIndex;
		size_t index = 0;
		do
		{
			if(realIndex >= currentCapacity)
			{
				realIndex %= currentCapacity; //loop over
			}

			if(myContainer[realIndex] == value) //if found, break do...while
			{
				break;
			}

			realIndex++;
			index++;
		} while(index < currentSize);

		if(index == currentSize)
		{
			throw(std::exception::exception("Not found!"));
		}

		return index;
	}

	/** Return the deque items 
	@returns string representation of contained objects.
	@returns "Empty" if container empty.*/
	string toString() const
	{
		stringstream bigStream;
		size_t realIndex = startIndex;

		if(!currentSize)
		{
			return string("Empty.");
		}

		for(size_t i = 0; i < currentSize; i++)
		{
			if(realIndex >= currentCapacity) //this way it doesn't need to modulo it every time.
			{
				realIndex %= currentCapacity;
			}

			bigStream << myContainer[realIndex];

			bigStream << ' ';
			realIndex++;
		}

		return bigStream.str();
	}

	/** Friend Ostream operator<<, required. Returns a representation of the container.*/
	friend std::ostream & operator<< (std::ostream & outy, const MyDeque<T> & theDeque)
	{
		outy << theDeque.toString();
		return outy;
	}

};


#endif
