#ifndef _cDog_HG_
#define _cDog_HG_

//#include "cCat.h"
class cCat;		// Forward declaration

class cDog
{
public:
	cDog()
	{
		this->theEvilCat = nullptr;
	}
	void Woof(void);
	cCat* theEvilCat;
};

#endif