/// 平面反射描画用シェーダ


//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

// シャドウマップ用テクスチャ
Texture2D shadowTex : register(t1);
SamplerComparisonState shadowTexSampler : register(s1);

// バンプ用テクスチャ
Texture2D normalTex : register(t2);
SamplerState normalTexSampler : register(s2);

// リフレクションマップ用テクスチャ
Texture2D reflectionTex : register(t3);
SamplerState reflectionTexSampler : register(s3);

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 cb_matWVP; // ワールド・ビュー・プロジェクションの合成行列
    row_major float4x4 cb_matNormal;
    row_major float4x4 cb_matW; // ワールド行列
    row_major float4x4 cb_matP;
    row_major float4x4 cb_matVP;
    row_major float4x4 cb_matInvW;
    row_major float4x4 cb_matRefView;
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

float CalcShadowFactor(float4 lightViewProj)
{
    float3 shadowCoord = lightViewProj.xyz / lightViewProj.w;
    
    if (shadowCoord.z > 1.0f || shadowCoord.z < 0.0f)
    {
        return 1.0f;
    }
    
    float2 texCoord;
    texCoord.x = (shadowCoord.x * 0.5f + 0.5f);
    texCoord.y = 1 - (shadowCoord.y * 0.5f + 0.5f);
    
    return shadowTex.SampleCmpLevelZero(shadowTexSampler, texCoord, shadowCoord.z - 0.0025f);
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
    float4 reflectPos : TEXCOORD2;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
PS_IN VS(const VS_IN _vsInput)
{
	//ピクセルシェーダーへ渡す情報
    PS_IN vsOutput;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    vsOutput.pos = _vsInput.pos;
    vsOutput.pos = mul(vsOutput.pos, cb_matWVP);
    
    // ワールド変換のみをかけた頂点座標
    vsOutput.worldPos = _vsInput.pos;
    vsOutput.worldPos = mul(vsOutput.worldPos, cb_matW);
    
    // UV座標は変換なし
    vsOutput.uv = _vsInput.uv;
    
    // 法線の変換（正規化はPSで行う）
    vsOutput.normal = mul(_vsInput.normal, (float3x3) cb_matNormal);
    vsOutput.tangent = mul(_vsInput.tangent, (float3x3) cb_matNormal);
    vsOutput.biNormal = mul(_vsInput.biNormal, (float3x3) cb_matNormal);
    
    vsOutput.lightVP = mul(vsOutput.worldPos, cb_matVP);
    
    float3 localLightPos = mul(cb_lightPos, cb_matInvW);
    
    vsOutput.lightTanDir = mul(-localLightPos, (float3x3) InvTangentMat(_vsInput.tangent, _vsInput.biNormal, _vsInput.normal));
    
    float4x4 refWVP;
    refWVP = mul(cb_matRefView, cb_matP);
    refWVP = mul(cb_matW, refWVP);
    vsOutput.reflectPos = mul(_vsInput.pos, refWVP);
    
    return vsOutput;
}

//─────────────────────────────────────── 
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(PS_IN _psInput) : SV_Target
{
    
    float lightDist;
    float atteneuationColor;
    
    // 影
    float shadowFactor = CalcShadowFactor(_psInput.lightVP);
    
    _psInput.normal = normalize(_psInput.normal);
    // ノーマルマップ
    if (cb_isNormalMap != 0)
    {
        float3x3 matTBN =
        {
            normalize(_psInput.tangent),
            normalize(_psInput.biNormal),
            normalize(_psInput.normal)
        };
        
        // ノーマルマップから読んだ値
        float4 normalColor = normalTex.Sample(normalTexSampler, _psInput.uv);
        float3 normalMapVec = 2 * normalColor - 1.0f;
        normalMapVec = normalize(normalMapVec);
        normalMapVec = normalize(mul(normalMapVec, matTBN));
        _psInput.normal = normalMapVec;
    }
    
    // ライトの方向と距離を求める
    _psInput.lightDir = cb_lightPos.xyz - _psInput.worldPos.xyz;
    lightDist = length(_psInput.lightDir);
    _psInput.lightDir = normalize(_psInput.lightDir);
    
    atteneuationColor = saturate(pow(1.0f - lightDist / 4.0, 2.0));
    
    
    // ライトの強さ = 陰影の度合い
    float lightIntensity = saturate(dot(normalize(_psInput.normal), _psInput.lightDir));

    // スポットライトサイズ
    float coneSize = cb_conePhi;
    
    float3 lightDir = cb_lightDir.xyz;
    lightDir.y *= -1;
    
    // スポットライト計算
    float spot = smoothstep(coneSize - 0.02f, coneSize, pow(max(dot(_psInput.lightDir, lightDir), 0.0f), coneSize));
    
    atteneuationColor = spot / dot(float3(1.0f, 0.22f, 0.2f), float3(1.0f, lightDist, lightDist * lightDist));
    
    // 拡散反射の計算
    float4 diffuse = float4(0, 0, 0, 0);
    if (cb_isTexture != 0)
    {
        // Texture color
        diffuse = g_texture.Sample(g_sampler, _psInput.uv);
    }
    else
    {
        // Default diffuse color
        diffuse = cb_diffuseColor;
    }
    float4 diffuseLight = saturate(lightIntensity * diffuse * atteneuationColor);
    
    // 鏡面反射の計算
    float ndotl = saturate(dot(_psInput.normal, _psInput.lightDir));
    float specularFactor = 0;
    if (cb_specularColor.a > 0 && ndotl > 0.0f)
    {
        float3 eyeVector = normalize(cb_cameraPos.xyz - _psInput.worldPos.xyz);
        
        float3 reflection = reflect(-_psInput.lightDir, normalize(_psInput.normal));

        specularFactor = pow(saturate(dot(reflection, eyeVector)), cb_shininess);
    }
    
    float4 ambientLight = (cb_ambientColor * diffuse);
    
    float ambientFactor = 0.5f;
    ambientLight = float4(ambientFactor, ambientFactor, ambientFactor, 1.0f) * diffuse;
    float4 specularLight = saturate(specularFactor * cb_specularColor * atteneuationColor);
    
    float4 outPutColor = ambientLight + ((diffuseLight + specularLight + cb_emissiveColor) * shadowFactor);
    
    float2 refCoord;
    refCoord.x = _psInput.reflectPos.x / _psInput.reflectPos.w * 0.5f + 0.5f;
    refCoord.y = -_psInput.reflectPos.y / _psInput.reflectPos.w * 0.5f + 0.5f;

    float4 refColor = reflectionTex.Sample(reflectionTexSampler, refCoord);
    
    outPutColor = lerp(outPutColor, refColor, 0.2f);
    //outPutColor.a = 1.0;
    
    return outPutColor;
}