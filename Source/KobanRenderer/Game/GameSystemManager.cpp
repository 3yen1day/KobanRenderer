#include "GameSystemManager.h"
#include "../DirectX/Direct3DManager.h"
#include "../framework.h"
#include <comdef.h>

GameSystemManager* GameSystemManager::mInstance = nullptr;

void GameSystemManager::Initialize(){
}

void GameSystemManager::Execute(){
    // ��ʂ�F�œh��Ԃ�
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.mDeviceContext->ClearRenderTargetView(D3D.mBackBufferView.Get(), color);

    // �O�p�`�`��
    {
        try {
            // 1���_�̌`��
            struct VertexType {
                DirectX::XMFLOAT3 Pos; //���W
            };
            VertexType v[3] = {
                {{0,0,0}},
                {{1,-1,0}},
                {{-1,-1,0}}
            };

            //-----------------------------
            // ���_�o�b�t�@�쐬
            //-----------------------------
            // �쐬����o�b�t�@�̎d�l�����肷��
            D3D11_BUFFER_DESC vbDesc = {};
            vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
            vbDesc.ByteWidth = sizeof(v);
            vbDesc.MiscFlags = 0;
            vbDesc.StructureByteStride = 0; //�\�����o�b�t�@�̏ꍇ�A�T�C�Y
            vbDesc.Usage = D3D11_USAGE_DEFAULT; //�o�b�t�@�̎g�p���@
            vbDesc.CPUAccessFlags = 0;

            //���_�o�b�t�@���쐬����
            ComPtr<ID3D11Buffer> vb;
            D3D11_SUBRESOURCE_DATA initData = { &v[0], sizeof(v), 0 };	// �������ރf�[�^
            D3D.mDevice->CreateBuffer(&vbDesc, &initData, &vb);

            //-----------------------------
            // �`��
            //-----------------------------
            //�ݒ�
            UINT stride = sizeof(VertexType);
            UINT offset = 0;
            D3D.mDeviceContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);//���_�o�b�t�@��`��Ŏg����悤�ɂ���
            D3D.mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//�v���~�e�B�u�g�|���W�[�̃Z�b�g

            //�V�F�[�_�[�̃Z�b�g
            D3D.mDeviceContext->VSSetShader(D3D.mSpriteVS.Get(), 0, 0);
            D3D.mDeviceContext->PSSetShader(D3D.mSpritePS.Get(), 0, 0);
            D3D.mDeviceContext->IASetInputLayout(D3D.mSpriteInputLayout.Get());
            //����Ȋ����Ńf�o�C�X�R���e�L�X�g�ɏ����Z�b�g���Ă���

            //�f�o�C�X�R���e�L�X�g�ւ̕`�施��
            D3D.mDeviceContext->Draw(3, 0);
        }
        catch (_com_error &e) {
            printf("hoge");
        }
       
    }

    //�o�b�N�o�b�t�@�̓��e����ʂɕ\��
    D3D.mSwapChain->Present(1, 0);


}
