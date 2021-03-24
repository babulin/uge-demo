/*-----------------------------------------------------------------------------
 * Unbelievable Game Engine 1.0
 * Copyright (C) 2021-2021, Ahlin Games
 * Maintained 2021-2021 by 787193801@qq.com (github @ahlin)
 * Github -- https://github.com/ahlin/uge | Discord -- http://ahlingames.com
 *
 * Old website: http://uge.ahlingames.com; Old forum: http://ahlingames.com
 *-----------------------------------------------------------------------------*/
#pragma once

#ifdef UGEDLL
#define UGE_EXPORT __declspec(dllexport)
#else
#define UGE_EXPORT __declspec(dllexport)
#endif // UGEDLL

#define UGE_CALL  __stdcall