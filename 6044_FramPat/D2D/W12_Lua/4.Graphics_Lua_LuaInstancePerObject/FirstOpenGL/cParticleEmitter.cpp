#include "cParticleEmitter.h"

#include "Utilities.h"		// for getRandomFloat

cParticleEmitter::sParticleDesc::sParticleDesc()
{	
	// Sets defaults

	// If lifetimes are the same, they all "die" at the same time
	this->lifetimeMin = 0.0f;		// If they are less than zero, they are dead.
	this->lifetimeMax = 60.0f;		

	this->bRegenerateParticles = true;

	// Controls the "spread" of the initial particles
	this->initVelMin = glm::vec3(-1.0f, -1.0f, -1.0f);
	this->initVelMax = glm::vec3( 1.0f,  1.0f,  1.0f);

	this->ongoingForce = glm::vec3( 0.0f, 0.0, 0.0f );

	// If this is the same as "numberOfParticles", they are ALL alive
	// (like in an explosion)
	// Default is one particle per update
	this->particlesCreatedPerUpdate = 1;

	// If zero, particles don't rotate. Can rotate at different rates
	//this->rotationPerSecond = ??;
	
	this->bRandomizeColours = false;

	this->initColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->colourRangeMin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->colourRangeMax = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->bAlphaProportionalToLifeTime = false;

	return;
};


// Allocates a "pool" of particles
bool cParticleEmitter::allocateParticles(int numberOfParticles, 
										 const sParticleDesc &initSettings, 
										 bool bOverwriteExisting /*=true*/)
{
	//
	this->particleSettings = initSettings;

	// TODO: think about if we are overwriting or not...
	this->m_vecParticles.reserve(numberOfParticles);

	cParticle tempParticle;
	// Load values from the default...
	// (which we may not want to do here... I'll exaplain in a moment)

	for ( int count = 0; count != numberOfParticles; count++ )
	{
		this->m_vecParticles.push_back(tempParticle);
	}

	return true;
}

void cParticleEmitter::m_resetParticle( cParticle &particle)
{
	// Pick a starting lifetime
	particle.life = getRandFloat( this->particleSettings.lifetimeMin, 
								  this->particleSettings.lifetimeMax );

	particle.colour = this->particleSettings.initColour;

	particle.velocity.x = getRandFloat( this->particleSettings.initVelMin.x, this->particleSettings.initVelMax.x );
	particle.velocity.y = getRandFloat( this->particleSettings.initVelMin.y, this->particleSettings.initVelMax.y );
	particle.velocity.z = getRandFloat( this->particleSettings.initVelMin.z, this->particleSettings.initVelMax.z );

	// Place particle at the location of the emitter
	// +- some bounding box value, perhaps.
	particle.position = this->position;

	// Other things, too, in a moment

	return;
}

// Returns INDEX of a particle is found, false if they are all "alive"
// Returns -1 if no particle is found.
int cParticleEmitter::m_findNextDeadParticle(void)
{
	// Linear search to find next "dead" particle
	for ( int index = 0; index != this->m_vecParticles.size(); index++ )
	{	// Is it dead, Jim?
		if ( this->m_vecParticles[index].life <= 0.0f )
		{	// It's dead, Jim.
			return index;
		}
	}//for ( int index = 0
	// Didn't find a live particle
	return -1;
}


void cParticleEmitter::Update(float deltaTime)
{
	// How many particles are we supposed to create? 
	for ( int count = 0; count != this->particleSettings.particlesCreatedPerUpdate; count++ )
	{
		// Find some "dead" particles... (less than equal to zero)
		int index = this->m_findNextDeadParticle();
		if ( index >= 0 )
		{	// Update the particle
			this->m_resetParticle( this->m_vecParticles[index] );
		}
	}//for ( int count = 0;

	// Update the particles using physics and math
	for ( int index = 0; index != this->m_vecParticles.size(); index++ )
	{
		if ( this->m_vecParticles[index].life > 0.0f )
		{	// Update it as it's alive

			// 1. Update acceleration based on forces
			this->m_vecParticles[index].acceleration 
				+= this->particleSettings.ongoingForce * deltaTime;	// y = -10

			// 2. Update velocity based on acceleration
			this->m_vecParticles[index].velocity 
				+= this->m_vecParticles[index].acceleration * deltaTime;

			// 3. Update postiion based on velocity
			this->m_vecParticles[index].position 
				+= this->m_vecParticles[index].velocity * deltaTime;


			// If they are an imposter, orient the object towards the camera
			if ( this->particleSettings.bIsImposter )
			{
				glm::vec3 rotationImposter = 
					this->m_calcImposterRotationFromEye( this->m_vecParticles[index].position );
				// We'll only set the y here as we might be altering the other angles
				this->m_vecParticles[index].rotation.y = rotationImposter.y;
			}

		}//if ( this->m_vecParticles[index].life > 0.0f )

		// "age" the particle
		this->m_vecParticles[index].life -= deltaTime;
	}//for ( int index = 0;


	return;
}




int cParticleEmitter::getParticlesCopy( std::vector<cParticle> &vecParticles )
{
	// OPTIONAL: 
	// Make sure the vector passed is already the right size, etc.
	if ( vecParticles.size() < this->m_vecParticles.size() )
	{	// It's not, so allocate and resize
		vecParticles.reserve( this->m_vecParticles.size() );
		unsigned int particleToCreate 
			= static_cast<unsigned int>( this->m_vecParticles.size() - vecParticles.size() );
		for ( int count = 0; count != particleToCreate; count++ )
		{	// Add a "dummy" particle
			cParticle emptyParticle;		// instantiating a new particle
			vecParticles.push_back( emptyParticle );	
		}
	}//if ( vecParticles.size() < this->m_vecParticles.size() )

	// At this point, the vector being copied into is big enough.
	unsigned int indexReturned = 0;
	for ( unsigned int index = 0; index != vecParticles.size(); index++ )
	{
		// Copy if particles are alive, only
		if ( this->m_vecParticles[index].life > 0.0f )
		{
			vecParticles[indexReturned] = this->m_vecParticles[index];
			indexReturned++;
		}
	}

	return indexReturned;
}

void cParticleEmitter::updateImposterEye(glm::vec3 eye)
{
	this->particleSettings.imposterEyeLocation = eye;
	return;
}


glm::vec3 cParticleEmitter::m_calcImposterRotationFromEye(glm::vec3 particlePos)
{
	// http://www.rastertek.com/dx11tut34.html
	// 	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	// angle = atan2(modelPosition.x - cameraPosition.x, 
	//               modelPosition.z - cameraPosition.z) 
	//			     * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	// rotation = (float)angle * 0.0174532925f

	float angle = atan2( particlePos.x - this->particleSettings.imposterEyeLocation.x, 
	  	                 particlePos.z - this->particleSettings.imposterEyeLocation.z );
								//* ( 180.0 / PI );
	// Since we aren't paying any attention to the x and z rotations.
	return glm::vec3(0.0f, angle, 0.0f);
}
