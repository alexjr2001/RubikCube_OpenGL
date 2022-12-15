#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform mat4 nullMatrix;


void main()
{
    //FragColor = vec4(ourColor, 1.0f);
    
    //FragColor = texture(texture1, TexCoord)+vec4(ourColor,1.0);
    FragColor = (texture(texture1, TexCoord)* nullMatrix) + vec4(ourColor, 1.0);
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}