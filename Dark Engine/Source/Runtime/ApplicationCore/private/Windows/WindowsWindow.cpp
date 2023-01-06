#include "../../public/Windows/WindowsWindow.h"
#include <windowsx.h>
#include <assert.h>

#include <Engine/public/DEngine.h>

#include <WinUser.h>
#include <Windows.h>



FWindowsWindow::FWindowsWindow(FWindowsWindowManager* maneger, UINT index)
{
	m_Manager = (maneger);
	m_Index = index;
}

void FWindowsWindow::Create(UINT w, UINT h, UINT x, UINT y, string name, FWindowsWindow* Parent)
{
	WNDCLASSEX wSex;

	wstring wClassName(name.begin(), name.end());

	LPCWSTR ClassName = wClassName.c_str();

	

	wSex.cbSize = sizeof(WNDCLASSEX);
	wSex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wSex.lpfnWndProc = FWindowsWindow::WndProc;
	wSex.cbClsExtra = 0;
	wSex.cbWndExtra = 0;
	wSex.hInstance = Application::GetInstance();
	wSex.hIcon = LoadIcon(Application::GetInstance(), IDI_APPLICATION);
	wSex.hCursor = LoadCursor(Application::GetInstance(), IDC_ARROW);
	wSex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wSex.lpszMenuName = NULL;
	wSex.lpszClassName = ClassName;
	wSex.hIconSm = LoadIcon(Application::GetInstance(), IDI_APPLICATION);
	




	if (!RegisterClassExW(&wSex))
	{
		auto er = GetLastError();

		Logger::log(L"Error register Class", LOGGER_ERROR);
		Logger::wait();
		abort();

	}

	
	m_Wnd = CreateWindowW(ClassName,
		ClassName,
		WS_OVERLAPPEDWINDOW,
		x, y, w, h,
		NULL,
		NULL,
		Application::GetInstance(),
		NULL);


	if (!m_Wnd)
	{
		Logger::log("Error create Window " + name);

		Logger::wait();
		
		abort();

		return;
	}


	SetWindowLongPtr(m_Wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));


	ShowWindow(m_Wnd, Application::CmdShow());
	UpdateWindow(m_Wnd);


	RECT rect2;
	GetWindowRect(m_Wnd, &rect2);

	leftX = rect2.left;
	topY = rect2.top;
	width = rect2.right - rect2.left;
	height = rect2.bottom - rect2.top;

	m_Close = false;


	Log("Success create window ", cstr(name), " " , icstr(width), "x", icstr(height));
}

void FWindowsWindow::Destroy()
{
	if (m_Close)
		return;


	onDestroyWindow.BroadCast();

	m_Close = true;
	DestroyWindow(m_Wnd);


	Log("Window has been Destroyed. - ", cstr(m_Name));



}

void FWindowsWindow::Update()
{
	if (!IsWindow(m_Wnd))
	{
		m_Close = true;

		return;
	}

	RECT rect; POINT p;

	GetWindowRect(m_Wnd, &rect);

	leftX = rect.left;
	topY = rect.top;


	if (!(width == rect.right - rect.left || height == rect.bottom - rect.top))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

	}


	


}

void FWindowsWindow::SetResolution(UINT w, UINT h)
{
	width = w;
	height = h;

	SetWindowPos(m_Wnd, NULL, leftX, topY, w, h, NULL);

}

void FWindowsWindow::SetPos(UINT x, UINT y)
{
	leftX = x;
	topY = y;

	SetWindowPos(m_Wnd, NULL, x, y, width, height, NULL);

}

void FWindowsWindow::SetResPos(UINT w, UINT h, UINT x, UINT y)
{
	leftX = x;
	topY = y;
	width = w;
	height = h;

	SetWindowPos(m_Wnd, NULL, x, y, w, h, NULL);
}

void FWindowsWindow::SetWindowTitle(std::string str)
{
	SetWindowText(m_Wnd, strw(str).c_str());
}

LRESULT FWindowsWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	FWindowsWindow* pThis = reinterpret_cast<FWindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	//GEngine.GetMouse()->SetWindow(hwnd);


	DirectX::Mouse::ProcessMessage(msg, wParam, lParam);




	switch (msg)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		break;


	//case WM_MOUSEMOVE:
	//{
	//	int xPos = GET_X_LPARAM(lParam);
	//	int yPos = GET_Y_LPARAM(lParam);

	//	POINT point;

	//	GetCursorPos(&point);

	//	SetCursorPos(pThis->GetWitdh() / 2, pThis->GetHeight() / 2);

	//	int xDiff = point.x - (pThis->GetWitdh() / 2);
	//	int yDiff = (point.y - (pThis->GetHeight() / 2));

	//	PrintLine(icstr(xDiff), "\t" , icstr(yDiff), "\n");

	//	pThis->MouseX = xDiff;
	//	pThis->MouseY = yDiff;
	//}
	//	break;

	//case WM_MOVE:
	//	break;
	///*case WM_PAINT:
	//	break;*/

	//case WM_SETCURSOR:
	//	SetCursor(NULL);
	//	break;
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		pThis->onResizeWindow.BroadCast(width, height);
	}
		break;

	case WM_CLOSE:
		pThis->Destroy();
		break;
	case WM_DESTROY:
		pThis->Destroy();
		break;
	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

FWindowsWindow* FWindowsWindowManager::CreateWindow(UINT Weight, UINT Height, std::string Name)
{
	FWindowsWindow* window = new FWindowsWindow(this, windows.size());

	window->Create(Weight, Height, 0, 0, Name + to_string(windows.size()), NULL);

	windows.push_back(window);


	return window;

}

void FWindowsWindowManager::Update()
{
	for (size_t i = 0; i < windows.size(); i++)
	{
		if (windows[i]->isClose())
		{
			Destroy(i);
		}
	}

	for (auto& i : windows)
	{
		i->Update();
	}

	PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);


	}

}

void FWindowsWindowManager::Destroy(UINT index)
{
	auto wnd = windows[index];

	wnd->Destroy();


	delete windows[index];

	windows.erase(windows.begin() + index);
}

void FWindowsWindowManager::Quit()
{
	for (size_t i = 0; i < windows.size(); i++)
	{
		Destroy(i);
	}
}

