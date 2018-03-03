#pragma once
#include "defs.h"

UNICODE_STRING StrToUnicode(wchar_t * str, int lenght)
{
	UNICODE_STRING retStr;

	retStr.Buffer = str;
	retStr.Length = lenght;
	retStr.MaximumLength = retStr.Length + 2;

	return retStr;
}

void catnull(wchar_t * dest, wchar_t * src)
{
	int dest_len = wcslen(dest);
	int src_len = wcslen(src);

	dest[dest_len] = '\0';

	//dest_len++;

	for (int i = 0; i < src_len; i++)
	{
		int app_pos = i + 1;
		dest[dest_len + app_pos] = src[i];
	}

}
void addnull(wchar_t *& src)
{
	int src_len = wcslen(src);
	int app_pos;

	wchar_t * buff = (wchar_t*)malloc(src_len + 1);
	buff[0] = '\0';

	for (int i = 0; i < src_len; i++)
	{
		if (i == 0)
			app_pos = 1;
		else
			app_pos = i + 1;

		buff[app_pos] = src[i];
	}

	src = buff;
}

int countnull(wchar_t * src)
{
	int i = 0;
	int scan_pos = 0;

	while (true)
	{
		if (scan_pos == 0)
			scan_pos = 1;
		else
			scan_pos = i + 1;
		i++;


		if (src[scan_pos] == '\0')
			return i;

	}
	return 0;
}