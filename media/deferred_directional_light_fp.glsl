#version 330 core

// // uniforms
uniform vec2 screenSize;
uniform vec3 cameraPos;
uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D positionSampler;
// light properties
uniform vec3 direction;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
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
	
	vec3 lightColor = ambientColor;
	float diffuseFactor = dot(normal, -direction);
	if (diffuseFactor > 0) {
		lightColor += diffuseColor * diffuseFactor;
		
		vec3 eyeDir = normalize(cameraPos - position);
		vec3 reflectionDir = normalize(reflect(direction, normal));
		float specularFactor = dot(eyeDir, reflectionDir);
		specularFactor = pow(specularFactor, specularPower);
		if (specularFactor > 0)
			lightColor += specularColor * specularIntensity * specularFactor;
	}

	_color = vec4(color * lightColor, 1.0);
}
