#include "RenderMode.h"

namespace Koban
{
	bool RenderMode::update()
	{
		if(mIsNeedInit)
		{
			mIsNeedInit = false;
			InitMode(mMode);
			return true;
		}
		return false;
	}

	void RenderMode::InitMode(MODE mode)
	{
		switch (mode)
		{
			case Koban::RenderMode::MODE::STANDARD:
				break;
			case Koban::RenderMode::MODE::BASE_COLOR:
				break;
			case Koban::RenderMode::MODE::NORMAL:
				break;
			case Koban::RenderMode::MODE::DEPTH:
				break;
			case Koban::RenderMode::MODE::POSITION:
				break;
			default:
				break;
		}

		mShaderFileName = L"Deferred.hlsl";
		mVSShaderName = L"VS_From_Tex";
		mPSShaderName = L"PS_From_Tex";
	}
}