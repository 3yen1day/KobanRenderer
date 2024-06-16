#pragma once
#include <intrin.h>
#include <cassert>
using namespace std;
namespace Koban {
	static class DebugLib {
	public:
		/// <summary>
		/// エラーを表示
		/// </summary>
		/// <param name="str"></param>
		static void error(const std::wstring& str) {
			//wprintf(L"%ls", str.data());
			MessageBox(0, LPWSTR(str.data()), NULL, MB_OK);
#ifdef _DEBUG
			__debugbreak();
#else
			assert(false);
#endif
		}

		/// <summary>
		/// エラーを表示
		/// </summary>
		/// <param name="str"></param>
		static void error(const std::string& str) {
			//wprintf(L"%ls", str.data());
			MessageBoxA(0, str.data(), NULL, MB_OK);
#ifdef _DEBUG
			__debugbreak();
#else
			assert(false);
#endif
		}
	};
}