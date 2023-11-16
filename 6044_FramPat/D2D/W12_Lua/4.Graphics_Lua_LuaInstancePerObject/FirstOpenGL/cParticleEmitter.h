#ifndef _cParticleEmitter_HG_
#define _cParticleEmitter_HG_

#include <glm/glm.hpp>
#include <vector>

class cParticleEmitter
{
public:
		
	static const int DEFAULT_NUMBER_OF_PARTICLES = 100;

	class cParticle
	{
	public:
		cParticle() : life(0.0f), colour(glm::vec4(0.0f,0.0f,0.0f,1.0f)) {};
		glm::vec3 position;
		float life;
		glm::vec3 rotation;
		glm::vec4 colour;	// RGBA
		glm::vec3 velocity;
		glm::vec3 acceleration;
		// Anything else you might want
	};

	// Used during allocation and generation to 
	// specify various characteristics of the particles
	struct sParticleDesc
	{
		sParticleDesc();	// Sets defaults
		// If lifetimes are the same, they all "die" at the same time
		float lifetimeMin, lifetimeMax;	
		bool bRegenerateParticles;
		// Controls the "spread" of the initial particles
		glm::vec3 initVelMin, initVelMax;
		glm::vec3 ongoingForce;
		// If this is the same as "numberOfParticles", they are ALL alive
		// (like in an explosion)
		int particlesCreatedPerUpdate;
		// If zero, particles don't rotate. Can rotate at different rates
		glm::vec3 rotationPerSecond;

		bool bIsImposter;
		glm::vec3 imposterEyeLocation;

		// If this is true, than a random colour between the range will
		//	be chosen, otherwise "initColour" will be used
		bool bRandomizeColours;
		glm::vec4 initColour, colourRangeMin, colourRangeMax;

		// If true, the alpha value is a ratio of the current vs the 
		//  max particle life, so they "fade" away over time
		bool bAlphaProportionalToLifeTime;
	};

	// Allocates a "pool" of particles
	bool allocateParticles(int numberOfParticles, 
						   const sParticleDesc &initSettings, 
						   bool bOverwriteExisting = true);

	void Update(float deltaTime);

	glm::vec3 position;

	// Return a COPY of the particles to be rendered. 
	// This is so this method can return very quickly and we aren't
	//  potentially "messing" with the actual vector of particles.
	// It's only "fast" if you pass a vector that's big enough and 
	//	has particles allocated for copying. 
	// +++Returns the number of particles we copied over.++
	int getParticlesCopy( std::vector<cParticle> &vecParticles );

	// This is public and can be changed at any time. 
	// It is checked at each update. 
	// You might want to more tightly control this, if you'd like
	sParticleDesc particleSettings;

	// Assumes that the imposter is rotating around the Y axis
	void updateImposterEye(glm::vec3 eye);

private:

	std::vector<cParticle> m_vecParticles;

	glm::vec3 m_calcImposterRotationFromEye(glm::vec3 particlePos);

	// This is called when we want to "re-spawn" a particle.	
	// NOTE: We AREN'T instantiating a new particle, just recylcing
	void m_resetParticle( cParticle &particle );

	// Returns INDEX of a particle is found, false if they are all "alive"
	// Returns -1 if no particle is found.
	int m_findNextDeadParticle(void);
};

#endif
