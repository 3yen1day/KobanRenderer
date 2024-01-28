#pragma once

namespace Koban {
	class ShaderObject {
	public:
		ShaderObject() {};
		~ShaderObject() {};
		virtual void updateShader() {};
		virtual void destroy() {
			SAFE_RELEASE(mpVertexShader);
			SAFE_RELEASE(mpPixelShader);
		};

	protected:
		//Shader
		ID3D11VertexShader* mpVertexShader = NULL;
		ID3D11PixelShader* mpPixelShader = NULL;
	};
}