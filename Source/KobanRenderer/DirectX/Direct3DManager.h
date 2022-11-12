#pragma once
#include "DirectX/DirectX.h"

//=========================================
// Direct3DManager�N���X
// DirectX�܂��̐ݒ�
//=========================================
class Direct3DManager {
	//============================================
	// ���\�b�h
	//============================================
public:
	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static void CreateInstance()
	{
		DeleteInstance();

		mInstance = new Direct3DManager();
	}

	/// <summary>
	/// �C���X�^���X�폜
	/// </summary>
	static void DeleteInstance()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	static Direct3DManager& GetInstance()
	{
		return *mInstance;
	}

	//============================================
	// �t�B�[���h
	//============================================
public:
	//Direct3D�f�o�C�X
	ComPtr<ID3D11Device>	mDevice;
	//Direct3D�f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11DeviceContext>	mDeviceContext;
	//�X���b�v�`�F�C��
	ComPtr<IDXGISwapChain>	mSwapChain;
	//�o�b�N�o�b�t�@�̂q�s�r���[
	ComPtr<ID3D11RenderTargetView>	mBackBufferView;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="width">��ʂ̕�</param>
	/// <param name="height">��ʂ̍���</param>
	/// <returns></returns>
	bool Initialize(HWND hWnd, int width, int height);

	// 2D�`��p�V�F�[�_�\
	ComPtr<ID3D11VertexShader> mSpriteVS = nullptr; // ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> mSpritePS = nullptr; // �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout> mSpriteInputLayout = nullptr; // ���̓��C�A�E�g

private:
	static Direct3DManager* mInstance;
	Direct3DManager() {
	};


	// Direct3D�̗B��̃C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
#define D3D Direct3DManager::GetInstance()
};