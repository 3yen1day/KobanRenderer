#pragma once
namespace Koban {
	class ModuleManager {
	public:
		ModuleManager() {};
		~ModuleManager() {};

		/// <summary>コンストラクタ後に呼ばれる</summary>
		virtual void start() = 0;
		/// <summary>更新</summary>
		virtual void update() = 0;
		/// <summary>描画</summary>
		virtual void draw() = 0;
		/// <summary>破棄</summary>
		virtual void destroy() = 0;
	};
}