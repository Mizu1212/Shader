Texture2D tex : register(t0); //�ǂ̃e�N�X�`�����g����������
SamplerState smp : register(s0);

Texture2D texNormal : register(t1);

cbuffer gloabl
{
	float4x4 matWVP;   //�s��������^
	float4x4 matNormal;
	float4x4 matW;
	float4   diffusecolor;    //�}�e���A���̐F
	float4   ambientcolor;
	float4   specularcolor;
	float4   camPos;
	float   shiness;
	bool     isTexture;//�e�N�X�`�����\���Ă��邩�ǂ���

};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 normal : NORMAL;
	float4 V : TEXCOORD1;
	float4 light : TEXCOORD2;

};

//���_�V�F�[�_�[
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)//:�Z�}���e�B�N�X�͉��̏�񂩁@
{
	VS_OUT outData;
	outData.pos = mul(pos, matWVP);   //�x�N�g�����s��ɂ���֐�
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

	float4 light = float4(1, 1, -1, 0);//���C�g
	light = normalize(light);
	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);

	return outData;
}

//�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g�Ɋ֌W�Ȃ��ɐF�����߂��
float4 PS(VS_OUT inData) : SV_TARGET //SV�͓񎟌� �s�N�Z���V�F�[�_�[�̈����͒��_�V�F�[�_�[�̖߂�l�Ɠ���
{

	float4 diffuse;
	float4 ambient;
	float4 specular;

	float4 normal = texNormal.Sample(smp, inData.uv) * 2 - 1;
	normal.w = 0;
	normal = normalize(normal);
	

	float4 S = dot(normal, inData.light);//����
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