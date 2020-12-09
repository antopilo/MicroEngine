#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float tiling;
layout(location = 4) in float textureId;

out vec2 v_TexCoord;
out vec4 v_TexColor;
out float v_TexId;

uniform mat4 u_MVP;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    gl_Position = u_Projection * u_View * vec4(position);
    v_TexCoord = texCoord;
    v_TexColor = color;
    v_TexId = textureId;
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in vec4 v_TexColor;
in float v_TexId;

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TexId);

    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    color = mix(texColor, v_TexColor, 0.5);
}
