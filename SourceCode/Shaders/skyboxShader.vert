#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;


void main()
{
    TexCoords = aPos;

    mat4 view = mat4(mat3(viewMatrix));  
    gl_Position = projMatrix * view * vec4(aPos, 1.0);
}  