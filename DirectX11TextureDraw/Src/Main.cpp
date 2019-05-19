#include <Windows.h>
#include "Window.h"
#include "DirectX.h"
#include "ModelData.h"
#include <vector>

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	Window window("DX11Practice", 1280, 720);

	std::vector<int*> list;

	// ウィンドウを作成
	if (window.Create() == false)
	{
		return 0;
	}

	DirectX dx;
	if (dx.Init() == false)
	{
		return 0;
	}

	ModelData *model;

	model = new ModelData();
	model->Create((ModelData::Type)0,
			dx.GetDevice(),
			dx.GetVertexShader()
		);

	while (true)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else 
		{
			dx.StartRendering();

			dx.RenderingPorigon(model);

			dx.FinishRendering();
		}
	}


	delete model;

	dx.Release();
}
