#include "RawParser.h"


enum eRawParseReadingState
{
	RPR_STATE_OBJECTFILENAME,
	RPR_STATE_RESOLUTION,
	RPR_STATE_SLICETHICKNESS,
	RPR_STATE_FORMAT
};

CRawParser::CRawParser(void): m_FileName(0), m_MaxStep(-1), m_MinStep(-1), m_Step(1), m_isTimeDependent(false), m_isVectorFieldData(false), m_WorkingDirectory(0), m_MeshFileName(0), m_TimeStep(1.0f)
{
}

void CRawParser::setWorkingDirectory(const char* _pWorkingDirectory)
{
	m_WorkingDirectory = _pWorkingDirectory;
}

void CRawParser::loadDataInfo(const char* _pFileName)
{
	m_FileName = _pFileName;

  m_isTimeDependent = false;
	m_MinStep = m_MaxStep = 0;
	m_Step = 1;

  std::string path = m_WorkingDirectory + m_FileName;
	
	char Buffer[100];
	m_File.open(path);

	while(!m_File.eof())
	{
		m_File.getline(Buffer, 100);
		
		char* tok = 0;
		const char Delim[] = " :";
    char* context;
		
		if(strlen(Buffer) > 0)
		tok = strtok_s(Buffer, Delim, &context);

		if(tok != NULL)
		{
			if(strcmp(tok, "ObjectFileName") == 0)
			{
				tok = strtok_s(NULL, Delim, &context);
				m_DataFileName = tok;
			}
			else if(strcmp(tok, "Resolution") == 0)
			{
				tok = strtok_s(NULL, Delim, &context);
				m_Resolution.x = atof(tok);

				tok = strtok_s(NULL, Delim, &context);
				m_Resolution.y = atof(tok);

				tok = strtok_s(NULL, Delim, &context);
				m_Resolution.z = atof(tok);

			}
			else if(strcmp(tok, "SliceThickness") == 0)
			{
				tok = strtok_s(NULL, Delim, &context);
				m_SliceThickness.x = (float)atof(tok);

				tok = strtok_s(NULL, Delim, &context);
				m_SliceThickness.y = (float)atof(tok);

				tok = strtok_s(NULL, Delim, &context);
				m_SliceThickness.z = (float)atof(tok);
			}
			else if(strcmp(tok, "Format") == 0)
			{
				tok = strtok_s(NULL, Delim, &context);
				if(strcmp(tok, "UCHAR") == 0)
				{
					m_Format = RD_FORMAT_UCHAR;
				}
				else if(strcmp(tok, "BYTE") == 0)
				{
					m_Format = RD_FORMAT_BYTE;
				}
				else if(strcmp(tok, "half3") == 0)
				{
					m_Format = RD_FORMAT_HALF3;
					m_isVectorFieldData = true;
				}
				else if(strcmp(tok, "half4") == 0)
				{
					m_Format = RD_FORMAT_HALF4;
					m_isVectorFieldData = true;
				}
				else if(strcmp(tok, "float3") == 0)
				{
					m_isVectorFieldData = true;
					m_Format = RD_FORMAT_FLOAT3;
				}
				else if(strcmp(tok, "float4") == 0)
				{
					m_isVectorFieldData = true;
					m_Format = RD_FORMAT_FLOAT4;
				}
			}
			else if(strcmp(tok, "ObjectIndices") == 0)
			{
				m_isTimeDependent = true;

				tok = strtok_s(NULL, Delim, &context);
				m_MinStep = atoi(tok);

				tok = strtok_s(NULL, Delim, &context);
				m_MaxStep = atoi(tok);

				tok = strtok_s(NULL, Delim, &context);
				if(tok != 0)
				{
					m_Step = atoi(tok);
				}

			}
			else if(strcmp(tok, "TimeStep") == 0)
			{
				tok = strtok_s(NULL, Delim, &context);
				m_TimeStep = atoi(tok);
			}
			else if((strcmp(tok, "MeshFileName") == 0))
			{
				tok = strtok_s(NULL, Delim, &context);
        m_MeshFileName = tok;
			}
			else if((strcmp(tok, "Timestep") == 0))
			{
				tok = strtok_s(NULL, Delim, &context);
				m_TimeStep = atof(tok);
			}
		}

		while(tok != 0)
		{
			tok = strtok_s(NULL, Delim, &context);
		}
	}

	m_RequiredBufferSize = (unsigned int)m_Resolution.x * (unsigned int)m_Resolution.y * (unsigned int)m_Resolution.z;

	m_File.close();
}

void CRawParser::fillBuffer(void* _pBuffer, const int& _pIndex)
{
  std::string path = m_WorkingDirectory + m_DataFileName;

	std::ifstream InFile;
	InFile.open(path, std::ios::binary);

	if(!InFile.good())
		return;

	size_t StrideLength = getDataTypeSize(m_Format);
	
	size_t offset = 0;
	float fZero = 0.0f;
	while(!InFile.eof())
	{
		if(m_Format == RD_FORMAT_FLOAT3)
		{
			InFile.read((char*)_pBuffer + offset, 12);
			memcpy((char*)_pBuffer + offset+12, &fZero, 4);
		}
		else if(m_Format == RD_FORMAT_HALF3)
		{
			InFile.read((char*)_pBuffer + offset, 6);
			memcpy((char*)_pBuffer + offset+12, &fZero, 2);
		}
		else
		{
			InFile.read((char*)_pBuffer + offset, StrideLength);
		}

		offset += StrideLength;
	}

	//InFile.read((char*)_pBuffer, m_Resolution.x * m_Resolution.y * m_Resolution.z * StrideLength);
	/*
	char Buff[16];
	for(int d = 0; d < m_Resolution.z; d++) // Depth
	{
		for(int c = 0; c < m_Resolution.y; c++) // Columns
		{
			for(int r = 0; r < m_Resolution.x; r++)	// Row
			{
				int Index = StrideLength * (((int)d * static_cast<int>(m_Resolution.y) * static_cast<int>(m_Resolution.x) ) + (c * static_cast<int>(m_Resolution.z)) + r);
				if(m_Format == RD_FORMAT_FLOAT3)
				{
					
					InFile.read((char*)_pBuffer + Index, 12);
					*((char*)_pBuffer + Index + 12) = 0;
					*((char*)_pBuffer + Index + 13) = 0;
					*((char*)_pBuffer + Index + 14) = 0;
					*((char*)_pBuffer + Index + 15) = 0;
					
					float arr[4] = {0, 0, 0, 1};
					memcpy((char*)_pBuffer + Index, arr, 16);
					
				}
				else if(m_Format == RD_FORMAT_HALF3)
				{
					InFile.read((char*)_pBuffer + Index, 6);
					*((char*)_pBuffer + Index + 6) = 0;
					*((char*)_pBuffer + Index + 7) = 0;
					
				}
				else
					
					InFile.read((char*)_pBuffer + StrideLength * (((int)d * static_cast<int>(m_Resolution.y) * static_cast<int>(m_Resolution.x) ) + (c * static_cast<int>(m_Resolution.x)) + r), StrideLength);
			}
		}
	}*/
}

unsigned int CRawParser::getRequiredBufferSize()
{
	return getDataTypeSize(m_Format) * m_RequiredBufferSize;
}

int CRawParser::getDataTypeSize(const eRawDataFormat& _pFormat)
{
	switch(_pFormat)
	{
	case RD_FORMAT_UCHAR:
	case RD_FORMAT_BYTE:
		return 1;

	case RD_FORMAT_HALF3:
	case RD_FORMAT_HALF4:
		return 8;
	case RD_FORMAT_FLOAT3:
	case RD_FORMAT_FLOAT4:
		return 16;
	default:
		return 0;
	}
}

CRawParser::~CRawParser(void)
{
	if(m_File)
	{
		m_File.close();
	}
}