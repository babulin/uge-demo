#include "hum.h"

namespace game {
	ugeLine line1, line2;

	Hum::Hum(UGE* pUge)
	{
		this->pUge = pUge;
	}

	void Hum::Load()
	{
		pUge->LoadWzl("data\\hum", 32, 4, &anima);
	}
	void Hum::Show()
	{
		line1 = { 512.0f ,0.0f,512.0f,768.0f,0xFFFFFFFF };
		line2 = { 0.0f ,384.0f,1024.0f,384.0f,0xFFFFFFFF };

		pUge->DxRenderQuad(&line1);
		pUge->DxRenderQuad(&line2);

		anima.x = 512.0f - 25.0f;
		anima.y = 384.0f - 22.0f;
		pUge->DxRenderQuad(&anima);
	}
}