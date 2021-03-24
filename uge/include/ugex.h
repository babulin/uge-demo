#pragma once
#include <uge.h>
using namespace uge;

bool Start();

int main(int argc, char** argv)
{
	Start();
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nstr, int nCmd) 
{
	Start();
	return 0;
}

bool Start() {
	// 获取引擎对象
	UGE* pUge = ugeCreate(UGE_VERSION);

	// 回调
	auto app = gameCreate(pUge);

	// 设置引擎启动类
	pUge->SetGame(app);

	// 引擎初始化
	if (pUge->Initiate() && app->Initiate())
	{
		// 引擎启动
		pUge->Start();
	}
	else
	{
		MessageBox(nullptr, pUge->GetErrMsg().c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}
	pUge->Release();
	return false;
}

