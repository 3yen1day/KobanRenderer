#include "GameSystemManager.h"
#include "../DirectX/Direct3DManager.h"
#include "../framework.h"
#include <comdef.h>

GameSystemManager* GameSystemManager::mInstance = nullptr;

void GameSystemManager::Initialize(){
}

void GameSystemManager::Execute(){
    // 画面を青色で塗りつぶす
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.mDeviceContext->ClearRenderTargetView(D3D.mBackBufferView.Get(), color);

    // 三角形描画
    {
        try {
            // 1頂点の形式
            struct VertexType {
                DirectX::XMFLOAT3 Pos; //座標
            };
            VertexType v[3] = {
                {{0,0,0}},
                {{1,-1,0}},
                {{-1,-1,0}}
            };

            //-----------------------------
            // 頂点バッファ作成
            //-----------------------------
            // 作成するバッファの仕様を決定する
            D3D11_BUFFER_DESC vbDesc = {};
            vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
            vbDesc.ByteWidth = sizeof(v);
            vbDesc.MiscFlags = 0;
            vbDesc.StructureByteStride = 0; //構造化バッファの場合、サイズ
            vbDesc.Usage = D3D11_USAGE_DEFAULT; //バッファの使用方法
            vbDesc.CPUAccessFlags = 0;

            //頂点バッファを作成する
            ComPtr<ID3D11Buffer> vb;
            D3D11_SUBRESOURCE_DATA initData = { &v[0], sizeof(v), 0 };	// 書き込むデータ
            D3D.mDevice->CreateBuffer(&vbDesc, &initData, &vb);

            //-----------------------------
            // 描画
            //-----------------------------
            //設定
            UINT stride = sizeof(VertexType);
            UINT offset = 0;
            D3D.mDeviceContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);//頂点バッファを描画で使えるようにする
            D3D.mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//プリミティブトポロジーのセット

            //シェーダーのセット
            D3D.mDeviceContext->VSSetShader(D3D.mSpriteVS.Get(), 0, 0);
            D3D.mDeviceContext->PSSetShader(D3D.mSpritePS.Get(), 0, 0);
            D3D.mDeviceContext->IASetInputLayout(D3D.mSpriteInputLayout.Get());
            //こんな感じでデバイスコンテキストに情報をセットしていく

            //デバイスコンテキストへの描画命令
            D3D.mDeviceContext->Draw(3, 0);
        }
        catch (_com_error &e) {
            printf("hoge");
        }
       
    }

    //バックバッファの内容を画面に表示
    D3D.mSwapChain->Present(1, 0);


}
