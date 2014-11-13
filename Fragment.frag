//shader version
#version 150 core

//mode of drawing
//if is true, then use Texture
//otherwise draw gradient
uniform int useTexture;

//texture object
uniform sampler2D textureSampler;

//retrieve this data form vertex shader
in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} VertexIn;

out vec4 fragColor;

//TODO: you should use VertexIn.normal value to evaluate Phong Lightning for this pixel
// 

void main()
{

    if (useTexture>0)
        //take color from texture using texture2D
        fragColor = vec4(texture(textureSampler,VertexIn.texcoord.xy).rgb,length(VertexIn.normal)*length(VertexIn.position));
    else
    {
        //use default color (brown)
        fragColor = vec4(0.5,0.2,0.1,1);
    }
    vec4 result = vec4(0.0);
    for (int li = 0; li < gl_MaxLights; ++li)
    {
        vec3 l;
        if (gl_LightSource[li].position.w != 0.0)
        {
            l = normalize(gl_LightSource[li].position.xyz - VertexIn.position);
        }
        else
        {
            l = normalize(gl_LightSource[li].position.xyz);
        }
        vec3 e = normalize(-VertexIn.position);
        vec3 r = normalize(-reflect(l, VertexIn.normal));

        vec4 Iamb = gl_FrontLightProduct[li].ambient;

        vec4 Idiff = gl_FrontLightProduct[li].diffuse * max(dot(VertexIn.normal, l), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);

        vec4 Ispec = gl_FrontLightProduct[li].specular
                * pow(max(dot(r, e), 0.0),
                      gl_FrontMaterial.shininess);
        Ispec = clamp(Ispec, 0.0, 1.0);

        result += Iamb + Idiff + Ispec;
    }

    //fragColor = gl_FrontLightModelProduct.sceneColor + result;
    fragColor += result;

}
