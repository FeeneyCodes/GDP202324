// Simple RK4 integration framework
// Copyright (c) 2004, Glenn Fiedler
// http://www.gaffer.org/articles

#include <stdio.h>
#include <math.h>

// Make it C++ish
#include <iostream>

struct State
{
	float x;
	float v;
};

struct Derivative
{
	float dx;
	float dv;
};

// F=ma -- This function simulates Hooke's law (aka springs)
//float acceleration(const State &state, float t)
//{
//	const float k = 10;
//	const float b = 1;
//	return - k*state.x - b*state.v;
//}

// We'll instead, change THIS function to simulate gravity.
float acceleration(const State &state, float t)
{
	const float k = 10;
	const float b = 1;
	return - k*state.x - b*state.v;

	//return -9.81f;
}

// Straight up implicit or explicit Euler...
Derivative evaluate(const State &initial, float t)
{
	Derivative output;
	output.dx = initial.v;		// Velocity = deriv of location over time
	output.dv = acceleration(initial, t);
	return output;
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;
	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t+dt);
	return output;
}

// WE DON'T TOUCH THIS.... 
void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);

	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const float dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);
	
	state.x = state.x + dxdt*dt;
	state.v = state.v + dvdt*dt;
}

int main() 
{
	State state;
	// These were for simulating a spring...
	//	x = 100 --> spring pulled WAAAAY back
	//	v = 0 --> spring not moving (yet)
	//state.x = 100;
	//state.v = 0;

	// To simulate a ball being tossed in the air
	state.x = 0.1f;		// "Just" above 0.0
	state.v = 25.0f;	// Toss up at 25 m/s
	
	float t = 0;
	float dt = 1.0f;
	
	// This while keeps going until location and velocity == 0
	// Which makes sense for simulating a spring.
	//while (fabs(state.x)>0.001f || fabs(state.v)>0.001f)

	// For tossing the ball, keep simulating until ball hits
	//	ground (i.e. x == 0.0 )
	while ( state.x > 0.0f )
	{
		//printf("%.2f, %.2f\n", state.x, state.v);
		// C++ it up, baby. Oh yeah! 
		std::cout 
			<< "t= " << t 
			<< ", x= " << state.x 
			<< ", v= " << state.v << std::endl;

		integrate(state, t, dt);
		t += dt;
	}

	getc(stdin);
	
    return 0;
}
