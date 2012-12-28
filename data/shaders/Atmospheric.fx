//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Atmospheric Effects
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// BlueSky
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Sky
//--------------------------------------------------------------//
string Atmospheric_Effects_BlueSky_Sky_Sky_Sphere : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 view_proj_matrix : ViewProjection;
float skyZBias
<
   string UIName = "skyZBias";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -100.00;
   float UIMax = 100.00;
> = float( -48.00 );
float4 view_position : ViewPosition;
float skyScale
<
   string UIName = "skyScale";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 20.00;
> = float( 20.00 );
float scale
<
   string UIName = "scale";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 0.06;
> = float( 0.03 );
struct VS_OUTPUT {
   float4 Pos: POSITION;
   float3 texCoord: TEXCOORD0;
};

VS_OUTPUT Atmospheric_Effects_BlueSky_Sky_Vertex_Shader_main(float4 Pos: POSITION){
   VS_OUTPUT Out;

   // Get the sky in place
   Pos.y += skyZBias;
   Out.Pos = mul(view_proj_matrix, float4(Pos.xyz * skyScale + view_position, 1));
   // Pass position to the fragment shader
   Out.texCoord = Pos.xyz * scale;

   return Out;
}







float4 cloudColor
<
   string UIName = "cloudColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 1.00, 1.00, 1.00 );
float4 skyColor
<
   string UIName = "skyColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.00, 0.50, 1.00, 1.00 );
float noiseScale
<
   string UIName = "noiseScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -5.00;
   float UIMax = 5.00;
> = float( 4.10 );
float noiseBias
<
   string UIName = "noiseBias";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -5.00;
   float UIMax = 5.00;
> = float( -1.00 );
float4 lightDir
<
   string UIName = "lightDir";
   string UIWidget = "Direction";
   bool UIVisible =  true;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.92, 0.28, 0.27, 0.00 );
float4 sunColor
<
   string UIName = "sunColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 0.82, 0.44, 1.00 );
float sunFallOff
<
   string UIName = "sunFallOff";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 25.00;
> = float( 3.50 );
float sunSharpness
<
   string UIName = "sunSharpness";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 2.00;
> = float( 0.42 );
float cloudSpeed
<
   string UIName = "cloudSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.00;
> = float( 0.02 );
float noiseSpeed
<
   string UIName = "noiseSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.10;
> = float( 0.01 );
float time_0_X : Time0_X;
texture Noise_Tex
<
   string ResourceName = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Textures\\NoiseVolume.dds";
>;
sampler Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
float4 Atmospheric_Effects_BlueSky_Sky_Pixel_Shader_main(float3 texCoord: TEXCOORD0) : COLOR {
   // Create a sun
   float3 l = lightDir - normalize(texCoord);
   float sun = saturate(sunFallOff * pow(dot(l, l), sunSharpness));
   float4 sky = lerp(sunColor, skyColor, sun);

   // Clouds are basically noise, we just need to scale and bias it.
   texCoord.xy += cloudSpeed * time_0_X;
   texCoord.z  += noiseSpeed * time_0_X;
   float noisy = tex3D(Noise, texCoord).r;

   float lrp = noiseScale * noisy + noiseBias;

   return lerp(cloudColor, sky, saturate(lrp));
}






//--------------------------------------------------------------//
// Terrain
//--------------------------------------------------------------//
string Atmospheric_Effects_BlueSky_Terrain_Terrain : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\Terrain.3ds";

float4x4 Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_view_proj_matrix : ViewProjection;
float terrainZBias
<
   string UIName = "terrainZBias";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -400.00;
   float UIMax = 400.00;
> = float( -200.00 );
struct Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_VS_OUTPUT {
   float4 Pos:      POSITION;
   float2 texCoord: TEXCOORD0;
   float3 normal:   TEXCOORD1;
};

Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_VS_OUTPUT Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_main(float4 Pos: POSITION, float2 texCoord: TEXCOORD0, float3 normal: NORMAL){
   Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_VS_OUTPUT Out;

   // Get terrain in place
   Pos.z += terrainZBias;

   Out.Pos = mul(Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_view_proj_matrix, Pos);
   Out.texCoord = texCoord;
   Out.normal = normal;

   return Out;
}





float4 Atmospheric_Effects_BlueSky_Terrain_Pixel_Shader_lightDir
<
   string UIName = "Atmospheric_Effects_BlueSky_Terrain_Pixel_Shader_lightDir";
   string UIWidget = "Direction";
   bool UIVisible =  true;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.92, 0.28, 0.27, 0.00 );
texture Terrain_Tex
<
   string ResourceName = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Textures\\Terrain.tga";
>;
sampler BaseMap = sampler_state
{
   Texture = (Terrain_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = ANISOTROPIC;
   MIPFILTER = LINEAR;
   MAXANISOTROPY = 16;
};
float4 Atmospheric_Effects_BlueSky_Terrain_Pixel_Shader_main(float2 texCoord: TEXCOORD0, float3 normal: TEXCOORD1) : COLOR {
   float4 base = tex2D(BaseMap, texCoord);

   // Simple lighting
   float diffuse = dot(normalize(normal), Atmospheric_Effects_BlueSky_Terrain_Pixel_Shader_lightDir);

   return (0.6 * diffuse + 0.8) * base;
}






//--------------------------------------------------------------//
// DarkSky
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Terrain
//--------------------------------------------------------------//
string Atmospheric_Effects_DarkSky_Terrain_Terrain : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\Terrain.3ds";

float4x4 Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_view_proj_matrix : ViewProjection;
float4 lightPos
<
   string UIName = "lightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -1200.00, -1200.00, -1200.00, -1200.00 );
   float4 UIMax = float4( 1200.00, 1200.00, 1200.00, 1200.00 );
   bool Normalize =  false;
> = float4( 1200.00, 0.00, 576.00, 1.00 );
float Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_terrainZBias
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_terrainZBias";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -400.00;
   float UIMax = 400.00;
> = float( -200.00 );
struct Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_VS_OUTPUT {
   float4 Pos:      POSITION;
   float2 texCoord: TEXCOORD0;
   float3 normal:   TEXCOORD1;
   float3 lightVec: TEXCOORD2;
   float3 pos:      TEXCOORD3;
};

Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_VS_OUTPUT Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_main(float4 Pos: POSITION, float2 texCoord: TEXCOORD0, float3 normal: NORMAL){
   Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_VS_OUTPUT Out;

   // Translate
   Pos.z += Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_terrainZBias;

   Out.Pos = mul(Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_view_proj_matrix, Pos);
   Out.texCoord = texCoord;
   // World-space lighting
   Out.normal = normal;
   Out.lightVec = lightPos - Pos.xyz;
   // Pass position to the fragment shader
   Out.pos = Pos.xyz;

   return Out;
}



float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyZBias
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyZBias";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -800.00;
   float UIMax = 800.00;
> = float( -720.00 );
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_scale
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_scale";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 0.01;
> = float( 0.00 );
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_cloudSpeed
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_cloudSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.20;
> = float( 0.10 );
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseSpeed
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.10;
> = float( 0.03 );
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_time_0_X : Time0_X;
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseScale
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -10.00;
   float UIMax = 10.00;
> = float( 6.20 );
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseBias
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseBias";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -10.00;
   float UIMax = 10.00;
> = float( -3.00 );
float4 Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_lightPos
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_lightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -1200.00, -1200.00, -1200.00, -1200.00 );
   float4 UIMax = float4( 1200.00, 1200.00, 1200.00, 1200.00 );
   bool Normalize =  false;
> = float4( 1200.00, 0.00, 576.00, 1.00 );
float Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyScale
<
   string UIName = "Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyScale";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 1200.00;
> = float( 996.00 );
sampler Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
sampler Terrain = sampler_state
{
   Texture = (Terrain_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
float4 Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_main(float2 texCoord: TEXCOORD0, float3 normal: TEXCOORD1, float3 lightVec: TEXCOORD2, float3 pos: TEXCOORD3) : COLOR {
   // Center of the sky sphere
   float3 center = float3(0, 0, Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyZBias);

   float3 v = Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_lightPos - pos;
   float t = dot(v, center - pos) / dot(v, v);
   // The point along the line to the moon perpendicular
   // to the center of the sky sphere
   float3 perp = pos + t * v;

   float l = (float)( perp - center );
   float x = sqrt(Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyScale * Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_skyScale - dot(l, l));

   // The point in sky the line from the moon down to
   // this pixel hits the sky sphere
   float3 skyPos = perp + x * normalize(v);

   // Create cloud like we'd do for the sky at that position
   skyPos *= Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_scale;   
   skyPos.xy += Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_cloudSpeed * Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_time_0_X;
   skyPos.z  += Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseSpeed * Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_time_0_X;
   float noisy = tex3D(Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_Noise, skyPos).r;
   float lrp = Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseScale * noisy + Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_noiseBias;

   // Define the shadow from the cloud
   float shadow = saturate(1 - lrp);

   // Apply some simple lighting
   float4 base = tex2D(Terrain, texCoord);
   float3 lVec = normalize(lightVec);
   float diffuse = dot(lVec, normal);

   return diffuse * base * shadow + 0.25 * base;
}


//--------------------------------------------------------------//
// Moon
//--------------------------------------------------------------//
string Atmospheric_Effects_DarkSky_Moon_Sky_Sphere : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_view_proj_matrix : ViewProjection;
float4 Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_lightPos
<
   string UIName = "Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_lightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -1200.00, -1200.00, -1200.00, -1200.00 );
   float4 UIMax = float4( 1200.00, 1200.00, 1200.00, 1200.00 );
   bool Normalize =  false;
> = float4( 1200.00, 0.00, 576.00, 1.00 );
struct Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
};

Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_VS_OUTPUT Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_main(float4 Pos: POSITION){
   Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_VS_OUTPUT Out;

   // Get the moon in place
   Pos.xyz += Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_lightPos;

   Out.Pos = mul(Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_view_proj_matrix, Pos);

   return Out;
}




float4 moonColor
<
   string UIName = "moonColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 0.96, 0.73, 1.00 );
float4 Atmospheric_Effects_DarkSky_Moon_Pixel_Shader_main() : COLOR {
   return moonColor;
}


//--------------------------------------------------------------//
// Sky
//--------------------------------------------------------------//
string Atmospheric_Effects_DarkSky_Sky_Sky_Sphere : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_view_proj_matrix : ViewProjection;
float Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_skyScale
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_skyScale";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 1200.00;
> = float( 996.00 );
float Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_skyZBias
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_skyZBias";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -800.00;
   float UIMax = 800.00;
> = float( -720.00 );
struct Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
   float3 texCoord: TEXCOORD0;
};

Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_VS_OUTPUT Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_main(float4 Pos: POSITION){
   Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_VS_OUTPUT Out;

   // Get the sky in place
   Pos.xyz = Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_skyScale * normalize(Pos.xyz);
   Pos.y += Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_skyZBias;

   Out.Pos = mul(Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_view_proj_matrix, Pos);
   // Pass the position to the fragment shader
   Out.texCoord = Pos.xyz;

   return Out;
}







float Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseSpeed
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.10;
> = float( 0.03 );
float Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_time_0_X : Time0_X;
float Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseScale
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -10.00;
   float UIMax = 10.00;
> = float( 6.20 );
float Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseBias
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseBias";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -10.00;
   float UIMax = 10.00;
> = float( -3.00 );
float4 Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_cloudColor
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_cloudColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.20, 0.40, 1.00, 1.00 );
float Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_scale
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_scale";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 0.01;
> = float( 0.00 );
float Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_cloudSpeed
<
   string UIName = "Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_cloudSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.20;
> = float( 0.10 );
sampler Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
float4 Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_main(float3 texCoord: TEXCOORD0) : COLOR {
   // Create a cloud effect. Clouds are basically noise,
   // we just need to Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_scale and bias it.
   texCoord *= Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_scale;
   texCoord.xy += Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_cloudSpeed * Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_time_0_X;
   texCoord.z  += Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseSpeed * Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_time_0_X;
   float noisy = tex3D(Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_Noise, texCoord).r;

   float lrp = Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseScale * noisy + Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_noiseBias;

   return float4(Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_cloudColor.xyz, lrp);
}






//--------------------------------------------------------------//
// CloudsEffect_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Atmospheric_Effects_CloudsEffect_ASM_Single_Pass_Screen_Aligned_Quad : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Atmospheric_Effects_CloudsEffect_ASM_Single_Pass_Vertex_Shader_ = asm
{
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   
   mov  r0, v0        // align the quad with the screen
   mov  r0.w, c0.z
   mov  r0.z, c0.x
   mov  oPos, r0
   
   mov  r4, v2      // output base tex coord
   mov  oT4, r4  
   
   mov  r0, c1.xxxx  // (time*cloud0vel)
   mul  r1, c9.xyxy, r0
   
   mul  r2, c9.zwzw, r0  // (time*cloud0disvel) 
   
   mad  oT0, r4.yx, c10.xyxy, r1.xy
   mad  oT1, r4.yx, c10.zwzw, r2.xy
   
   mul  r1, c11.xyxy, r0  // (time*cloud1vel)
   
   mul  r2, c11.zwzw, r0  // (time*cloud1disvel)
   
   mad  oT2, r4.yx, c12.xyxy, r1.xy
   mad  oT3, r4.yx, c12.zwzw, r2.xy
   
   
   
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Atmospheric_Effects_CloudsEffect_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   texld  r0, t1
   texld  r1, t3
   texcrd r2.rgb, t0
   texcrd r3.rgb, t2
   
      mad r0.rgb, c1, r0_bx2, r2 ; perturb base map coords by cloud coords
      mad r1.rgb, c3, r1_bx2, r3 ; perturb base map coords by cloud coords
   
   phase
   
   texld r0, r0
   texld r1, r1
   texld r2, t4
   
      mov_x2 r3.rgb, r2.a              ; alpha of horizon (glow map)
      +mul_sat r3.a, 1-r0.a, 1-r1.a    ; mask edges for cloud glow
    
      mad_sat r0.rgb, 1-r0.a, c2, c2.a ; scale and bias inverted clouds layer 0
      +mov_x2_sat r0.a, r0.a           ; boost cloud alpha 0
   
      mad_sat r1.rgb, 1-r1.a, c4, c4.a ; scale and bias inverted clouds layer 1
      +mov_x2_sat r1.a, r1.a           ; boost cloud alpha 1
   
      lrp r0.rgb, r1.a, r1, r0         ; combine cloud layers
      +mov r4.a, c5.a                  ; ...
   
      mul_x2 r3, r3.a, r3              ; multiply glow by edges of clouds
      mad_x2 r3, r3, c5, r4.a          ; scale and bias glowing sections of clouds
   
      mul r0.rgb, r3, r0               ; multiply glow by combination of cloud layers
      +add_d2_sat r0.a, r0.a, r1.a     ; add alphas of cloud layers
   
      lrp_sat r0, r0.a, r0, r2         ; lerp between clouds and horizon
   
   

};

//--------------------------------------------------------------//
// AtmosphericScatteringEffect_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Terrain pass
//--------------------------------------------------------------//
string Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Terrain_pass_Terrain : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\Terrain.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Terrain_pass_Vertex_Shader_ = asm
{
    
   ; Scattering Vertex shader
   ; (c) Preetham ATI Research, 2002.
   ; V View direction
   ; L Sun direction
   ; Theta Scattering angle
   ; s Distance
   ; E Total extinction (including reflectance).
   ;
   vs.2.0
   
   dcl_position   v0
   dcl_texcoord   v2
   
   
   ; Transformation.
   m4x4 oPos, v0, c0  ; Transform position to viewport
   
   ; Texture Coordinates.
   ; 0 Normal/Horizon map
   ; 1 Terrain texture
   ; 2 Cloud texture
   mov oT0.xy, v2
   mov oT1.xy, v2
   
   mov r1, c32
   mad oT2.xy, r1, c33, v2
   
   
   mov  r0, c22               ; get eye position in eye space (origin)
   m4x4 r1, r0, c8            ; eye position in model space
   sub  r1, v0, r1            ; V = vertex eye
   dp3  r1.w, r1, r1          ; normalize V
   rsq  r1.w, r1.w            ;
   mul  r1, r1, r1.w          ;
   
   ; calculate sun's position in sky
   mov r8.x,  c19.x
   mul r8.x,  r8.x, c32.x
   frc r8.xy, r8.x
   sub r8.x, r8.x, c31.z
   mul r8.x, r8.x, c31.w
   
   add r8.x, r8.x, c17.x
   max r8.x, r8.x, -c31.x
   min r8.x, r8.x, c31.x
   
   ; calculate sun direction
   ; r2 = [cos(theta), sin(theta)] of sun's rotation from east to west
   mov r2.x, r8.x
   max r2.y, r2.x, -r2.x
   sub r2.y, c31.x, r2.y
   
   ; rotate sun from east to west depending on east_to_west value [-1,1]
   ; y' = y
   mov r5, c18
   
   ; x' = z sin(theta)
   mul r5.x, c18.z, r2.y
   
   ; x' = (z sin(theta)) + (x cos(theta))
   mad r5.x, c18.x, r2.x, r5.x
   
   ; z' = (x sin(theta))
   mul r5.z, c18.x, r2.y
   
   ; z' = (z cos(theta)) (x sin(theta))
   mad r5.z, c18.z, r2.x, -r5.z
   
   ; normalize sun direction vector
   dp3 r5.w, r5, r5
   rsq r5.w, r5.w
   mul r5.xyz, r5.xyz, -r5.w
   
   ; scale and bias to [0,1]
   add r7, r5, c31.xxx
   mul oT5, r7, c31.zzz
   
   ; Angle (theta) between sun direction (L) and view direction (V).
   dp3 r0.x, r1, r5           ; r0.x = [cos(theta)] = V.L
   mad r0.y, r0.x, r0.x, c31.x ; r0.y = [1+cos^2(theta)] = Phase1(theta)
   
   ; Distance (s)
   m4x4 r1, v0, c4           ; r1.z = s
   mov r0.z, r1.z            ; store in r0.z for future use.
   
   ; Terms used in the scattering equation.
   ; r0 = [cos(theta), 1+cos^2(theta), s] 
   
   ; at noon we want no shadows
   dp3 r10.x, r5, c14
   add r10.x, r10.x, c31.x
   mul r10.x, r10.x, c31.z     ; sun angle for shadowing
   sub oT3, c31.x, r10.x
   
   
   ; Approximate theta r10.x = theta / (pi/2)
   mov r10, r5
   dp3 r10.x, r10, c14
   max r10.x, r10.x, -r10.x     ; get abs of cos(theta) 
   //add r10.x, r10.x, c31.x    ; scale and bias cos(theta) to [0,1] range
   //mul r10.x, r10.x, c31.z    ; ...
   mov oT4.xy, r10.x    ; ; 
   
   ; Extinction term E
   mov r1, c26.xxx
   add r1, r1, c26.yyy         ;  beta_1 + beta_2
   
   mul r1, r1, -r0.z           ; -(beta_1+beta_2) * s
   mul r1, r1, c31.y           ; -(beta_1+beta_2) * s * log_2 e
   exp r1.x, r1.x          ; r1 = e^(-(beta_1 + beta_2) * s) = E1
   exp r1.y, r1.y          
   exp r1.z, r1.z          
   
   
   
   ; Apply Reflectance to E to get total net effective E
   mul r3, r1, c13     ;r3 = E (Total extinction) 
   
   
   ; Phase2(theta) = (1-g^2)/(1+g-2g*cos(theta))^(3/2)
   ; theta is 180 actual theta (this corrects for sign)
   ; c[28] = [(1-g)^2, 1+g^2, 2g]
   mov r8.xyz, c28.xxx
   sub r8.x, c31.x, r8.x
   mul r8.xy, r8.xy, r8.xy ; r8.xy = [(1-g)^2, g^2]
   add r8.y, r8.y, c31.x   ; r8.y = 1+g^2
   mul r8.z, r8.z, c31.w  // 2g
   
   mad r4.x, r8.z, r0.x, r8.y; 
   
   
   rsq r4.x, r4.x                
   mul r4.y, r4.x, r4.x       
   mul r4.x, r4.y, r4.x;
   mul r0.w, r4.x, r8.x              ; r0.w = Phase2(theta)
   
   
   ; Inscattering (I) = (Beta'_1 * Phase_1(theta) + Beta'_2 * Phase_2(theta)) * 
   ;        [1-exp(-Beta_1*s).exp(-Beta_2*s)] / (Beta_1 + Beta_2)
   
   mul r4, c26.xxx, r0.y 
   mul r5, c26.yyy, r0.w  
   sub r6, c31.x, r1
   
   mov r1, c26.xxx
   add r1, r1, c26.yyy          ;  beta_1 + beta_2
   
   mov r7, r1
   
   add r4, r4, r5
   mul r4, r4, r6
   
   rcp r1.x, r1.x
   mul r4, r4, r1.xxx ; r4 = I (inscattering)
   
   
   ; Apply Inscattering contribution factors.
   
   mul r4, r4, c25.y
   
   
   ; Scale with Sun color & intesity.
   mul r4, r4, c15    
   mul r4, r4, c15.w  
   
   mul r3, r3, c15    
   mul r3, r3, c15.w  
   
   ; Outputs.
   mov oD0, r3                             ; Extinction
   mov oD1, r4                             ; Inscattering
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
     
   
   
   
   
   
   
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Terrain_pass_Pixel_Shader_ = asm
{
    
   ; Terrain Pixel shader with Scattering
   ; (c) Kenny Mitchell Westwood Studios EA 2002
   ; (c) Preetham ATI Research, 2002. 
   
   ps.2.0
   ; Equation is 
   ;
   ; Surf_color = (N.L) * terrain_tex * cloud_covering * self_shadow_term;
   ; Final_color =  Surf_color * extinction + inscattering 
   ;
   ; 
   ; Inputs:
   ; VP_EXTINCTION (v0)
   ; VP_INSCATTERING (v1)
   ;
   
   def c7, 0.2f, 0.1f, 0.1f, 1.0f
   def c8, -1.0f, 2.0f, 0.0f, 0.0f
   
   dcl v0.xyzw
   dcl v1.xyzw
   
   dcl t0.xy  
   dcl t1.xy  
   dcl t2.xy  
   dcl t3.xyz  
   dcl t4.xy  
   dcl t5.xyz  
   
   dcl_2d     s0
   dcl_2d     s1
   dcl_2d     s2
   dcl_2d     s3
   
   texld  r0, t0, s0    ; Normal Horizon map
   texld  r1, t1, s1    ; Terrain color
   texld  r2, t2, s2    ; Cloud layer
   texld  r3, t4, s3    ; Sun attenuation map
   mov r4.rgb, t3  ; Horizon angle for shadows
   mov r5.rgb, t5  ; L
   
   mov r4.a, r4.r
   
   mad r5.rgb, r5, c8.y, c8.x
   mad r6.rgb, r0, c8.y, c8.x
   dp3_sat  r0.rgb, r5, r6        ; Lighting (N.L)
   
   mul r5, v0, r3
   mul r3, v1, r3
   
   mul r0.rgb, r0, r1             ; Terrain texture.
   sub_sat r0.a, r4.a, r0.a       ; Shadow term (from Horizon angle)
   
   mad r0, -r0, r0.a, r0          ; Add Shadow.
   mad r0, -r0, r2, r0            ; Add cloud covering
   
   mad_sat r0, r0, r5, r3         ; Final color
   
   
   mov oC0, r0
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
     
   
   
   
   
   
   
   
   

};

//--------------------------------------------------------------//
// Sky Dome
//--------------------------------------------------------------//
string Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Sky_Dome_Sky_Dome : ModelData = "C:\\Program Files\\ATI Research Inc\\RenderMonkey 1.62\\Examples\\Media\\Models\\SkyDome.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Sky_Dome_Vertex_Shader_ = asm
{
    
   ; Scattering Vertex shader
   ; (c) Preetham ATI Research, 2002.
   ; V View direction
   ; L Sun direction
   ; Theta Scattering angle
   ; s Distance
   ; E Total extinction (including reflectance).
   ;
   vs.2.0
   
   dcl_position   v0
   //dcl_normal     v1
   dcl_texcoord   v2
   //dcl_tangent    v3
   //dcl_binormal   v4
   
   
   ; Transformation.
   m4x4 oPos, v0, c0  ; Transform position to viewport
   
   ; Texture Coordinates.
   ; 0 Normal/Horizon map
   ; 1 Terrain texture
   ; 2 Cloud texture
   mov oT0.xy, v2
   
   mov  r0, c22               ; get eye position in eye space (origin)
   m4x4 r1, r0, c8            ; eye position in model space
   sub  r1, r1, v0            ; V = vertex eye
   dp3  r1.w, r1, r1          ; normalize V
   rsq  r1.w, r1.w            ;
   mul  r1, r1, r1.w          ;
   
   ; calculate sun's position in sky
   mov r8.x,  c19.x
   mul r8.x,  r8.x, c32.x
   frc r8.xy, r8.x
   sub r8.x, r8.x, c31.z
   mul r8.x, r8.x, c31.w
   
   add r8.x, r8.x, c17.x
   max r8.x, r8.x, -c31.x
   min r8.x, r8.x, c31.x
   
   ; calculate sun direction
   ; r2 = [cos(theta), sin(theta)] of sun's rotation from east to west
   mov r2.x, r8.x
   max r2.y, r2.x, -r2.x
   sub r2.y, c25.z, r2.y
   
   ; rotate sun from east to west depending on east_to_west value [-1,1]
   ; y' = y
   mov r5, c18
   
   ; x' = z sin(theta)
   mul r5.x, c18.z, r2.y
   
   ; x' = (z sin(theta)) + (x cos(theta))
   mad r5.x, c18.x, r2.x, r5.x
   
   ; z' = (x sin(theta))
   mul r5.z, c18.x, r2.y
   
   ; z' = (z cos(theta)) (x sin(theta))
   mad r5.z, c18.z, r2.x, -r5.z
   
   ; normalize sun direction vector
   dp3 r5.w, r5, r5
   rsq r5.w, r5.w
   mul r5.xyz, r5.xyz, r5.w
   mov oT5, r5
   
   ; Angle (theta) between sun direction (L) and view direction (V).
   dp3 r0.x, r1, r5           ; r0.x = [cos(theta)] = V.L
   //sub r0.x, c31.x, r0.x  ;; <-?
   
   mul r0.x, r0.x, c31.z      ; scale & bias into [0,1] range
   add r0.x, r0.x, c31.z
   mov r0.y, c26.x            ; beta_r
   //sub r0.y, c31.x, r0.y      ; complement since d3d's texture origin at top left
   mov oT1, r0.xy
   
   mov r0.y, c26.y            ; beta_m
   //sub r0.y, c31.x, r0.y      ; complement since d3d's texture origin at top left
   mov oT2, r0.xy
   
   mov oD0, r0.xxx
   
   
   dp3 r0.x, c14, r5
   max r0.x, r0.x, -r0.x
   mov oT5, r0.xxx
   
   
   m4x4 r2, v0, c4
   add r0.y, r0.y, c26.x
   mov r0.x, r2.z
   mul r0.x, r0.x, c27.x
   
   mov oT3, r0.xy
   
   
   
   
   
   
     
   
   
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Sky_Dome_Pixel_Shader_ = asm
{
    
   
   
   ; Sky Pixel Shader
   
   ps.2.0
   
   
   dcl t1.xy  
   dcl t2.xy
   //dcl t3.xy  
   dcl t5.xy  
   
   dcl_2d     s0
   dcl_2d     s1
   dcl_2d     s2
   //dcl_2d     s3
   
   texld  r0,    t5, s0   ; sun attenuation
   texld  r1,    t1, s1   ; Br(theta, beta_r)
   texld  r2,    t2, s2   ; Bm(theta)
   //texld  r3,    t3, s3   ; Fex(s)
   //mov r4.xyz,t2          ; z = 1/(betam + betar)
   
   
   add r1.x, r1.x, r2.x      ; Br(theta) + Bm(theta)
   //mul r3, r3.xxx, c4        ; Extintction * Soil Reflectance = Etotal
   //sub r2, c1.xxx, r3        ; 1 Etotal
   //mul r1, r1.xxx, r2        ; Lin(theta) = (Br(theta) + Bm(theta)) * (1-Etotal)
   mul r0, r0, r1.x            ; Lin(theta) * Sun Attenuation
   
   
   mov oC0, r0
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
     
   
   
   
   
   

};

//--------------------------------------------------------------//
// Technique Section for Atmospheric Effects
//--------------------------------------------------------------//
technique BlueSky
{
   pass Sky
   {
      ZWRITEENABLE = FALSE;
      CULLMODE = CW;

      VertexShader = compile vs_1_1 Atmospheric_Effects_BlueSky_Sky_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Atmospheric_Effects_BlueSky_Sky_Pixel_Shader_main();
   }

   pass Terrain
   {
      ZWRITEENABLE = TRUE;
      CULLMODE = CCW;

      VertexShader = compile vs_1_1 Atmospheric_Effects_BlueSky_Terrain_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Atmospheric_Effects_BlueSky_Terrain_Pixel_Shader_main();
   }

}

technique DarkSky
{
   pass Terrain
   {
      CULLMODE = CCW;
      ALPHABLENDENABLE = FALSE;

      VertexShader = compile vs_1_1 Atmospheric_Effects_DarkSky_Terrain_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Atmospheric_Effects_DarkSky_Terrain_Pixel_Shader_main();
   }

   pass Moon
   {
      VertexShader = compile vs_1_1 Atmospheric_Effects_DarkSky_Moon_Vertex_Shader_main();
      PixelShader = compile ps_1_1 Atmospheric_Effects_DarkSky_Moon_Pixel_Shader_main();
   }

   pass Sky
   {
      ZWRITEENABLE = TRUE;
      SRCBLEND = SRCALPHA;
      DESTBLEND = INVSRCALPHA;
      CULLMODE = CW;
      ALPHABLENDENABLE = TRUE;

      VertexShader = compile vs_1_1 Atmospheric_Effects_DarkSky_Sky_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Atmospheric_Effects_DarkSky_Sky_Pixel_Shader_main();
   }

}

technique CloudsEffect_ASM
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;
      ZFUNC = ALWAYS;

      VertexShader = (Atmospheric_Effects_CloudsEffect_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (Atmospheric_Effects_CloudsEffect_ASM_Single_Pass_Pixel_Shader_);
   }

}

technique AtmosphericScatteringEffect_ASM
{
   pass Terrain_pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      ZWRITEENABLE = TRUE;
      CULLMODE = NONE;
      ZFUNC = LESS;
      ALPHABLENDENABLE = FALSE;

      VertexShader = (Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Terrain_pass_Vertex_Shader_);
      PixelShader = (Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Terrain_pass_Pixel_Shader_);
   }

   pass Sky_Dome
   {
      FILLMODE = SOLID;
      ZWRITEENABLE = FALSE;
      CULLMODE = CW;
      ZFUNC = LESS;

      VertexShader = (Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Sky_Dome_Vertex_Shader_);
      PixelShader = (Atmospheric_Effects_AtmosphericScatteringEffect_ASM_Sky_Dome_Pixel_Shader_);
   }

}

