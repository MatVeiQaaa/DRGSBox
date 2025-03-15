#include "BaseModels/ModFeature.hpp"

#include "il2cpp/il2cpp-api-types.h"

#include <unordered_map>
#include <string>

class Player : public ModFeature {
public:
	bool Init(uintptr_t moduleBase);
	bool Deinit();

	void Update();

	bool ToggleInvulnerability();
	bool GetInvulnerability();
	bool SetInvulnerability(bool value);
	bool IncreaseLevel();
	bool GiveRandomWeapon();
	bool GiveStarterWeapon();
	bool KYS();
	int GetWeaponCount();
	void OpenCheatScreen();
	void OpenShopScreen();
	void SetDropPodTimeout(int seconds);

private:
#pragma pack(push, 1)
	struct SkillCollectionManager {
		char void1[0x18];
		uintptr_t weaponDataCollection;
	};
	struct PlayerStruct {
		bool OnMovingChanged;
		char void1[0x27];
		bool IsVulnerable;
		char void2[0x7];
		uintptr_t baseStats;
		uintptr_t stats;
		char void3[0x268];
		uintptr_t runSettingsManager;
		char void4[0x30];
		SkillCollectionManager* skillCollectionManager;
	};
#pragma pack(pop)
	uintptr_t moduleBase = 0;
	std::unordered_map<std::string, const MethodInfo*> methods;
	Il2CppClass* cObjectiveProgress;
	Il2CppClass* cPlayer;
	Il2CppClass* cRunSettingsManager;
	Il2CppClass* cWeaponSkillDataCollection;
	Il2CppClass* cSkillCollectionManager;
	Il2CppClass* cUICheatScreen;
	Il2CppClass* cUIShopScreen;
	PlayerStruct* player;
	uintptr_t gameController;
};