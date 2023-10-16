#include "cDog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

float weight = 0;

void cDog::Woof(void)
{
	return;
}


void hey(void)
{
	std::string name = "Michael";
	int age = 99;
	printf("Hey there, my name is: %s and I'm %d old", name.c_str(), age );

	std::cout << "Hey there, my name is: " 
		<< name 
		<< " and I'm " 
		<< age << std::endl;
}

void Hey(void)
{
	cDog b;	// regular constructor


	// Copy constructor
	cDog c(b);

	// operator=() assignment
	cDog x = b;
	return;
}