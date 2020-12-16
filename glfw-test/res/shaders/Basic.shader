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
out float v_Visibility;

uniform mat4 u_MVP;
uniform mat4 u_Projection;
uniform mat4 u_View;

const float density = 0.003;
const float gradient = 5.0;

void main()
{
    gl_Position = u_Projection * u_View * vec4(position, 1.0f);
    v_TexCoord = texCoord;
    v_TexColor = color;
    v_TexId = textureId;
    vTexTiling = tiling;
    
    
    vec4 positionRelativeToCam = u_View * vec4(vec3(position.x, 0 , position.z), 1.0f);

    float distance = length(vec3(positionRelativeToCam.x, position.y, positionRelativeToCam.z));
    v_Visibility = exp(-pow((distance * density), gradient));
    v_Visibility = clamp(v_Visibility, 0.0, 1.0);
}

#shader fragment
#version 460 core

in vec2 v_TexCoord;
in vec4 v_TexColor;
in flat float v_TexId;
in float vTexTiling;
in float v_Visibility;
layout(location = 0) out vec4 color;

uniform vec3 lightPos;
uniform sampler2D u_Textures[2];

void main()
{
    vec3 haha = lightPos;
    int index = int(v_TexId);
    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    color = texColor * v_TexColor;
    color = mix(vec4(52.0 / 255.0f, 219.0f / 255.0f, 235.0f / 255.0f, 1.0f), color, v_Visibility);
}
