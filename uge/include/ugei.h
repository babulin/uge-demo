#pragma once
#include "uge.h"
#include "ugedx9.h"
#include <string>

namespace uge {

	// 回调函数类型
	typedef bool (*ugeCallback)();

	// 引擎接口类
	class UGEI : public UGE {
	public:
		
		virtual void UGE_CALL SetFrameCallback(const ugeCallback value) override;	//帧回调
		virtual void UGE_CALL SetUpdateCallback(const ugeCallback value) override;	//数据更新
		virtual void UGE_CALL SetGame(Game* game) override;
		virtual void UGE_CALL SetScreen(int width, int height) override;			//屏幕宽高
		virtual void UGE_CALL SetWindowed(bool value) override;						//是否窗口模式
		virtual void UGE_CALL SetTitle(const char* title) override;					//窗口标题
		virtual bool UGE_CALL Initiate() override;									//初始化引擎
		virtual bool UGE_CALL Start() override;										//运行
		virtual void UGE_CALL Release() override;									//引擎释放
		virtual std::string GetErrMsg() override;

		// 成员函数
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static UGEI* Create();

	private:
		UGEI();
		~UGEI();
		void SetErrMsg(const char* error);
	private:
		HINSTANCE h_instance;			//当前实例
		HWND hwnd;						//当前窗口句柄
		HWND hwnd_parent;				//父-窗口句柄
		bool active;					//窗口是否激活
		std::string win_title;			//窗口标题
		bool windowed;					//窗口模式
		int screen_width;				//屏幕宽
		int screen_height;				//屏幕高
		RECT rect_windowed;				//窗口客户区矩形
		LONG style_windowed;			//窗口风格

		bool (*frame_func)();			//帧回调函数指针
		bool (*update_func)();			//更新回调函数指针
		Game* game;						//应用程序指针

		bool splash_screen_enabled;		//是否启用开场动画

		std::string err_msg;			//错误信息
	
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
		Graphics *pGraphics;
	};

	extern UGEI* pUge;
}