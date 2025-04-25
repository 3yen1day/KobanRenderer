#pragma once
#include "../Core/Component.h"
namespace Koban {
	class Transform : public Component {
	public:
		Transform() {};
		~Transform() {};

		void start() override {};
		void update() override {};
		void draw() override {};
		void destroy() override {};

		D3DXMATRIX getWorldMatrix() {
			D3DXMATRIX matScale, matRot, matTrans;
			//TODO:í¥éGÇ»ÇÃÇ≈ÅAå„Ç≈íºÇ∑
			D3DXMatrixScaling(&matScale, mScale.x, mScale.y, mScale.z);
			D3DXMatrixRotationQuaternion(&matRot, &mRotation);
			D3DXMatrixTranslation(&matTrans, mPosition.x, mPosition.y, mPosition.z);
			return matScale * matRot * matTrans;
		};
		D3DXVECTOR3 mPosition = { 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 mScale = { 1.0f, 1.0f, 1.0f };
		D3DXQUATERNION mRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}