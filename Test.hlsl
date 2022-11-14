Texture2D tex : register(t0); //�ǂ̃e�N�X�`�����g����������
SamplerState smp : register(s0);


cbuffer gloabl
{
	float4x4 matWVP;   //�s��������^
	float4   color;    //�}�e���A���̐F
	bool     isTexture;//�e�N�X�`�����\���Ă��邩�ǂ���
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
};

//���_�V�F�[�_�[
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)//:�Z�}���e�B�N�X�͉��̏�񂩁@
{
	VS_OUT outData;
	outData.pos = mul(pos,matWVP);   //�x�N�g�����s��ɂ���֐�
	outData.uv = uv;
	return outData;
}

//�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g�Ɋ֌W�Ȃ��ɐF�����߂��
float4 PS(VS_OUT inData) : SV_TARGET //SV�͓񎟌� �s�N�Z���V�F�[�_�[�̈����͒��_�V�F�[�_�[�̖߂�l�Ɠ���
{
	if (isTexture)
	{
		return tex.Sample(smp, inData.uv);
	}
    else
    {
		return color;
    }
}