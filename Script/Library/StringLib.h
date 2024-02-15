#pragma once
using namespace std;

namespace Koban {
	/// <summary>
	/// string—pƒ‰ƒCƒuƒ‰ƒŠ
	/// </summary>
	static class StringLib
	{
	public:
		StringLib() {};
		~StringLib() {};

		/// <summary>
		/// wstr¨str
		/// </summary>
		/// <param name="str"></param>
		/// <param name="dest"></param>
		static std::unique_ptr<std::string> wstr2str(const std::wstring& str) {
			auto mbs = new char[str.length() * MB_CUR_MAX + 1];
			wcstombs(mbs, str.c_str(), str.length() * MB_CUR_MAX + 1);
			string ret = mbs;
			delete[] mbs;
			return std::make_unique<std::string>(ret);
		};

		/// <summary>
		/// str¨wstr
		/// </summary>
		/// <param name="str"></param>
		/// <param name="dest"></param>
		static std::unique_ptr<std::wstring> str2wstr(const std::string& src) {
			wchar_t* wcs = new wchar_t[src.length() + 1];
			mbstowcs(wcs, src.c_str(), src.length() + 1);
			wstring ret = wcs;
			delete[] wcs;
			return std::make_unique<std::wstring>(ret);
		}

		/// <summary>
		/// wstring¨LPWSTR
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		static LPWSTR wstring2LPWSTR(const std::wstring& str) {
			return (LPWSTR)str.data();
		}
	};
}