// ShadowMap書き込み用シェーダ

cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 cb_matWVP; // ワールド・ビュー・プロジェクションの合成行列
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_IN
{
    float3 pos : POSITION;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
PS_IN VS(const VS_IN _vsInput)
{
	//ピクセルシェーダーへ渡す情報
    PS_IN vsOutput;

    float4 localPos = float4(_vsInput.pos.xyz, 1.0f);
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    vsOutput.pos = mul(localPos, cb_matWVP);
    
    vsOutput.depthPosition = vsOutput.pos;
    
    //vsOutput.pos = float4(_vsInput.pos, 1.0f);
    
    return vsOutput;
}

//─────────────────────────────────────── 
// ピクセルシェーダ
//───────────────────────────────────────

float4 PS(PS_IN _input) : SV_Target
{
    float  depthValue;
    float4 color;
    
    depthValue = _input.depthPosition.z / _input.depthPosition.w;
    
    color = float4(depthValue, depthValue, depthValue, 1.0f);
    
    return color;
}