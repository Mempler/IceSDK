..\..\..\tools\win\shaderc.exe -f fs_sprite.sc -o compiled/fs_sprite.d3d9.h   --type fragment --varyingdef varying.def.sc --bin2c fs_sprite_d3d9   --platform windows -p ps_3_0  -O 3
..\..\..\tools\win\shaderc.exe -f fs_sprite.sc -o compiled/fs_sprite.d3d11.h  --type fragment --varyingdef varying.def.sc --bin2c fs_sprite_d3d11  --platform windows -p ps_4_0  -O 3
..\..\..\tools\win\shaderc.exe -f fs_sprite.sc -o compiled/fs_sprite.d3d12.h  --type fragment --varyingdef varying.def.sc --bin2c fs_sprite_d3d12  --platform windows -p ps_5_0  -O 3
..\..\..\tools\win\shaderc.exe -f fs_sprite.sc -o compiled/fs_sprite.vulkan.h --type fragment --varyingdef varying.def.sc --bin2c fs_sprite_vulkan --platform linux   -p spirv
..\..\..\tools\win\shaderc.exe -f fs_sprite.sc -o compiled/fs_sprite.metal.h  --type fragment --varyingdef varying.def.sc --bin2c fs_sprite_metal  --platform osx     -p metal
..\..\..\tools\win\shaderc.exe -f fs_sprite.sc -o compiled/fs_sprite.glsl.h   --type fragment --varyingdef varying.def.sc --bin2c fs_sprite_glsl   --platform windows -p 120
