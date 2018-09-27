//**** YOU MAY NOT MODIFY THIS DOCUMENT ****/
#ifndef MAP_INTERFACE_H
#define MAP_INTERFACE_H
#include <string>

template <typename K, typename V>
class MapInterface
{
public:
	static const int HashTableSize = 31;
	static const int BonusHashTableSize = 7;

	MapInterface() {}
	virtual ~MapInterface() {}

	/** Read/write index access operator.
	If the key is not found, an entry is made for it.
	@return: Read and write access to the value mapped to the provided key. */
	virtual V& operator[](const K& key) = 0;

	/** @return: the number of elements removed from the Map. */
	virtual size_t erase(const K& key) = 0;

	/** Removes all items from the Map. */
	virtual void clear() = 0;

	/** @return: number of Key-Value pairs stored in the Map. */
	virtual size_t size() const = 0;

	/** @return: maximum number of Key-Value pairs that the Map can hold. */
	virtual size_t max_size() const = 0;

	/** @return: string representation of Key-Value pairs in Map. */
	virtual std::string toString() const = 0;
};
#endif // MAP_INTERFACE_H