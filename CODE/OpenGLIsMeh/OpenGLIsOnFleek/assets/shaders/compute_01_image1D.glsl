// Inspired by https://learnopengl.com/Guest-Articles/2022/Compute-Shaders/Introduction 

#version 430 core

// Take a 1D array of floats and do something with it.
// The array is 1000 locations to start
// 
// we'll call glDispatch( 1000, 1, 1)
// 
layout (local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

// https://www.khronos.org/opengl/wiki/Image_Load_Store
layout (rgba32f) uniform image1D arrayInput;
layout (rgba32f) writeonly uniform image1D arrayOutput;

void main()
{
	// Get the 1D index into this array
	// gl_WorkGroupID.x will be between 0 and 999
	// gl_NumWorkGroups will be 1000 (based on glDispatch(1000,1,1)
	// Or you could use gl_LocalInvocationIndex, which will also be 0 to 999
	
	int index = int( gl_WorkGroupID.x / gl_NumWorkGroups );
	
	// Similar to atexture: 
	// vec4 rgba = texture( texture_00, fTextureCoords.st ).rgba
	// 
	// gvec4 imageLoad(	gimage1D image, int P);
	vec4 theValue = imageLoad( arrayInput, index ).rgba;
	
	// Halve the value
	theValue.x = theValue.x / 2.0f;
	theValue.yzw = vec3(0.0f);
	
	// Save the value
	// void imageStore(	gimage1D image, int P, gvec4 data);
	imageStore(arrayOutput, index, theValue);	
}
