#version 330 core

// // uniforms
uniform vec2 screenSize;
uniform vec3 cameraPos;
uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D positionSampler;
// light properties
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
// material properties
float specularPower;
float specularIntensity;
// outputs
out vec4 _color;

void main() {
	vec2 texCoord = gl_FragCoord.xy / screenSize;
	vec3 color = texture(colorSampler, texCoord).xyz;
	vec3 normal = normalize(texture(normalSampler, texCoord).xyz);
	vec3 position = texture(positionSampler, texCoord).xyz;
	// TODO: these values should come from the gbuffer
	specularPower = 32;
	specularIntensity = 1;
	
	vec3 lightContrib = vec3(0, 0, 0);
	float diffuseFactor = dot(normal, -lightDir);
	if (diffuseFactor > 0) {
		lightContrib += lightColor * lightDiffuseIntensity * diffuseFactor;
		
		vec3 eyeDir = normalize(cameraPos - position);
		vec3 reflectionDir = normalize(reflect(lightDir, normal));
		float specularFactor = pow(dot(eyeDir, reflectionDir), specularPower);
		if (specularFactor > 0)
			lightContrib += lightColor * lightSpecularIntensity * specularIntensity * specularFactor;
	}

	_color = vec4(color * lightContrib, 1.0);
}
