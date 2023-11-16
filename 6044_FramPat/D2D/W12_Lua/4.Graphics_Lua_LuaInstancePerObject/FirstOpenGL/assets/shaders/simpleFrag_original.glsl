#version 400

// FRAGMENT Shader

in vec3 color;

out vec4 out_Colour;

void main()
{
    //gl_FragColor = vec4(color, 1.0f);

	out_Colour = vec4(color, 1.0f);
}
