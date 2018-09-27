//personal touch
#ifndef TNODE_H_
#define TNODE_H_

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

template <typename Xobject>
struct TNode
{
	TNode* leftChild;
	TNode* rightChild;

	Xobject data;

	TNode(Xobject toBeContained) : leftChild(NULL), rightChild(NULL), data(toBeContained) {}
	TNode(Xobject toBeContained, TNode* lefty, TNode* righty) 
		: leftChild(lefty), rightChild(righty), data(toBeContained) {}

	~TNode()
	{
		/*
		if(leftChild != NULL)
		{
			delete leftChild;
		}
		if(rightChild != NULL)
		{
			delete rightChild;
		}
	*/
	}

	string toString() const
	{
		stringstream toConvert;
		toConvert << data;
		return toConvert.str();
	}

	friend std::ostream operator<< (std::ostream & theOut, TNode<Xobject> & theNode)
	{
		theOut << theNode.toString();
		return theOut;
	}


};



#endif
