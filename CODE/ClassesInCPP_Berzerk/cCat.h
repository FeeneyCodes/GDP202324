#pragma once
#include <string>
#include <iostream>

//#include "cDog.h"
class cDog;

template <class T>
void Meow(T sound);


class cCat
{
public:
	void Meow(void);
	cDog* animalIHate;
private:
//	float m_weight;
};











template <class T>
void Meow(T sound)
{
	std::cout << sound << std::endl;
	return;
}
