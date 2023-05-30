#pragma once
#include "TestShader.h"

//�}�e���A���\����
struct TestMaterial
{
public:
	std::wstring mName;
	std::wstring mTextureName;//�e�N�X�`���[�t�@�C����
	std::wstring shaderPath;
	D3DXVECTOR4 mKa;//�A���r�G���g
	D3DXVECTOR4 mKd;//�f�B�t���[�Y
	D3DXVECTOR4 mKs;//�X�y�L�����[
	uint32_t mFaceNum;//���̃}�e���A���ł���|���S����
	ID3D11ShaderResourceView* mpTexture;
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@
	/// </summary>
	ID3D11Buffer* mpIndexBuffer;

	TestMaterial():
		mName{L""},
		mTextureName{L""},
		shaderPath{L""},
		mFaceNum{0},
		mpTexture{nullptr},
		mpIndexBuffer{nullptr}
	{
	}

	~TestMaterial()
	{
		SAFE_RELEASE(mpTexture);
		SAFE_RELEASE(mpIndexBuffer);
	}
};


class TestMesh
{
public:
	TestMesh();
	~TestMesh();
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT LoadMaterialFromFile(std::wstring FileName);
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	/// <summary>
	/// ���_��
	/// </summary>
	uint32_t mVertNum;
	/// <summary>
	/// �|���S����
	/// </summary>
	uint32_t mFaceNum;
	/// <summary>
	/// ���_�o�b�t�@
	/// </summary>
	ID3D11Buffer* mpVertexBuffer;
	/// <summary>
	/// Shader��Dic
	/// </summary>
	std::unordered_map<std::wstring, TestShader> mShaderDic;
	/// <summary>
	/// Material��Dic
	/// �\���I�ɂ�Material��Shader�����`�����z�����AShader���ɏ������s�����ق��������������񗥔w���B��U�����Ă����B
	/// </summary>
	std::unordered_map<std::wstring, std::list<TestMaterial*>*> mMaterialDic;
	/// <summary>
	/// �T���v���[�X�e�[�g
	/// </summary>
	ID3D11SamplerState* mpSampleLinear;

private:
	//���_�̍\����
	struct MY_VERTEX
	{
		D3DXVECTOR3 mPos;
		D3DXVECTOR3 mNorm;
		D3DXVECTOR2 mUV;
	};

	// �g�����X�t�H�[�����
	D3DXVECTOR3 mPos;
	float mYaw, mPitch, mRoll, mScale;

	/// <summary>
	/// ���f���̃��\�[�X�p�X
	/// </summary>
	std::wstring mModelPath;
	/// <summary>
	/// �f�o�C�X
	/// </summary>
	ID3D11Device* mpDevice;
	/// <summary>
	/// �f�o�C�X�R���e�L�X�g
	/// </summary>
	ID3D11DeviceContext* mpDeviceContext;

	/// <summary>
	/// �t�@�C������Mesh��ǂݍ���
	/// </summary>
	/// <param name="FileName"></param>
	/// <returns></returns>
	HRESULT LoadResources(std::wstring FileName);
};
