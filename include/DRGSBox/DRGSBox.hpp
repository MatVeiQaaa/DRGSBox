#include "BaseModels/ModBody.hpp"
#include "BaseModels/ImGuiMenu.hpp"

#include "../DRGSBox/Features/Player.hpp"

class DRGSBoxMenu : public ImGuiMenu {
public:
	void Loop();
	void MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
};

class DRGSBox : public ModBody {
public:

	static DRGSBox& Get();
	bool Hook();
	bool Unhook();

	uintptr_t gameAssembly = 0;

	DRGSBoxMenu mMenu;

	Player player;
private:
	DRGSBox() = default;
	~DRGSBox() = default;
	DRGSBox(const DRGSBox&) = delete;
	DRGSBox& operator=(const DRGSBox&) = delete;
};