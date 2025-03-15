#include "DRGSBox/DRGSBox.hpp"
#include "ImGuiInjector/ImGuiInjector.hpp"
#include "imgui/imgui.h"
#include "Helpers/Helpers.hpp"

#include "Features/il2cpp/il2cpp.hpp"

#pragma comment(lib, "ImGuiInjector.lib")
#pragma comment(lib, "BaseModels.lib")

DRGSBox& DRGSBox::Get() {
	static DRGSBox instance;
	return instance;
}

bool DRGSBox::Hook() {
	mLogger.SetPath("./DRGSBox.log");

	ImGuiInjector::Get().AddMenu(&(DRGSBox::mMenu));

	DRGSBox::mModuleBase = (uintptr_t)GetModuleHandle(NULL);
	DRGSBox::gameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

	il2cpp::Wrapper::Get().Init(mModuleBase);
	player.Init(gameAssembly);

	return true;
}

bool DRGSBox::Unhook() {
	return true;
}

int dropPodTimeoutSeconds = 30;

void DRGSBoxMenu::Loop() {
	ImGui::Begin("DRGSBox", &(DRGSBoxMenu::mIsOpen));
	ImGui::Text("DRGSBox menu");
	DRGSBox& mainMod = DRGSBox::Get();
	mainMod.player.Update();
	bool isInvulnerable = mainMod.player.GetInvulnerability();
	ImGui::Checkbox("Toggle Invulnerability", &isInvulnerable);
	mainMod.player.SetInvulnerability(isInvulnerable);
	
	ImGui::InputInt("Drop pod timeout seconds", &dropPodTimeoutSeconds);
	
	if (ImGui::Button("Set drop pod timeout")) {
		mainMod.player.SetDropPodTimeout(dropPodTimeoutSeconds);
	}
	if (ImGui::Button("Increase player level")) {
		mainMod.player.IncreaseLevel();
	}
	if (ImGui::Button("Give random weapon")) {
		mainMod.player.GiveRandomWeapon();
	}
	if (ImGui::Button("Give starter weapon")) {
		mainMod.player.GiveStarterWeapon();
	}
	if (ImGui::Button("Kill Yourself Now")) {
		mainMod.player.KYS();
	}
	if (ImGui::Button("GetWeaponCount")) {
		mainMod.player.GetWeaponCount();
	}
	/*if (ImGui::Button("OpenCheatScreen")) {
		mainMod.player.OpenCheatScreen();
	}
	if (ImGui::Button("OpenShopScreen")) {
		mainMod.player.OpenShopScreen();
	}*/
	ImGui::End();
}

void DRGSBoxMenu::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_INSERT: {
			DRGSBoxMenu::ToggleOpen();
			break;
		}
		}
		break;
	}
	}
}