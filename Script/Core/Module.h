#pragma once
namespace Koban {
	class Module {
	public:
		Module() {};
		~Module() {};

		/// <summary>コンストラクタ後に呼ばれる</summary>
		virtual void awake() = 0;
		/// <summary>更新</summary>
		virtual void update() = 0;
		/// <summary>描画</summary>
		virtual void draw() = 0;
		/// <summary>破棄</summary>
		virtual void destroy() = 0;
	};
}