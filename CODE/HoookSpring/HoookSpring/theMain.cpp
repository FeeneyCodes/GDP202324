#include <iostream>
#include <fstream>

int main()
{
	double position = 0.0;
//	double old_position = 0.0;
	double velocity = 0.0;
//	double acceleration = -9.81;
	double acceleration = 0.0;

	double springLength = 5.1;			// Starts at 3 cm	
	double springRestLength = 5.0;		// 5 cm?
	const double K = 1.0;				// Spring stiffness

	double time = 0.0;
	double deltaTime = 0.01;
	const double MAX_TIME = 10.0;

	std::ofstream outFile("plot.txt");

	while ( (time < MAX_TIME) /*&& (position >= 0.0)*/ )
	{
		//  Hooke's law
		// Fs = kx
		// F = m*a ---> Force is linearly tied to acceleration
		// 
		// Spring is 1 kg
		// F = 1*a
		// a = 1/F

		position = springLength;

		double springDiff = springLength - springRestLength;
		double springForce = K * springDiff;

		if ( springForce <= 0.0 )
		{
			acceleration = 0.0;
		}
		else
		{
			acceleration = 1.0 / springForce;
		}


		// Euler integration
		//float deltaVelocity = acceleration * deltaTime;
		velocity += acceleration * deltaTime;

		//float deltaPosition = velocity * deltaTime;
		position += velocity * deltaTime;


		springLength = position;


		std::cout << time << "   position: " << position << std::endl;
		outFile << time << ",    " << position << std::endl;

		time += deltaTime;
	}

	outFile.close();













	return 0;
}
