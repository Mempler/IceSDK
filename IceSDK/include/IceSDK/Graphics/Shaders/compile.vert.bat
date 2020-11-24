..\..\..\tools\win\shaderc.exe -f vs_sprite.sc -o compiled/vs_sprite.d3d9.h   --type vertex --varyingdef varying.def.sc --bin2c vs_sprite_d3d9   --platform windows -p vs_3_0
..\..\..\tools\win\shaderc.exe -f vs_sprite.sc -o compiled/vs_sprite.d3d11.h  --type vertex --varyingdef varying.def.sc --bin2c vs_sprite_d3d11  --platform windows -p vs_4_0
..\..\..\tools\win\shaderc.exe -f vs_sprite.sc -o compiled/vs_sprite.d3d12.h  --type vertex --varyingdef varying.def.sc --bin2c vs_sprite_d3d12  --platform windows -p vs_5_0
..\..\..\tools\win\shaderc.exe -f vs_sprite.sc -o compiled/vs_sprite.vulkan.h --type vertex --varyingdef varying.def.sc --bin2c vs_sprite_vulkan --platform linux   -p spirv
..\..\..\tools\win\shaderc.exe -f vs_sprite.sc -o compiled/vs_sprite.metal.h  --type vertex --varyingdef varying.def.sc --bin2c vs_sprite_metal  --platform osx     -p metal
..\..\..\tools\win\shaderc.exe -f vs_sprite.sc -o compiled/vs_sprite.glsl.h   --type vertex --varyingdef varying.def.sc --bin2c vs_sprite_glsl   --platform windows -p 120
