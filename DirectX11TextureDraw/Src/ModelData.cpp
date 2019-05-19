#include "ModelData.h"
#include "GraphicsUtility.h"

// 頂点バッファデータ
CustomVertex g_VertexList01[] {
    { { -0.2f,  0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { {  0.2f, 0.2f, 0.2f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { -0.2f, -0.2f, 0.2f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

	{ { 0.2f,  0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.2f,  -0.2f, 0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { -0.2f, -0.2f, 0.2f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
};

/*
	D3D11_INPUT_ELEMENT_DESC
		入力属性設定用構造体

		LPCSTR SemanticName
			頂点構造の項目名を表しており、
			シェーダの入力セマンティクスとの整合性を図るために使われる
		
		UINT SemanticIndex
			SemanticNameの識別ID
			POSITIONという項目でIDが0の場合は
			入力セマンティクスでPosition0として扱われる

		DXGI_FORMAT Format
			この項目のフォーマット

		UINT InputSlot
			スロット番号
			シェーダに流し込む入口の番号
			最初は0を指定していれば問題ない

		UINT AlignedByteOffset
			先頭からのoffset値
			COLORはfloatのx,y,zの次の項目なのでfloat*3の位置になる
			(D3D11_APPEND_ALIGNED_ELEMENTというマクロがあるが、
			 中身が全ビットがonになっている状態で機能が不明のため、未使用)

		D3D11_INPUT_CLASSIFICATION InputSlotClass
			この項目が頂点情報か否かを設定する

		UINT InstanceDataStepRate
			１度に描画するインスタンスの数を指定できる
			頂点情報を設定している場合は0で問題ない
*/
D3D11_INPUT_ELEMENT_DESC g_VertexDesc[] {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

bool ModelData::Create(Type model_type, ID3D11Device* device, VertexShader *vertex_shader)
{
	//頂点バッファ作成
    D3D11_BUFFER_DESC buffer_desc;
    buffer_desc.ByteWidth           = sizeof( CustomVertex ) * 6;	// バッファのサイズ
    buffer_desc.Usage               = D3D11_USAGE_DEFAULT;			// 使用方法
    buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;		// BIND設定
	/*
		リソースへのCPUのアクセス権限についての設定
			ReadとWriteがあるが、権限について現状は考える必要はないはずなので、
			デフォルト値であると思われる0をしておく

			※．readとwriteはenum(D3D11_CPU_ACCESS_FLAG)で用意されていた

	*/
    buffer_desc.CPUAccessFlags      = 0;
	/*
		リソースオプションのフラグ
			Microsoft Docsでフラグを確認する限りは
			通常使用でフラグを設定する必要はないと思われるので
			0としておく
	*/
    buffer_desc.MiscFlags           = 0;
    buffer_desc.StructureByteStride = sizeof(CustomVertex);			// 構造体のサイズ
 
    D3D11_SUBRESOURCE_DATA sub_resource_data;
	/*
		pSysMem
			バッファの中身の設定
			頂点座標やカラー、法線などの情報が入った
			配列の先頭ポインタを指定する
	*/
    sub_resource_data.pSysMem          = g_VertexList01;
	/*
		SysMemPitchとSysMemSlicePitchは
		textureデータを使用する際に使用するメンバなので
		今回は0指定で問題ない
	*/
    sub_resource_data.SysMemPitch      = 0;		
    sub_resource_data.SysMemSlicePitch = 0;
 
	// バッファ作成
	if (FAILED(device->CreateBuffer(
			&buffer_desc,			// バッファ情報
			&sub_resource_data,		// リソース情報
			&m_VertexBuffer)))		// 作成されたバッファの格納先
	{
		return false;
	}
 
    //頂点レイアウト作成
	if (FAILED(device->CreateInputLayout(
			g_VertexDesc,				// レイアウト設定
			ARRAYSIZE(g_VertexDesc),	// 配列サイズ
			vertex_shader->GetData(),	// レイアウトと関連付ける頂点シェーダのデータ
			vertex_shader->GetSize(),	// レイアウトと関連付ける頂点シェーダのサイズ
			&m_InputLayout)))			// 作成された頂点レイアウトの格納先
	{
		return false;
	}

	return true;
}
