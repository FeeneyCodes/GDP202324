

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "cMesh.h"

#include "cVAOManager/cVAOManager.h"
#include "TextureManager/cBasicTextureManager.h"

#include <iostream>

// HACK:
extern float g_HeightAdjust;
extern glm::vec2 g_UVOffset;
extern glm::vec3 g_cameraEye;
extern cVAOManager* g_pMeshManager;
extern cBasicTextureManager* g_pTextureManager;


// ***********************************
// Named buffer object example
//
//  uniform debugSettingsBlock
//  {
//      bool bUseDebugColour;	// if this is true, then use debugColourRGBA for the colour
//      vec4 debugColourRGBA;
//  } debugDrawSettings;
//
struct sDebugSettingNUB
{
    // vec4
    bool bUseDebugColour;       // vec4   x
    float padding_discard[3];   //        y, x, w

    // vec4
    glm::vec4 debugColourRGBA;  // vec4 
};

sDebugSettingNUB g_ShaderDebugDrawSettings_Data;
GLuint g_ShaderDebugDrawSettingsBuffer_ID = -1;

bool InitNamedUniformBuffer(GLuint shaderProgramID)
{
    // Please give us one buffer "name" (i.e. number)
    glGenBuffers(1, &::g_ShaderDebugDrawSettingsBuffer_ID);

    // Init the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, ::g_ShaderDebugDrawSettingsBuffer_ID);

    // How big should this buffer be, in bytes
    glBufferData(GL_UNIFORM_BUFFER,                 // Type of buffer
                 sizeof(sDebugSettingNUB),          // Number of bytes in size
                 NULL,                              // If NOT null, then copy the data
                 GL_DYNAMIC_DRAW);                  // Hint for how this will be used


    // Unbind 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);     // 0 releases any binding


    // Connect this buffer to a binding point

    // Get the ID of the Named Uniform Block
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformBlockIndex.xhtml
    // NOTE: glGetUniformBlockIndex returns the special identifier, GL_INVALID_INDEX
    GLuint NUB_debugSettingsBlock_ID = glGetUniformBlockIndex(shaderProgramID, "debugSettingsBlockNUB");

    if ( NUB_debugSettingsBlock_ID == GL_INVALID_INDEX )
    {
        std::cout << "Error: could not find the NUB" << std::endl;
    }
    else
    {
        std::cout << "NUM is at location: " << NUB_debugSettingsBlock_ID << std::endl;
    }

    
    // Now bind the buffer to the binding point (which you pick)
    const GLuint NUM_debugSettingsBlock_binding_Point_number = 3;
    glUniformBlockBinding(shaderProgramID, NUB_debugSettingsBlock_ID, 
                          NUM_debugSettingsBlock_binding_Point_number);

    // Tie the buffer we created to the same binding point
    // Uniform block binding indices refer to blocks bound to indices in the 
    //  GL_UNIFORM_BUFFER indexed target with glBindBufferRange.
    glBindBufferRange(GL_UNIFORM_BUFFER,                                // Type of buffer
                      NUM_debugSettingsBlock_binding_Point_number,      // Binding point
                      ::g_ShaderDebugDrawSettingsBuffer_ID,             // Buffer ID ('name')
                      0,                                                // Offset (in bytes)
                      sizeof(sDebugSettingNUB) );                       // Number of bytes (size)

    return true;
}

void UpdateNamedUniformBuffer(void)
{
    // Connect ('bind') to this buffer
    glBindBuffer(GL_UNIFORM_BUFFER, ::g_ShaderDebugDrawSettingsBuffer_ID);

    // Copy the data from the C++ side
    glBufferSubData(GL_UNIFORM_BUFFER,
                    0,
                    sizeof(sDebugSettingNUB),
                    &::g_ShaderDebugDrawSettings_Data);

    // Release the binding
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return;
}

void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgramID);

void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModelParent, GLuint shaderProgramID)
{

    //         mat4x4_identity(m);
    glm::mat4 matModel = matModelParent;




    // Translation
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                                            glm::vec3(pCurrentMesh->drawPosition.x,
                                                      pCurrentMesh->drawPosition.y,
                                                      pCurrentMesh->drawPosition.z));


       // Rotation matrix generation
//    glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f),
//                                       pCurrentMesh->drawOrientation.x, // (float)glfwGetTime(),
//                                       glm::vec3(1.0f, 0.0, 0.0f));
//
//
//    glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
//                                       pCurrentMesh->drawOrientation.y, // (float)glfwGetTime(),
//                                       glm::vec3(0.0f, 1.0, 0.0f));
//
//    glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
//                                       pCurrentMesh->drawOrientation.z, // (float)glfwGetTime(),
//                                       glm::vec3(0.0f, 0.0, 1.0f));

    // Now we are all bougie, using quaternions
    glm::mat4 matRotation = glm::mat4(pCurrentMesh->get_qOrientation());


    // Scaling matrix
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                    glm::vec3(pCurrentMesh->drawScale.x,
                                              pCurrentMesh->drawScale.y,
                                              pCurrentMesh->drawScale.z));
    //--------------------------------------------------------------

    // Combine all these transformation
    matModel = matModel * matTranslate;         // Done last

//    matModel = matModel * matRotateX;
//    matModel = matModel * matRotateY;
//    matModel = matModel * matRotateZ;
    //
    matModel = matModel * matRotation;


    matModel = matModel * matScale;             // Mathematically done 1st

//        m = m * rotateZ;
//        m = m * rotateY;
//        m = m * rotateZ;



   //mat4x4_mul(mvp, p, m);
//    glm::mat4 mvp = matProjection * matView * matModel;

//    GLint mvp_location = glGetUniformLocation(shaderProgramID, "MVP");
//    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
//    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    GLint matModel_UL = glGetUniformLocation(shaderProgramID, "matModel");
    glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matModel));


    // Also calculate and pass the "inverse transpose" for the model matrix
    glm::mat4 matModel_InverseTranspose = glm::inverse(glm::transpose(matModel));

    // uniform mat4 matModel_IT;
    GLint matModel_IT_UL = glGetUniformLocation(shaderProgramID, "matModel_IT");
    glUniformMatrix4fv(matModel_IT_UL, 1, GL_FALSE, glm::value_ptr(matModel_InverseTranspose));


    if (pCurrentMesh->bIsWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }



//        glPointSize(10.0f);


        // uniform bool bDoNotLight;
    GLint bDoNotLight_UL = glGetUniformLocation(shaderProgramID, "bDoNotLight");

    if (pCurrentMesh->bDoNotLight)
    {
        // Set uniform to true
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_TRUE);
    }
    else
    {
        // Set uniform to false;
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_FALSE);
    }

//    //uniform bool bUseDebugColour;	
//    GLint bUseDebugColour_UL = glGetUniformLocation(shaderProgramID, "bUseDebugColour");
//    if (pCurrentMesh->bUseDebugColours)
//    {
//        glUniform1f(bUseDebugColour_UL, (GLfloat)GL_TRUE);
//        //uniform vec4 debugColourRGBA;
//        GLint debugColourRGBA_UL = glGetUniformLocation(shaderProgramID, "debugColourRGBA");
//        glUniform4f(debugColourRGBA_UL,
//                    pCurrentMesh->wholeObjectDebugColourRGBA.r,
//                    pCurrentMesh->wholeObjectDebugColourRGBA.g,
//                    pCurrentMesh->wholeObjectDebugColourRGBA.b,
//                    pCurrentMesh->wholeObjectDebugColourRGBA.a);
//    }
//    else
//    {
//        glUniform1f(bUseDebugColour_UL, (GLfloat)GL_FALSE);
//    }

    // Now uses a Named Uniform Block
    if ( pCurrentMesh->bUseDebugColours )
    {
        ::g_ShaderDebugDrawSettings_Data.bUseDebugColour = true;
        ::g_ShaderDebugDrawSettings_Data.debugColourRGBA = pCurrentMesh->wholeObjectDebugColourRGBA;
    }
    else
    {
        ::g_ShaderDebugDrawSettings_Data.bUseDebugColour = false;
    }

    UpdateNamedUniformBuffer();


    // FOR NOW, hardcode the texture settings

    // uniform bool bUseVertexColours;
    GLint bUseVertexColours_UL = glGetUniformLocation(shaderProgramID, "bUseVertexColours");
    glUniform1f(bUseVertexColours_UL, (GLfloat)GL_FALSE);



    SetUpTextures(pCurrentMesh, shaderProgramID);

// *********************************************************************
    // Is this using the heigth map?
    // HACK:
    GLint bUseHeightMap_UL = glGetUniformLocation(shaderProgramID, "bUseHeightMap");
    // uniform bool bUseHeightMap;
    if (pCurrentMesh->friendlyName == "Ground")
    {
        glUniform1f(bUseHeightMap_UL, (GLfloat)GL_TRUE);

        //uniform float heightScale;
        GLint heightScale_UL = glGetUniformLocation(shaderProgramID, "heightScale");
        glUniform1f(heightScale_UL, ::g_HeightAdjust);

        //uniform vec2 UVOffset;
        GLint UVOffset_UL = glGetUniformLocation(shaderProgramID, "UVOffset");
        glUniform2f(UVOffset_UL, ::g_UVOffset.x, ::g_UVOffset.y);


    }
    else
    {
        glUniform1f(bUseHeightMap_UL, (GLfloat)GL_FALSE);
    }
// *********************************************************************


// *********************************************************************
    //  Discard texture example
    //    uniform bool bUseDiscardMaskTexture;
    //    uniform sampler2D maskSamplerTexture01;
    {
        GLint bUseDiscardMaskTexture_UL = glGetUniformLocation(shaderProgramID, "bUseDiscardMaskTexture");


        // uniform bool bUseHeightMap;
//        if ( pCurrentMesh->friendlyName == "Ground" )
//        {
//            glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_TRUE);
//
//            //uniform sampler2D maskSamplerTexture01; 	// Texture unit 25
//            GLint textureUnitNumber = 25;
//            GLuint stencilMaskID = ::g_pTextureManager->getTextureIDFromName("FAKE_Stencil_Texture_612x612.bmp");
//            glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
//            glBindTexture(GL_TEXTURE_2D, stencilMaskID);
//
//            GLint bUseDiscardMaskTexture_UL = glGetUniformLocation(shaderProgramID, "maskSamplerTexture01");
//            glUniform1i(bUseDiscardMaskTexture_UL, textureUnitNumber);
//
//        }
//        else
//        {
        glUniform1f(bUseDiscardMaskTexture_UL, (GLfloat)GL_FALSE);
//        }
    }
// *********************************************************************

    std::string meshToDraw = pCurrentMesh->meshName;

    // See if there is any LOD info in this mesh? 
    if (!pCurrentMesh->vecLODs.empty())
    {
        // Yes, so choose the LOD based on distance from camera

        float distToCamera = glm::distance(pCurrentMesh->drawPosition, ::g_cameraEye);

        // Choose mesh based on distance
        for (cMesh::sLODInfo& curLOD : pCurrentMesh->vecLODs)
        {
            if (distToCamera < curLOD.minDistance)
            {
                meshToDraw = curLOD.meshName;
            }
        }

    }//if ( ! pCurrentMesh->vecLODs.empty() )

// HACK: Colour the pixel based on the vertexID
#pragma region Colour the pixel based on the vertexID
//    uniform float numVerticesInModel;
//    uniform bool bColourBasedOnVertexID;

    GLint numVerticesInModel_UL = glGetUniformLocation(shaderProgramID, "numVerticesInModel");
    GLint bColourBasedOnVertexID_ID = glGetUniformLocation(shaderProgramID, "bColourBasedOnVertexID");

    glUniform1f(bColourBasedOnVertexID_ID, (GLfloat)GL_TRUE);

#pragma endregion 


    sModelDrawInfo modelInfo;
    if (::g_pMeshManager->FindDrawInfoByModelName(meshToDraw, modelInfo))
    {
        // Found it!!!
        glUniform1f(numVerticesInModel_UL, modelInfo.numberOfVertices);


        glBindVertexArray(modelInfo.VAO_ID); 		//  enable VAO (and everything else)
        glDrawElements(GL_TRIANGLES,
                       modelInfo.numberOfIndices,
                       GL_UNSIGNED_INT,
                       0);
        glBindVertexArray(0); 			            // disable VAO (and everything else)

    }

    // Are there any child meshes on this mesh
    // std::vector<cMesh*> vec_pChildMeshes;

    glm::mat4 matRemoveScaling = glm::scale(glm::mat4(1.0f),
                                            glm::vec3(
                                                1.0f / pCurrentMesh->drawScale.x,
                                                1.0f / pCurrentMesh->drawScale.y,
                                                1.0f / pCurrentMesh->drawScale.z));

    matModel = matModel * matRemoveScaling;

    for (cMesh* pChild : pCurrentMesh->vec_pChildMeshes)
    {

        // Notice we are passing the "parent" (already transformed) matrix
        // NOT an identiy matrix

        // if you are using scaling, you can "undo" the scaling
        // i.e. add the opposite of the scale the parent had



        DrawObject(pChild, matModel, shaderProgramID);

    }//for ( cMesh* pChild 



    return;
}


void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgramID)
{
//    GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[0]);
//    if (Texture00 == 0)
//    {
//        Texture00 = ::g_pTextureManager->getTextureIDFromName("Blank_UV_Text_Texture.bmp");
//    }
//
//    // We are just going to pick texture unit 5 (for no reason, just as an example)
//    //    glActiveTexture(GL_TEXTURE5);       // #5 TEXTURE UNIT
//    glActiveTexture(GL_TEXTURE0 + 5);       // #5 TEXTURE UNIT
//    glBindTexture(GL_TEXTURE_2D, Texture00);
//
//    //uniform sampler2D texture_00;
//    GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
//    glUniform1i(texture_00_UL, 5);     // <- 5, an integer, because it's "Texture Unit #5"
//    // ***************************************************************

//    uniform sampler2D texture_00;			// 2D meaning x,y or s,t or u,v
//    uniform sampler2D texture_01;
//    uniform sampler2D texture_02;
//    uniform sampler2D texture_03;
//    uniform sampler2D texture_04;			// 2D meaning x,y or s,t or u,v
//    uniform sampler2D texture_05;
//    uniform sampler2D texture_06;
//    uniform sampler2D texture_07;
//    //... and so on
//    //uniform float textureMixRatio[8];
//    uniform vec4 textureMixRatio_0_3;
//    uniform vec4 textureMixRatio_4_7;

    //    uniform vec4 textureMixRatio_0_3;
//    uniform vec4 textureMixRatio_4_7;

    GLint textureMixRatio_0_3_UL = glGetUniformLocation(shaderProgramID, "textureMixRatio_0_3");
//    GLint textureMixRatio_4_7_UL = glGetUniformLocation(shaderProgramID, "textureMixRatio_4_7");

    glUniform4f(textureMixRatio_0_3_UL,
                pCurrentMesh->textureRatios[0],
                pCurrentMesh->textureRatios[1],
                pCurrentMesh->textureRatios[2],
                pCurrentMesh->textureRatios[3]);
//    glUniform4f(textureMixRatio_4_7_UL,
//                pCurrentMesh->textureRatios[4],
//                pCurrentMesh->textureRatios[5],
//                pCurrentMesh->textureRatios[6],
//                pCurrentMesh->textureRatios[7]);


    // Also set up the height map and discard texture

    {
        // uniform sampler2D heightMapSampler;		// Texture unit 20
        GLint textureUnitNumber = 20;
        GLuint Texture20 = ::g_pTextureManager->getTextureIDFromName("NvF5e_height_map.bmp");
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture20);
        GLint texture_20_UL = glGetUniformLocation(shaderProgramID, "heightMapSampler");
        glUniform1i(texture_20_UL, textureUnitNumber);
    }



    // Is this taking a texture from an FBO??
    if (pCurrentMesh->textureIsFromFBO)
    {
        GLint textureUnitNumber = 0;
//        GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
//        glBindTexture(GL_TEXTURE_2D, Texture00);
        glBindTexture(GL_TEXTURE_2D, pCurrentMesh->FBOTextureNumber);
        GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
        glUniform1i(texture_00_UL, textureUnitNumber);

        // Likely don't want anything else
        return;
    }


    {
        GLint textureUnitNumber = 0;
        GLuint Texture00 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture00);
        GLint texture_00_UL = glGetUniformLocation(shaderProgramID, "texture_00");
        glUniform1i(texture_00_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 1;
        GLuint Texture01 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture01);
        GLint texture_01_UL = glGetUniformLocation(shaderProgramID, "texture_01");
        glUniform1i(texture_01_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 2;
        GLuint Texture02 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture02);
        GLint texture_02_UL = glGetUniformLocation(shaderProgramID, "texture_02");
        glUniform1i(texture_02_UL, textureUnitNumber);
    }

    {
        GLint textureUnitNumber = 3;
        GLuint Texture03 = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureName[textureUnitNumber]);
        glActiveTexture(GL_TEXTURE0 + textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, Texture03);
        GLint texture_03_UL = glGetUniformLocation(shaderProgramID, "texture_03");
        glUniform1i(texture_03_UL, textureUnitNumber);
    }
    // and so on to however many texture you are using





    // Set up a skybox
    {
        // uniform samplerCube skyBoxTexture;		// Texture unit 30
        GLint textureUnit30 = 30;
        GLuint skyBoxID = ::g_pTextureManager->getTextureIDFromName("SunnyDay");
        glActiveTexture(GL_TEXTURE0 + textureUnit30);
        // NOTE: Binding is NOT to GL_TEXTURE_2D
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxID);
        GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgramID, "skyBoxTexture");
        glUniform1i(skyBoxSampler_UL, textureUnit30);
    }


    return;
}