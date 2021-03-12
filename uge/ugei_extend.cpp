#include "ugei.h"

namespace uge {

	extern UGEI* pUGE;

	//+------------------------
	//| 设置错误信息
	//+------------------------
	void UGEI::SetErrMsg(const char* error)
	{
		Log("%s\n", error);
		err_msg = error;
	}

	//+------------------------
	//| 设置错误信息
	//+------------------------
	std::string UGEI::GetErrMsg()
	{
		return err_msg;
	}
}