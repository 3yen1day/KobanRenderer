#pragma once
namespace Koban {
	class ModuleManager {
	public:
		ModuleManager() {};
		~ModuleManager() {};

		/// <summary>�R���X�g���N�^��ɌĂ΂��</summary>
		virtual void start() = 0;
		/// <summary>�X�V</summary>
		virtual void update() = 0;
		/// <summary>�`��</summary>
		virtual void draw() = 0;
		/// <summary>�j��</summary>
		virtual void destroy() = 0;
	};
}