#pragma once
namespace Koban {
	class Module {
	public:
		Module() {};
		~Module() {};

		/// <summary>�R���X�g���N�^��ɌĂ΂��</summary>
		virtual void awake() = 0;
		/// <summary>�X�V</summary>
		virtual void update() = 0;
		/// <summary>�`��</summary>
		virtual void draw() = 0;
		/// <summary>�j��</summary>
		virtual void destroy() = 0;
	};
}