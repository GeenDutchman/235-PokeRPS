//personal touch
#ifndef PAIRS_H_
#define PAIRS_H_

#include<ostream>
#include<sstream>
#include<string>

template<typename X, typename Y>
struct Pair
{
	X first;
	Y second;

	/**constructor*/
	Pair(X one, Y two) : first(one), second(two) {}

	/**Required toString*/
	std::string toString() const
	{
		std::stringstream theOut;
		theOut << first << ":" << second;
		return theOut.str();
	}

	/**Required friend ostream*/
	friend std::ostream& operator<<(std::ostream& theOut, const Pair<X, Y>& thePair)
	{
		theOut << toString();
		return theOut;
	}
	
};

template<typename Kval, typename Vtype>
struct KVpair
{
	Kval key;
	Vtype value;
	KVpair* nextPair;

		//big 3: constructor, destructor, and copy constructor.
	/**Default constructor*/
	KVpair() : nextPair(NULL) {}

	/**Parameterized constructor*/
	KVpair(Kval aKey, Vtype aValue) : key(aKey), value(aValue), nextPair(NULL) {}

	/**Parameterized constructor*/
	KVpair(Kval aKey, Vtype aValue, KVpair* nextPointer) : key(aKey), value(aValue), nextPair(nextPointer) {}

	/**Destructor*/
	~KVpair()
	{
		if(nextPair != NULL)
		{
			delete nextPair;
			nextPair = NULL;
		}
	}

	/**Copy constructor*/
	KVpair(const KVpair & other) : key(other.key), value(other.value), nextPair(NULL) {}

	/**Required toString*/
	std::string toString() const
	{
		std::stringstream theOut;
		theOut << key << ":" << value;
		return theOut.str();
	}

	/**Required friend ostream*/	
	friend std::ostream& operator<<(std::ostream& theOut, const KVpair<Kval, Vtype>& thePair)
	{
		theOut << toString();
		return theOut;
	}
	
};

template<typename Kval, typename Vtype>
struct BasePair
{
	size_t bucketSize;
	KVpair<Kval, Vtype>* nextPair;

	/**Constructor*/
	BasePair() : bucketSize(0), nextPair(NULL) {}

	/**Destructor*/
	~BasePair()
	{
		if(nextPair != NULL)
		{
			delete nextPair;
			nextPair = NULL;
		}
	}

	/**Required toString*/
	std::string toString() const
	{
		std::stringstream theOut;
		theOut << bucketSize;
		return theOut.str();
	}

	/**Required friend ostream*/
	friend std::ostream& operator<<(std::ostream& theOut, const BasePair<Kval, Vtype>& thePair)
	{
		theOut << toString();
		return theOut;
	}
	
};



#endif
