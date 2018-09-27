#ifndef BST_H_
#define BST_H_

#include "BSTInterface.h"

#define ITERATOR_DEBUG true

#if ITERATOR_DEBUG
#include "MyStack.h"
#include "TNode.h"
enum order { PRE_ORDER, IN_ORDER, POST_ORDER, LEVEL_ORDER };

#endif


template <typename Xobject>
class BST : public BSTInterface<Xobject>
{
private:
	TNode<Xobject> * theRoot;

	/**Code for printing a level-order representation of the tree.  Recursive.
	@param aRoot, a pointer to a node.
	@param theLevel, the level to print
	@param outData, stringstream collecting data to print.
	@return boolean value if done or not.
	*/
	bool printTreeLevel(const TNode<Xobject> * aRoot, int theLevel, std::stringstream & outData) const
	{
		//pretty much the provided code
		if(aRoot == NULL)
		{
			return false;
		}

		if(theLevel == 0) //if we have reached the desired level
		{
			outData << " " << (*aRoot).data; //print it
			if(((*aRoot).leftChild != NULL) || ((*aRoot).rightChild != NULL))
			{
				return true;
			}
			return false;
		}


		if((theLevel == 1) && (aRoot->leftChild == NULL) && (aRoot->rightChild != NULL))
		{
			outData << " _";
		}

		bool lefty = printTreeLevel(aRoot->leftChild, theLevel - 1, outData);
		bool righty = printTreeLevel(aRoot->rightChild, theLevel - 1, outData);

		if((theLevel == 1) && (aRoot->leftChild != NULL) && (aRoot->rightChild == NULL))
		{
			outData << " _";
		}

		return lefty || righty;
	}

	/**Recursive function that adds a new node to the tree.
	@param toHere, pointer to a node.
	@param newData, new information to put in the tree.
	@return boolean value indicating if completed or not. Returns false if newData is a duplicate.
	*/
	bool addNode(TNode<Xobject> * toHere, Xobject newData)
	{
		Xobject theData = (*toHere).data;

		if(theData == newData)
		{
			return false; //duplicate
		}
		else if(newData < theData)
		{
			if((*toHere).leftChild == NULL)
			{
				(*toHere).leftChild = new TNode<Xobject>(newData);
				return true;
			}
			else
			{
				//recursive call
				return addNode((*toHere).leftChild, newData);
			}
		}
		else if(newData > theData)
		{
			if((*toHere).rightChild == NULL)
			{
				(*toHere).rightChild = new TNode<Xobject>(newData);
				return true;
			}
			else
			{
				//recursive call
				return addNode((*toHere).rightChild, newData);
			}

		}

		//in case nothing goes off
		return false;
	}

	/**Recursive function to clear the tree.
	@param toRemove, address of a pointer to a node.*/
	void clearTree(TNode <Xobject> *& toRemove)

	{
		if(toRemove != NULL)
		{
			if((*toRemove).leftChild != NULL)
			{
				clearTree((*toRemove).leftChild);
			}
			if((*toRemove).rightChild != NULL)
			{
				clearTree((*toRemove).rightChild);
			}
		}

		delete toRemove; //will do nothing if toRemove is NULL
		toRemove = NULL;

		return;
	}

	/**Recursive function to get rid of a node.
	@param getRidOf, address of data to...get rid of.
	@param aNode, address of a pointer to a node.
	@return boolean value indicating if completed or not.  Returns false if not found.
	*/
	bool removeNode(const Xobject & getRidOf, TNode<Xobject> *& aNode)

	{
		if(aNode == NULL)
		{
			return false;
		}

		Xobject compareTo = aNode->data;
		if(getRidOf > compareTo)
		{
			return removeNode(getRidOf, aNode->rightChild);
		}
		else if(getRidOf < compareTo)
		{
			return removeNode(getRidOf, aNode->leftChild);
		}
		else
		{
			TNode<Xobject>* areLookingAt = aNode;

			if((aNode->leftChild == NULL) && (aNode->rightChild == NULL))
			{
				aNode = NULL;
			}
			else if(aNode->leftChild == NULL) //if no left, then point to right child
			{
				aNode = aNode->rightChild;
			}
			else if(aNode->rightChild == NULL) //if no right, the point to left child
			{
				aNode = aNode->leftChild;
			}
			else //two children
			{
				findPredecessor(aNode, aNode->leftChild);
			}

			delete areLookingAt;
			return true;
		}

		return false;
	}

	/**Recursively searches for predecessor of toReplace, and once found, switches them.
	@param toReplace, address of pointer to the node to replace.
	@param possPredecessor, a possible predecessor.
	*/
	void findPredecessor(TNode<Xobject> *& toReplace, TNode<Xobject> *& possPredecessor)
	{

		if(possPredecessor->rightChild != NULL) //if it has a right child, look at next right child
		{
			return findPredecessor(toReplace, possPredecessor->rightChild);
		}

		TNode<Xobject>* thePredecessor = possPredecessor;

		if(possPredecessor->leftChild != NULL) //if a left child
		{
			possPredecessor = possPredecessor->leftChild;
		}
		else //if no children
		{
			possPredecessor = NULL;
		}

		//edit some pointers to point in order to replace toReplace
		thePredecessor->rightChild = toReplace->rightChild;
		thePredecessor->leftChild = toReplace->leftChild;
		toReplace = thePredecessor;
	}

	/**Recursively searches for an item in the tree.
	Pre: Xobject must have comparison operators defined.
	@param thing to look for.
	@param Tnode pointer to look at.
	@return bool true if found, false if not.
	*/
	bool contains(const Xobject& thing, TNode<Xobject> * searchNode) const
	{
		if(searchNode == NULL)
		{
			return false;
		}

		Xobject searchThing = searchNode->data;
		if(thing == searchThing)
		{
			return true;
		}
		else if(thing < searchThing)
		{
			return contains(thing, searchNode->leftChild);
		}
		else// if(thing > searchThing)
		{
			return contains(thing, searchNode->rightChild);
		}
	}

	/**Creates a copy of another tree recursively*/
	void copier(const TNode<Xobject> * otherRoot)
	{
		if(otherRoot != NULL)
		{
			this->addNode(otherRoot->data);
			if(otherRoot->leftChild != NULL)
			{
				this->copier(otherRoot->leftChild);
			}
			if(otherRoot->rightChild != NULL)
			{
				this->copier(otherRoot->rightChild);
			}
		}
	}


public:
	//de/constructor
	/**Constructor for BST object.*/
	BST() : theRoot(NULL) {}

	/**Deconstructor for BST object.*/
	~BST()
	{
		if(theRoot != NULL)
		{
			clearTree(theRoot);
		}
	}

	//I may need to make a copy constructor, so...
	BST(const BST& other) : theRoot(NULL)
	{
		copier(other.theRoot);
	}

	//wrapper functions for the private recursive functions above.

	/**Recursive wrapper function to add a Node to the tree. Rejects duplicate values.
	@param newData, the new node to insert.
	@return boolean value true if added, false if duplicate.
	*/
	bool addNode(const Xobject & newData)
	{
		if(theRoot == NULL)
		{
			theRoot = new TNode<Xobject>(newData);
			return true;
		}

		return addNode(theRoot, newData);
	}

	/**Recursive wrapper function for removing a node.
	@param getRidOf, node to ...well...get rid of.
	@return boolean value true if removed, false if not found.
	*/
	bool removeNode(const Xobject & getRidOf)
	{
		return removeNode(getRidOf, theRoot);
	}

	/**Recursive wrapper function to clear the tree.
	@return true if accomplished...how could I ever get a false value?
	*/
	bool clearTree()

	{
		clearTree(theRoot);
		return true;
	}

	/**Recursive wrapper function for finding object thing.
	@param thing...to look for.
	@return boolean value indicating if found or not.
	*/
	bool contains(const Xobject& thing) const
	{
		return contains(thing, theRoot);
	}

#if ITERATOR_DEBUG
	//inside class iterator
	class Iterator
	{
	private:
		order iteratorOrder;
		TNode<Xobject> * pointy;
		MyStack< TNode<Xobject>* > whereBeen; //keeps track of all the steps to the left
		size_t level;

	public:
		//default constructor
		Iterator() : iteratorOrder(PRE_ORDER), pointy(NULL), level(0) {}

		//copy constructor
		Iterator(const Iterator & rhs) : iteratorOrder(rhs.iteratorOrder), pointy(rhs.pointy), whereBeen(rhs.whereBeen), level(0) {}

		/**Constructor for Iterator.
		@param order of travel (in-, pre-, post-, level- order) Only in-order coded.
		@param iteratorRoot, a pointer to a node for where the iterator is supposed to begin.
		*/
		Iterator(order toTravel, TNode<Xobject> * iteratorRoot) :
			iteratorOrder(toTravel), pointy(iteratorRoot), level(0)
			
		{
			if(pointy != NULL)
			{
				if(toTravel != PRE_ORDER && toTravel != LEVEL_ORDER)
				{
					while((*pointy).leftChild != NULL) //get down to the bottom left
					{
						whereBeen.push(pointy); //keep track of those lefts
						pointy = (*pointy).leftChild;
					}
				}
			}
		}

		/**Inequality operator.*/
		bool operator!=(const Iterator& rhs) const
		{
			if(this->pointy != rhs.pointy)
			{
				return true;
			}
			return false;
		}

		/**Equality operator.*/
		bool operator==(const Iterator& rhs) const
		{
			if(this->pointy == rhs.pointy)
			{
				return true;
			}
			return false;
		}

		/**Post-increment operator.  So far I only have it working with in-order.
		A wee bit complicated, thus lots of commentary.
		*/
		Iterator operator++()
		{
			if(pointy == NULL)
			{
				return *this; //make no changes
			}

			switch(iteratorOrder)
			{
				default:
				case IN_ORDER:
					if(this->pointy->rightChild != NULL) //if it has a right child
					{
						pointy = (*pointy).rightChild; //go to it
						while((*pointy).leftChild != NULL) //but if that right child has a leftmost (grand)child
						{
							whereBeen.push(pointy);
							pointy = (*pointy).leftChild; //go there
						}
					}
					else
					{

						if(whereBeen.size())
						{
							pointy = this->whereBeen.top(); //move to the location on top of the stack
							this->whereBeen.pop();
						}
						else
						{
							this->pointy = NULL; //stop moving
						}
					}
					break;
			}

			return *this;

		}

		/**Dereferencing operator.*/
		Xobject operator*() const
		{
			return (*pointy).data;
		}
	
	};

	/**Returns an in-order iterator pointing at the "beginning" of the tree.
	*/
	Iterator begin() const
	{
		return Iterator(IN_ORDER, theRoot);
	}

	/**Returns an in-order iterator pointing at the "beginning" of the tree.
	*/
	Iterator begin(order theOrder) const
	{
		return Iterator(theOrder, theRoot);
	}
	
	/**Returns an iterator pointing at the "end" of the tree.
	*/
	Iterator end() const
	{
		return Iterator();
	}

	/**Uses an iterator to find...toFind.
	@param toFind, object to find in the tree.
	@return boolean value true if found, false if not.
	*/
	bool find(const Xobject & toFind) const
	{
		Iterator scanner = begin();
		Iterator theEnd = end();

		while((scanner != theEnd) && (*scanner != toFind))
		{
			++scanner;
		}

		if(scanner == theEnd)
		{
			return false;
		}

		//otherwise, it has been found
		return true;

	}

	/**Returns an in-order string representation of the tree.
	@return in-order representation.
	*/
	std::string printInOrder() const
	{
		std::stringstream bigStream;

		Iterator scanner = begin();
		Iterator theEnd = end();

		if(scanner == theEnd) //if already at the end
		{
			return string("Empty tree.");
		}

		while(scanner != theEnd)
		{
			bigStream << *scanner << " ";
			++scanner;
		}

		return bigStream.str();
	}


#endif
	
	//toString for BST
	/**Required toString function.
	Acts as a recursive wrapper function for printTreeLevel.
	@return a formatted string of a level-order tree.
	*/
	std::string toString() const
	{
		if(theRoot == NULL)
		{
			return string(" Empty tree.");
		}

		std::stringstream bigString;

		int level = -1;
		do
		{
			bigString << std::endl << "  " << ++level << ":"; //it should increment level before printing
		} while(printTreeLevel(theRoot, level, bigString));

		return bigString.str();
	}


	/**Required friend ostream insertion operator*/
	friend std::ostream & operator<< (std::ostream & theOut, BST<Xobject> & theTree)
	{
		return theOut << theTree.toString();
	}
};


#endif