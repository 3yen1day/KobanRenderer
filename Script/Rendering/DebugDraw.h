#pragma once
namespace Koban
{
	class DebugDraw
	{
	public:
		DebugDraw() = default;
		~DebugDraw() = default;
		void start();
		void draw();
		void destroy();

	private:
		ID3D11Buffer* createBuffer();

		//Shader
		ID3D11VertexShader* mpVertexShader = NULL;
		ID3D11PixelShader* mpPixelShader = NULL;
		ID3D11InputLayout* mpVertexLayout;

		//コンスタントバッファ
		ID3D11Buffer* mpConstantBuffer_Transform;
	};
}


