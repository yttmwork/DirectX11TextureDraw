#ifndef VERTEX_SHADER_H_
#define VERTEX_SHADER_H_

#include <d3d11.h>
#include "ShaderBase.h"

//=====================================================================//
//! Vertexシェーダ情報を保持するクラス
//=====================================================================//
class VertexShader : public ShaderBase
{
public:
	/** Constructor */
	VertexShader() :
		m_Interface(nullptr)
	{}

	/** Destructor */
	virtual ~VertexShader()
	{
		if (m_Interface != nullptr)
		{
			m_Interface->Release();
			m_Interface = nullptr;
		}
	}

	/**
	* @brief VertexShader作成関数@n
	* 指定されたファイルからVertexShaderを作成する@n
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char*file_name)
	{
		// ファイル読み込み
		if (Load(file_name) == false)
		{
			return false;
		}

		// VertexShader作成
		if (FAILED(device->CreateVertexShader(
				m_Data,					// 生成元となるデータ
				m_Size,					// データのサイズ
				/*
					ID3D11ClassLinkageのポインタを設定
						ID3D11ClassLinkageはシェーダの実行関数を
						動的に切り替えることができる
						それによって、条件分岐をシェーダ内に書かなくてもすむので、
						シェーダの速度低下を抑えることができる。
						今回は使用しないのでnullptr
				*/
				nullptr,				
				&m_Interface)))			// 作成後のInterface格納先
		{
			return false;
		}

		return true;
	}

	/** ShaderInterfaceのゲッター */
	inline ID3D11VertexShader* GetShaderInterface()
	{
		return m_Interface;
	}
private:
	ID3D11VertexShader *m_Interface;		//!< @brief VertexShaderInterface
};

#endif
