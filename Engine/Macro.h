#pragma once
//�z��̍폜
#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}
//�폜
#define SAFE_DELETE(p) if(p != nullptr){delete p; p = nullptr;}
//�������̊J��
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}