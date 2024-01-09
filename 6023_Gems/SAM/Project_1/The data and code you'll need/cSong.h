#ifndef _cSong_HG_
#define _cSong_HG_

#include <string>

// This represents a song in the Snotify class

class cSong
{
public:
	cSong()
	{
		this->rating = 0;
		this->numberOfTimesPlayed = 0;
	}
	std::string name;
	std::string artist;
	// These are used for user play statistics
	int rating;					// rating, by the user, from 0 to 5 (5 is best)
	int numberOfTimesPlayed;	// number of times the user has listened to this song

	// You can implement this any way you'd like, but I will be using 
	//	this method rather than the public variable. 
	// The cPerson has a common method for generating unique IDs, if you 
	//	want to use that. 
	unsigned int getUniqueID(void) { return this->uniqueID; }

	unsigned int uniqueID;
}; 

#endif

