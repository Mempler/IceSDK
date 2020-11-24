$input v_texcoord0, v_color0, v_batchinfo

#include "../../../third-party/bgfx/src/bgfx_shader.sh"

// looks like we have to do that like that
SAMPLER2D(s_texColour0, 0);
SAMPLER2D(s_texColour1, 1);
SAMPLER2D(s_texColour2, 2);
SAMPLER2D(s_texColour3, 3);
SAMPLER2D(s_texColour4, 4);
SAMPLER2D(s_texColour5, 5);
SAMPLER2D(s_texColour6, 6);
SAMPLER2D(s_texColour7, 7);
SAMPLER2D(s_texColour8, 8);
SAMPLER2D(s_texColour9, 9);
SAMPLER2D(s_texColour10, 10);
SAMPLER2D(s_texColour11, 11);
SAMPLER2D(s_texColour12, 12);
SAMPLER2D(s_texColour13, 13);
SAMPLER2D(s_texColour14, 14);
SAMPLER2D(s_texColour15, 15); // current limit
// u_Texture[32] - for glsl, no idea in bgfx

void main()
{
	vec4 texColor = v_color0;

	if (v_batchinfo.x == 0)
		texColor *= texture2D(s_texColour0,  v_texcoord0);
	else if (v_batchinfo.x == 1)
		texColor *= texture2D(s_texColour1,  v_texcoord0);
	else if (v_batchinfo.x == 2)
		texColor *= texture2D(s_texColour2,  v_texcoord0);
	else if (v_batchinfo.x == 3)
		texColor *= texture2D(s_texColour3,  v_texcoord0);
	else if (v_batchinfo.x == 4)
		texColor *= texture2D(s_texColour4,  v_texcoord0);
	else if (v_batchinfo.x == 5)
		texColor *= texture2D(s_texColour5,  v_texcoord0);
	else if (v_batchinfo.x == 6)
		texColor *= texture2D(s_texColour6,  v_texcoord0);
	else if (v_batchinfo.x == 7)
		texColor *= texture2D(s_texColour7,  v_texcoord0);
	else if (v_batchinfo.x == 8)
		texColor *= texture2D(s_texColour8,  v_texcoord0);
	else if (v_batchinfo.x == 9)
		texColor *= texture2D(s_texColour9,  v_texcoord0);
	else if (v_batchinfo.x == 10)
		texColor *= texture2D(s_texColour10, v_texcoord0);
	else if (v_batchinfo.x == 11)
		texColor *= texture2D(s_texColour11, v_texcoord0);
	else if (v_batchinfo.x == 12)
		texColor *= texture2D(s_texColour12, v_texcoord0);
	else if (v_batchinfo.x == 13)
		texColor *= texture2D(s_texColour13, v_texcoord0);
	else if (v_batchinfo.x == 14)
		texColor *= texture2D(s_texColour14, v_texcoord0);
	else if (v_batchinfo.x == 15)
		texColor *= texture2D(s_texColour15, v_texcoord0);

	gl_FragColor = texColor;
}
