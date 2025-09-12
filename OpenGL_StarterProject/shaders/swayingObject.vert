#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aOffset;

uniform float timeStep;
uniform vec2 windVector;
uniform mat4 modelTransformationMatrix;
uniform mat4 viewTransformationMatrix;
uniform mat4 projectionTransformationMatrix;

float minSwayingHeight = 0.1f;
float waggingHeight = 0.5f;
float swayingAmplitude = 0.25f;
float baseSwayFrequency = 3.0f;
float minHeightForRandomVerticalDisplacement = 0.5f;
float swayMultiplier = 1.3f;

out VS_OUT
{
	vec2 TexCoord;
	vec3 Normal;
	vec3 FragPos;
} vs_out;	


void SwayVertices()
{
	float windX = windVector.x;
	float windY = windVector.y;
	float swayMultiplier = 1.0f;
	float currentSwayingAmplitude = swayingAmplitude;


	vec3 baseDisplacementVector = vec3(0.0f);
	vec3 swayInWindDirection = vec3(0.0f);
	vec3 swayingVector = vs_out.FragPos;

	if(aPos.y > minHeightForRandomVerticalDisplacement)
	{
		if(aOffset.y < 0.0f)
		{
			vs_out.FragPos.y += aOffset.y * -1.0f;
		}

		else
		{
			vs_out.FragPos.y += aOffset.y;
		}
	}

	if(aPos.y > minSwayingHeight)
	{
		baseDisplacementVector = vec3(
		vs_out.FragPos.x + windX * aPos.y,
		vs_out.FragPos.y,
		vs_out.FragPos.z + windY * aPos.y);  

		float swayX = 0.0f;
		float swayZ = 0.0f;

		if(aPos.y > waggingHeight)
		{
			currentSwayingAmplitude *= swayMultiplier;
		}
		
		swayX = sin(timeStep * baseSwayFrequency * windX + gl_InstanceID)
				* windX * currentSwayingAmplitude * aPos.y;
		swayZ = sin(timeStep * baseSwayFrequency * windY + gl_InstanceID)
				* windY * currentSwayingAmplitude * aPos.y;

		if(aPos.y > waggingHeight)
		{
			if(swayX > 0.0f)
				swayX *= swayMultiplier * aPos.y;

			if(swayZ > 0.0f)
				swayZ *= swayMultiplier * aPos.y;
		}

		swayInWindDirection = vec3(swayX, 0.0f, swayZ);
		 
		swayingVector = baseDisplacementVector + swayInWindDirection;
	}

	vs_out.FragPos = swayingVector;
}

void main()
{
	vec3 randomLateralOffset = vec3(aOffset.x, 0.0f, aOffset.z);
	vs_out.FragPos = vec3(modelTransformationMatrix * vec4(aPos + randomLateralOffset, 1.0));
	vs_out.Normal = mat3(transpose(inverse(modelTransformationMatrix))) * aNormal;
	vs_out.TexCoord = aTexCoord;

	SwayVertices();

	gl_Position = projectionTransformationMatrix * viewTransformationMatrix * vec4(vs_out.FragPos, 1.0);
}