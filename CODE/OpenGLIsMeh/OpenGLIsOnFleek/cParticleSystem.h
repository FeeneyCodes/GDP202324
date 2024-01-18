#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <vector>





class cParticleSystem
{
public:
	cParticleSystem();
	~cParticleSystem();



	struct sEmitterInfo
	{
		// Maximum possible particles on screen at once
		unsigned int maxNumParticles = 0;
		// Location of emitter (where particles are appearing from)
		glm::vec3 emitterPosition = glm::vec3(0.0f);

		// Optional offset of each particle from the emitter
		glm::vec3 positionOffsetMin = glm::vec3(0.0f);
		glm::vec3 positionOffsetMax = glm::vec3(0.0f);
		// HACK: Think about a better way to do this. Particles per second, perhaps?
		unsigned int minNumParticlesPerUpdate = 0;
		unsigned int maxNumParticlesPerUpdate = 0;
//		minPerSecond = 5
//			7.5 partciles / second  --> 1 particle per 8 ms
//		maxPerSecond = 10
		float minLifetime = 0.0f; 
		float maxLifetime = 0.0f;
		glm::vec3 initVelocityMin = glm::vec3(0.0f);
		glm::vec3 initVelocityMax = glm::vec3(0.0f);

		glm::vec3 orientationChangeMinRadians = glm::vec3(0.0f);
		glm::vec3 orientationChangeMaxRadians = glm::vec3(0.0f);
		// Gravity, a 'thrust', etc.
		glm::vec3 constantForce = glm::vec3(0.0f);
	};

	// Reset the particles to some known state
	void InitializeParticles(sEmitterInfo& initialState);

	// Interesting things, maybe?
	void Explode(unsigned int numParticles, float minVelocity, float maxVelocity);

	void Enable(void);
	void Disable(void);

	void Show_MakeVisible(void);
	void Hide_MakeInvisible(void);

	void Update(double deltaTime);
	void UpdatePosition(glm::vec3 newPosition);
	void UpdateConstantForce(glm::vec3 newForce, bool bUpdateParticlesToo = false);

	glm::vec3 getPosition(void);

	// What you return depends on how much information you need.
	// Like if there are colours, do you need that? 
	struct cParticleRender
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat orientation = glm::quat(glm::vec3(0.0f));
		// Maybe scale, orientation, etc.
	};
	void getParticleList(std::vector< cParticleRender >& vecParticles);

private:

	class cParticle
	{
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 velocity = glm::vec3(0.0f);
		glm::vec3 acceleration = glm::vec3(0.0f);
		// When this hits zero (0.0), it's "dead"
		float lifetime = 0.0f;
		//
		glm::quat orientation = glm::quat(glm::vec3(0.0f));
		glm::quat rotSpeed = glm::quat(glm::vec3(0.0f));
		glm::vec3 scaleXYZ = glm::vec3(0.0f);
	};

	std::vector< cParticle > m_particles;

	sEmitterInfo m_EmitterInitState;

	bool m_isEnabled = false;
	bool m_isVisible = false;

	double m_SecondElapsedCounter = 0.0;

	void m_SecondElapsedEvent(void);

	float m_getRandFloat(float a, float b);
	glm::vec3 m_getRandVec3Float(glm::vec3 min, glm::vec3 max);

	unsigned int m_getRandomInt(unsigned int min, unsigned int max);
	// Side note about the C++ 11 random:
	unsigned int m_getRandomInt_C11_bad(unsigned int min, unsigned int max);
	unsigned int m_getRandomInt_C11_good(unsigned int min, unsigned int max);

	// Max update time is 60Hz
	const double MAX_DELTATIME = 1.0 / 60.0;

};
