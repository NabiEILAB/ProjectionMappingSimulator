
#version 150

uniform sampler2DRect projectorTex;

in vec3 eyeNormal;
in vec4 eyePosition;
in vec4 projTextCoord;

out vec4 fragColor;

void main(){
	vec4 projTexColor = vec4( 0.0, 0.0, 0.0, 1.0);
    
	
	if( projTextCoord.z > 0.0 ) {

		projTexColor = textureProj( projectorTex, projTextCoord );
	}
	
	fragColor = projTexColor;
}
