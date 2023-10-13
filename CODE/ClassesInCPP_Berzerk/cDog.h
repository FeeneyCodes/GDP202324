#ifndef _cDog_HG_
#define _cDog_HG_

//#include "cCat.h"
class cCat;		// Forward declaration

class cDog
{
public:
	void Woof(void);
	cCat* theEvilCat;
};

#endif