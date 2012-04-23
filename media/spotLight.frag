#version 330 core

// gbuffer textures
uniform sampler2D colorBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D positionBuffer;
// screen size and camera position
uniform vec2 screenSize;
uniform vec3 cameraPos;
// common light properties
uniform vec3 lightColor;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
// point light properties
uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform float lightAngleCos;
uniform float lightAttenuationRange;
uniform float lightAttenuationConstant;
uniform float lightAttenuationLinear;
uniform float lightAttenuationQuadratic;
// outputs
out vec4 _color;

void main() {
	vec2 texCoord = gl_FragCoord.xy / screenSize;
	// get color, normal and position
	vec3 color = texture(colorBuffer, texCoord).xyz;
	vec3 normal = texture(normalBuffer, texCoord).xyz;
	vec3 position = texture(positionBuffer, texCoord).xyz;
	// get specular parameters
	float specularIntensity = texture(colorBuffer, texCoord).w;
	float specularPower = texture(normalBuffer, texCoord).w;
	// discard fragment if not within radius
	float dist = length(position - lightPos);
	if (dist > lightAttenuationRange)
		discard;
	// discard fragment if not within the cone
	vec3 lightDir = normalize(position - lightPos);
	if (dot(lightDirection, lightDir) < lightAngleCos)
		discard;
	// discard fragment if facing away
	float diffuseFactor = dot(normal, -lightDir);
	if (diffuseFactor <= 0)
		discard;
	// calculate attenuation
	float k = dist / lightAttenuationRange;
	float attenuation = lightAttenuationConstant + lightAttenuationLinear * k + lightAttenuationQuadratic * k * k;
	attenuation = clamp(1.0 - attenuation, 0.0, 1.0);
	// calculate diffuse light contribution
	vec3 lightContrib = lightColor * lightDiffuseIntensity * diffuseFactor;
	// calculate specular light contribution
	vec3 eyeDir = normalize(cameraPos - position);
	vec3 reflectionDir = normalize(reflect(lightDir, normal));
	float specularFactor = pow(dot(eyeDir, reflectionDir), specularPower);
	if (specularFactor > 0)
		lightContrib += lightColor * lightSpecularIntensity * specularIntensity * specularFactor;
	// calculate final color
	_color = vec4(color * lightContrib * attenuation, 1.0);
}
