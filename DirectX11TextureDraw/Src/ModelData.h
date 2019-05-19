#ifndef MODEL_DATA_H_
#define MODEL_DATA_H_

#include <d3d11.h>
#include "VertexShader.h"

//=====================================================================//
//! Modelクラス
//=====================================================================//
class ModelData
{
public:
	/** モデルの種類 */
	enum Type
	{
		Test01,
		Test02,
	};
public:
	/** Constructor */
	ModelData() :
		m_VertexBuffer(nullptr),
		m_InputLayout(nullptr)
	{}

	/** Destructor */
	~ModelData()
	{
		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Release();
			m_VertexBuffer = nullptr;
		}

		if (m_InputLayout != nullptr)
		{
			m_InputLayout->Release();
			m_InputLayout = nullptr;
		}
	}

	/**
	* @brief モデル生成@n
	* モデルデータを作成する
	* @return 作成の成否 成功(true)
	* @param[in] model_type モデルデータの種類(テストとしてモデルを分けたかったので作った)
	* @param[in] device デバイス
	* @param[in] vertex_shader モデルと関連付けるVertexShader
	*/
	bool Create(Type model_type, ID3D11Device *device, VertexShader* vertex_shader);

	/** バッファーのゲッター */
	ID3D11Buffer* GetBuffer()
	{
		return m_VertexBuffer;
	}

	/** InputLayoutのゲッター */
	ID3D11InputLayout* GetInputLayout()
	{
		return m_InputLayout;
	}

private:
	ID3D11Buffer* m_VertexBuffer;		//!< @brief 頂点バッファ
	ID3D11InputLayout* m_InputLayout;	//!< @brief 入力レイアウト
};

#endif
