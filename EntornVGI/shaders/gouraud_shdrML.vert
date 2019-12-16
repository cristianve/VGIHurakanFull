//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interficie MFC i Qt amb Status Bar
//******** Enric Marti (Desembre 2016)
// gouraud_shdrML.vert: Vertex Program en GLSL per implementar il.luminacio de Gouraud

uniform sampler2D texture0;
uniform bool texture;
uniform bool sw_intensity[4];
uniform bool sw_lights[gl_MaxLights];

varying vec3 WorldPos;
varying vec3 normal;
uniform float angle;
void main ()
{
	float angleInRadians=angle*0.0174533;
    mat3 rotationMatrix;
	rotationMatrix[0]=vec3(1, 0, 0);
	rotationMatrix[1]=vec3(0, cos(angleInRadians), -sin(angleInRadians));
	rotationMatrix[2]=vec3(0, sin(angleInRadians), cos(angleInRadians));
	normal = rotationMatrix*gl_Normal;

    if (texture) {
        gl_TexCoord[0] = gl_MultiTexCoord0;
    }

    
    gl_Position = ftransform();
	WorldPos = gl_Vertex.xyz;

    gl_FrontColor.rgb = vec3(1.0,1.0,1.0);
    gl_FrontColor.a = 0.0;
    gl_BackColor = vec4(0.0,0.0,0.0,0.0);
}
