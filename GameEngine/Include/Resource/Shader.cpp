#include "Shader.h"

CShader::CShader()
	: m_pInputLayout(nullptr),m_iInputLayoutSize(0),
	m_iInstanceInputLayoutSize(0)
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
	//GPU���� Vertex Buffer�� ������ �� �޸� ����� ������.
	//���� GPU�� ������ ��Ȯ�� ���� �Ӽ��� �����ϱ� ���ؼ���
	//Vertext Layout�� �˾ƾ��Ѵ�. �� ���ؽ� �Ӽ��� D3D11_INPUT_ELEMENT_DESC
	//����ü�� �� ���ִ�.
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	//�ش� ����� ������ �����ϴ� ���ڿ�
	tDesc.SemanticName = pSemanticName;

	//���� �Ӽ��� ������ ���� �� �ִµ�(ex Vector4����� 2�� ����)
	//�̸� �����ϱ� ���ؼ� index���� ����Ѵ�.
	tDesc.SemanticIndex = iSemanticIndex;

	//Format
	//�� ��ҿ��� ����ϴ� ������ Ÿ��
	tDesc.Format = eFmt;

	//���ؽ� �����͸� ���ؽ� ���۸� ���ؼ� GPU�� �ѱ� ��
	//���ÿ� �������� ���ý� ���۸� ������ �� �ִ�.(16��)
	//������ ���۴� 0~15�� ������ �Է� ���� �ѹ��� ���ε�ǰ�
	//InputSlot�ʵ�� GPU���� �� ��Ҹ� � ���ؽ� ���۷κ���
	//�����;� �ϴ��� ��Ÿ���� ������.
	tDesc.InputSlot = iInputSlot;

	//GPU�� �޸� ����� �ѱ� �� ���� ������ �˷��ش�.
	if (eSlotClass == D3D11_INPUT_PER_INSTANCE_DATA)
	{
		tDesc.AlignedByteOffset = m_iInstanceInputLayoutSize;
		m_iInstanceInputLayoutSize += iSize;
	}

	else
	{
		tDesc.AlignedByteOffset = m_iInputLayoutSize;
		m_iInputLayoutSize += iSize;
	}

	tDesc.InputSlotClass = eSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;

	m_vecInputLayoutDesc.push_back(tDesc);

	return true;
}

bool CShader::CreateInputLayout()
{
	return false;
}

void CShader::SetShader()
{
}
