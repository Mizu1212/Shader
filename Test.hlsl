Texture2D tex : register(t0); //�ǂ̃e�N�X�`�����g����������
SamplerState smp : register(s0);


cbuffer gloabl
{
	float4x4 matWVP;   //�s��������^
	float4x4 matNormal;
	float4x4 matW;
	float4   diffusecolor;    //�}�e���A���̐F
	float4   ambientcolor;
	float4   specularcolor;
	float4   camPos;
	float   shinesscolor;
	bool     isTexture;//�e�N�X�`�����\���Ă��邩�ǂ���
	
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 normal : NORMAL;
	float4 V : TEXCOORD1;
	
};

//���_�V�F�[�_�[
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)//:�Z�}���e�B�N�X�͉��̏�񂩁@
{
	VS_OUT outData;
	outData.pos = mul(pos,matWVP);   //�x�N�g�����s��ɂ���֐�
	outData.uv = uv;

	normal.w = 0;
	
	outData.normal = mul(normal, matNormal);
	outData.normal = normalize(outData.normal);
	
	outData.V = normalize(mul(pos, matW) - camPos);

	

	return outData;
}

//�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g�Ɋ֌W�Ȃ��ɐF�����߂��
float4 PS(VS_OUT inData) : SV_TARGET //SV�͓񎟌� �s�N�Z���V�F�[�_�[�̈����͒��_�V�F�[�_�[�̖߂�l�Ɠ���
{

	float4 diffuse;
    float4 ambient;
	float4 specular;

	float4 light = float4(1, 1, -1, 0);//���C�g
	light = normalize(light);

	float4 S = dot(inData.normal, light);//����
	S = clamp(S, 0, 1);
	
	float4 R = reflect(light, inData.normal);
	specular = pow(clamp(dot(R, inData.V), 0, 1), shinesscolor) * specularcolor * 2;
	//specular = pow(clamp(dot(R, inData.V), 0, 1), 10) * 3;
	if (isTexture)
	{
		diffuse = tex.Sample(smp, inData.uv)*S;
		ambient = tex.Sample(smp, inData.uv) * ambientcolor;
		//ambient = tex.Sample(smp, inData.uv) *0.2;
	}
    else
    {
		diffuse = diffusecolor*S;
		ambient = diffusecolor * ambientcolor;
		//ambient = diffusecolor * 0.2;
    }
	
	return diffuse + ambient + specular;
}