class Main
{
public:
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	void awake();
	void start();
	void update();
	void draw();
	void destroy();

private:
	HWND mHwnd;
	HINSTANCE* mpHInstance;
	std::unique_ptr<Koban::GlobalAccess> mpGlobalAccess;
};
