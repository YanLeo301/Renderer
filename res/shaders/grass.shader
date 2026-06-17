#shader vertex
#version 330 core
	
layout(location = 0) in vec4 position;

uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec3 cameraPosition;

uniform int instancesPerSide;
uniform float patchSize;

out vec4 v_color;

const vec4 brightColor = vec4(175.0 / 255.0, 218.0 / 255.0, 44.0 / 255.0, 1.0);
const vec4 darkColor = vec4(53.0 / 255.0, 85.0 / 255.0, 7.0 / 255.0, 1.0) * 0.5;

float hash1(uint n)
{
	n = (n << 13u) ^ n;
	n = n * (n * n * 15731U + 789221U) + 1376312589U;
	return float(n & 0x7fffffffU) / 2147483647.0;
}
	
void main()
{
	int ix = int(gl_InstanceID) % instancesPerSide;
	int iz = int(gl_InstanceID) / instancesPerSide;

	float fx = float(ix);
	float fz = float(iz);

	vec2 gridPos;
	gridPos.x = (fx / float(instancesPerSide) - 0.5) * patchSize;
	gridPos.y = (fz / float(instancesPerSide) - 0.5) * patchSize;

	float jitterX = (hash1(uint(gl_InstanceID) * 73856093U) - 0.5) * (patchSize / float(instancesPerSide));
    float jitterZ = (hash1(uint(gl_InstanceID) * 19349663U) - 0.5) * (patchSize / float(instancesPerSide));

	vec3 instancePos = vec3(gridPos.x + jitterX, 0.0, gridPos.y + jitterZ);

	vec3 pos = position.xyz;
	float height = pos.y;

	float effect = 1.0 - cos(3.14159 * 0.5 * height);

	float phase = hash1(uint(gl_InstanceID) * 7654321u) * 6.28318;
	float windStrength = 0.15;
	float windSpeed = 1;
	float wave = sin(time * windSpeed + phase) * windStrength;

	pos.x += wave * effect;
	pos.z += wave * effect;

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 toCamera = cameraPosition - instancePos;
	toCamera.y = 0.0;
	toCamera = normalize(toCamera);

	vec3 right = normalize(cross(up, toCamera));
	vec3 forward = normalize(cross(right, up));

	vec3 worldPos = instancePos + right * pos.x + up * pos.y + forward * pos.z;

	gl_Position = projection * view * vec4(worldPos, 1.0);

	v_color = mix(darkColor, brightColor, height);
};

#shader fragment
#version 330 core
	
out vec4 color;

in vec4 v_color;
	
void main()
{
	color = v_color;
};