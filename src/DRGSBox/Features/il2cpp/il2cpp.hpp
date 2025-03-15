#pragma once

#include <windows.h>
#include "BaseModels/ModFeature.hpp"

#include "il2cpp-api-types.h"

#include <vector>

namespace il2cpp {
	static HMODULE handle = GetModuleHandle("GameAssembly.dll");
	
	// domain
	typedef Il2CppDomain*(*til2cpp_domain_get)();
	static til2cpp_domain_get il2cpp_domain_get = (til2cpp_domain_get)GetProcAddress(handle, "il2cpp_domain_get");

	typedef const Il2CppAssembly*(__thiscall* til2cpp_domain_assembly_open)(Il2CppDomain* domain, const char* name);
	static til2cpp_domain_assembly_open il2cpp_domain_assembly_open = (til2cpp_domain_assembly_open)GetProcAddress(handle, "il2cpp_domain_assembly_open");

	// thread
	typedef Il2CppThread*(__thiscall* til2cpp_thread_attach)(Il2CppDomain* domain);
	static til2cpp_thread_attach il2cpp_thread_attach = (til2cpp_thread_attach)GetProcAddress(handle, "il2cpp_thread_attach");

	// assembly
	typedef const Il2CppImage*(__thiscall* til2cpp_assembly_get_image)(const Il2CppAssembly* assembly);
	static til2cpp_assembly_get_image il2cpp_assembly_get_image = (til2cpp_assembly_get_image)GetProcAddress(handle, "il2cpp_assembly_get_image");

	// runtime
	typedef Il2CppObject*(__thiscall* til2cpp_runtime_invoke)(const MethodInfo* method, void* obj, void** params, Il2CppException** exc);
	static til2cpp_runtime_invoke il2cpp_runtime_invoke = (til2cpp_runtime_invoke)GetProcAddress(handle, "il2cpp_runtime_invoke");

	typedef void(__thiscall* til2cpp_runtime_class_init)(Il2CppClass* klass);
	static til2cpp_runtime_class_init il2cpp_runtime_class_init = (til2cpp_runtime_class_init)GetProcAddress(handle, "il2cpp_runtime_class_init");

	typedef void(__thiscall* til2cpp_runtime_object_init)(Il2CppObject* obj);
	static til2cpp_runtime_object_init il2cpp_runtime_object_init = (til2cpp_runtime_object_init)GetProcAddress(handle, "il2cpp_runtime_object_init");

	// class
	typedef Il2CppClass*(__thiscall* til2cpp_class_from_name)(const Il2CppImage* image, const char* namespaze, const char* name);
	static til2cpp_class_from_name il2cpp_class_from_name = (til2cpp_class_from_name)GetProcAddress(handle, "il2cpp_class_from_name");

	typedef const MethodInfo*(__thiscall* til2cpp_class_get_method_from_name)(Il2CppClass* klass, const char* name, int argsCount);
	static til2cpp_class_get_method_from_name il2cpp_class_get_method_from_name = (til2cpp_class_get_method_from_name)GetProcAddress(handle, "il2cpp_class_get_method_from_name");

	typedef const const char*(__thiscall* til2cpp_class_get_name)(Il2CppClass* klass);
	static til2cpp_class_get_name il2cpp_class_get_name = (til2cpp_class_get_name)GetProcAddress(handle, "il2cpp_class_get_name");

	typedef bool(__thiscall* til2cpp_class_is_inited)(const Il2CppClass* klass);
	static til2cpp_class_is_inited il2cpp_class_is_inited = (til2cpp_class_is_inited)GetProcAddress(handle, "il2cpp_class_is_inited");

	typedef FieldInfo*(__thiscall* til2cpp_class_get_field_from_name)(Il2CppClass* klass, const char* name);
	static til2cpp_class_get_field_from_name il2cpp_class_get_field_from_name = (til2cpp_class_get_field_from_name)GetProcAddress(handle, "il2cpp_class_get_field_from_name");

	// field
	
	typedef size_t(__thiscall* til2cpp_field_get_offset)(FieldInfo* field);
	static til2cpp_field_get_offset il2cpp_field_get_offset = (til2cpp_field_get_offset)GetProcAddress(handle, "il2cpp_field_get_offset");

	typedef void(__thiscall* til2cpp_field_get_value)(Il2CppObject* obj, FieldInfo* field, void* value);
	static til2cpp_field_get_value il2cpp_field_get_value = (til2cpp_field_get_value)GetProcAddress(handle, "il2cpp_field_get_value");

	typedef void(__thiscall* til2cpp_field_set_value)(Il2CppObject* obj, FieldInfo* field, void* value);
	static til2cpp_field_set_value il2cpp_field_set_value = (til2cpp_field_set_value)GetProcAddress(handle, "il2cpp_field_set_value");

	// method
	typedef Il2CppClass*(__thiscall* til2cpp_method_get_class)(const MethodInfo* method);
	static til2cpp_method_get_class il2cpp_method_get_class = (til2cpp_method_get_class)GetProcAddress(handle, "il2cpp_method_get_class");

	typedef uint32_t(__thiscall* til2cpp_method_get_param_count)(const MethodInfo* method);
	static til2cpp_method_get_param_count il2cpp_method_get_param_count = (til2cpp_method_get_param_count)GetProcAddress(handle, "il2cpp_method_get_param_count");

	// object
	typedef Il2CppObject*(__thiscall* til2cpp_object_new)(const Il2CppClass* klass);
	static til2cpp_object_new il2cpp_object_new = (til2cpp_object_new)GetProcAddress(handle, "il2cpp_object_new");

	typedef Il2CppClass*(__thiscall* til2cpp_object_get_class)(Il2CppObject* obj);
	static til2cpp_object_get_class il2cpp_object_get_class = (til2cpp_object_get_class)GetProcAddress(handle, "il2cpp_object_get_class");

	class Wrapper;
	static void OnUnityMainLoop();

	struct PendingInvoke {
		const MethodInfo* method;
		void* pThis;
		void** params;
		bool* isComplete;
		uintptr_t* container;
	};
	static std::vector<PendingInvoke> InvokeQueue;
}

class il2cpp::Wrapper : public ModFeature {
public:
	static Wrapper& Get();
	bool Init(uintptr_t moduleBase);
	bool Deinit();

	Il2CppClass* GetClass(const char* namespaze, const char* name);
	const MethodInfo* GetMethod(Il2CppClass* _class, const char* name, int argc);
	Il2CppObject* InvokeMethod(const MethodInfo* method, void* pThis, void** params);
	void AddToInvokeQueue(const MethodInfo* method, void* pThis, void** params, bool* isComplete, uintptr_t* container);
	void GetFieldValue(void* pThis, const char* name, void* value);
	void SetFieldValue(void* pThis, const char* name, void* value);

private:
	Wrapper() = default;
	~Wrapper() = default;
	Wrapper(const Wrapper&) = delete;
	Wrapper& operator=(const Wrapper&) = delete;

	bool isInit = false;

	Il2CppDomain* domain;
	Il2CppThread* thread;
	const Il2CppAssembly* assembly;
	const Il2CppImage* image;
};

