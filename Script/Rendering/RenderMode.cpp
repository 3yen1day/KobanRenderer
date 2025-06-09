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
		mShaderFileName_VS = L"Shader/VS_GBufferToBackBuffer.hlsl";
		mShaderFileName_PS = L"Shader/PS_GBufferToBackBuffer.hlsl";

		switch (mode)
		{
			case Koban::RenderMode::MODE::STANDARD:
				break;
			case Koban::RenderMode::MODE::BASE_COLOR:
				break;
			case Koban::RenderMode::MODE::NORMAL:
				mShaderFileName_PS = L"Shader/PS_RenderNormal.hlsl";
				break;
			case Koban::RenderMode::MODE::DEPTH:
				mShaderFileName_PS = L"Shader/PS_RenderDepth.hlsl";
				return;
			case Koban::RenderMode::MODE::UV:
				mShaderFileName_PS = L"Shader/PS_RenderUV.hlsl";
				return;
			case Koban::RenderMode::MODE::POSITION:
				mShaderFileName_PS = L"Shader/PS_RenderWorldPos.hlsl";
				break;

			default:
				break;
		}
	}
}