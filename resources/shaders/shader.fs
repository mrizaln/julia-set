#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
    //vec4 textureColor = texture(tex, TexCoords);
    //FragColor = vec4(textureColor.rgb * color, textureColor.a);
    FragColor = vec4(1.0);
}
