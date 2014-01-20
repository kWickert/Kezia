#version 330
uniform sampler2D u_FullScreenTexture;

uniform vec2 u_TextureRange;
uniform vec2 u_LensCenterOffset;
uniform vec4 u_Distortion;
uniform float u_AspectRatio;
uniform float u_FillScale;

in vec2 TextureCoordinates;

out vec4 FinalColor;

vec2 TextureSpaceToDistortionSpace(vec2 coords)
{
	vec2 result = coords * 2.0 - 1.0;

	result -= u_LensCenterOffset;

	result.y /= u_AspectRatio;

	return result;
}

vec2 DistortionSpaceToTextureSpace(vec2 coords)
{
	vec2 result = coords / u_FillScale;

	result.y *= u_AspectRatio;

	result += u_LensCenterOffset;

	result /= 2.0;
	result += 0.5;

	return result;
}

float DistortionScale(vec2 coords)
{
	float radiusSquared = coords.x * coords.x + coords.y * coords.y;
	float distortionScale = 
		u_Distortion[0] +
		u_Distortion[1] * radiusSquared +
		u_Distortion[2] * radiusSquared * radiusSquared +
		u_Distortion[3] * radiusSquared * radiusSquared * radiusSquared;

	return distortionScale;
}

void main()
{
	vec2 distortedCoords = TextureSpaceToDistortionSpace(TextureCoordinates);
	distortedCoords *= DistortionScale(distortedCoords);

	vec2 finalCoords = DistortionSpaceToTextureSpace(distortedCoords);// * u_TexRange;

	if(!all(
		equal(	clamp(finalCoords, vec2(0, 0), vec2(1, 1)), 
				finalCoords))
	{
		FinalColor = vec4(1, 0, 0, 1);
	}
	else
	{
		FinalColor = texture2D(u_FullScreenTexture, finalCoords);
	}
}