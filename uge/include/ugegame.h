#pragma once
#include "ugedefs.h"

namespace uge {
	
	class UGE;

	// ”¶”√≥Ã–Ú
	class UGE_EXPORT ugeGame {
	protected:
		UGE* pUge;
	public:
		ugeGame(UGE* pUge);
		virtual ~ugeGame();
		virtual bool Initiate();
		virtual bool Update();
		virtual bool Show();
	};
}