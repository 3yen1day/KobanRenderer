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
			UV,
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
		MODE getMode()
		{
			return mMode;
		};
		const std::wstring& getShaderFileName_VS()
		{
			return mShaderFileName_VS;
		};
		const std::wstring& getShaderFileName_PS()
		{
			return mShaderFileName_PS;
		};

	private:
		void InitMode(MODE mode);

		bool mIsNeedInit = true;
		MODE mMode = MODE::STANDARD;
		std::wstring mShaderFileName_VS;
		std::wstring mShaderFileName_PS;
	};
}