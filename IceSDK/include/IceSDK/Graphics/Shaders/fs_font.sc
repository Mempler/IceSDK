$input v_texcoord0, v_color0

#include "../../../../third-party/bgfx/src/bgfx_shader.sh"

SAMPLER2D(s_texColour, 0);

void main()
{
	gl_FragColor = texture2D(s_texColour, v_texcoord0) * v_color0;
}
