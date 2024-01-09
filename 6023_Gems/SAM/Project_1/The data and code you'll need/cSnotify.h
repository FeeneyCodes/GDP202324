#ifndef _cSnotify_HG_
#define _cSnotify_HG_

#include "cPerson.h"
#include "cSong.h"


class cSnotify
{
public:
	cSnotify();
	~cSnotify();

	// All these return true if they work, false if not (for any reason)
	// You can add some error information to the by referenece error string, if you want. 
	bool AddUser(cPerson* pPerson, std::string& errorString);
	// This will overwrite the data contained in user 
	// NOTE: It "finds" the user by matching BOTH the "unique Snofity ID" AND the "SIN" 
	//        (Social Insurance Number)
	// So if either of these don't match (or aren't found) then it reutrns false. 
	bool UpdateUser(cPerson* pPerson, std::string& errorString);
	bool DeleteUser(unsigned int SnotifyUserID, std::string& errorString);

	bool AddSong(cSong* pSong, std::string& errorString);
	// This finds a match, based on the "unique ID", and overwrites the data. 
	bool UpdateSong(cSong* pSong, std::string& errorString);
	// ************************************************************************
	// NOTE: Deleting the song deletes it from ALL users that have this song!!!
	// ************************************************************************
	bool DeleteSong(unsigned int UniqueSongID, std::string& errorString);

	// This associates a particular song with a particular user. 
	// (returns true if the song is already there)
	bool AddSongToUserLibrary(unsigned int snotifyUserID, cSong* pNewSong, std::string& errorString);
	bool RemoveSongFromUserLibrary(unsigned int snotifyUserID, unsigned int SnotifySongID, std::string& errorString);

	bool UpdateRatingOnSong(unsigned int SnotifyUserID, unsigned int songUniqueID, unsigned int newRating);

	// Returns 0, NULL, or nullptr if you can't find the song.
	// (Can return optional information on errorString)
	// 
	// ****************************************************************************
	// NOTE: This updates the "numberOfTimesPlayed" in THIS USER'S playlist-library
	// ****************************************************************************
	cSong* GetSong(unsigned int SnotifyUserID, unsigned int songUniqueID, std::string& errorString);
	
	// Note that the songRating is "returned" by reference. 
	// If it can't find a match (i.e. returns false), then the rating isn't valid. 
	bool GetCurrentSongRating(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int &songRating);
	bool GetCurrentSongNumberOfPlays(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int &numberOfPlays);

	// Returns 0, NULL, or nullptr if these fail
	cPerson* FindUserBySIN(unsigned int SIN);
	cPerson* FindUserBySnotifyID(unsigned int SnotifyID);
	// See notes about passing by pointer.
	// The TLDR is that if I update what this is pointed to, IT'S UPDATED ACROSS THE ENTIRE SYSTEM
	// (You can also assume that with duplicates, it will return the 1st one it finds. I don't care which.)
	cSong* FindSong(std::string title, std::string artist);
	cSong* FindSong(unsigned int uniqueID);

	// Returns false if these don't work
		// This returns a COPY of the users library, in the form of a regular dynamic array.
	// See the cSnotify.cpp file for more infomration on how this is supposed to work
	// It will return false if it can't find that user
	bool GetUsersSongLibrary(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary);
	// Same, but array is sorted by song title in ascending order ("A to Z")
	bool GetUsersSongLibraryAscendingByTitle(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary);
	// Same, but array is sorted by artist name, ascending ("A to Z")
	bool GetUsersSongLibraryAscendingByArtist(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary);


	bool GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray);
	// Sorded ascending by Snotify ID
	bool GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray);

	// These should all be sorted ascending ("A to Z") IF there are duplicates
	bool FindUsersFirstName(std::string firstName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray);
	bool FindUsersLastName(std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray);
	// Sorted ascending by last name THEN first name.
	bool FindUsersFirstLastNames(std::string firstName, std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray);


};

#endif // !_cSnotify_HG_
