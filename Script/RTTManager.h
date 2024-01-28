#pragma once
#include "RenderObject.h"

// NormalTexture�����쐬���āA�����_�[�^�[�Q�b�g�ɂ���
// Shader���ł́A�����_�[�^�[�Q�b�g�ɏo��
// ���ۂ�SRV��ShaderResource�Ƃ��Ă��g������
// RTT�ɂ����Ă����AResource�ɂ��ݒ�ł���Ȃ炻������
// �ł��Ȃ��Ȃ�A��p�ӂ��āA���t������ւ���

namespace Koban {
	class RTTManager : public RenderObject {
	public:
		enum RTT_TYPE
		{
			COLOR,
			NORMAL,
			POSITION
		};

		RTTManager();
		~RTTManager() {};

		void destroy() override;

		/// <summary>
		/// RTT�����ɃV�[���֕`��
		/// </summary>
		void renderToScene();

		/// <summary>
		/// RTT��SRV���擾
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		ID3D11ShaderResourceView* const getRTTSRV(RTT_TYPE type);

		ID3D11DepthStencilView* const getDepthStensilSRV() { return mpDepthStencil_SRV; }

	private:
		// RTT, RTV
		ID3D11Texture2D* mpDepthStencil_Tex;
		ID3D11DepthStencilView* mpDepthStencil_SRV;

		ID3D11Texture2D* mpColor_Tex;
		ID3D11RenderTargetView* mpColor_RTV;
		ID3D11ShaderResourceView* mpColor_SRV;

		ID3D11Texture2D* mpNormal_Tex;
		ID3D11RenderTargetView* mpNormal_RTV;
		ID3D11ShaderResourceView* mpNormal_SRV;

		ID3D11Texture2D* mpPosition_Tex;
		ID3D11RenderTargetView* mpPosition_RTV;
		ID3D11ShaderResourceView* mpPosition_SRV;
	};
}