#pragma once
#include <dinput.h>
#include <iostream>
#include "../Core/Module.h"

namespace Koban {
	class Input:ModuleManager {
	public:
		Input() {};
		Input(const HINSTANCE& hInstance, const HWND& hWnd);
		~Input();

		void start() override {};
		void update() override;
		void draw() override {};
		void destroy() override {};

		bool isKeyPressed(BYTE key);
		int getMouseDeltaX() { return mMouseState.lX; }
		int getMouseDeltaY() { return mMouseState.lY; }
		bool isMouseButtonDown(int button) { return (mMouseState.rgbButtons[button] & 0x80) != 0; }

	private:
		LPDIRECTINPUT8 mpDirectInput = nullptr;
		LPDIRECTINPUTDEVICE8 mpKeyboard = nullptr;
		LPDIRECTINPUTDEVICE8 mpMouse = nullptr;
		DIMOUSESTATE mMouseState;
		BYTE mKeyState[256];
	};
}