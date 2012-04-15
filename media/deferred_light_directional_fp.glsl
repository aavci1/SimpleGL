#version 330 core

// gbuffer textures
uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D positionSampler;
// screen size and camera position
uniform vec2 screenSize;
uniform vec3 cameraPos;
// common light properties
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
uniform vec3 lightColor;
// directional light properties
uniform vec3 lightDir;
// outputs
out vec4 _color;

void main() {
	vec2 texCoord = gl_FragCoord.xy / screenSize;
	// get color, normal and position
	vec3 color = texture(colorSampler, texCoord).xyz;
	vec3 normal = texture(normalSampler, texCoord).xyz;
	vec3 position = texture(positionSampler, texCoord).xyz;
	// get specular parameters
	float specularIntensity = texture(normalSampler, texCoord).w;
	float specularPower = texture(positionSampler, texCoord).w;
	// discard fragment if facing away
	float diffuseFactor = dot(normal, -lightDir);
	if (diffuseFactor <= 0)
		discard;
	// calculate diffuse light contribution
	vec3 lightContrib = lightColor * lightDiffuseIntensity * diffuseFactor;
	// calculate specular light contribution
	vec3 eyeDir = normalize(cameraPos - position);
	vec3 reflectionDir = normalize(reflect(lightDir, normal));
	float specularFactor = pow(dot(eyeDir, reflectionDir), specularPower);
	if (specularFactor > 0)
		lightContrib += lightColor * lightSpecularIntensity * specularIntensity * specularFactor;
	// calculate final color
	_color = vec4(color * lightContrib, 1.0);
}
