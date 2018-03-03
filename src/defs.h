#pragma once
#include <Windows.h>
#include <iostream>
#ifndef NT_SUCCESS
#define NT_SUCCESS(x) ((x)>=0)
#define STATUS_SUCCESS ((NTSTATUS)0)
#endif

#define OBJ_CASE_INSENSITIVE 64L

typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;


typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES,*POBJECT_ATTRIBUTES;

typedef NTSTATUS(__stdcall *PROC_NtOpenKey)(
	PHANDLE KeyHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes
	);

typedef VOID(__stdcall *PROC_RtlInitUnicodeString)(
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	);
typedef NTSTATUS(__stdcall *PROC_NtCreateKey)(
	PHANDLE KeyHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	ULONG TitleIndex,
	PUNICODE_STRING Class,
	ULONG CreateOptions,
	PULONG Disposition
	);
typedef NTSTATUS(__stdcall * PROC_NtDeleteKey)(
	HANDLE KeyHandle
);
typedef NTSTATUS(__stdcall *PROC_NtSetValueKey)(
	HANDLE KeyHandle,
	PUNICODE_STRING ValueName,
	ULONG TitleIndex,
	ULONG Type,
	PVOID Data,
	ULONG DataSize
	);
typedef NTSTATUS(__stdcall *PROC_NtClose)(
	HANDLE Handle
	);


PROC_NtDeleteKey _NtDeleteKey;
PROC_NtCreateKey _NtCreateKey;
PROC_NtOpenKey _NtOpenKey;
PROC_NtSetValueKey _NtSetValueKey;
PROC_RtlInitUnicodeString _RtlInitUnicodeString;
PROC_NtClose _NtClose;