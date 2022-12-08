Texture2D tex : register(t0); //どのテクスチャを使うかを入れる
SamplerState smp : register(s0);

Texture2D texNormal : register(t1);

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
	float4 light : TEXCOORD2;

};

//頂点シェーダー
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)//:セマンティクスは何の情報か　
{
	VS_OUT outData;
	outData.pos = mul(pos, matWVP);   //ベクトルを行列にする関数
	outData.uv = uv;

	float3 binormal = cross(normal, tangent);
	

	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normalize(normal);

    tangent.w = 0;
	tangent = mul(tangent, matNormal);
	tangent = normalize(tangent);

	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	float4 eye = normalize(mul(pos, matW) - camPos);
	outData.V.x = dot(eye, tangent);
	outData.V.y = dot(eye, binormal);
	outData.V.z = dot(eye, normal);

	float4 light = float4(1, 1, -1, 0);//ライト
	light = normalize(light);
	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);

	return outData;
}

//ピクセルシェーダー オブジェクトに関係なしに色を決めれる
float4 PS(VS_OUT inData) : SV_TARGET //SVは二次元 ピクセルシェーダーの引数は頂点シェーダーの戻り値と同じ
{

	float4 diffuse;
	float4 ambient;
	float4 specular;

	float4 normal = texNormal.Sample(smp, inData.uv) * 2 - 1;
	normal.w = 0;
	normal = normalize(normal);
	

	float4 S = dot(normal, inData.light);//内積
	S = clamp(S, 0, 1);
	/*if (S.r < 0.3)
		S = 0.3;
	else
		S = 1;*/
	float2 uv;
	uv.x = S;
	uv.y = 0;
	//return texToon.Sample(smp, uv);

	float4 R = reflect(inData.light, normal);
	specular = pow(clamp(dot(R, inData.V), 0, 1), shiness) * specularcolor * 2;
	//specular = pow(clamp(dot(R, inData.V), 0, 1), 10) * 3;

	float alpha;

	if (isTexture)
	{
		diffuse = tex.Sample(smp, inData.uv) * S;
		ambient = tex.Sample(smp, inData.uv) * ambientcolor;
		alpha = tex.Sample(smp, inData.uv).a;
		//ambient = tex.Sample(smp, inData.uv) *0.2;
	}
	else
	{
		diffuse = diffusecolor * S;
		ambient = diffusecolor * ambientcolor;
		alpha = diffusecolor.a;
		//ambient = diffusecolor * 0.2;
	}

	float4 result = diffuse + ambient + specular;
	result.a = alpha;
	return result;
}