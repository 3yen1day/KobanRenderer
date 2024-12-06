#pragma once

/// <summary>
/// GBuffer�̏���BuckBuffer�Ƀ����_�����O����
/// </summary>
namespace Koban {
	class GBufferToBackBuffer
	{
	public:
		GBufferToBackBuffer();
		~GBufferToBackBuffer() {};
		void start();
		void draw();
		void destroy();

	private:
		//vertexBuffer�p�\����
		struct MY_VERTEX
		{
			D3DXVECTOR3 vPos;
			D3DXVECTOR3 vNorm;
			D3DXVECTOR2 vTex;
		};

		//ConstantBuffer�i���W�֌W�j�p�\����
		struct CBUFER_COORD
		{
			D3DMATRIX WMat;
			D3DMATRIX wvpMat;
			D3DXVECTOR4 lightPos[MAX_Light];
			D3DXVECTOR4 eyePos;
		};

		//ConstantBuffer
		//screen�`��p���_�o�b�t�@
		ID3D11Buffer* mpVertexBuffer; //unique_ptr�ɂ���ƃf�X�g���N�g�ł̃q�[�v�̉���ŗ�O
		std::unique_ptr <ID3D11Buffer> mpConstantBuffer;

		ID3D11VertexShader* mpVertexShader = NULL;
		ID3D11PixelShader* mpPixelShader = NULL;

		const std::wstring mShaderFileName = L"Deferred.hlsl";
		const std::wstring mVSShaderName = L"VS_From_Tex";
		const std::wstring mPSShaderName = L"PS_From_Tex";
	};
}