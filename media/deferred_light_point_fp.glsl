#version 330 core

// uniforms
uniform vec2 screenSize;
uniform vec3 cameraPos;
uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D positionSampler;
// light properties
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightRadius;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
uniform float lightLinearAttenuation;
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

	vec3 lightVector = position - lightPos;
	float attenuation = clamp(1.0 - length(lightVector) / lightRadius, 0.0, 1.0);
	if (length(lightVector) > lightRadius)
		discard;
	
	vec3 lightDir = normalize(lightVector);
	vec3 lightContrib = vec3(0, 0, 0);
	float diffuseFactor = dot(normal, -lightDir);
	if (diffuseFactor > 0) {
		lightContrib += attenuation * lightColor * lightDiffuseIntensity * diffuseFactor;
		
		vec3 eyeDir = normalize(cameraPos - position);
		vec3 reflectionDir = normalize(reflect(lightDir, normal));
		float specularFactor = pow(dot(eyeDir, reflectionDir), specularPower);
		if (specularFactor > 0)
			lightContrib += lightColor * lightSpecularIntensity * specularIntensity * specularFactor;
	}

	_color = vec4(color * lightContrib, 1.0);
}
