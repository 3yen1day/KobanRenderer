class Main
{
public:
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	void Awake();
	void Loop();
	void start();
	void update();
	void draw();
	void destroy();

	HWND mHwnd;

private:
	std::unique_ptr<Koban::GlobalAccess> mpGlobalAccess;
};
