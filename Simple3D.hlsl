//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
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

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 normal : NORMAL;
	float4 V : TEXCOORD1;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)//�Z�}���e�B�N�X�@���̏�񂪓����Ă��邩
{
	VS_OUT outData;
	outData.pos = mul(pos, matWVP);   //�x�N�g�����s��ɂ���֐�
	outData.uv = uv;

	normal.w = 0;

	outData.normal = mul(normal, matNormal);
	outData.normal = normalize(outData.normal);

	outData.V = normalize(mul(pos, matW) - camPos);



	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	float4 diffuse;
	float4 ambient;
	float4 specular;

	float4 light = float4(1, 1, -1, 0);//���C�g
	light = normalize(light);

	float4 S = dot(inData.normal, light);//����
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
		diffuse = g_texture.Sample(g_sampler, inData.uv); /** texToon.Sample(smp, uv);*/
		/*ambient = g_texture.Sample(g_sampler, inData.uv) * ambientcolor;*/
		//ambient = tex.Sample(smp, inData.uv) *0.2;
	}
	else
	{
		diffuse = diffusecolor; /** texToon.Sample(smp, uv);*/
		/*ambient = diffusecolor * ambientcolor;*/
		//ambient = diffusecolor * 0.2;
	}

	return diffuse +/*+ ambient +*/ specular;
}