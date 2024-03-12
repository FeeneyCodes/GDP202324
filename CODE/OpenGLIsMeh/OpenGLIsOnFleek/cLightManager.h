#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

// This structure matches what's in the shader
class cLight
{
public:
    cLight();

    glm::vec4 position;
    glm::vec4 diffuse;	// Colour of the light (used for diffuse)
    glm::vec4 specular;	// rgb = highlight colour, w = power
    glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
    glm::vec4 direction;	// Spot, directional lights
    glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
                    // 0 = pointlight
                    // 1 = spot light
                    // 2 = directional light
    glm::vec4 param2;	// x = 0 for off, 1 for on

    void TurnOn(void);
    void TurnOff(void);

    // These are the uniform locations for this light in the shader
    GLint position_UL;
    GLint diffuse_UL;
    GLint specular_UL;
    GLint atten_UL;
    GLint direction_UL;
    GLint param1_UL;
    GLint param2_UL;
};


// This is the structure that matches the NUB for the lights:
//struct sLight
//{
//    vec4 position;
//    vec4 diffuse;	// Colour of the light (used for diffuse)
//    vec4 specular;	// rgb = highlight colour, w = power
//    vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//    vec4 direction;	// Spot, directional lights
//    vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//                    // 0 = pointlight
//                    // 1 = spot light
//                    // 2 = directional light
//    vec4 param2;	// x = 0 for off, 1 for on
//};
//
//// Now the lights are in a NUB (Named Uniform Block)
//layout (std140) uniform lightDataNUB
//{
//    sLight theLights[NUMBEROFLIGHTS];  	// 70 uniforms
//} lightInfo;


class cLightManager
{
public:
    cLightManager();

    // This is called once
    void SetUniformLocations(GLuint shaderID);

    // This is called every frame
    void UpdateUniformValues(GLuint shaderID);

    // This is for the NUB version of the shader
    bool SetUpLightNUB(GLuint shaderID);
    // NOTE: This will also copy the values from the theLights array
    //          to the theLightsNUB array
    // (So we can keep the code that updates the theLights array)
    void UpdateLightNUB(GLuint shaderID);

    static const unsigned int NUMBER_OF_LIGHTS_IM_USING = 10;
    cLight theLights[NUMBER_OF_LIGHTS_IM_USING];

private:

    // This matches the NUB in the updated shader
    class cLlightNUB
    {
    public:
        cLlightNUB();
        glm::vec4 position;
        glm::vec4 diffuse;	// Colour of the light (used for diffuse)
        glm::vec4 specular;	// rgb = highlight colour, w = power
        glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
        glm::vec4 direction;	// Spot, directional lights
        glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
                        // 0 = pointlight
                        // 1 = spot light
                        // 2 = directional light
        glm::vec4 param2;	// x = 0 for off, 1 for on
    };

    cLlightNUB theLightsForNUB[NUMBER_OF_LIGHTS_IM_USING];
    GLuint m_lightNUB_Buffer_ID = 0;

};

