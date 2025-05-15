#include "Transform.h"
#include "../../lib/imgui/imgui.h"

namespace Koban {
	void Transform::drawUI() {
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::InputFloat3("Position", (float*)(mPosition));
			if(ImGui::InputFloat3("Rotation", (float*)(&mRotaionDegree)))
			{
				mRotation = DegreesToQuaternion(mRotaionDegree);
			}
			ImGui::InputFloat3("Scale", (float*)(mScale));
		}
	}

	void Transform::setRotation(const D3DXQUATERNION& rotation)
	{
		D3DXQUATERNION normalizedRot;
		D3DXQuaternionNormalize(&normalizedRot, &rotation);
		mRotation = normalizedRot;
		mRotaionDegree = QuaternionToDegree(mRotation);
	}

	D3DXQUATERNION Transform::DegreesToQuaternion(const D3DXVECTOR3& degree)
	{
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(degree.x), D3DXToRadian(degree.y), D3DXToRadian(degree.z));
		return q;
	};

	D3DXVECTOR3 Transform::QuaternionToDegree(const D3DXQUATERNION& q)
	{
		float pitch = atan2f(2.0f * (q.w * q.x + q.y * q.z), 1.0f - 2.0f * (q.x * q.x + q.y * q.y));
		float yaw = asinf(2.0f * (q.w * q.y - q.z * q.x)); // -90ìx Å` 90ìx
		float roll = atan2f(2.0f * (q.w * q.z + q.x * q.y), 1.0f - 2.0f * (q.y * q.y + q.z * q.z));

		return D3DXVECTOR3(D3DXToDegree(pitch), D3DXToDegree(yaw), D3DXToDegree(roll));
	};
}