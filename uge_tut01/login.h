#pragma once
#include <uge.h>
using namespace uge;
namespace game {
	class Login : public ugeGame {
		bool kDownSpace = false;
	public:
		Login(UGE* pUge) :ugeGame(pUge) {}
		// Í¨¹ý Game ¼Ì³Ð
		virtual bool Initiate() override;
		virtual bool Update() override;
		virtual bool Show() override;
	};
}