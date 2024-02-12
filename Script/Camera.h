#pragma once
#include "Object.h"

namespace Koban {

	class Camera : public Object
	{
	public:
		Camera();
		~Camera();

		void update() override;

		/// <summary>
		/// ���W�擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getPosition(){
			return mPosition;
		};

		/// <summary>
		/// ViewMatrix�擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getViewMat() {
			return mViewMat;
		};

		/// <summary>
		/// ProjectionMatrix�擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getProjMat() {
			return mProjMat;
		};

	private:
		D3DXMATRIX mPosition;
		D3DXMATRIX mViewMat;
		D3DXMATRIX mProjMat;
	};
}