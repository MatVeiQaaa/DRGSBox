#include "il2cpp.hpp"

#include "minhook/include/MinHook.h"

template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

il2cpp::Wrapper& il2cpp::Wrapper::Get() {
	static Wrapper instance;
	return instance;
}

typedef void(*tUnityMainLoop)();
tUnityMainLoop UnityMainLoop = nullptr;

static void il2cpp::OnUnityMainLoop() { // TODO: figure a dynamic way of finding a place to hook for portability.
	for (auto it = InvokeQueue.begin(); it != InvokeQueue.end(); it++) {
		Il2CppException* exception;
		Il2CppObject* object = il2cpp::il2cpp_runtime_invoke(it->method, it->pThis, it->params, &exception);
		if (it->container) *it->container = (uintptr_t)object;
		delete[](it->params);
		if (it->isComplete) *it->isComplete = true;
	}
	InvokeQueue.clear();
	return UnityMainLoop();
}

bool il2cpp::Wrapper::Init(uintptr_t moduleBase) {
	domain = il2cpp::il2cpp_domain_get();
	if (domain == nullptr) return false;
	thread = il2cpp::il2cpp_thread_attach(domain);
	if (thread == nullptr) return false;
	assembly = il2cpp::il2cpp_domain_assembly_open(domain, "Assembly-CSharp.dll");
	if (assembly == nullptr) return false;
	image = il2cpp::il2cpp_assembly_get_image(assembly);
	if (image == nullptr) return false;

	uintptr_t unityPlayer = (uintptr_t)GetModuleHandle("UnityPlayer.dll");
	UnityMainLoop = (tUnityMainLoop)(unityPlayer + 0x7FE3C0);

	if (MH_CreateHookEx((LPVOID)UnityMainLoop, &OnUnityMainLoop, &UnityMainLoop) != MH_OK) return false;

	if (MH_QueueEnableHook(MH_ALL_HOOKS) || MH_ApplyQueued() != MH_OK) return false;

	isInit = true;
	return true;
}

bool il2cpp::Wrapper::Deinit() {
	isInit = false;
	return true;
}

Il2CppClass* il2cpp::Wrapper::GetClass(const char* namespaze, const char* name) {
	if (!isInit) return nullptr;
	return il2cpp::il2cpp_class_from_name(image, namespaze, name);
}

const MethodInfo* il2cpp::Wrapper::GetMethod(Il2CppClass* _class, const char* name, int argc) {
	if (!isInit) return nullptr;
	return il2cpp::il2cpp_class_get_method_from_name(_class, name, argc);
}

Il2CppObject* il2cpp::Wrapper::InvokeMethod(const MethodInfo* method, void* pThis, void** params) {
	if (!isInit) return nullptr;
	Il2CppException* exception;
	Il2CppObject* object = il2cpp::il2cpp_runtime_invoke(method, pThis, params, &exception);
	if (exception) return nullptr;
	return object;
}

void il2cpp::Wrapper::AddToInvokeQueue(const MethodInfo* method, void* pThis, void** params, bool* isComplete, uintptr_t* container) {
	PendingInvoke invoke = { method, pThis, params, isComplete, container };
	InvokeQueue.push_back(invoke);
}

void il2cpp::Wrapper::GetFieldValue(void* pThis, const char* name, void* value) {
	Il2CppClass* _class = il2cpp_object_get_class((Il2CppObject*)pThis);
	FieldInfo* field = il2cpp_class_get_field_from_name(_class, name);
	il2cpp_field_get_value((Il2CppObject*)pThis, field, value);
}

void il2cpp::Wrapper::SetFieldValue(void* pThis, const char* name, void* value) {
	Il2CppClass* _class = il2cpp_object_get_class((Il2CppObject*)pThis);
	FieldInfo* field = il2cpp_class_get_field_from_name(_class, name);
	il2cpp_field_set_value((Il2CppObject*)pThis, field, value);
}