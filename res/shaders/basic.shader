#shader vertex
#version 330 core
	
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
	
void main()
{
	gl_Position = projection * view * model * position;
	vTexCoord = texCoord;
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;
	
void main()
{
	color = texture(uTexture, vTexCoord);
};