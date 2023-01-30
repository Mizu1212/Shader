Texture2D		tex : register(t0);	//テクスチャー
Texture2D		texToon : register(t1);	//テクスチャー
SamplerState	smp : register(s0);	//サンプラー

cbuffer gloabal	//FBX.hのCBと同じ順番
{
	float4x4 matWVP;
	float4x4 matNormal;
	float4x4 matW;
	float4	 diffusecolor;
	float4   ambientcolor;
	float4	specularcolor;
	float4	 camPos;
	float	shiness;
	float		scroll;
	float       movepos;
	bool		isTexture;
	
};

struct VS_OUT
{
	float4 pos	 : SV_POSITION;
	float2 uv	 : TEXCOORD;
	float4 eye	 : TEXCOORD1;
	float4 normal: NORMAL;
	float4 postrans   :TEXCOORD3;
};


VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	VS_OUT outData;
	outData.pos = mul(pos, matWVP);
    outData.uv = uv;

	normal.w = 0;
	
	outData.normal = mul(normal, matNormal);
	outData.normal = normalize(outData.normal);

	outData.eye = normalize(mul(pos, matW) - camPos);
	outData.postrans = pos;
	return outData;
}



float4 PS(VS_OUT inData) : SV_Target
{
	float4 diffuse;
	float4 ambient;
	float4 specular;

	float4 light = float4(1, 1, -1, 0);//ライト
	light = normalize(light);

	float4 normal = normalize(inData.normal);

	float2 uv = float2(saturate(dot(normal, light)),0);

	float4 S = dot(inData.normal, light);//内積
	S = clamp(S, 0, 1);
	
	float4 R = reflect(light, inData.normal);
	specular = pow(clamp(dot(R, inData.eye), 0, 1), shiness) * specularcolor * 2;
	float alpha;
	if (isTexture)
	{
		diffuse = tex.Sample(smp, inData.uv)*S;
		ambient = tex.Sample(smp, inData.uv) * ambientcolor;
		alpha = tex.Sample(smp, inData.uv).a;
	}
	else
	{
		diffuse = diffusecolor*S;
		ambient = diffusecolor * ambientcolor;
		alpha = diffusecolor.a;
	}

	if (0 > movepos + inData.postrans.y)
	{
		float4 color = diffuse + specular + ambient;
		return color;
	}

	return float4(0, 0, 0, 1);

}

float4 VS_Outline(float4 pos : POSITION, float4 normal : NORMAL) : SV_POSITION
{
	pos.x += normal.x * 0.01;
	pos.y += normal.y * 0.01;
	pos.z += normal.z * 0.01;
	return mul(pos, matWVP);
}
float4 PS_Outline(float4 pos : SV_POSITION) : SV_Target
{
	return float4(0, 0, 0, 1);
}