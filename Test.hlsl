//���_�V�F�[�_�[
float4 VS( float4 pos : POSITION ) : SV_POSITION//:�Z�}���e�B�N�X�͉��̏�񂩁@
{
	return pos;
}

//�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g�Ɋ֌W�Ȃ��ɐF�����߂��
float4 PS(float4 pos : SV_POSITION) : SV_TARGET //SV�͓񎟌�
{
	
	return float4(1-(pos.x/800), 1, 1, 1);
}