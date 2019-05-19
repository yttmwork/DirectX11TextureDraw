#include <stdio.h>
#include "DirectX.h"
#include "ShaderBase.h"
#include "Window.h"
#include "GraphicsUtility.h"

#pragma comment(lib,"d3d11.lib")

bool DirectX::Init()
{
	// DeviceとSwapChainの作成
	if (CreateDeviceAndSwapChain() == false)
	{
		return true;
	}

	// RenderTargetViewの作成
	if (CreateRenderTargetView() == false)
	{
		return false;
	}

	// Depth、StencilViewの作成
	if (CreateDepthAndStencilView() == false)
	{
		return false;
	}

	// シェーダ作成
	if (CreateShader() == false)
	{
		return false;
	}

	SetUpViewPort();

	return true;
}

void DirectX::Release()
{
	if (m_VertexShader != nullptr)
	{
		delete m_VertexShader;
		m_VertexShader = nullptr;
	}

	if (m_PixelShader != nullptr)
	{
		delete m_PixelShader;
		m_PixelShader = nullptr;
	}

	if (m_Context != nullptr)
	{
		m_Context->ClearState();
		m_Context->Flush();
		m_Context->Release();
		m_Context = nullptr;
	}

	if (m_DepthStencilView != nullptr)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = nullptr;
	}

	if (m_RenderTargetView != nullptr)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	if (m_SwapChain != nullptr)
	{
		m_SwapChain->Release();
		m_SwapChain = nullptr;
	}

	if (m_Device != nullptr)
	{
		m_Device->Release();
		m_Device = nullptr;
	}
}

void DirectX::StartRendering()
{
	float clear_color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RenderTarget塗りつぶしカラー(RGBA)

	// DirecX9ではRenderTargetとDepth、Stencilバッファのクリアは別々にする
    m_Context->ClearRenderTargetView( 
				m_RenderTargetView,							// クリア対象のView
				clear_color );								// クリアカラー

    m_Context->ClearDepthStencilView( 
				m_DepthStencilView,							// クリア対象のView	
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// クリアフラグ(今回はDepth、Stencilともにクリア)
				1.0f,										// 深度クリア値
				0 );										// ステンシルクリア値
}

void DirectX::FinishRendering()
{
	// 描画コマンドを送信する
	m_SwapChain->Present(
			/*
				垂直同期のタイミング
					0なら即時描画
					1以上なら指定した値の垂直同期後に描画
			*/
			0,
			/*
				出力オプション
					フレーム出力を行うためのオプションで
					基本は0で問題ないと思われる
					その他のフラグはDXGI_PRESENTで確認可能
			*/
			0 );	// 出力オプション
}

void DirectX::RenderingPorigon(ModelData* model)
{
    UINT strides = sizeof( CustomVertex );
    UINT offsets = 0;
	ID3D11Buffer* buff = model->GetBuffer();
	// プリミティブの形状を指定
    m_Context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	// VerteXShader、PixelShaderを設定
    m_Context->VSSetShader( 
		m_VertexShader->GetShaderInterface(),	// 使用するVertexShder
		/*
			ClassInstanceのポインタを設定する
				これはShader作成時のID3D11ClassLinkageを使用した場合に
				用いる項目なので今回はnullptrを指定する
		*/
		nullptr,								
		0 );									// ClassInstanceの数
    m_Context->PSSetShader( m_PixelShader->GetShaderInterface(), nullptr, 0 );

	// (OutputManger)RnderTagetの指定
    m_Context->OMSetRenderTargets( 
			1,							// 使用するViewの数
			&m_RenderTargetView,		// 使用するRenderTargetView
			m_DepthStencilView );		// 使用するDepthStencilView

	/*
		IA(InputAssemblerStage)に入力レイアウトを設定する
		IAはShader側の最初のステージと考えてもよさそう
		ここに描画予定の頂点バッファやシェーダを設定することで、
		GPU側がそれをパイプラインの流れ上で行っていく
	*/
	m_Context->IASetInputLayout(model->GetInputLayout());
	// IAに設定する頂点バッファの指定
	m_Context->IASetVertexBuffers(
		0,			// バッファ送信のスロット番号
		1,			// バッファの数
		&buff,		// 頂点バッファ
		&strides,	// バッファに使用している構造体のサイズ
		&offsets);	// 開始オフセット
	
	// 描画
    m_Context->Draw( 
			6,		// 頂点数
			0 );	// 開始頂点のインデックス
}

void DirectX::SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC *dxgi)
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	/*
		DirectX11版PresentationParameter
			バッファの数やサイズ、カラーフォーマット等を設定する
	*/
	ZeroMemory(dxgi, sizeof(DXGI_SWAP_CHAIN_DESC));
	dxgi->BufferCount = 1;									// バッファの数
	dxgi->BufferDesc.Width = (rect.right - rect.left);		// バッファの横幅
	dxgi->BufferDesc.Height = (rect.bottom - rect.top);		// バッファの縦幅
	dxgi->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラーフォーマット
	dxgi->BufferDesc.RefreshRate.Numerator = 60;			// リフレッシュレートの分母
	dxgi->BufferDesc.RefreshRate.Denominator = 1;			// リフレッシュレートの分子
	dxgi->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使い方 Usage => 使用方法
	dxgi->OutputWindow = window_handle;						// 出力対象のウィンドウハンドル
	dxgi->SampleDesc.Count = 1;								// マルチサンプリングのサンプル数(未使用は1)
	dxgi->SampleDesc.Quality = 0;							// マルチサンプリングの品質(未使用は0)
	dxgi->Windowed = true;									// ウィンドウモード指定
}

bool DirectX::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC dxgi;
	SetUpDxgiSwapChainDesc(&dxgi);

	D3D_FEATURE_LEVEL level;
	// デバイス生成とスワップチェーン作成を行う
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,					// ビデオアダプタ指定(nullptrは既定)
		D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプ
		nullptr,					// D3D_DRIVER_TYPE_SOFTWARE指定時に使用
		0,							// フラグ指定
		nullptr,					// D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
		0,							// 上のD3D_FEATURE_LEVEL配列の要素数
		D3D11_SDK_VERSION,			// SDKバージョン
		&dxgi,						// DXGI_SWAP_CHAIN_DESC
		&m_SwapChain,				// 関数成功時のSwapChainの出力先 
		&m_Device,					// 関数成功時のDeviceの出力先
		&level,						// 成功したD3D_FEATURE_LEVELの出力先
		&m_Context)))				// 関数成功時のContextの出力先
	{
		return false;
	}

	return true;
}

bool DirectX::CreateRenderTargetView()
{
	// RenderTargetViewの対象となるBufferの取得
	ID3D11Texture2D* back_buffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer)))
	{
		return false;
	}
	
	// BufferからRenderTargetViewの作成
	if (FAILED(m_Device->CreateRenderTargetView(back_buffer, NULL, &m_RenderTargetView)))
	{
		return false;
	}
	
	// Targetの取得終わったのでBufferを解放
	back_buffer->Release();

	return true;
}

bool DirectX::CreateDepthAndStencilView()
{
	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	//深度ステンシルバッファ作成
    D3D11_TEXTURE2D_DESC texture_desc;
    ZeroMemory( &texture_desc, sizeof( D3D11_TEXTURE2D_DESC ) );
    texture_desc.Width              = (rect.right - rect.left);			// 横幅
    texture_desc.Height             = (rect.bottom - rect.top);			// 縦幅
	/*
		ミップマップのレベル指定
			どのレベルまで生成するかという設定
			1はマルチサンプリングされたテクスチャを使用するあるので、
			ミップマップはなしと考えられる
			0は全生成とあるので可能な限りのレベルまでテクスチャが生成されると思われる
	*/
    texture_desc.MipLevels          = 1;
	/*
		テクスチャ配列のサイズ指定
			テクスチャ配列について調べ切れていないので
			他のサンプルと同様に1を設定しておく
	*/
    texture_desc.ArraySize          = 1;
	/*
		テクスチャのフォーマット
			DXGI_FORMAT_D24_UNORM_S8_UINTを使用する
			これはおそらくDepth24bit、Stencil8bitとされると思う

			※．UNORMはUnsigned NORMalizedの略で指定された範囲を0.0～1.0にするみたい
	*/
    texture_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;	// テクスチャーフォーマット
	// マルチサンプリング設定(使わない)
	texture_desc.SampleDesc.Count   = 1;								
    texture_desc.SampleDesc.Quality = 0;
    texture_desc.Usage              = D3D11_USAGE_DEFAULT;				// テクスチャの使用方法(デフォルト)
    texture_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;			// Bind設定はDepth、Stencilに設定
	/*
		リソースへのCPUのアクセス権限についての設定
			ReadとWriteがあるが、権限について現状は考える必要はないはずなので、
			デフォルト値であると思われる0をしておく

			※．readとwriteはenum(D3D11_CPU_ACCESS_FLAG)で用意されていた

	*/
    texture_desc.CPUAccessFlags     = 0;
	/*
		リソースオプションのフラグ
			Microsoft Docsでフラグを確認する限りは
			通常使用でフラグを設定する必要はないと思われるので
			0としておく
	*/
    texture_desc.MiscFlags          = 0;

	// texture_descの情報でテクスチャを作成
	if (FAILED(m_Device->CreateTexture2D(&texture_desc, NULL, &m_DepthStencilTexture)))
	{
		return false;
	}

	// Depth、StencilViewの設定
	// DepthStencilView 
    D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
    ZeroMemory( &dsv_desc, sizeof( dsv_desc ) );
    dsv_desc.Format             = texture_desc.Format;				// Viewのフォーマット(Textureのものを使用)
	/*
		DSVが何次元であるかとTextureの種類を設定する値
			D3D11_DSV_DIMENSION_TEXTURE2Dのほかにも
			D3D11_DSV_DIMENSION_TEXTURE1Dや
			D3D11_DSV_DIMENSION_TEXTURE2D_ARRAYなどがあったので
			DSVが何次元であるかとTextureの種類を設定するメンバと思われる
			今回は何も通常のTextureとして用意しているはず、
			なので、D3D11_DSV_DIMENSION_TEXTURE2Dを指定
	*/
    dsv_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;	
    dsv_desc.Texture2D.MipSlice = 0;								// 最初に使用するミップマップのレベルを指定 

	// CreateTexture2Dとdsv_descからDepthとStencilバッファを作る
	if (FAILED(m_Device->CreateDepthStencilView(
				m_DepthStencilTexture,				// DSVとして使用されるTexture
				&dsv_desc,							// DSVの設定
				&m_DepthStencilView)))				// ID3D11DepthStencilViewの出力先
	{
		return false;
	}

	return true;
}

bool DirectX::CreateShader()
{
	/*
		シェーダの生成はクラスを生成して行う
		※シェーダクラスは自前

		クラス生成した理由
			・ShaderInterface生成のメソッドがShader毎に異なる
				例：
					PixelShader => CreatePixelShader
					VertexShader => CreateVertexShader

			・元データとサイズを使用することがある
				VertexShaderで使用するVertexShaderの元データとサイズを必要とすることがあり、
				他のShaderでも起こりえる可能性があるためクラスを作り、
				そこで保持するようにした

		シェーダはcso(コンパイラ済み)を使用する
	*/
	m_VertexShader = new VertexShader();
	if (m_VertexShader->Create(m_Device, "Res/Shader/VertexShader.cso") == false)
	{
		return false;
	}

	m_PixelShader = new PixelShader();
	if (m_PixelShader->Create(m_Device, "Res/Shader/PixelShader.cso") == false)
	{
		return false;
	}

	return true;
}

void DirectX::SetUpViewPort()
{
 	HWND window_handle = FindWindow(Window::ClassName, nullptr);
	RECT rect;
	GetClientRect(window_handle, &rect);

	//ビューポートの設定
	D3D11_VIEWPORT view_port;
	view_port.TopLeftX = 0;							// 左上X座標
	view_port.TopLeftY = 0;							// 左上Y座標
	view_port.Width = (rect.right - rect.left);		// 横幅
	view_port.Height = (rect.bottom - rect.top);	// 縦幅
	view_port.MinDepth = 0.0f;						// 最小深度
	view_port.MaxDepth = 1.0f;						// 最大深度

	m_Context->RSSetViewports(	
		1,					// 設定するビューポートの数
		&view_port );		// 設定するビューポート情報のポインタ
}
