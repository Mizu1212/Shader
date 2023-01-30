#include "Model.h"

namespace Model
{
	//ロード済みのモデルデータ一覧
	std::vector<ModelData*> vm_Data;

	//初期化
	void Initialize()
	{
		AllRelease();
	}

	//モデルをロード
	int Load(std::string fileName,SHADER_TYPE Type_)
	{
		ModelData* pMData = new ModelData;//構造体のインスタンスを作成

		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;//同じファイルがあるかどうか見る Exist=存在
		for (int i = 0; i < vm_Data.size(); i++)
		{
			//すでに開いている場合
			if (vm_Data[i] != nullptr && vm_Data[i]->filename_ == fileName)//vm_Dataにデータが入っているかつ入っているファイルのなまえが同じとき
			{
				pMData->pFbx = vm_Data[i]->pFbx;
				isExist = true;
				break;
			}
		}

		//新たにファイルを開く
		if (isExist == false)
		{
			pMData->pFbx = new Fbx;//Fbxオブジェクトを作成し、ロードする
			if (FAILED(pMData->pFbx->Load(fileName, Type_)))
			{
				//開けなかった
				SAFE_DELETE(pMData->pFbx);
				SAFE_DELETE(pMData);
				return -1;
			}
			//無事開けた
			pMData->filename_ = fileName;
		}

		//使ってない番号が無いか探す
		for(int i = 0; i < vm_Data.size(); i++)
		{
			if (vm_Data[i] == nullptr)
			{
				vm_Data[i] = pMData;
				return i;
			}
		}
		//新たに追加
		vm_Data.push_back(pMData); //構造体の中身が埋まったので動的配列に突っ込む
		return (int)vm_Data.size() - 1;//番号（配列の要素数-1）を返す

	}
	void Draw(int handle)
	{
		//モデルの確認
		if (handle < 0 || handle >= vm_Data.size() || vm_Data[handle] == nullptr)
		{
			return;
		}

		if (vm_Data[handle]->pFbx)
		{
			vm_Data[handle]->pFbx->Draw(vm_Data[handle]->transform_);
		}

	}
	//任意のモデルを開放
	void Release(int handle)
	{
		//モデルの確認
		if (handle < 0 || handle >= vm_Data.size() || vm_Data[handle] == nullptr)
		{
			return;
		}
		//同じモデルを他でも使っていないか
		bool isExist = false;//使ってないかの存在確認
		for (int i = 0; i < vm_Data.size(); i++)
		{
			//すでに開いている場合
			if (vm_Data[i] != nullptr && i != handle && vm_Data[i]->pFbx == vm_Data[handle]->pFbx)
			{
				isExist = true;
				break;
			}
		}
		//使ってなければモデル解放
		if (isExist == false)
		{
			SAFE_DELETE(vm_Data[handle]->pFbx);
		}


		SAFE_DELETE(vm_Data[handle]);
	}
	//ワールド行列を設定
	void SetTransform(int handle, Transform& transform_)
	{
		if (handle < 0 || handle >= vm_Data.size())
		{
			return;
		}

		vm_Data[handle]->transform_ = transform_;
	}
	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle)
	{
		return vm_Data[handle]->transform_.GetWorldMatrix();
	}
	//全てのモデルを解放
	void AllRelease()
	{
		for (int i = 0; i < vm_Data.size(); i++)
		{
			if (vm_Data[i] != nullptr)
			{
				Release(i);
			}
		}
		vm_Data.clear();
	}
}