#pragma once
#include <uge.h>
using namespace uge;

UGE* pUge = nullptr;

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
	pUge = ugeCreate(UGE_VERSION);

	auto app = gameCreate();
	pUge->SetGame(app);

	if (!app->Config())
	{
		return 0;
	}

	if (pUge->Initiate() && app->Initiate())
	{
		pUge->Start();
	}
	else
	{
		MessageBox(nullptr, pUge->GetErrMsg().c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}

	delete app;
	pUge->Release();

	return false;
}

