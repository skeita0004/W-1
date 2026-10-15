cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 cb_matWVP; // ワールド・ビュー・プロジェクションの合成行列
    row_major float4x4 cb_matNormal;
    row_major float4x4 cb_matW; // ワールド行列
    row_major float4x4 cb_matVP;
    row_major float4x4 cb_matInvW;
}

cbuffer MaterialBuffer : register(b1)
{
    float4 cb_diffuseColor; // ディフューズカラー（マテリアルの色）
    float4 cb_ambientColor;
    float4 cb_specularColor;
    float4 cb_emissiveColor;
    
    float cb_diffuseFactor;
    float cb_shininess;
    
    int cb_isTexture; // テクスチャ貼ってあるかどうか
    int cb_isNormalMap;
    
    int4 padding;
}

cbuffer LightBuffer : register(b2)
{
    float4 cb_lightPos;
    float4 cb_lightDir;
    
    float cb_lightAttenuation;
    float cb_conePhi;
    
    int2 lbPadding;
}

cbuffer CameraBuffer : register(b3)
{
    float4 cb_cameraPos;
}

// 関数
float4x4 InvTangentMat(
    float3 tangent,
    float3 binormal,
    float3 normal)
{
    float4x4 mat =
    {
        float4(tangent, 0.0f),
        float4(binormal, 0.0f),
        float4(normal, 0.0f),
        float4(0, 0, 0, 1)
    };

    return transpose(mat);
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_IN
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 biNormal : BINORMAL;
};

struct PS_IN
{
    float4 pos : SV_POSITION; // 位置
    float4 worldPos : POSITION0;
    float2 uv : TEXCOORD0; // UV座標
    float3 normal : NORMAL0; // 法線
    float3 tangent : NORMAL1;
    float3 biNormal : NORMAL2;
    float3 lightDir : POSITION1;
    float4 lightVP : TEXCOORD1;
    float3 lightTanDir : POSITION2;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
PS_IN VS(const VS_IN _vsInput)
{
	//ピクセルシェーダーへ渡す情報
    PS_IN vsOutput;
    
    float3 outLinePos = _vsInput.pos.xyz + _vsInput.normal * 0.0125;

    vsOutput.pos = float4(outLinePos, 1.0f);
    
    vsOutput.pos = mul(vsOutput.pos, cb_matWVP);
    
    return vsOutput;
}

//─────────────────────────────────────── 
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(PS_IN _psInput) : SV_Target
{
    float4 outPutColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    return outPutColor;
}