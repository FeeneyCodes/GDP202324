#include "cParticleSystem.h"


cParticleSystem::cParticleSystem()
{
}

cParticleSystem::~cParticleSystem()
{
}

// Reset the particles to some known state
void cParticleSystem::InitializeParticles(sEmitterInfo& initialState)
{
	this->m_EmitterInitState = initialState;

	// Allocate all the particles now, not when they are needed. 
	// ("pool" of particles to draw from
	this->m_particles.clear();
	this->m_particles.reserve(initialState.maxNumParticles);

	// Assume the particles start invisible and dead.
	for ( unsigned int count = 0; count != initialState.maxNumParticles; count++ )
	{
		cParticle newParticle;
		// TODO: Maybe we want to set some of the particles attributes?
		//
		this->m_particles.push_back(newParticle);
	}
	// At this point, we have maxNumParticles 'dead' particles


	return;
}


void cParticleSystem::Update(double deltaTime)
{
	// Enabled?
	if ( ! this->m_isEnabled )
	{
		return;
	}

	if ( deltaTime > MAX_DELTATIME )
	{
		deltaTime = MAX_DELTATIME;
	}

	// Update the seconds elapsed (for things that happen 'per second')
	this->m_SecondElapsedCounter += deltaTime;
	if ( this->m_SecondElapsedCounter > 1.0 )
	{
		// A second has passed
		this->m_SecondElapsedEvent();
		this->m_SecondElapsedCounter -= 1.0;
	}//if ( this->m_SecondElapsedCounter > 1.0 )

	// 'create' some particles this frame
	unsigned int numParticlesToCreate = this->m_getRandomInt(this->m_EmitterInitState.minNumParticlesPerUpdate,
															 this->m_EmitterInitState.maxNumParticlesPerUpdate);
	unsigned int numParticlesCreatedSoFar = 0;
	for (cParticle &curParticle : this->m_particles)
	{
		// Created enough?
		if ( numParticlesCreatedSoFar >= numParticlesToCreate)
		{
			// Exit for loop
			break;
		}

		if (curParticle.lifetime <= 0.0)
		{
			// It's dead, Jim
			// 'Revive' it
			curParticle.lifetime = this->m_getRandFloat(this->m_EmitterInitState.minLifetime,
														this->m_EmitterInitState.maxLifetime);

			// TODO: Set the particle state
			curParticle.position = this->m_EmitterInitState.emitterPosition;
			curParticle.position += this->m_getRandVec3Float(this->m_EmitterInitState.positionOffsetMin,
			                                                 this->m_EmitterInitState.positionOffsetMax);

			curParticle.velocity = this->m_getRandVec3Float(this->m_EmitterInitState.initVelocityMin,
			                                                this->m_EmitterInitState.initVelocityMax);

			curParticle.rotSpeed = glm::quat(this->m_getRandVec3Float(this->m_EmitterInitState.orientationChangeMinRadians,
																	  this->m_EmitterInitState.orientationChangeMaxRadians));

			// TODO: Maybe we don't want this here? 
			curParticle.acceleration = this->m_EmitterInitState.constantForce;

			numParticlesCreatedSoFar++;
		}
	}//for (cParticle &curParticle :

	// Perfomrm a basic Euler integration step on any 'alive' partciles
	// std::vector< cParticle > m_particles;
	for (cParticle &curParticle : this->m_particles)
	{
		if (curParticle.lifetime > 0.0 )
		{
			// It's alive!!

			curParticle.velocity += curParticle.acceleration * (float)deltaTime;

			curParticle.position += curParticle.velocity * (float)deltaTime;

			// Orientation, too
			curParticle.orientation *= curParticle.rotSpeed;

			curParticle.lifetime -= (float)deltaTime;

		}
	}//for (cParticle &curParticle :


	return;
}


void cParticleSystem::Explode(unsigned int numParticles, float minVelocity, float maxVelocity)
{
	unsigned int numParticlesCreatedSoFar = 0;
	for (cParticle& curParticle : this->m_particles)
	{
		// Created enough?
		if (numParticlesCreatedSoFar >= numParticles)
		{
			// Exit for loop
			break;
		}

		if (curParticle.lifetime <= 0.0)
		{
			// It's dead, Jim
			// 'Revive' it
			curParticle.lifetime = this->m_getRandFloat(this->m_EmitterInitState.minLifetime,
														this->m_EmitterInitState.maxLifetime);

			// TODO: Set the particle state
			curParticle.position = this->m_EmitterInitState.emitterPosition;
//			curParticle.position += this->m_getRandVec3Float(this->m_EmitterInitState.positionOffsetMin,
//															 this->m_EmitterInitState.positionOffsetMax);

			// Pick a random direction for the particle
			glm::vec3 randDirection = this->m_getRandVec3Float(glm::vec3(-1.0f), glm::vec3(1.0f));
			// And a random velocity
			curParticle.velocity = randDirection * this->m_getRandFloat(minVelocity, maxVelocity);

			curParticle.rotSpeed = glm::quat(this->m_getRandVec3Float(this->m_EmitterInitState.orientationChangeMinRadians,
																	  this->m_EmitterInitState.orientationChangeMaxRadians));


			// TODO: Maybe we don't want this here? 
			curParticle.acceleration = this->m_EmitterInitState.constantForce;

			numParticlesCreatedSoFar++;
		}
	}//for (cParticle &curParticle :

	return;
}



void cParticleSystem::getParticleList(std::vector< cParticleRender >& vecParticles)
{
	vecParticles.clear();

	// Visible? 
	if ( ! this->m_isVisible )
	{
		return;
	}

	for (cParticle& curParticle : this->m_particles)
	{
		if ( curParticle.lifetime > 0.0 )
		{
			cParticleRender drawnParticle;
			drawnParticle.position = curParticle.position;
			// 
			drawnParticle.orientation = curParticle.orientation;

			vecParticles.push_back(drawnParticle);
		}
	}
	return;
}




void cParticleSystem::UpdatePosition(glm::vec3 newPosition)
{
	this->m_EmitterInitState.emitterPosition = newPosition;
	return;
}

void cParticleSystem::UpdateConstantForce(glm::vec3 newForce, bool bUpdateParticlesToo /*=false*/)
{
	this->m_EmitterInitState.constantForce = newForce;

	if (bUpdateParticlesToo)
	{
		for (cParticle& curParticle : this->m_particles)
		{
			curParticle.acceleration = this->m_EmitterInitState.constantForce;
		}
	}

	return;
}


void cParticleSystem::m_SecondElapsedEvent(void)
{
	// TODO: Do something? 

	return;
}


glm::vec3 cParticleSystem::getPosition(void)
{
	return this->m_EmitterInitState.emitterPosition;
}




void cParticleSystem::Enable(void)
{
	this->m_isEnabled = true;
	return;
}

void cParticleSystem::Disable(void)
{
	this->m_isEnabled = false;
}

void cParticleSystem::Show_MakeVisible(void)
{
	this->m_isVisible = true;
	return;
}

void cParticleSystem::Hide_MakeInvisible(void)
{
	this->m_isVisible = false;
	return;
}



unsigned int cParticleSystem::m_getRandomInt(unsigned int min, unsigned int max)
{
	// Got this from: https://stackoverflow.com/questions/5008804/how-do-you-generate-uniformly-distributed-random-integers
	unsigned int output = min + (rand() % static_cast<unsigned int>(max - min + 1));

	return output;
}

float cParticleSystem::m_getRandFloat(float a, float b)
{
// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}


glm::vec3 cParticleSystem::m_getRandVec3Float(glm::vec3 min, glm::vec3 max)
{
	glm::vec3 randPoint;
	randPoint.x = this->m_getRandFloat(min.x, max.x);
	randPoint.y = this->m_getRandFloat(min.y, max.y);
	randPoint.z = this->m_getRandFloat(min.z, max.z);

	return randPoint;
}











// NOTE: NEVER place an #include in the middle here. 
// You 'can', but it won't make you any pals at work...
#include <random>

// Side note about the C++ 11 random:
// The c++11 random objects are large and take time to create, 
//	so you shouldn't do something like this below, where
//	you are creating and destroying them every time you call.
// Instead, you should make them static, or init them one time
//	and use those created objects over and over
unsigned int cParticleSystem::m_getRandomInt_C11_bad(unsigned int min, unsigned int max)
{
	// Got this from: https://stackoverflow.com/questions/5008804/how-do-you-generate-uniformly-distributed-random-integers
	std::random_device rd;     // Only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min, max); // Guaranteed unbiased

	int random_integer = uni(rng);
	return random_integer;
}

//std::random_device rd;     // Only used once to initialise (seed) engine
//std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
//std::uniform_int_distribution<int> uni(min, max); // Guaranteed unbiased
//
//unsigned int cParticleSystem::m_getRandomInt_C11_good(unsigned int min, unsigned int max)
//{
//}





