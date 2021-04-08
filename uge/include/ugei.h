#pragma once
#include "uge.h"
#include "ugeapi.h"
#include <string>
#include <queue>

namespace uge {

	// 顶点结构体
	struct ugeVertex {
		float x, y; // screen position
		float z; // Z-buffer depth 0..1
		//float rhw;
		uint32_t col; // color
		float tx, ty; // texture coordinates
	};

	//纹理结构
	struct ugeQuad {
		UTEXTURE tex;
		ugeVertex v[4];
		ugeBlendMode blend;
	};

	// UGE 图源类型常量(顶点数量)
	enum {
		UGEPRIM_LINES = 2,
		UGEPRIM_TRIPLES = 3,
		UGEPRIM_QUADS = 4,
	};

	//d3d定义
	const int D3DFVF_UGEVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	const int VERTEX_BUFFER_SIZE = 40000;

	// 引擎实现接口类
	class UGEI : public UGE {
	public:
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) override;	//帧回调
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) override;	//数据更新
		virtual void UGE_CALL SetGame(ugeGame* game) override;
		virtual void UGE_CALL SetScreen(int width, int height) override;			//屏幕宽高
		virtual void UGE_CALL GetScreen(int* width, int* height) override;
		virtual void UGE_CALL SetWindowed(bool value) override;						//是否窗口模式
		virtual void UGE_CALL SetTitle(const char* title) override;					//窗口标题
		virtual bool UGE_CALL Initiate() override;									//初始化引擎
		virtual bool UGE_CALL Start() override;										//运行
		virtual void UGE_CALL Release() override;									//引擎释放
		virtual std::string GetErrMsg() override;

		//提供给wzl.dll
		virtual UTEXTURE UGE_CALL CreateWzlTexture(int width, int height, int pixelFormat, const RGBQUAD palette[256], byte* dstBuffer) override;
		virtual void UGE_CALL ReleaseWzlTexture(UTEXTURE tex) override;

		//渲染
		virtual bool LoadTexture(const char* filename, bool bMipmap = false) override;
		virtual void DxRenderQuad(ugeImage* image, bool fillMode = false) override;
		virtual void DxRenderQuad(ugeAnimation* animation, bool fillMode = false) override;
		virtual void DxRenderQuad(ugeLine* line) override;

		//按键
		virtual void GetMousePos(float* x, float* y) override;
		virtual bool KeyDown(const int key) override;
		virtual bool KeyUp(const int key) override;
		virtual bool KeyState(const int key) override;

		// 成员函数
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static UGEI* Create();

	private:
		UGEI();
		~UGEI();
		void _SetErrMsg(const char* error);

		//键盘事件
		void _InputInit();
		void _SetMousePos(const float x, const float y);
		void _UpdateMouse();
		void _KeyboardEvents(const int type,const int key, const int scan, int flags, const int x,const int y);
		void _ClearQueue();

		//directX9
		bool _DxInit();
		bool _InitResource();									//初始化资源
		void _BeginScene();
		void _EndScene();
		void _Clear(const ugeColor32 color);					//清屏
		void _SetProjectionMatrix();							//设置矩阵
		const char* _FormatStr(const D3DFORMAT fmt);			//获取FMT格式文案

	private:
		// 其他
		std::string err_msg;			//错误信息

		// 窗口信息
		HWND hwnd;						//当前窗口句柄
		HWND hwnd_parent;				//父-窗口句柄
		HINSTANCE h_instance;			//当前实例
		bool windowed;					//窗口模式
		bool active;					//窗口是否激活
		std::string win_title;			//窗口标题
		int screen_width;				//屏幕宽
		int screen_height;				//屏幕高
		RECT rect_windowed;				//窗口客户区矩形
		LONG style_windowed;			//窗口风格

		//键盘
		int _v_key;						//当前按下的键
		int _input_char;				//按下字符
		int _zpos;						//滚轮位置
		float _xpos;					//鼠标x
		float _ypos;					//鼠标y
		bool _mouse_over;				//鼠标是否经过
		bool _is_captured;				//是否压住一个键

		struct KeyStates {
			bool pressed : 1;
			bool released : 1;
		};
		KeyStates _key_table[256];				//键盘码对应状态
		std::queue<ugeInputEvent> _kbev_queue;	//键盘事件队列

		// Resoure

		// 游戏参数
		bool (*frame_func)();			//帧回调函数指针
		bool (*update_func)();			//更新回调函数指针
		bool splash_screen_enabled;		//是否启用开场动画
		ugeGame* game;						//应用程序指针
	
		// Timer
		float	 _game_time_s;			//游戏运行时间(s)
		uint32_t _pre_time_ms;			//前一帧时间(ms)
		uint32_t _fps_time_ms;			//计算1s的帧时间(ms)
		uint32_t _delay_time_ms;		//两帧间隔时间(ms)
		float	 _delay_time_s;			//两帧间隔时间(s)
		uint32_t _fixed_delay_ms;		//固定延时(ms)
		uint32_t _run_fps;				//1秒内计算中的fps
		uint32_t _fps;					//前1秒的帧率

		// Graphics
		bool _z_buffer;					//是否开启z缓冲
		UgeIDirect3D* _d3d;				//d3d对象
		UgeDevice* _d3d_device;			//d3d设备
		D3DXMATRIX _proj_matrix;		//投影矩阵
		D3DXMATRIX _view_matrix;		//视口矩阵
		UgeVertexBuffer* _vertex_buffer;//顶点缓存
		UgeIndexBuffer* _index_buffer;	//索引缓存
		ugeVertex* _vert_array;			//顶点缓冲数组

		int _n_prim;					//当前图源数
	};

	extern UGEI* pUgei;
}