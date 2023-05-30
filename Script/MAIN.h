#include"Render.h"

class Main
{
public:
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	void Awake();
	void Start();
	void Loop();
	void Update();
	void Draw();
	void Destroy();

	HWND mHwnd;

private:
	Render* mpRender;
};
