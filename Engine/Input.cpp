#include "Input.h"
namespace Input
{

	//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUT8   pDInput_ = nullptr;//LP����n�܂���̂̓|�C���^

	//�L�[�{�[�h
	LPDIRECTINPUTDEVICE8 pKeyDevice_ = nullptr;//�f�o�C�X�I�u�W�F�N�g�@�L�[�{�[�h�̏���
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256];    //�O�t���[���ł̊e�L�[�̏��

	//�}�E�X
	LPDIRECTINPUTDEVICE8	pMouseDevice_;	//�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState_;				//�}�E�X�̏��
	DIMOUSESTATE prevMouseState_;			//�O�t���[���̃}�E�X�̏��				
	XMFLOAT3 mousePosition;                 //�}�E�X�J�[�\���̈ʒu
 
	//�R���g���[���[
	const int MAX_PAD_NUM = 4;
	XINPUT_STATE controllerState_[MAX_PAD_NUM];//�R���g���[���[�̏��
	XINPUT_STATE prevControllerState_[MAX_PAD_NUM];

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr;
		//DirectInput�{��
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput_, nullptr);
		if (FAILED(hr))
		{
			return hr;
		}

		//�L�[�{�[�h
		hr = pDInput_->CreateDevice(GUID_SysKeyboard, &pKeyDevice_, nullptr);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pKeyDevice_->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pKeyDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr))
		{
			return hr;
		}

		//�}�E�X
		hr = pDInput_->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pMouseDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	void Update()
	{
		//�L�[�{�[�h
		pKeyDevice_->Acquire();
		memcpy(prevKeyState,keyState, sizeof(keyState));//�������ɃR�s�[��̃������u���b�N�̃|�C���^�������ŃR�s�[���̃|�C���^��O�����ŃR�s�[�T�C�Y
		pKeyDevice_->GetDeviceState(sizeof(keyState), &keyState);//���ׂẴL�[�̏�񂪔z��ɓ���

		//�}�E�X
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));//�������ɃR�s�[��̃������u���b�N�̃|�C���^�������ŃR�s�[���̃|�C���^��O�����ŃR�s�[�T�C�Y
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);

		//�R���g���[���[
		for (int i = 0; i < MAX_PAD_NUM; i++)
		{
			memcpy(&prevControllerState_[i], &controllerState_[i], sizeof(controllerState_[i]));//�������ɃR�s�[��̃������u���b�N�̃|�C���^�������ŃR�s�[���̃|�C���^��O�����ŃR�s�[�T�C�Y
			XInputGetState(i, &controllerState_[i]);
		}
	}


	void Release()
	{
		SAFE_RELEASE(pDInput_);
		SAFE_RELEASE(pKeyDevice_);
		SAFE_RELEASE(pMouseDevice_);
	}

	/////////////////////////////�@�L�[�{�[�h���擾�@//////////////////////////////////
	//�L�[��������Ă��邩���ׂ�
	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)//�A���h���Ƃ�@�P�U�i�\�L
		{
			return true;
		}
		return false;
	}

	//�L�[���������������ׂ�i�������ςȂ��͖����j
	bool IsKeyDown(int keyCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (!(prevKeyState[keyCode] & 0x80) && IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	//�L�[���������������ׂ�
	bool IsKeyUp(int keyCode)
	{
		//�������͉����ĂāA���͉����ĂȂ����
		if ((prevKeyState[keyCode] & 0x80) && !IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	

	/////////////////////////////�@�}�E�X���擾�@//////////////////////////////////

	//�}�E�X�̃{�^����������Ă��邩���ׂ�
	bool IsMouseButton(int buttonCode)
	{
		//�����Ă�
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�
	bool IsMouseButtonUp(int buttonCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
		if (!IsMouseButton(buttonCode) && prevMouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//�}�E�X�J�[�\���̈ʒu���擾
	XMFLOAT3 GetMousePosition()
	{
		return mousePosition;
	}

	//�}�E�X�J�[�\���̈ʒu���Z�b�g
	void SetMousePosition(int x, int y)
	{
		XMVECTOR v = XMVectorSet((float)x, (float)y, 0, 0);
		XMFLOAT3 f;
		XMStoreFloat3(&f, v);

		mousePosition = f;
	}

	//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ);
		return result;
	}

	/////////////////////////////�@�R���g���[���[���擾�@//////////////////////////////////

	//�R���g���[���[�̃{�^����������Ă��邩���ׂ�
	bool IsPadButton(int buttonCode, int padID)
	{
		if (controllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true; //�����Ă�
		}
		return false; //�����ĂȂ�
	}

	//�R���g���[���[�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsPadButtonDown(int buttonCode, int padID)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsPadButton(buttonCode, padID) && !(prevControllerState_[padID].Gamepad.wButtons & buttonCode))
		{
			return true;
		}
		return false;
	}

	//�R���g���[���[�̃{�^�����������������ׂ�
	bool IsPadButtonUp(int buttonCode, int padID)
	{
		//�������ĂȂ��āA�O��͉����Ă�
		if (!IsPadButton(buttonCode, padID) && prevControllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true;
		}
		return false;
	}

	float GetAnalogValue(int raw, int max, int deadZone)
	{
		float result = (float)raw;//padID�����肻��ID�̃R���g���[���[�̏�Ԃ�����

		if (result > 0)
		{
			//�f�b�h�]�[��
			if (result < deadZone)
			{
				result = 0;
			}
			else
			{
				result = (result - deadZone) / (max - deadZone);
			}
		}

		else
		{
			//�f�b�h�]�[��
			if (result > -deadZone)
			{
				result = 0;
			}
			else
			{
				result = (result + deadZone) / (max - deadZone);
			}
		}

		return result;
	}

	//���X�e�B�b�N�̌X�����擾
	XMFLOAT3 GetPadStickL(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLX, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLY, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//�E�X�e�B�b�N�̌X�����擾
	XMFLOAT3 GetPadStickR(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRX, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRY, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//���g���K�[�̉������݋���擾
	float GetPadTrrigerL(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bLeftTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//�E�g���K�[�̉������݋���擾
	float GetPadTrrigerR(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bRightTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//�U��������
	void SetPadVibration(int l, int r, int padID)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = l; // �����[�^�[�̋���
		vibration.wRightMotorSpeed = r;// �E���[�^�[�̋���
		XInputSetState(padID, &vibration);
	}
}