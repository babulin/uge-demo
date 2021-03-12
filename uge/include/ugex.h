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

	auto app = CreateGame();
	pUge->SetGame(app);

	if (!app->Initiate())
	{
		return 0;
	}

	if (pUge->Initiate())
	{
		pUge->Start();
	}
	else
	{
		MessageBox(nullptr, pUge->GetErrMsg().c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}

	pUge->Release();

	return false;
}

