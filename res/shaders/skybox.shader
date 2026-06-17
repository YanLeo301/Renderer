#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

out vec3 v_texCoord;

void main()
{
	v_texCoord = position;
	vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
};

#shader fragment
#version 330 core

in vec3 v_texCoord;
out vec4 color;

uniform samplerCube skybox;

void main()
{
	color = texture(skybox, v_texCoord);
};