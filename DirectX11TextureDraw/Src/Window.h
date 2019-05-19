#ifndef WINDOW_H_
#define WINDOW_H_

#include <Windows.h>

//=====================================================================//
//! ウィンドウの作成を行うクラス
//=====================================================================//
class Window
{
public:
	static const char *ClassName;	//!< ウィンドウクラス登録時のクラス名

public:
	/**
	* @brief ウィンドウプロシージャ
	* @param[in] window_handle イベントが発生したウィンドウのハンドル
	* @param[in] message_id イベントメッセージ
	* @param[in] wparam イベント情報
	* @param[in] lparam イベント情報
	* @return メッセージ処理結果
	*/
	static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam);

public:
	/**
	* @brief Constructor
	* @param[in] title タイトル名
	* @param[in] width クライアント領域の横幅
	* @param[in] height クライアント領域の縦幅
	*/
	Window(const char *title, unsigned int width, unsigned int height) :
		m_Title(title),
		m_Width(width),
		m_Height(height)
	{
	}

	/** @brief Destructor */
	~Window() {}

	/**
	* @brief ウィンドウ生成関数@n
	* ウィンドウを生成して成功したらtrue、失敗したらfalseを返す
	* @return 生成の成否 成功(true)
	*/
	bool Create();

private:
	/**
	* @brief ウィンドウクラス登録関数@n
	* ウィドウを作成するためのウィンドウクラス情報を登録して成功したらtrue、失敗したらfalseを返す
	* @return 登録の成否 成功(true)
	*/
	bool EntryWindowClass();

	/**
	* @brief ウィドウのリサイズ関数@n
	* 生成後にクライアント領域のサイズにリサイズする
	* @param[in] リサイズ対象となるウィンドウハンドル
	*/
	void ResizeWindow(HWND window_handle);

private:
	const char *m_Title;		//!< @brief ウィンドウタイトル名
	unsigned int m_Width;		//!< @brief 横幅(クライアント領域)
	unsigned int m_Height;		//!< @brief 縦幅(クライアント領域)
};

#endif
