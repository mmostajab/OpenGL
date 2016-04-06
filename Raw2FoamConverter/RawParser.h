#pragma once
#include <fstream>
#include <string>

#include "Vector3.h"

enum eRawDataFormat
{
	RD_FORMAT_BYTE		= 0,
	RD_FORMAT_UCHAR		= 1,
	RD_FORMAT_HALF3		= 2,
	RD_FORMAT_HALF4		= 3,
	RD_FORMAT_FLOAT3	= 4,
	RD_FORMAT_FLOAT4	= 5
};

class CRawParser
{
private:
	std::string m_WorkingDirectory;
	std::string m_FileName;
	std::string m_DataFileName;
  std::string m_MeshFileName;

	CVector3 m_Resolution;
	CVector3 m_SliceThickness;

	std::ifstream m_File;
	eRawDataFormat m_Format;

	float m_TimeStep;
	int m_Step;
	int m_MinStep;
	int m_MaxStep;

	bool m_isTimeDependent;
	bool m_isVectorFieldData;

	
	unsigned int m_RequiredBufferSize;

public:
	CRawParser(void);

	void loadDataInfo(const char* _pFileName);
	void fillBuffer(void* _pBuffer, const int& _pIndex);

	unsigned int getRequiredBufferSize();
	
	unsigned int getWidth()
	{	return (unsigned int)m_Resolution.x;	}

	unsigned int getHeight()
	{	return (unsigned int)m_Resolution.y;	}

	unsigned int getDepth()
	{	return (unsigned int)m_Resolution.z;	}

	unsigned int getNBuffers()
	{	return (m_MaxStep - m_MinStep) / m_Step + 1;	}

	unsigned int getStep()
	{	return m_Step;				}

	CVector3 getSliceThickness()
	{	return m_SliceThickness;	}

	bool isTimeDependent()
	{	return m_isTimeDependent;	}
	
	void setWorkingDirectory(const char* _pWorkingDirector);

	bool isScalarData()
	{
		if(m_Format == RD_FORMAT_BYTE || m_Format == RD_FORMAT_UCHAR)
			return true;
		return false;
	}

	bool isVectorData()
	{
		if(m_Format == RD_FORMAT_FLOAT3 || m_Format == RD_FORMAT_FLOAT4 || m_Format == RD_FORMAT_HALF3 || m_Format == RD_FORMAT_HALF4)
			return true;
		return false;
	}

	static int getDataTypeSize(const eRawDataFormat& _pFormat);
	eRawDataFormat getDataType()
	{
		return m_Format;
	}

	~CRawParser(void);
};