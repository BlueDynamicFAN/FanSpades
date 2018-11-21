#version 420 

uniform mat4 matModel;
uniform mat4 matModelInvTrans;
uniform mat4 matView;
uniform mat4 matProj;

in vec3 vCol;
in vec3 vPos;
in vec3 vNorm;
in vec4 vUV_x2;  /*Texture coordinates (2 of them)*/

out vec3 color;
out vec4 vertPosWorld;
out vec3 vertNormal;
out vec4 vertUV_x2; /*To the next shader*/

void main()
{
	vec3 posTemp = vPos;
	mat4 MVP = matProj * matView * matModel;

    gl_Position = MVP * vec4(posTemp, 1.0);
	vertPosWorld = matModel * vec4(posTemp, 1.0);

	vertNormal = vec3( matModelInvTrans * vec4(normalize(vNorm), 1.0f));

	/*if(posTemp.y > 0.2)
	{
		color = vec3(0.215, 0.525, 0.074);
	}
	else
	{
		color = vec3(0.176, 0.823, 0.788);
	}*/

    color = vCol;
	// Pass the texture coordinates out, unchanged.
	vertUV_x2.xy = vUV_x2.xy;
}