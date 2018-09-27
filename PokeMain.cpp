// personal touch

//memory leak checker
#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

//other includes
#include<string>
#include<iostream>
#include<iomanip>
#include<sstream>
#include<fstream>

#include "Set.h"
#include "HasherMap.h"

//other defines

//using declaration
using namespace std;

//other functions for main
void openInOut(const int argCount, char* argVec[], std::ifstream& theInStream, std::ofstream& theOutStream);
void chunker(string & toChunck, string & inHere, const char delimiter);

void populateMap(HasherMap<string, string>& blankAndType, ifstream& inny);
string reverseLookup(HasherMap<string, Set<string>>& fectives, const string & toFind);
void populateMap(HasherMap<string, Set<string>>& fectives, ifstream& inny);

//enum
enum Damage { IN_EFFECTIVE, NORMAL, EFFECTIVE };
Damage battleEffect(const string& moveType, const string& pokeType, HasherMap<string, Set<string>>& work, HasherMap<string, Set<string>>& notWork, ostream& outy);

//************************************************************************************************

//A pound define for boring matching the output, if you want, you can change it...
#define MATCHY_MATCHY false


//**************************************************************

int main(int argc, char* argv[])
{
    //check for leaks
	VS_MEM_CHECK

	
	ifstream inny;
	ofstream outy;

	try
	{
		openInOut(argc, argv, inny, outy);
	}
	catch(exception & e) //if there was a problem...deal with it.
	{
		cout << e.what() << endl;
	}
	

	string theLine;
	string key, value;
	
	//an array is not an STL container. 
	const string toFill[] = { "Pokemon:", "Moves:", "Effectivities:", "Ineffectivities:", "Battles:" };
	const size_t FILLERS = 5;
	size_t filler = 0;

	HasherMap<string, string> pokemonAndType;
	HasherMap<string, string> moveAndType;

	HasherMap<string, Set<string>> effectives;
	HasherMap<string, Set<string>> inEffectives;

	HasherMap<size_t, string> battles;

	while(getline(inny, theLine))
	{
		filler = 0;
		do //find out which to fill
		{
			if(theLine == toFill[filler])
			{
				break; //break out of filler<FILLER
			}
			++filler;
		}while(filler < FILLERS);
		
		outy << toFill[filler] << endl;
		switch(filler)
		{
			case 0://pokemon
				populateMap(pokemonAndType, inny);
				outy << pokemonAndType << endl;
				break;
			case 1://moves
				populateMap(moveAndType, inny);
				outy << moveAndType << endl;
				break;
			case 2://effectivities
				populateMap(effectives, inny);
				outy << effectives << endl;
				break;
			case 3://ineffectivities
				populateMap(inEffectives, inny);
				outy << inEffectives << endl;
				break;
			case 4://battles
				filler = 0; //just using it, will get reset to 0 anyway
				while(getline(inny, theLine))
				{
					battles.insert(filler, theLine);
					++filler;
				}
				//outy << battles << endl;
				break;
			default:
				break;
		}

	}

	//now to execute battles
	size_t battle = 0;
	string pokeOne, moveOne; //name and move
	string pokeTwo, moveTwo;
	Damage oneWins = IN_EFFECTIVE, twoWins = IN_EFFECTIVE; //initiate to a default value

	while(battles.size())
	{
		theLine = battles[battle];
		//populate variables
		chunker(theLine, pokeOne, ':');
		chunker(theLine, moveOne, ' ');
		chunker(theLine, pokeTwo, ':');
		chunker(theLine, moveTwo, ' ');

#if !MATCHY_MATCHY
		outy << pokeOne << " attacks " << pokeTwo << " with " << moveOne << "!" << endl;
		oneWins = battleEffect(moveAndType[moveOne], pokemonAndType[pokeTwo], effectives, inEffectives, outy);

		outy << pokeTwo << " attacks " << pokeOne << " with " << moveTwo << "!" << endl;
		twoWins = battleEffect(moveAndType[moveTwo], pokemonAndType[pokeOne], effectives, inEffectives, outy);

		//report on the battle
		if(oneWins > twoWins)
		{
			outy << pokeOne << " wins! " << pokeTwo << " fainted!" << endl;
		}
		else if(twoWins > oneWins)
		{
			outy << pokeTwo << " wins! " << pokeOne << " fainted!" << endl;
		}
		else
		{
			outy << "It was a tie!" << endl;
		}
		outy << endl;

#else
		//pass in type of move & type of pokemon
		oneWins = battleEffect(moveAndType[moveOne], pokemonAndType[pokeTwo], effectives, inEffectives, outy);
		twoWins = battleEffect(moveAndType[moveTwo], pokemonAndType[pokeOne], effectives, inEffectives, outy);

		outy << pokeOne << " (" << moveOne << ") vs " << pokeTwo << " (" << moveTwo << "): ";
		if(oneWins > twoWins)
		{
			outy << pokeOne << " wins!" << endl;
		}
		else if(twoWins > oneWins)
		{
			outy << pokeTwo << " wins!" << endl;
		}
		else
		{
			outy << "Tie!" << endl;
		}
#endif
		
		//we don't need that battle anymore
		battles.erase(battle);
		++battle;
	}


	//************bonus
	outy << endl << "Pokedex: ***************** Bonus ************ Pokedex" << endl;
	HasherMap<string, string>::Iterator scanner = pokemonAndType.begin();
	HasherMap<string, string>::Iterator ender = pokemonAndType.end();
	while(scanner != ender)
	{
		Pair<string*, string*> pokeResult = *scanner;
		outy << "Name: " << *(pokeResult.first) << endl;
		outy << "+ Type: " << *(pokeResult.second) << endl;
		outy << "+ Weak against " << ": ";
		outy << reverseLookup(effectives, *(pokeResult.second)) << endl;
		outy << "+ Resilent to " << ": ";
		outy << reverseLookup(inEffectives, *(pokeResult.second)) << endl;
		//outy << endl;

		++scanner;
	}
	outy << "End Pokedex. ***********";


	inny.close();
	outy.close();
	return 0;
}

/**Opens the in and out streams. Requires that argVec has a listed input and output stream to open.
If not, throws exception!
@param argCount count of arguments for use in looking through argVec
@param argVec an array of pointers to characters, *should* hold input and output filenames
@param theInStream address of already created ifstream object
@param theOutStream address of already created ofstream object
*/
void openInOut(const int argCount, char * argVec[], std::ifstream & theInStream, std::ofstream & theOutStream)
{

	//open in and out file streams
	if(argCount != 3) //check if three arguments//if so, uh-oh!
	{
		std::cerr << "Hey!  I need the input *and* output files!" << std::endl;
		throw(std::invalid_argument("Give 3 arguments"));
	}

	std::cout << "The input file is: " << argVec[1] << std::endl;
	theInStream.open(argVec[1]);

	if(!theInStream) //if it did not open correctly, uh-oh!
	{
		std::cerr << "I can't open this-->" << argVec[1] << std::endl;
		throw(std::invalid_argument("Cannot open input file"));
	}

	std::cout << "The output file is: " << argVec[2] << std::endl;
	theOutStream.open(argVec[2]);


	if(!theOutStream) //if did not open correctly, uh-oh!
	{
		theInStream.close();
		std::cerr << "I can't out to this-->" << argVec[2] << std::endl;
		throw(std::invalid_argument("Cannot open output file"));

	}

}


/**Grabs the part of the string from the beginning to delimiter, sets that portion to inHere,
and then chops off the part of the toChunck that it just grabbed.
@param toChunck string to process
@param inHere storage for section you are looking for
@param delimiter limits how big of a chunk you grab
*/
void chunker(string & toChunck, string & inHere, const char delimiter = ' ')
{
	const int CORRECTION = 1; //we don't want to include the delimiter

	size_t delim = toChunck.find(delimiter); //find the delimiter position

	while(delim == 0) //if the delimiter character is the first in the toChunck, get rid of it
	{
		toChunck = toChunck.substr(delim + CORRECTION);
		delim = toChunck.find(delimiter);
	}

    //if delim == string::npos then will get rest of the string
	inHere = toChunck.substr(0, delim); //adjusts for zero based

	if(delim != string::npos)//if it found the delimiter
	{
		toChunck = toChunck.substr(delim + CORRECTION); //truncate toChunck//we don't need what we just got
	}
	else//if it didn't find it
	{
		toChunck = "";//that means we have processed the whole string and don't need it anymore
	}
}

/**Populates a map of <string, string>, using instream.
@param HasherMap of <string, string>
@param ifstream object.
*/
void populateMap(HasherMap<string, string>& blankAndType, ifstream& inny)
{
	string key, value, theLine;
	while(getline(inny, theLine) && theLine.size()) //while can get a line, and line has stuff
	{
		chunker(theLine, key, ' ');
		chunker(theLine, value, ' ');
		blankAndType[key] = value;
	}
}

/**Reverse lookup of a HasherMap.
Finds all instances of object and reports all keys.
@param hasherMap<string, set<string>>
@param string to find.
@return a string of all keys with toFind inside.*/
string reverseLookup(HasherMap<string, Set<string>> & fectives, const string & toFind)
{
	HasherMap<string, Set<string>>::Iterator scanner = fectives.begin();
	HasherMap<string, Set<string>>::Iterator ender = fectives.end();
	Set<string> * tempSet;
	stringstream result;

	while(scanner != ender)
	{
		tempSet = (*scanner).second;
		if((*tempSet).count(toFind))
		{
			result << *((*scanner).first); //hoo boy, dereference scanner to dereference first
			result << ' ';
		}

		++scanner;
	}

	return result.str();
}

/**Populates a map of <string, Set<string>>, using instream.
@param HasherMap of <string, Set<string>>
@param ifstream object.
*/
void populateMap(HasherMap<string, Set<string>>& fectives, ifstream& inny)
{
	string key, value, theLine;
	while(getline(inny, theLine) && theLine.size()) //while can get a line, and line has stuff
	{
		chunker(theLine, key, ':');
		Set<string>& theSet = fectives[key]; //temporary set

		chunker(theLine, value, ' ');
		while(value.size()) //while there is a value to insert
		{
			theSet.insert(value);
			chunker(theLine, value, ' ');
		}
	}
}

/**Returns Damage value based on whether an attack was effective, ineffective, or normal.
Depending on preprocessor commands, gives extended report.
@param string type of move.
@param type of pokemon.
@param HasherMap of <string, Set<string>> holding effective attacks.
@param HasherMap of <string, Set<string>> holding ineffective attacks.
@param outstream for reporting.
*/
Damage battleEffect(const string & moveType, const string & pokeType, HasherMap<string, Set<string>>& work, HasherMap<string, Set<string>>& notWork, ostream& outy)
{

	if((work[moveType]).count(pokeType))
	{
#if !MATCHY_MATCHY
		outy << "It was super effective!" << std::endl;
#endif
		return EFFECTIVE;
	}
	else if((notWork[moveType]).count(pokeType))
	{
#if !MATCHY_MATCHY
		outy << "It didn't work so well..." << std::endl;
#endif
		return IN_EFFECTIVE;
	}
	else
	{
#if !MATCHY_MATCHY
		outy << "It did something for sure." << std::endl;
#endif
		return NORMAL;
	}
}

