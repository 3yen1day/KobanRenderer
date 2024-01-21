#include "Render.h"

class Main
{
public:
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	void Awake();
	void Start();
	void Loop();
	void update();
	void draw();
	void destroy();

	HWND mHwnd;

private:
	Koban::Render* mpRender;
};
