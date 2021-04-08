#include "ugei.h"

namespace uge {

	//+------------------------
	//| ¼üÅÌÊÂ¼þ
	//+------------------------
	void UGEI::_InputInit() {
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hwnd, &pt);
		_xpos = static_cast<float>(pt.x);
		_ypos = static_cast<float>(pt.y);

		memset(&_key_table, 0, sizeof(_key_table));
	}

	void UGEI::_SetMousePos(const float x, const float y) {
		POINT pt;
		pt.x = static_cast<long>(x);
		pt.y = static_cast<long>(y);
		ClientToScreen(hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void UGEI::GetMousePos(float* x, float* y) {
		*x = _xpos;
		*y = _ypos;
	}

	bool UGEI::KeyDown(const int key) {
		return _key_table[key].pressed;
	}

	bool UGEI::KeyUp(const int key) {
		return _key_table[key].released;
	}	
	
	bool UGEI::KeyState(const int key) {
		return ((GetKeyState(key) & 0x8000) != 0);
	}

	void UGEI::_UpdateMouse() {
		POINT pt;
		RECT rc;

		GetCursorPos(&pt);
		GetClientRect(hwnd, &rc);
		MapWindowPoints(hwnd, nullptr, reinterpret_cast<LPPOINT>(&rc), 2);

		if (_is_captured || (PtInRect(&rc, pt) && WindowFromPoint(pt) == hwnd)) {
			_mouse_over = true;
		}
		else {
			_mouse_over = false;
		}
	}

	void UGEI::_KeyboardEvents(const int type, const int key, const int scan, int flags, const int x, const int y) {
		unsigned char kbstate[256]{};
		POINT pt;
		ugeInputEvent new_event(type, 0);
		pt.x = x;
		pt.y = y;
		GetKeyboardState(kbstate);

		if (INPUT_KEYDOWN == type)
		{
			if ((flags & UGEINP_REPEAT) == 0) {
				_key_table[key].pressed = true;
			}
			ToAscii(key, scan, kbstate, reinterpret_cast<uint16_t*>(&new_event.chr), 0);
		}

		if (type == INPUT_KEYUP) {
			_key_table[key].released = true;
			ToAscii(key, scan, kbstate, reinterpret_cast<uint16_t*>(&new_event.chr), 0);
		}

		if (type == INPUT_MOUSEWHEEL)
		{
			new_event.key = 0;
			new_event.wheel = key;
			ScreenToClient(hwnd, &pt);
		}
		else
		{
			new_event.key = key;
			new_event.wheel = 0;
		}

		if (type == INPUT_MBUTTONDOWN) {
			_key_table[key].pressed = true;
			SetCapture(hwnd);
			_is_captured = true;
		}

		if (type == INPUT_MBUTTONUP) {
			_key_table[key].released = true;
			ReleaseCapture();
			_SetMousePos(_xpos, _ypos);
			pt.x = static_cast<int>(_xpos);
			pt.y = static_cast<int>(_ypos);
			_is_captured = false;
		}

		if (kbstate[VK_SHIFT] & 0x80) {
			flags |= UGEINP_SHIFT;
		}
		if (kbstate[VK_CONTROL] & 0x80) {
			flags |= UGEINP_CTRL;
		}
		if (kbstate[VK_MENU] & 0x80) {
			flags |= UGEINP_ALT;
		}
		if (kbstate[VK_CAPITAL] & 0x1) {
			flags |= UGEINP_CAPSLOCK;
		}
		if (kbstate[VK_SCROLL] & 0x1) {
			flags |= UGEINP_SCROLLLOCK;
		}
		if (kbstate[VK_NUMLOCK] & 0x1) {
			flags |= UGEINP_NUMLOCK;
		}
		new_event.flags = flags;

		if (pt.x == -1) {
			new_event.x = _xpos;
			new_event.y = _ypos;
		}
		else {
			if (pt.x < 0) {
				pt.x = 0;
			}
			if (pt.y < 0) {
				pt.y = 0;
			}
			if (pt.x >= screen_width) {
				pt.x = screen_width - 1;
			}
			if (pt.y >= screen_height) {
				pt.y = screen_height - 1;
			}

			new_event.x = static_cast<float>(pt.x);
			new_event.y = static_cast<float>(pt.y);
		}

		if (new_event.type == INPUT_KEYDOWN
			|| new_event.type == INPUT_MBUTTONDOWN) {
			_v_key = new_event.key;
			_input_char = new_event.chr;
		}
		else if (new_event.type == INPUT_MOUSEMOVE) {
			_xpos = new_event.x;
			_ypos = new_event.y;
		}
		else if (new_event.type == INPUT_MOUSEWHEEL) {
			_zpos += new_event.wheel;
		}

		_kbev_queue.push(new_event);
	}

	void UGEI::_ClearQueue() {
		while (!_kbev_queue.empty()) { _kbev_queue.pop(); }
		memset(&_key_table, 0, sizeof(_key_table));
		_v_key = 0;
		_input_char = 0;
		_zpos = 0;
	}
}