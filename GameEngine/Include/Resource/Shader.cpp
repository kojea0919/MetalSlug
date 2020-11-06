#include "Shader.h"

CShader::CShader()
	: m_pInputLayout(nullptr),m_iInputLayoutSize(0)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pInputLayout);
}

SHADER_TYPE CShader::GetShaderType() const
{
	return m_eType;
}

bool CShader::AddInputLayoutDesc(const char* pSemanticName, UINT iSemanticIndex, DXGI_FORMAT eFmt, UINT iInputSlot, UINT iSize, D3D11_INPUT_CLASSIFICATION eSlotClass, UINT iInstanceDataStepRate)
{
	//GPU에게 Vertex Buffer를 전송할 때 메모리 덩어리로 보낸다.
	//따라서 GPU는 버퍼의 정확한 정보 속성을 추출하기 위해서는
	//Vertext Layout을 알아야한다. 각 버텍스 속성은 D3D11_INPUT_ELEMENT_DESC
	//구조체로 알 수있다.
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	//해당 요소의 목적을 설명하는 문자열
	tDesc.SemanticName = pSemanticName;

	//동일 속성을 여러개 가질 수 있는데(ex Vector4멤버가 2개 존재)
	//이를 구분하기 위해서 index값을 사용한다.
	tDesc.SemanticIndex = iSemanticIndex;

	//Format
	//이 요소에서 사용하는 데이터 타입
	tDesc.Format = eFmt;

	//버텍스 데이터를 버텍스 버퍼를 통해서 GPU로 넘길 때
	//동시에 여러개의 버택스 버퍼를 전송할 수 있다.(16개)
	//각각의 버퍼는 0~15개 까지의 입력 슬롯 넘버에 바인드되고
	//InputSlot필드는 GPU에게 이 요소를 어떤 버텍스 버퍼로부터
	//가져와야 하는지 나타내는 변수다.
	tDesc.InputSlot = iInputSlot;

	//GPU에 메모리 덩어리를 넘길 때 시작 지점을 알려준다.
	tDesc.AlignedByteOffset = m_iInputLayoutSize;
	tDesc.InputSlotClass = eSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;

	m_vecInputLayoutDesc.push_back(tDesc);

	m_iInputLayoutSize += iSize;

	return true;
}

bool CShader::CreateInputLayout()
{
	return false;
}

void CShader::SetShader()
{
}
