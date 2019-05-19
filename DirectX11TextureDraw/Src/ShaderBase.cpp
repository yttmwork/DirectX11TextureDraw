#include <stdio.h>
#include "ShaderBase.h"

bool ShaderBase::Load(const char* file_name)
{
	FILE *fp = nullptr;
	// 拡張子チェック
	if (strstr(file_name, ".cso") == nullptr)
	{
		return false;
	}

	fopen_s(&fp, file_name, "rb");
	if (fp == nullptr)
	{
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	m_Size = ftell(fp);
	if (m_Size == 0)
	{
		fclose(fp);
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	m_Data = new char[m_Size];
	fread_s(m_Data, m_Size, m_Size, 1, fp);
	fclose(fp);

	return true;
}
