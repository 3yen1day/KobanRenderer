#pragma once
namespace Koban
{
	class RenderMode
	{
	public:
		enum class MODE
		{
			STANDARD,
			BASE_COLOR,
			NORMAL,
			DEPTH,
			POSITION
		};

		RenderMode() {};
		~RenderMode() {};

		bool update();
		void setMode(MODE mode) 
		{
			mMode = mode;
			mIsNeedInit = true;
		};
		const std::wstring& getShaderFileName()
		{
			return mShaderFileName;
		};
		const std::wstring& getVSShaderName()
		{
			return mVSShaderName;
		};
		const std::wstring& getPSShaderName()
		{
			return mPSShaderName;
		};

	private:
		void InitMode(MODE mode);

		bool mIsNeedInit = false;
		MODE mMode = MODE::STANDARD;
		std::wstring mShaderFileName = NULL;
		std::wstring mVSShaderName = NULL;
		std::wstring mPSShaderName = NULL;
	};
}