#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float textureId;
layout(location = 4) in float tiling;

out vec2 v_TexCoord;
out vec4 v_TexColor;
out flat float v_TexId;
out float vTexTiling;

uniform mat4 u_MVP;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    gl_Position = u_Projection * u_View * vec4(position, 1.0f);
    v_TexCoord = texCoord;
    v_TexColor = color;
    v_TexId = textureId;
    vTexTiling = tiling;
}

#shader fragment
#version 460 core

in vec2 v_TexCoord;
in vec4 v_TexColor;
in flat float v_TexId;
in float vTexTiling;

layout(location = 0) out vec4 color;

uniform vec3 lightPos;
uniform sampler2D u_Textures[2];

void main()
{
    vec3 haha = lightPos;
    int index = int(v_TexId);
    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    color = texColor * v_TexColor;
}
