#include <iostream>
#include <uge.h>
using namespace uge;

UGE* pUge = nullptr;

bool update();
bool frame();

int main() {

	pUge = ugeCreate(UGE_VERSION);

	//ÉèÖÃ²ÎÊý
	pUge->SetTitle("UGE Initiate - tut02");
	pUge->SetWindowed(true);
	pUge->SetScreen(800, 600);
	pUge->SetUpdateCallback(update);
	pUge->SetFrameCallback(frame);

	if (pUge->Initiate())
	{
		pUge->Start();
	}
	else {
		MessageBox(nullptr, pUge->GetErrMsg().c_str(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}

	pUge->Release();
	return 0;
}

bool update() {

	return true;
}

bool frame() {

	return true;
}
