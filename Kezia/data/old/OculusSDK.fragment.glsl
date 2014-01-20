#version 330
uniform sampler2D u_FullScreenTexture;

uniform vec2 u_LensCenter;
uniform vec2 u_ScreenCenter;
uniform vec2 u_Scale;
uniform vec2 u_ScaleIn;
uniform vec4 u_HMDWarpParam;

in vec2 textureCoordinates;

out vec4 finalColor;

vec2 HmdWarp(vec2 tc)
{
	vec2 theta = (tc - u_LensCenter) * u_ScaleIn; //scales to [-1, 1]
	float rSq = theta.x * theta.x + theta.y * theta.y;
	vec2 rVector = theta *(		u_HMDWarpParam.x + u_HMDWarpParam.y * rSq + 
								u_HMDWarpParam.z * rSq * rSq +
								u_HMDWarpParam.w * rSq * rSq * rSq);
								
	return u_LensCenter + u_Scale * rVector;
}

bool anyCheck(vec2 v)
{
	return v.x != 0 && v.y != 0;
}

void main()
{
	vec2 tc = HmdWarp(textureCoordinates);
	if(anyCheck(clamp(tc, 	u_ScreenCenter - vec2(0.25, 0.5),
							u_ScreenCenter + vec2(0.25, 0.5)) - tc))
		discard;
		
	finalColor = texture2D(u_FullScreenTexture, tc);
}