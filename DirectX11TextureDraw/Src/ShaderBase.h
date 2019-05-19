#ifndef SHADER_BASE_H_
#define SHADER_BASE_H_

#include <d3d11.h>
#include <stdio.h>

//=====================================================================//
//! 汎用的に使用するためのメンバを用意したSharder規定クラス
//=====================================================================//
class ShaderBase
{
public:
	/** Constructor */
	ShaderBase() :
		m_Size(0),
		m_Data(nullptr)
	{}

	/** Destructor */
	virtual ~ShaderBase() 
	{
		if (m_Data != nullptr)
		{
			delete[] m_Data;
		}
	}

	/** Shaderデータ(バイナリ)のゲッター */
	inline const char* GetData()
	{
		return m_Data;
	}

	/** Shaderのデータサイズのゲッター */
	inline long GetSize()
	{
		return m_Size;
	}

	/**
	* @brief シェーダ作成関数@n
	* 指定されたファイルからシェーダを作成する@n
	* 継承先では各シェーダを作成するように実装する
	* @return 作成結果 成功(true)
	* @param[in] device ID3D11Device
	* @param[in] file_name 読み込み対象のファイル名
	*/
	virtual bool Create(ID3D11Device* device, const char*file_name) = 0;
protected:

	/**
	* @brief シェーダ読み込み関数@n
	* 指定されたファイルを読み込む@
	* ファイルはcso(コンパイル済み)を対象としているので@n
	* hlslファイルを読み込むとエラーとして扱う
	* @return 読み込み結果 成功(true)
	* @param[in] file_name 読み込み対象のファイル名
	*/
	bool Load(const char*file_name);

protected:
	char* m_Data;	//!< @brief シェーダデータ(バイナリ)
	long m_Size;	//!< @brief データのサイズ
};

#endif
