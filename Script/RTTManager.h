#pragma once
#include "Include/RenderInclude.h"
#include "RenderObject.h"

// NormalTexture�����쐬���āA�����_�[�^�[�Q�b�g�ɂ���
// Shader���ł́A�����_�[�^�[�Q�b�g�ɏo��
// ���ۂ�SRV��ShaderResource�Ƃ��Ă��g������
// RTT�ɂ����Ă����AResource�ɂ��ݒ�ł���Ȃ炻������
// �ł��Ȃ��Ȃ�A��p�ӂ��āA���t������ւ���

namespace Koban {
	class RTTManager : public RenderObject {
	public:
		RTTManager();
		~RTTManager();

		void destroy() override;
	private:
		// RTT
		ID3D11Texture2D* mpColorTexture;
		ID3D11RenderTargetView* mpColor_RTV;
		ID3D11ShaderResourceView* mpColor_SRV;

		ID3D11Texture2D* mpNormalTexture;
		ID3D11RenderTargetView* mpNormal_RTV;
		ID3D11ShaderResourceView* mpNormal_SRV;

		ID3D11Texture2D* mpPositionTexture;
		ID3D11RenderTargetView* mpPosition_RTV;
		ID3D11ShaderResourceView* mpPosition_SRV;
	};
}