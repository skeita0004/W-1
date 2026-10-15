Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー


cbuffer MatrixBufferRef : register(b0)
{
    row_major float4x4 cb_matWVP;
}

cbuffer MaterialBufferRef : register(b1)
{
    float4 cb_diffuseColor;    
    int4 cb_isTexture;
}

struct VS_IN
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION; // 位置
    float2 uv : TEXCOORD0; // UV座標
};

PS_IN VS(const VS_IN _vsInput)
{
	//ピクセルシェーダーへ渡す情報
    PS_IN vsOutput;

    vsOutput.pos = _vsInput.pos;
    vsOutput.pos = mul(vsOutput.pos, cb_matWVP);
    
    vsOutput.uv = _vsInput.uv;
    
    return vsOutput;
}

float4 PS(PS_IN _psInput) : SV_Target
{
    // 拡散反射の計算
    float4 diffuse = float4(0, 0, 0, 1);
    if (cb_isTexture.x != 0)
    {
        // Texture color
        diffuse = g_texture.Sample(g_sampler, _psInput.uv);
    }
    else
    {
        // Default diffuse color
        diffuse = cb_diffuseColor;
    }
    diffuse.a = 1.0f;
    return diffuse;
}