#include "ugei.h"

const char* WINDOW_CLASS_NAME = "UGE__WNDCLASS";

namespace uge {

	UGEI* pUGE = nullptr;

	//+------------------------
	//| 导出函数
	//+------------------------
	UGE* UGE_CALL ugeCreate(const int ver)
	{
		if (ver == UGE_VERSION)
		{
			return static_cast<UGE *>(UGEI::Create());
		}

		return nullptr;
	}

	//+------------------------
	//| 单例模式获取一个引擎对象
	//+------------------------
	UGEI* UGEI::Create()
	{
		if (!pUGE)
		{
			pUGE = new UGEI();
		}

		return pUGE;
	}

	//+------------------------
	//| 引擎构造函数
	//+------------------------
	UGEI::UGEI()
	{
		//窗口信息
		hwnd = nullptr;
		hwnd_parent = nullptr;
		h_instance = GetModuleHandle(nullptr);
		windowed = true;				//窗口模式
		active = false;					//是否被激活
		win_title = "UGE";				//窗口名称
		rect_windowed = {};				//窗口矩形
		screen_width = 800;				//窗口宽
		screen_height = 600;			//窗口高
		rect_windowed = {};				//窗口矩形
		style_windowed = 0;				//窗口类型

		//资源
		_uge_resource = new UgeResource();

		//游戏参数
		frame_func = nullptr;			//帧回调函数
		update_func = nullptr;			//更新回调
		splash_screen_enabled = true;	//启动动画
		game = nullptr;					//应用程序

		//time
		_game_time_s = 0.0f;
		_pre_time_ms = 0;
		_fps_time_ms = 0;
		_delay_time_ms = 0;
		_delay_time_s = 0.0f;
		_fixed_delay_ms = 0;			//固定延时
		_run_fps = 0;
		_fps = 0;


		//dx
		_z_buffer = false;
		_d3d = nullptr;
		_d3d_device = nullptr;
		_proj_matrix = {};
		_view_matrix = {};
		_vertex_buffer = nullptr;
		_index_buffer = nullptr;
		_vert_array = nullptr;

		_n_prim = 0;
	}

	//+------------------------
	//| 设置屏幕模式
	//+------------------------
	void UGE_CALL UGEI::SetWindowed(bool value)
	{
		this->windowed = value;
	}

	//+------------------------
	//| 设置屏幕参数
	//+------------------------
	void UGE_CALL UGEI::SetScreen(int width, int height)
	{
		this->screen_width = width;
		this->screen_height = height;
	}

	//+------------------------
	//| 窗口标题
	//+------------------------
	void UGE_CALL UGEI::SetTitle(const char* title)
	{
		this->win_title = title;
	}

	//+------------------------
	//| 渲染帧回调
	//+------------------------
	void UGEI::SetFrameCallback(const ugeCallback value)
	{
		this->frame_func = value;
	}

	//+------------------------
	//| 渲染帧回调
	//+------------------------
	void UGEI::SetUpdateCallback(const ugeCallback value)
	{
		this->update_func = value;
	}

	//+------------------------
	//| 应用程序
	//+------------------------
	void UGE_CALL UGEI::SetGame(Game* game)
	{
		this->game = game;
	}

	//+------------------------
	//| 初始化引擎
	//+------------------------
	bool UGE_CALL UGEI::Initiate()
	{
		if (!game && (!frame_func || !update_func))
		{
			_SetErrMsg("引擎初始化:游戏应用或回调帧函数未设置");
			return false;
		}

		OSVERSIONINFO os_ver;
		SYSTEMTIME time;
		MEMORYSTATUS mem_st;
		WNDCLASS winclass = {};

		Log("+========[Your Game Engine]========+");
		Log("UGE 引擎初始化...");
		// 引擎版本
		Log("UGE 版本: %X.%X",UGE_VERSION >> 8,UGE_VERSION & 0xFF);
		// 获取系统时间
		GetLocalTime(&time);
		Log("日期:%d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		// 获取系统版本
		os_ver.dwOSVersionInfoSize = sizeof(os_ver);
		GetVersionEx(&os_ver);
		Log("系统: Windows %ld.%ld.%ld", os_ver.dwMajorVersion, os_ver.dwMinorVersion, os_ver.dwBuildNumber);
		// 获取内存信息
		GlobalMemoryStatus(&mem_st);
		Log("内存: %ldK 总, %ldK 空闲", mem_st.dwTotalPhys / 1024L,mem_st.dwAvailPhys / 1024L);

		// 注册窗口类
		winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		winclass.lpfnWndProc = UGEI::WindowProc;
		winclass.cbClsExtra = 0;
		winclass.cbWndExtra = 0;
		winclass.hInstance = h_instance;
		winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		winclass.lpszMenuName = nullptr;
		winclass.lpszClassName = WINDOW_CLASS_NAME;

		if (!RegisterClass(&winclass)) {
			_SetErrMsg("引擎初始化:注册窗口类失败!");
			return false;
		}

		if (screen_width == 0 || screen_height == 0)
		{
			_SetErrMsg("引擎初始化:窗口的宽高未设置\n");
			return false;
		}

		// 获取客户区域实际大小
		const auto scr_width = GetSystemMetrics(SM_CXSCREEN);
		const auto scr_height = GetSystemMetrics(SM_CYSCREEN);
		rect_windowed.left = (scr_width - screen_width) / 2;
		rect_windowed.top = (scr_height - screen_height) / 2;
		rect_windowed.right = rect_windowed.left + screen_width;
		rect_windowed.bottom = rect_windowed.top + screen_height;

		style_windowed = WS_BORDER | WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		AdjustWindowRect(&rect_windowed, style_windowed, false);

		if (windowed)
		{
			hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, win_title.c_str(), style_windowed,
				rect_windowed.left, rect_windowed.top,
				rect_windowed.right - rect_windowed.left,
				rect_windowed.bottom - rect_windowed.top,
				hwnd_parent, nullptr, h_instance, nullptr);
		}
		else {

		}

		if (!hwnd) {
			_SetErrMsg("引擎初始化:创建窗口失败");
			return false;
		}

		// 初始化子系统
		// 游戏时间
		// 输入控制
		// 图像渲染
		if (! _DxInit())
		{
			return false;
		}
		// 音频控制

		Log("初始化完成...");
		Log("+========[Your Game Engine]========+");
		ShowWindow(hwnd, SW_SHOW);

		// 时间
		_pre_time_ms = _fps_time_ms = timeGetTime();

		// 显示启动动画
		if (splash_screen_enabled)
		{

		}

		return true;
	}

	//+------------------------
	//| 运行引擎
	//+------------------------
	bool UGE_CALL UGEI::Start()
	{
		MSG msg;

		// 窗口状态
		active = true;

		// 主循环
		for (;;)
		{
			if (!hwnd_parent) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						break;
					}
					// TranslateMessage(&msg);
					DispatchMessage(&msg);
					continue;
				}
			}

			// 更新鼠标

			if (active)
			{
				do {
					//两帧间隔1毫秒
					_delay_time_ms = timeGetTime() - _pre_time_ms;
				} while (_delay_time_ms < 1);


				//是否大于固定延时
				if (_delay_time_ms >= _fixed_delay_ms)
				{
					// 时长
					_delay_time_s = _delay_time_ms / 1000.0f;

					// 时长超过0.2
					if (_fixed_delay_ms > 0) {
						_delay_time_s = _fixed_delay_ms / 1000.0f;
					}
					else {
						_delay_time_s = 0.001f;
					}

					// 累计时长
					_game_time_s += _delay_time_s;

					// 新时长
					_pre_time_ms = timeGetTime();
					if (_pre_time_ms - _fps_time_ms <= 1000) {
						//计算1s内的帧数
						_run_fps++;
					}
					else {
						//1秒累计结束
						_fps = _run_fps;
						_run_fps = 0;
						_fps_time_ms = _pre_time_ms;

						// 设置标题
						static char title[255] = {};
						sprintf_s(title, "%s FPS: %d", win_title.c_str(), _fps);
						SetWindowText(hwnd, title);
					}

					//开始
					_BeginScene();

					// 更新回调函数
					if (game)
					{
						if (!game->Update()) {
							break;
						}

						// 回调帧函数
						if (!game->Show()) {
							break;
						}
					}
					else {
						if (!update_func()) {
							break;
						}

						// 回调帧函数
						if (!frame_func()) {
							break;
						}
					}

					//结束翻转
					_EndScene();

					if (hwnd_parent) {
						break;
					}
				}
				else 
				{
					if (_fixed_delay_ms && _delay_time_ms + 3 < _fixed_delay_ms) {
						Sleep(1);
					}
				}
			}
			else 
			{
				Sleep(1);
			}
		}

		active = false;

		return true;
	}

	//+------------------------
	//| 系统消息回调
	//+------------------------
    LRESULT CALLBACK UGEI::WindowProc(HWND hwnd, const UINT msg, WPARAM wparam, LPARAM lparam) {
        switch (msg) {
			case WM_CREATE:
				return FALSE;
			case WM_CLOSE:
				DestroyWindow(hwnd);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				return FALSE;
			case WM_PAINT:
				break;
			default:
				break;
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

	//+------------------------
	//| 引擎析构函数
	//+------------------------
	UGEI::~UGEI()
	{
		if (_d3d)
			_d3d->Release();

		if (_d3d_device)
			_d3d_device->Release();

		if (_vertex_buffer)
			_vertex_buffer->Release();

		if (_index_buffer)
			_index_buffer->Release();

		if (_uge_resource)
		{
			delete _uge_resource;
			_uge_resource = nullptr;
		}

		Log("UGEI析构");
	}

	//+------------------------
	//| 释放资源
	//+------------------------
	void UGE_CALL UGEI::Release()
	{
		if (pUGE)
		{
			delete pUGE;
			pUGE = nullptr;
		}

		Log("释放完成");
	}
}