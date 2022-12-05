Texture2D tex : register(t0); //どのテクスチャを使うかを入れる
SamplerState smp : register(s0);

Texture2D texToon : register(t1); //どのテクスチャを使うかを入れる


cbuffer gloabl
{
	float4x4 matWVP;   //行列を扱う型
	float4x4 matNormal;
	float4x4 matW;
	float4   diffusecolor;    //マテリアルの色
	float4   ambientcolor;
	float4   specularcolor;
	float4   camPos;
	float   shiness;
	bool     isTexture;//テクスチャが貼られているかどうか
	
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 normal : NORMAL;
	float4 V : TEXCOORD1;
	
};

//頂点シェーダー
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)//:セマンティクスは何の情報か　
{
	VS_OUT outData;
	outData.pos = mul(pos,matWVP);   //ベクトルを行列にする関数
	outData.uv = uv;

	normal.w = 0;
	
	outData.normal = mul(normal, matNormal);
	outData.normal = normalize(outData.normal);
	
	outData.V = normalize(mul(pos, matW) - camPos);

	

	return outData;
}

//ピクセルシェーダー オブジェクトに関係なしに色を決めれる
float4 PS(VS_OUT inData) : SV_TARGET //SVは二次元 ピクセルシェーダーの引数は頂点シェーダーの戻り値と同じ
{

	float4 diffuse;
    float4 ambient;
	float4 specular;

	float4 light = float4(1, 1, -1, 0);//ライト
	light = normalize(light);

	float4 S = dot(inData.normal, light);//内積
	S = clamp(S, 0, 1);
	/*if (S.r < 0.3)
		S = 0.3;
	else
		S = 1;*/
	float2 uv;
	uv.x = S;
	uv.y = 0;
	//return texToon.Sample(smp, uv);
	
	float4 R = reflect(light, inData.normal);
	specular = pow(clamp(dot(R, inData.V), 0, 1), shiness) * specularcolor * 2;
	//specular = pow(clamp(dot(R, inData.V), 0, 1), 10) * 3;
	if (isTexture)
	{
		diffuse = tex.Sample(smp, inData.uv)*S;
		ambient = tex.Sample(smp, inData.uv) * ambientcolor;
		//ambient = tex.Sample(smp, inData.uv) *0.2;
	}
    else
    {
		diffuse = diffusecolor* S;
		ambient = diffusecolor * ambientcolor;
		//ambient = diffusecolor * 0.2;
    }
	
	return diffuse /*+ ambient*/ + specular;
}