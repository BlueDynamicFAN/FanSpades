#version 420 

in vec3 color;
in vec4 vertPosWorld;
uniform vec3 objectColor;
in vec3 vertNormal;
in vec4 vertUV_x2;

uniform vec4 objectDiffuse;  
uniform vec4 objectSpecular;
uniform vec3 eyeLocation;
uniform bool bDontLight;
uniform bool useVertexColour;

uniform float wholeObjectAlphaTransperancy;

out vec4 finalOutputColor; 

struct sLight
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 atten;
	vec4 direction;
	vec4 param1; 
	vec4 param2;
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

const int NUMBEROFLIGHTS = 12;
uniform sLight theLights[NUMBEROFLIGHTS];


// CAN'T put texture samplers into an array (sadly)
//uniform sampler textures[10];

uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D texture04;
uniform sampler2D texture05;
uniform sampler2D texture06;
uniform sampler2D texture07;

// This is 4 x 2 floats or 8 floats
uniform vec4 texBlendWeights[2];	// x is 0, y is 1, z is 2


void main()
{
	vec4 materialDiffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 materialSpecular = objectSpecular;

	 if( useVertexColour )
	 {
	   materialDiffuse = vec4(color, 1.0f);
	 }
	 else {
		// materialDiffuse = objectDiffuse;
		//materialDiffuse = texture( texture00, vertUV_x2.st ).rgba; //Single textute

		vec4 tex0Col = texture( texture00, vertUV_x2.st ).rgba;
		vec4 tex1Col = texture( texture01, vertUV_x2.st ).rgba;
		vec4 tex2Col = texture( texture02, vertUV_x2.st ).rgba;
		vec4 tex3Col = texture( texture03, vertUV_x2.st ).rgba;
		vec4 tex4Col = texture( texture04, vertUV_x2.st ).rgba;
		vec4 tex5Col = texture( texture05, vertUV_x2.st ).rgba;
		vec4 tex6Col = texture( texture06, vertUV_x2.st ).rgba;
		vec4 tex7Col = texture( texture07, vertUV_x2.st ).rgba;
		
		//materialDiffuse = (tex0Col *0.5f) + (tex1Col *0.5f);

		materialDiffuse =  objectDiffuse
						  + (tex0Col * texBlendWeights[0].x) 	 // 0
		                  + (tex1Col * texBlendWeights[0].y)     // 1
						  + (tex2Col * texBlendWeights[0].z)     // 2
		                  + (tex3Col * texBlendWeights[0].w)     // 3
		                  + (tex4Col * texBlendWeights[1].x)     // 4
		                  + (tex5Col * texBlendWeights[1].y)     // 5
		                  + (tex6Col * texBlendWeights[1].z)     // 6
		                  + (tex7Col * texBlendWeights[1].w);    // 7

	 }

	 if(bDontLight)
	 {
		finalOutputColor = objectDiffuse;
		return;
	 }

	vec3 norm = normalize(vertNormal);

	vec4 finalObjectColor;
	vec3 lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
	vec3 lightSpecularContrib = vec3(0.0f);


	for(int i =0; i<NUMBEROFLIGHTS; i++)
	{
		if(theLights[i].param2.x == 0.0)
		{
			continue; /*Checkig if light is on*/
		}
		
		int intLightType = int(theLights[i].param1.x);
		
		/*DIRECTIONAL TYPE*/
		if (intLightType == 2)
		{
			vec3 lightContrib = theLights[i].diffuse.rgb;
			float dotProduct = dot( -theLights[i].direction.xyz,  
										normalize(norm.xyz) );

			dotProduct = max( 0.0f, dotProduct );
			lightContrib *= dotProduct;		
			finalObjectColor.rgb += (materialDiffuse.rgb * theLights[i].diffuse.rgb*lightContrib);
			finalOutputColor.rgb = finalObjectColor.rgb;
			finalOutputColor.a = wholeObjectAlphaTransperancy;

			return;
		}
		
		/*point light*/
		vec3 vLightToVertex = theLights[i].position.xyz - vertPosWorld.xyz;
	
		float distanceToLight = length(theLights[i].position.xyz - vertPosWorld.xyz);
	
		vec3 lightVector = normalize(vLightToVertex);

		float dotProduct = dot(lightVector, norm);

		dotProduct = max (0.0f, dotProduct);

		lightDiffuseContrib = dotProduct * theLights[i].diffuse.rgb; /* how bright */


		/*Specular*/	
		vec3 reflectVector = reflect(-lightVector, normalize(norm) );
		vec3 eyeVector = normalize(eyeLocation.xyz - vertPosWorld.xyz);
		float objectSpecularPower = objectSpecular.w;
		
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
					* objectSpecular.rgb;

		/*ENDOF specular

		attenuation*/
		float constant = theLights[i].atten.x;
		float linear = theLights[i].atten.y;
		float quadratic = theLights[i].atten.z;


		float attenuation =  1.0f / (constant + linear * distanceToLight + quadratic * distanceToLight*distanceToLight);

		lightDiffuseContrib*=attenuation;
		lightSpecularContrib*=attenuation;
		

				if (intLightType == 1)
		{	
			/*Calculate light vector (light to vertex, in world)*/
			
			vec3 vertexToLight =  vertPosWorld.xyz - theLights[i].position.xyz;
			vertexToLight = normalize(vertexToLight);
			
			float currentLightRayAngle = dot(vertexToLight.xyz, theLights[i].direction.xyz);
			currentLightRayAngle = max(0.0f, currentLightRayAngle);
			
			/*inside the cone*/
			float outerConeAngleCos = cos(radians(theLights[i].param1.z));
			float innerConeAngleCos = cos(radians(theLights[i].param1.y));
							
			/* Is it completely outside of the spot?*/
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				/*
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone*/
				
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
				
			}
			/* else it's 'in' the spotlight, so dont't change
			 reduce the amount of light hitting the object */

		}


		finalObjectColor.rgb += (materialDiffuse.rgb * lightDiffuseContrib.rgb)
								+ (materialSpecular.rgb * lightSpecularContrib.rgb);
		
	}

		finalOutputColor.rgb = finalObjectColor.rgb;
		finalOutputColor.a = wholeObjectAlphaTransperancy;
}