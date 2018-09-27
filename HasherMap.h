//personal touch
#ifndef HASHER_MAP_H_
#define HASHER_MAP_H_

#include "MapInterface.h"
#include "Hasher.h"
#include "Pairs.h"


template<typename Kval, typename Vtype>
class HasherMap : public MapInterface<Kval, Vtype>
{
private:
	const size_t MAX_LINK_SIZE = 3;
	const double MAX_LOAD = .75;

	size_t hashSize; //size of table
	size_t theSize; //number of elements

	BasePair<Kval, Vtype>* hashTable;//pointers to "bucket" vectors//value is the size of the bucket

	/**Rehashes the table.*/
	void rehash()
	{
		size_t nextSize = nextPrimeSize();
		BasePair<Kval, Vtype>* nextTable = new BasePair<Kval, Vtype>[nextSize];

		Hasher h;

		HasherMap<Kval, Vtype>::Iterator scanner = this->begin();
		HasherMap<Kval, Vtype>::Iterator ender = this->end();
		while(scanner != ender)
		{
			//hash the value at scanner to find the place it belongs on the nextTable.
			Pair<Kval*, Vtype*> toMove = *scanner;
			size_t nextIndex = h.hashIt(*(toMove.first)) % nextSize;
			BasePair<Kval, Vtype>* nextLocation = nextTable + nextIndex;
			nextLocation->nextPair = new KVpair<Kval, Vtype>(*(toMove.first), *(toMove.second), nextLocation->nextPair);
			(nextLocation->bucketSize)++;

			++scanner;
		}		

		//get rid of old table
		BasePair<Kval, Vtype>* temp = hashTable;
		hashTable = nextTable;
		hashSize = nextSize;
		delete[] temp;
	}

	/**Checks the load of the table.
	@return true if load > MAX_LOAD, false otherwise.*/
	bool checkLoad()
	{
		if(double(theSize / hashSize) > MAX_LOAD)
		{
			return true;
		}
		return false;
	}

	/**Checks if number is prime.*/
	bool isPrime(size_t number) const
	{
		const size_t TWO = 2;
		const size_t THREE = 3;
		size_t scanNum = TWO;

		//just for completeness
		if(number <= THREE)
		{
			return true;
		}

		//this next part is to make it be able to just do odd numbers
		if(number % scanNum == 0)
		{
			return false;
		}
		++scanNum;

		while(scanNum < number)
		{
			if(number % scanNum == 0)
			{
				return false;
			}
			scanNum += TWO;//just do odd numbers
		}

		return true;
	}

	/**Finds the next prime number for size. Looks between the range of 2*hashSize and 4*hashSize.
	Finds the greatest prime inside of that range.  Returns that number.*/
	size_t nextPrimeSize() const
	{
		const size_t TWICE = 2;

		size_t twiceSize = TWICE * hashSize;
		size_t twiceAgain = TWICE * twiceSize;
		return nextPrime(twiceSize + 1, twiceAgain - 1); //the lower bound is the next odd number
	}

	/**recursive inside finding next prime number
	@param lower bound to check.
	@param upper bound to check.
	*/
	size_t nextPrime(size_t lower, size_t upper) const
	{
		if(isPrime(upper))
		{
			return upper;
		}
		else if(isPrime(lower))
		{
			return lower;
		}
		if(lower >= upper)
		{
			return 0;
		}

		const size_t TWO = 2;
		const size_t ONE = 1;
		size_t halfPoint = ((upper - lower) / TWO) + lower;

		if(isPrime(halfPoint))
		{
			return halfPoint;
		}

		//check right next to midpoint
		return (nextPrime(halfPoint + ONE, upper - TWO) || nextPrime(lower + TWO, halfPoint - ONE));
	}

public:
	//constructor
	HasherMap() : hashSize(MapInterface<Kval, Vtype>::BonusHashTableSize), theSize(0), hashTable(new BasePair<Kval, Vtype>[hashSize]) {}

	//destructor
	~HasherMap()
	{
		this->clear();
		delete[] hashTable;
	}

	/**Inside iterator class*/
	class Iterator
	{
	private:
		BasePair<Kval, Vtype>* target;
		KVpair<Kval, Vtype>* scanner;
		size_t maxIndex;
		size_t currIndex;

	public:
		/**Iterator constructor*/
		Iterator() : target(NULL), scanner(NULL), maxIndex(0), currIndex(0) {}

		//parameter constructor
		Iterator(BasePair<Kval, Vtype>* aPointy, size_t size) : target(aPointy), scanner(NULL), maxIndex(size), currIndex(0)
		{
			//get to the first element
			if(target == NULL)
			{
				currIndex = maxIndex; //short circuit the target scanner
			}

			while(currIndex < maxIndex && target->nextPair == NULL)
			{
				++currIndex;
				++target;
			}

			if(currIndex == maxIndex)
			{
				target = NULL;
				scanner = NULL;
			}
			else
			{
				scanner = target->nextPair;
			}
		}

		//deconstructor
		~Iterator() {}

		//copy constructor
		Iterator(const Iterator& other) : target(other.target), scanner(other.scanner), maxIndex(other.maxIndex), currIndex(other.currIndex) {}

		/**Equal comparison operator.  Returns bool.*/
		bool operator==(Iterator& rhs)
		{
			if(this->scanner == rhs.scanner) //if scanner is the same, then so is target to rhs.target
			{
				return true;
			}
			return false;
		}

		/**Inequality comparison operator.  Returns bool.*/
		bool operator!=(Iterator& rhs)
		{
			if(this->scanner != rhs.scanner) //if scanner is different, then so is target to rhs.target
			{
				return true;
			}
			return false;
		}

		/**Pre-increment operator.*/
		Iterator operator++()
		{
			if(target == NULL || scanner == NULL)
			{
				//do nothing
			}
			else if(scanner->nextPair == NULL)
			{
				++currIndex;
				++target;
				while(currIndex < maxIndex && target->nextPair == NULL)
				{
					++currIndex;
					++target;
				}

				if(currIndex == maxIndex)
				{
					target = NULL;
					scanner = NULL;
				}
				else
				{
					scanner = target->nextPair;
				}
			}
			else
			{
				scanner = scanner->nextPair; //crawl through
			}

			return *this;
		}

		/**Post-increment operator.*/ //same code as pre
		Iterator operator++(int)
		{
			if(target == NULL || scanner == NULL)
			{
				//do nothing
			}
			else if(scanner->nextPair == NULL) //like a new line feed
			{
				++currIndex;
				++target;
				while(currIndex < maxIndex && target->nextPair == NULL)
				{
					++currIndex;
					++target;
				}

				if(currIndex == maxIndex)
				{
					target = NULL;
					scanner = NULL;
				}
				else
				{
					scanner = target->nextPair;
				}
			}
			else
			{
				scanner = scanner->nextPair; //crawl through
			}

			return *this;
		}

		/**Dereferencing operator.  Returns Pair of values.*/
		/*
		Pair<Kval, Vtype> operator*()
		{
			//the outside world doesn't need to know about scanner->nextPair, so I just use a pair
			return Pair<Kval, Vtype>(scanner->key, scanner->value);
		}
		*/

		/**Dereferencing operator.  Returns Pair of pointers to values.*/
		Pair<Kval*, Vtype*> operator*() //using pointers to cut down on unnessecary copies
		{
			Kval* kPoint = &(scanner->key);
			Vtype* vPoint = &(scanner->value);
			return Pair<Kval*, Vtype*>(kPoint, vPoint);
		}

		/**Friend ostream.*/
		friend ostream& operator<<(ostream& outy, const Iterator & rhs)
		{
			outy << (*rhs).first << ":" << (*rhs).second << std::endl;
			return outy;
		}

	};

	/**Returns an Iterator pointed at beginning of HasherMap.*/
	Iterator begin()
	{
		return Iterator(hashTable, hashSize);
	}

	/**Returns an Iterator pointed at end of HasherMap.*/
	Iterator end()
	{
		return Iterator(NULL, hashSize);
	}

	/**Finds object in HasherMap using iterators.
	@return Pair of bool indicating if found, and if found, the value.  Otherwise default.*/
	Pair<bool, Vtype> find(const Kval & theKey) const
	{
		Iterator scanner = begin();
		Iterator ender = end();
		Pair<Kval, Vtype> scannedPair;
		
		while(scanner != ender)
		{
			scannedPair = *scanner;
			if(scannedPair.first == theKey)
			{
				break;
			}

			++scanner;
		}

		if(scanner == ender)
		{
			return Pair<bool, Vtype>(false, Vtype());
		}
		else
		{
			return Pair<bool, Vtype>(true, scannedPair.second);
		}
	}

	/**Reverse find.  Finds the */

	/*Adds nextThing to the map.  Does not allow duplicate entries.
	@return true if inserted, false if duplicate found.*/
	bool insert(Kval theKey, Vtype nextThing)
	{
		Hasher h;
		size_t hashCode = h.hashIt(theKey);
		size_t theIndex = hashCode % hashSize;
		BasePair<Kval, Vtype>* target = hashTable + theIndex;

		if(checkLoad())
		{
			rehash();
			//recalculate
			theIndex = hashCode % hashSize;
			target = hashTable + theIndex;
		}


		if(target->nextPair) //if there is stuff in the bucket
		{
			KVpair<Kval, Vtype>* scanner = target->nextPair; //moves scanner onto the bucket
			do
			{
				if(scanner->key == theKey) //is duplicate
				{
					return false;
				}

			}while(scanner->nextPair && (scanner = scanner->nextPair)); //first check if there is a next, then move it there

			scanner->nextPair = new KVpair<Kval, Vtype>(theKey, nextThing);
		}
		else //otherwise it didn't start scanning the bucket
		{
			target->nextPair = new KVpair<Kval, Vtype>(theKey, nextThing);
		}

		(target->bucketSize)++; //the parenthesis are just in case
		theSize++;

		return true;

	}

	/*Removes nextThing from the map.
	@return 1 if found and done, 0 if not found and not done.*/
	size_t erase(const Kval& theKey)
	{
		size_t erased = 0;
		Hasher h;
		size_t hashCode = h.hashIt(theKey);
		size_t theIndex = hashCode % hashSize;
		BasePair<Kval, Vtype>* target = hashTable + theIndex;

		if(target->nextPair) //if there is stuff in the bucket
		{
			KVpair<Kval, Vtype>* scanner = target->nextPair; //moves scanner onto the bucket
			KVpair<Kval, Vtype>* trailer = NULL;

			do
			{
				if(scanner->key == theKey) //is duplicate
				{
					if(trailer)
					{
						trailer->nextPair = scanner->nextPair; //link around the node to delete
					}
					else
					{
						target->nextPair = scanner->nextPair;
					}
					scanner->nextPair = NULL;
					delete scanner;
					scanner = NULL;

					theSize--;
					(target->bucketSize)--;
					erased++;
				}
				trailer = scanner;
			} while(scanner && scanner->nextPair && (scanner = scanner->nextPair)); //first check if there is a next, then move it there

			
		}

		return erased;
	}

	/** Read/write index access operator.
	If the key is not found, an entry is made for it.
	@return: Read and write access to the value mapped to the provided key. */
	Vtype& operator[](const Kval& key)
	{
		Hasher h;
		size_t hashCode = h.hashIt(key);
		size_t theIndex = hashCode % hashSize;

		BasePair<Kval, Vtype>* target = hashTable + theIndex;

		if(checkLoad())
		{
			rehash();
			//recalculate
			theIndex = hashCode % hashSize;
			target = hashTable + theIndex;
		}

		if(target->nextPair)//if there is a bucket
		{
			KVpair<Kval, Vtype>* scanner = target->nextPair;
			do
			{
				if(scanner->key == key) //if it is the same
				{
					return scanner->value;
				}
			} while(scanner->nextPair && (scanner = scanner->nextPair));

			//if it survives the search, then we insert
			scanner->nextPair = new KVpair<Kval, Vtype>(key, Vtype());
			scanner = scanner->nextPair;
			(target->bucketSize)++;
			theSize++;

			return scanner->value;
		}

		//if bucket empty, add to bucket
		target->nextPair = new KVpair<Kval, Vtype>(key, Vtype());
		(target->bucketSize)++;
		theSize++;

		return target->nextPair->value;
		
	}

	/** @return: number of Key-Value pairs stored in the Map. */
	size_t size() const
	{
		return theSize;
	}

	/** @return: maximum number of Key-Value pairs that the Map can hold. */
	size_t max_size() const
	{
		return hashSize * MAX_LINK_SIZE;
	}

	/** Removes all items from the Map. */
	void clear()
	{
		BasePair<Kval, Vtype>* temp = hashTable;

		size_t i = 0;
		while(i < hashSize) //goes down the table, deletes buckets
		{
			temp->bucketSize = 0; //tell us that it is an empty bucket

			if(temp->nextPair) //if nextPair is != NULL
			{
				delete temp->nextPair;
				temp->nextPair = NULL;
			}
			temp++;
			i++;
		}

		theSize = 0;
	}

	/** @return: string representation of Key-Value pairs in Map. */
	std::string toString() const
	{
		if(theSize == 0)
		{
			return std::string("Empty!");
		}

		std::stringstream outStuff;
		BasePair<Kval, Vtype>* target = hashTable;
		size_t i = 0;
		//outStuff << std::endl << "Contains:";
		while(i < hashSize)
		{
			if(target->bucketSize != 0) //if there is stuff in the bucket//the first key is the size of the bucket
			{

				KVpair<Kval, Vtype>* scanner = target->nextPair;
				do
				{
					outStuff << scanner->key << ": " << scanner->value << std::endl;
				}while(scanner->nextPair != NULL && (scanner = scanner->nextPair)); //crawl through the bucket.	
			}

			i++;
			target++; //pointer arithmatic
		}

		return outStuff.str();
	}

	//required friend ostream
	friend std::ostream& operator<< (std::ostream& theOut, const HasherMap& theMap)
	{
		theOut << theMap.toString();
		return theOut;
	}

};


#endif
