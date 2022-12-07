#include "Fbx.h"
#include"Direct3D.h"
#include "Camera.h"
#include"Texture.h"

//コンストラクタ
Fbx::Fbx()
{
    vertexCount_ = 0;
    polygonCount_ = 0;
	materialCount_ = 0;
    pVertexBuffer_ = nullptr;
    pIndexBuffer_ = nullptr;
    pConstantBuffer_ = nullptr;
	pMaterialList_ = nullptr;
}

//ロード
HRESULT Fbx::Load(std::string fileName)
{
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* pRootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = pRootNode->GetChild(0);
	FbxMesh* pMesh = pNode->GetMesh();

	//各情報の個数を取得

	vertexCount_ = pMesh->GetControlPointsCount();	//頂点の数
	polygonCount_ = pMesh->GetPolygonCount();	//ポリゴンの数
	materialCount_ = pNode->GetMaterialCount(); //マテリアルの個数


	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

		//引数のfileNameからディレクトリ部分を取得
		char dir[MAX_PATH];
		_splitpath_s(fileName.c_str(), nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
		sprintf_s(dir,dir);

		//カレントディレクトリ変更
		SetCurrentDirectory(dir);



	InitVertex(pMesh);		//頂点バッファ準備
	InitIndex(pMesh);		//インデックスバッファ準備
	IntConstantBuffer();	//コンスタントバッファ準備
	InitMaterial(pNode);    //マテリアル準備

	//終わったら戻す
	SetCurrentDirectory(defaultCurrentDir);

	//マネージャ解放
	pFbxManager->Destroy();


	pTexToon_ = new Texture;
	pTexToon_->Load(L"Assets\\無題.png");


    return S_OK;
}

//頂点バッファ準備
HRESULT Fbx::InitVertex(fbxsdk::FbxMesh* pMesh)
{
	//頂点情報を入れる配列
	VERTEX* vertices = new VERTEX[vertexCount_];

	//全ポリゴン
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = pMesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = pMesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//頂点のUV
			FbxLayerElementUV* pUV = pMesh->GetLayer(0)->GetUVs();
			int uvIndex = pMesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//頂点の法線
			FbxVector4 Normal;
			pMesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}

	//タンジェント取得
	for (int i = 0; i < polygonCount_; i++)
	{
		int startIndex = pMesh->GetPolygonVertexIndex(i);




		FbxGeometryElementTangent* t = pMesh->GetElementTangent(0);
		FbxVector4 tangent = t->GetDirectArray().GetAt(startIndex).mData;




		for (int j = 0; j < 3; j++)
		{
			int index = pMesh->GetPolygonVertices()[startIndex + j];
			vertices[index].tangent = XMVectorSet((float)tangent[0], (float)tangent[1], (float)tangent[2], 0.0f);
		}
	}


	// 頂点バッファ作成
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//インデックスバッファ準備
HRESULT Fbx::InitIndex(fbxsdk::FbxMesh* pMesh)
{

	pIndexBuffer_ = new ID3D11Buffer* [materialCount_];
	int* index = new int[polygonCount_ * 3];

	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;

		//全ポリゴン
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = pMesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3頂点分
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index[count] = pMesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
		}
		//インデックスバッファ作成
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * count;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		HRESULT hr;
		hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
			return hr;
		}
	}
	return S_OK;
}

//コンスタントバッファ準備
HRESULT Fbx::IntConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	// コンスタントバッファ作成
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}
//マテリアル準備
HRESULT Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		FbxSurfacePhong* pMaterial = (FbxSurfacePhong*)pNode->GetMaterial(i);
		FbxDouble3 diffuse = pMaterial->Diffuse;
		FbxDouble3 ambient = pMaterial->Ambient;
		FbxDouble3 specular = FbxDouble3(0,0,0);
		FbxDouble shiness = 1;
		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			 specular = pMaterial->Specular;
			 shiness = pMaterial->Shininess;
			 
		}
		
		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
		pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		pMaterialList_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);
		pMaterialList_[i].shiness = (float)shiness;
		
		//i番目のマテリアル情報を取得
		//FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);


		//普通のテクスチャ
		{
			//テクスチャ情報
			FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

			//テクスチャの数
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

			//テクスチャあり
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//ファイル名+拡張だけにする
				char name[_MAX_FNAME];	//ファイル名
				char ext[_MAX_EXT];	//拡張子
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				sprintf_s(name, "%s%s", name, ext);


				//ファイルからテクスチャ作成
				HRESULT hr;
				pMaterialList_[i].pTexture = new Texture;
				wchar_t wtext[FILENAME_MAX];
				size_t strlen(
					const char* textureFilePath //文字列の長さを求める
				);
				size_t ret;
				mbstowcs_s(&ret, wtext, name, strlen(textureFilePath));//マルチバイトからワイド変更

				hr = pMaterialList_[i].pTexture->Load(wtext);
				if (FAILED(hr))
				{
					MessageBox(NULL, "テクスチャの作成に失敗しました", "エラー", MB_OK);
					return hr;
				}

			}

			//テクスチャ無し
			else
			{
				pMaterialList_[i].pTexture = nullptr;

				//マテリアルの色
				//FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
				//FbxDouble3  diffuse = pMaterial->Diffuse;
				//pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
			}
		}

		//ノーマルテクスチャ
		{
			//テクスチャ情報
			FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);

			//テクスチャの数
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

			//テクスチャあり
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//ファイル名+拡張だけにする
				char name[_MAX_FNAME];	//ファイル名
				char ext[_MAX_EXT];	//拡張子
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				sprintf_s(name, "%s%s", name, ext);


				//ファイルからテクスチャ作成
				HRESULT hr;
				pMaterialList_[i].pTextureNormal = new Texture;
				wchar_t wtext[FILENAME_MAX];
				size_t strlen(
					const char* textureFilePath //文字列の長さを求める
				);
				size_t ret;
				mbstowcs_s(&ret, wtext, name, strlen(textureFilePath));//マルチバイトからワイド変更

				hr = pMaterialList_[i].pTextureNormal->Load(wtext);
				if (FAILED(hr))
				{
					MessageBox(NULL, "テクスチャの作成に失敗しました", "エラー", MB_OK);
					return hr;
				}

			}

			//テクスチャ無し
			else
			{
				pMaterialList_[i].pTextureNormal = nullptr;

				//マテリアルの色
				//FbxSurfaceLambert* pMaterial = (FbxSurfaceLambert*)pNode->GetMaterial(i);
				//FbxDouble3  diffuse = pMaterial->Diffuse;
				//pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
			}
		}

	}
}
//描画
void Fbx::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_3D);
	


	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	for (int i = 0; i < materialCount_; i++)
	{

		// インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//コンスタントバッファ
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

		
		CONSTANT_BUFFER cb;
		//cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		//cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());//影の付き方　法線
		//cb.isTexture = pMaterialList_[i].pTexture != nullptr;
		cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());
		cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
		XMStoreFloat4(&(cb.camPos) , Camera::GetPosition());
		cb.diffusecolor = pMaterialList_[i].diffuse;
		cb.ambientcolor = pMaterialList_[i].ambient;
		cb.specularcolor = pMaterialList_[i].specular;
		cb.shiness = pMaterialList_[i].shiness;
		
		if (pMaterialList_[i].pTexture == nullptr)
		{
			cb.isTexture = false;
		}
		else
		{
			cb.isTexture = true;
		}
		if (pMaterialList_[i].pTexture) {
			
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
			
		}

		if (pMaterialList_[i].pTextureNormal) {

			
			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTextureNormal->GetSRV();
			Direct3D::pContext->PSSetShaderResources(1, 1, &pSRV);

		}
		//ID3D11ShaderResourceView* pSRVToon = pTexToon_->GetSRV();
		//Direct3D::pContext->PSSetShaderResources(1, 1, &pSRVToon);

		//cb.diffuseColor = pMaterialList_[i].diffuse;
		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
		
		Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開
		//描画
		Direct3D::pContext->DrawIndexed(polygonCount_ * 3, 0, 0);
	}
}
//解放
void Fbx::Release()
{
	pTexToon_->Release();
	SAFE_DELETE(pTexToon_);
    SAFE_RELEASE(pVertexBuffer_);
	for (int i = 0; i < materialCount_; i++)
	{
		SAFE_RELEASE(pIndexBuffer_[i]);
    }
	SAFE_DELETE_ARRAY(pIndexBuffer_);
    SAFE_RELEASE(pConstantBuffer_);
}
