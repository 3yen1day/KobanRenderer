#include "Input.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace Koban {
	Input::Input(const HINSTANCE& hInstance, const HWND& hWnd) {
		// DirectInput の初期化
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mpDirectInput, nullptr)))
			return;

		// キーボードの初期化
		if (FAILED(mpDirectInput->CreateDevice(GUID_SysKeyboard, &mpKeyboard, nullptr)))
			return;
		if (FAILED(mpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
			return;
		if (FAILED(mpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			return;

		// マウスの初期化
		if (FAILED(mpDirectInput->CreateDevice(GUID_SysMouse, &mpMouse, nullptr)))
			return;
		if (FAILED(mpMouse->SetDataFormat(&c_dfDIMouse)))
			return;
		if (FAILED(mpMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
			return;
	}

	Input::~Input() {
		if (mpKeyboard) {
			mpKeyboard->Unacquire();
			mpKeyboard->Release();
			mpKeyboard = nullptr;
		}
		if (mpMouse) {
			mpMouse->Unacquire();
			mpMouse->Release();
			mpMouse = nullptr;
		}
		if (mpDirectInput) {
			mpDirectInput->Release();
			mpDirectInput = nullptr;
		}
	}

	void Input::update() {
		// キーボードの状態取得
		if (mpKeyboard) {
			mpKeyboard->Acquire();
			mpKeyboard->GetDeviceState(sizeof(mKeyState), (LPVOID)&mKeyState);
		}
		// マウスの状態取得
		if (mpMouse) {
			mpMouse->Acquire();
			mpMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);
		}
	}

	bool Input::isKeyPressed(BYTE key) {
		return (mKeyState[key] & 0x80) != 0;
	}
}