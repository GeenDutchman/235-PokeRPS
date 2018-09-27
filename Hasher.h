//personal touch
#ifndef HASHER_H_
#define HASHER_H_

#include <string>

using std::string;

//Hasher class object
class Hasher
{
private:
	static const size_t PRIME_BASE = 23;

	/**Returns the base to the power exponet.*/
	size_t toPowerOf(size_t base, size_t power)
	{
		size_t result = 1;
		while(power)
		{
			result *= base;
			--power;
		}
		return result;
	}

public:
	//Hasher() {}
	
	/**Hashes a string.*/
	size_t hashIt(const std::string& toKey)
	{
		
		size_t index = 0;
		std::string::const_iterator ender = toKey.end();
		size_t stringLen = toKey.length();
		for(std::string::const_iterator scanner = toKey.begin(); scanner != ender; scanner++)
		{
			index += *scanner * toPowerOf(PRIME_BASE, --stringLen);
		}

		return index;		
	}
	
	/**Returns the hash of a size_t*/
	size_t hashIt(const size_t& toKey) const
	{
		return toKey;
	}

	/**Returns the hash of an int.*/
	size_t hashIt(const int & toKey) const
	{
		return size_t(toKey);
	}
	
};



#endif


