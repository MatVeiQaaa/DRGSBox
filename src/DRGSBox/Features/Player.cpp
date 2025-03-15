#include "Features/Player.hpp"
#include "Helpers/Helpers.hpp"

#include "il2cpp/il2cpp.hpp"

#include <iostream>
#include <thread>

bool Player::Init(uintptr_t moduleBase) {
	Player::moduleBase = moduleBase;
	cPlayer = il2cpp::Wrapper::Get().GetClass("", "Player");
	cRunSettingsManager = il2cpp::Wrapper::Get().GetClass("", "RunSettingsManager");
	cWeaponSkillDataCollection = il2cpp::Wrapper::Get().GetClass("", "WeaponSkillDataCollection");
	cSkillCollectionManager = il2cpp::Wrapper::Get().GetClass("", "SkillCollectionManager");
	cUICheatScreen = il2cpp::Wrapper::Get().GetClass("DRS.UI", "UICheatScreen");
	cUIShopScreen = il2cpp::Wrapper::Get().GetClass("DRS.UI", "UIShopScreen");

	methods["LevelUpByArtifact"] = il2cpp::Wrapper::Get().GetMethod(cPlayer, "LevelUpByArtifact", 0);
	methods["Die"] = il2cpp::Wrapper::Get().GetMethod(cPlayer, "Die", 2);
	methods["EquipNewWeapon"] = il2cpp::Wrapper::Get().GetMethod(cPlayer, "EquipNewWeapon", 2);

	methods["GetStarterWeapon"] = il2cpp::Wrapper::Get().GetMethod(cRunSettingsManager , "GetStarterWeapon", 0);

	methods["GetRandom"] = il2cpp::Wrapper::Get().GetMethod(cWeaponSkillDataCollection, "GetRandom", 0);

	methods["UnlockWeapon"] = il2cpp::Wrapper::Get().GetMethod(cSkillCollectionManager, "UnlockWeapon", 2);

	methods["CheatScreenInject"] = il2cpp::Wrapper::Get().GetMethod(cUICheatScreen, "Inject", 2);
	methods["CheatScreenStart"] = il2cpp::Wrapper::Get().GetMethod(cUICheatScreen, "Start", 0);
	methods["CheatScreenShowPopup"] = il2cpp::Wrapper::Get().GetMethod(cUICheatScreen, "ShowPopup", 0);
	methods["CheatScreenCtor"] = il2cpp::Wrapper::Get().GetMethod(cUICheatScreen, ".ctor", 0);

	methods["ShopScreenInject"] = il2cpp::Wrapper::Get().GetMethod(cUIShopScreen, "Inject", 5);
	methods["ShopScreenStart"] = il2cpp::Wrapper::Get().GetMethod(cUIShopScreen, "Start", 0);
	methods["ShopScreenShow"] = il2cpp::Wrapper::Get().GetMethod(cUIShopScreen, "Show", 2);
	methods["ShopScreenCtor"] = il2cpp::Wrapper::Get().GetMethod(cUIShopScreen, ".ctor", 0);

	cObjectiveProgress = il2cpp::Wrapper::Get().GetClass("", "ObjectiveProgress");
	Player::gameController = mem::FindDMAAddy((uintptr_t)cObjectiveProgress + 0xB8, { 0x10, 0x20, 0x10, 0x28, 0x0 });
	std::cout << "GameController: " << gameController << std::endl;
	il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "player", &player);
	std::cout << "Player: " << player << std::endl;

	return true;
}

void Player::Update() {
	Player::gameController = mem::FindDMAAddy((uintptr_t)cObjectiveProgress + 0xB8, { 0x10, 0x20, 0x10, 0x28, 0x0 });
	il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "player", &player);
}

bool Player::Deinit() {
	return true;
}

bool Player::ToggleInvulnerability() {
	if (!player) return false;

	bool value;
	il2cpp::Wrapper::Get().GetFieldValue(player, "IsInvulnerable", &value);
	value = !value;
	il2cpp::Wrapper::Get().SetFieldValue(player, "IsInvulnerable", &value);
	return true;
}

bool Player::GetInvulnerability() {
	if (!player) return false;
	bool value;
	il2cpp::Wrapper::Get().GetFieldValue(player, "IsInvulnerable", &value);
	return value;
}

bool Player::SetInvulnerability(bool value) {
	if (!player) return false;

	il2cpp::Wrapper::Get().SetFieldValue(player, "IsInvulnerable", &value);
	return true;
}

bool Player::IncreaseLevel() {
	if (!player) return false;
	il2cpp::Wrapper::Get().AddToInvokeQueue(methods["LevelUpByArtifact"], (void*)player, nullptr, nullptr, nullptr);
	return true;
}

bool Player::GiveRandomWeapon() {
	std::thread thread([this]() {
		uintptr_t weapon = 0;
		{
			bool isComplete = false;
			uintptr_t container;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["GetRandom"], (void*)player->skillCollectionManager->weaponDataCollection, nullptr, &isComplete, &container);
			while (!isComplete) Sleep(1);
			weapon = container;
		}
		if (!weapon) return false;

		{
			bool isComplete = false;
			bool isGrantedByArtifact = true;
			void** params = new void* [2];
			params[0] = (void*)weapon;
			params[1] = &isGrantedByArtifact;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["EquipNewWeapon"], (void*)player, params, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
	});
	thread.detach();
	return true;
}

bool Player::GiveStarterWeapon() {
	std::thread thread([this]() {
		uintptr_t weapon = 0;
		{
			void* runSettingsManager;
			il2cpp::Wrapper::Get().GetFieldValue(player, "runSettingsManager", &runSettingsManager);

			bool isComplete = false;
			uintptr_t container;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["GetStarterWeapon"], runSettingsManager, nullptr, &isComplete, &container);
			while (!isComplete) Sleep(1);
			weapon = container;
		}
		if (!weapon) return false;

		{
			bool isComplete = false;
			bool isGrantedByArtifact = true;
			void** params = new void* [2];
			params[0] = (void*)weapon;
			params[1] = &isGrantedByArtifact;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["EquipNewWeapon"], (void*)player, params, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
	});
	thread.detach();
	return true;
}

bool Player::KYS() {
	std::thread thread([this]() {
		unsigned int damageType = 0;
		int overdamage = 0;
		bool isComplete = false;
		void** params = new void* [2];
		params[0] = &damageType;
		params[1] = &overdamage;
		il2cpp::Wrapper::Get().AddToInvokeQueue(methods["Die"], player, params, &isComplete, nullptr);
		while (!isComplete) Sleep(1);
	});
	thread.detach();
	return true;
}

int Player::GetWeaponCount() {
	typedef int(__thiscall* tGetWeaponCount)(PlayerStruct* player);
	tGetWeaponCount GetWeaponCount = (tGetWeaponCount)(moduleBase + 0x611D00);
	int val = GetWeaponCount(player);
	std::cout << val << std::endl;
	return val;
}

void Player::SetDropPodTimeout(int seconds) {
	void* dropPod;
	il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "dropPod", &dropPod);
	il2cpp::Wrapper::Get().SetFieldValue((void*)dropPod, "secondsToTimeOut", &seconds);
}

void Player::OpenCheatScreen() {
	std::thread thread([this]() {
		void* pThis = il2cpp::il2cpp_object_new(cUICheatScreen);
		{
			bool isComplete = false;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["CheatScreenCtor"], pThis, nullptr, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		{
			bool isComplete = false;
			void** params = new void* [2];
			params[0] = (void*)gameController;
			params[1] = player->skillCollectionManager;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["CheatScreenInject"], pThis, params, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		{
			bool isComplete = false;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["CheatScreenStart"], pThis, nullptr, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		{
			bool isComplete = false;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["CheatScreenShowPopup"], pThis, nullptr, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		});
	thread.detach();
}

void Player::OpenShopScreen() {
	std::thread thread([this]() {
		void* pThis = il2cpp::il2cpp_object_new(cUIShopScreen);
		{
			bool isComplete = false;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["ShopScreenCtor"], pThis, nullptr, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		{
			uintptr_t dropRateHandler;
			il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "dropRateHandler", &dropRateHandler);
			uintptr_t audioManager;
			il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "audioManager", &audioManager);
			uintptr_t wallet;
			il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "wallet", &wallet);
			uintptr_t runSettingsManager;
			il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "runSettingsManager", &runSettingsManager);

			bool isComplete = false;
			void** params = new void*[5];
			params[0] = (void*)dropRateHandler;
			params[1] = player;
			params[2] = (void*)audioManager;
			params[3] = (void*)wallet;
			params[4] = (void*)runSettingsManager;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["ShopScreenInject"], pThis, params, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		{
			bool isComplete = false;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["ShopScreenStart"], pThis, nullptr, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		{
			uintptr_t skillCollectionManager;
			il2cpp::Wrapper::Get().GetFieldValue((void*)gameController, "skillCollectionManager", &skillCollectionManager);

			bool isComplete = false;
			unsigned int skillTypes[] = { 0, 1, 2, 3, 4 };
			void** params = new void* [2];
			params[0] = (void*)skillCollectionManager;
			params[1] = skillTypes;
			il2cpp::Wrapper::Get().AddToInvokeQueue(methods["ShopScreenShow"], pThis, params, &isComplete, nullptr);
			while (!isComplete) Sleep(1);
		}
		});
	thread.detach();
}