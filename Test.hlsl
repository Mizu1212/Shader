Texture2D tex : register(t0); //�ǂ̃e�N�X�`�����g����������
SamplerState smp : register(s0);


cbuffer gloabl
{
	float4x4 matWVP;   //�s��������^
	float4x4 matNormal;
	float4x4 matW;
	float4   color;    //�}�e���A���̐F
	float4   camPos;
	bool     isTexture;//�e�N�X�`�����\���Ă��邩�ǂ���
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 color : COLOR0;
	float4 specular : COLOR1;
};

//���_�V�F�[�_�[
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)//:�Z�}���e�B�N�X�͉��̏�񂩁@
{
	VS_OUT outData;
	outData.pos = mul(pos,matWVP);   //�x�N�g�����s��ɂ���֐�
	outData.uv = uv;

	float4 light = float4(1, 1, -1, 0);
	light = normalize(light);
	normal = mul(normal, matNormal);//�@����]
	outData.color = dot(normal, light);
	outData.color = clamp(outData.color, 0, 1); //�؂�l��
	
	float4 V = normalize(mul(pos, matW) - camPos);//���_����
	float4 R = reflect(light, normal);
	
	//outData.specular = pow(clamp(dot(R, V),0,1), 200) * 1;
	outData.specular = clamp(dot(R, V),0,1);
	outData.specular = pow(outData.specular, 10);

	return outData;
}

//�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g�Ɋ֌W�Ȃ��ɐF�����߂��
float4 PS(VS_OUT inData) : SV_TARGET //SV�͓񎟌� �s�N�Z���V�F�[�_�[�̈����͒��_�V�F�[�_�[�̖߂�l�Ɠ���
{

	float4 diffuse;
    float4 ambient;


	if (isTexture)
	{
		diffuse = tex.Sample(smp, inData.uv)*inData.color;
		ambient = tex.Sample(smp, inData.uv) * 0.3;
	}
    else
    {
		diffuse = color*inData.color;
		ambient = color * 0.3;
    }
	return /*diffuse + ambient +*/ inData.specular;
}