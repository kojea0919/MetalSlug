#include "Transform.h"
#include "../Device.h"
#include "../Resource/ShaderManager.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "Camera.h"

CTransform::CTransform()
    : m_pParent(nullptr),
    m_pOwner(nullptr),
    m_pScene(nullptr),
    m_bInheritScale(true),
    m_bInheritRotX(true),
    m_bInheritRotY(true),
    m_bInheritRotZ(true),
    m_bUpdateScale(true),
    m_bUpdateRot(true),
    m_bUpdatePos(true),
    m_b2D(false),
    m_bIsUseParentZValue(false)
{
    for (size_t iCnt = 0; iCnt < AXIS_END; ++iCnt)
    {
        m_vRelativeAxis[iCnt] = Vector3::Axis[iCnt];
        m_vWorldAxis[iCnt] = Vector3::Axis[iCnt];
    }
}

CTransform::CTransform(const CTransform& transform)
{
    //���� ����
    *this = transform;

    m_vecChild.clear();
    m_pParent = nullptr;
    m_pOwner = nullptr;
    m_pScene = nullptr;

    m_bUpdateScale = true;
    m_bUpdateRot = true;
    m_bUpdatePos = true;
}

CTransform::~CTransform()
{
}

void CTransform::InheritScale()
{
    //�θ��� Scale�� ��ӹ޾� ����ϴ� ��� �θ��� Scale���� ����Ͽ�
    //�ڽ��� WorldScale�� Setting
    if (m_bInheritScale)
        m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

    //�ش� �����ӿ��� Scale�� Update���� ǥ��
    m_bUpdateScale = true;

    //�ڽ��� �����ϴ� ��� �ڽĵ��� Scale�� ����
    //----------------------------------------
    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
    {
        m_vecChild[iCnt]->InheritScale();
    }
    //----------------------------------------
}

void CTransform::InheritRot()
{
    //�θ��� X�� ȸ���� ��ӹ޾� ����ϴ� ��� �θ���
    //X�� ȸ������ ����Ͽ� �ڽ��� X�� ȸ���� Setting
    if (m_bInheritRotX)
    {
        m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
    }

    //�θ��� Y�� ȸ���� ��ӹ޾� ����ϴ� ��� �θ���
    //Y�� ȸ������ ����Ͽ� �ڽ��� Y�� ȸ���� Setting
    if (m_bInheritRotY)
    {
        m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
    }

    //�θ��� Z�� ȸ���� ��ӹ޾� ����ϴ� ��� �θ���
    //Z�� ȸ������ ����Ͽ� �ڽ��� Z�� ȸ���� Setting
    if (m_bInheritRotZ)
    {
        m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
    }

    //�θ� ȸ���� ��� �ڽ��� ��ġ�� ���ϹǷ� Pos�� ����
    if (m_pParent)
        InheritPos();

    //ȸ���� ����Ǵ� ��� Component�� ���⵵  Update
    //------------------------------------------------
    Matrix matRot;

    //�θ� ������ ��
    //-------------------------------------
    matRot.Rotation(m_vRelativeRot);

    for (size_t iCnt = 0; iCnt < AXIS_END; ++iCnt)
    {
        m_vRelativeAxis[iCnt] = Vector3::Axis[iCnt].TransformNormal(matRot);
        m_vRelativeAxis[iCnt].Normalize();
    }
    //-------------------------------------

    //���� ������ ��
    //-------------------------------------
    matRot.Identity();
    matRot.Rotation(m_vWorldRot);

    for (size_t iCnt = 0; iCnt < AXIS_END; ++iCnt)
    {
        m_vWorldAxis[iCnt] = Vector3::Axis[iCnt].TransformNormal(matRot);
        m_vWorldAxis[iCnt].Normalize();
    }
    //-------------------------------------

    //------------------------------------------------


    m_bUpdateRot = true;

    //�ڽĵ��� ���� ����
    //----------------------------------------
    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
    {
        m_vecChild[iCnt]->InheritRot();
    }
    //----------------------------------------
}

void CTransform::InheritPos()
{
    Matrix matRot;

    //�ش� Transform�� Pos����
    //Parent�� Pos�Ӹ� �ƴ϶� Parent�� Rotation�� ������ ��ģ��.

    //1. �θ��� ȸ�� ���
    matRot.Rotation(m_pParent->GetWorldRot());

    //2. �θ��� Pos �߰�
    memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

    //�ڽ��� RelativePos�� �θ��� ȸ���� Pos��ȯ�� ���
    m_vWorldPos = m_vRelativePos.TransformCoord(matRot);

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();
    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
    {
        m_vecChild[iCnt]->InheritPos();
    }
}

void CTransform::SetRelativeScale(const Vector3& vScale)
{
    //���� �����ӿ��� �ӵ� Update
    m_vVelocityScale += vScale - m_vRelativeScale;

    m_vRelativeScale = vScale;

    m_vWorldScale = m_vRelativeScale;

    //�θ� �����ϰ� �θ��� Scale�� ��ӹ޾� ����ϴ°��
    //�θ��� Scale���� ����Ͽ� �ڽ��� Scale�� Setting
    if (m_pParent && m_bInheritScale)
        m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritScale();
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
    //���� �����ӿ��� �ӵ� Update
    m_vVelocityScale += Vector3(x, y, z) - m_vRelativeScale;

    m_vRelativeScale = Vector3(x, y, z);

    m_vWorldScale = m_vRelativeScale;

    //�θ� �����ϰ� �θ��� Scale�� ��ӹ޾� ����ϴ°��
    //�θ��� Scale���� ����Ͽ� �ڽ��� Scale�� Setting
    if (m_pParent && m_bInheritScale)
        m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritScale();
}

void CTransform::SetRelativeRotation(const Vector3& vRot)
{
    m_vVelocityRot += vRot - m_vRelativeRot;

    m_vRelativeRot = vRot;

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

        if (m_bInheritRotY)
            m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

        if (m_bInheritRotZ)
            m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
    m_vVelocityRot += Vector3(x, y, z) - m_vRelativeRot;

    m_vRelativeRot = Vector3(x, y, z);

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;

        if (m_bInheritRotY)
            m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;

        if (m_bInheritRotZ)
            m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
    {
        m_vecChild[i]->InheritRot();
    }
}

void CTransform::SetRelativeRotationX(float x)
{
    m_vVelocityRot.x += x - m_vRelativeRot.x;

    m_vRelativeRot.x = x;

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::SetRelativeRotationY(float y)
{
    m_vVelocityRot.y += y - m_vRelativeRot.y;

    m_vRelativeRot.y = y;

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotY)
            m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::SetRelativeRotationZ(float z)
{
    m_vVelocityRot.z += z - m_vRelativeRot.z;

    m_vRelativeRot.z = z;

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotZ)
            m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();

}

void CTransform::SetRelativePos(const Vector3& vPos)
{
    m_vVelocity += vPos - m_vRelativePos;

    m_vRelativePos = vPos;

    m_vWorldPos = m_vRelativePos;

    //�θ� �����ϴ� ��� �θ��� ȸ���� �̵���ȯ ����
    //�θ��� ȸ�������� �ڽ��� ��ġ�� �ٲ�� ����
    //�̸� Rot���� �����ִ� ������ �θ����� ȸ����
    //ó���� ȸ���� ������ �̵��� ��������ϱ� ������
    if (m_pParent)
    {
        Matrix matRot;

        matRot.Rotation(m_pParent->GetWorldRot());

        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritPos();
}

void CTransform::SetRelativePos(float x, float y, float z)
{
    m_vVelocity += Vector3(x, y, z) - m_vRelativePos;

    m_vRelativePos = Vector3(x, y, z);

    m_vWorldPos = m_vRelativePos;

    //�θ� �����ϴ� ��� �θ��� ȸ���� �̵���ȯ ����
    if (m_pParent)
    {
        Matrix matRot;

        matRot.Rotation(m_pParent->GetWorldRot());

        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritPos();
}

void CTransform::AddRelativeScale(const Vector3& vScale)
{
    m_vVelocityScale += vScale;

    m_vRelativeScale += vScale;

    m_vWorldScale = m_vRelativeScale;

    if (m_pParent && m_bInheritScale)
        m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritScale();
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
    m_vVelocityScale += Vector3(x, y, z);

    m_vRelativeScale += Vector3(x, y, z);

    m_vWorldScale = m_vRelativeScale;

    if (m_pParent && m_bInheritScale)
        m_vWorldScale = m_vRelativeScale * m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritScale();
}

void CTransform::AddRelativePos(const Vector3& vPos)
{
    m_vVelocity += vPos;

    m_vRelativePos += vPos;

    m_vWorldPos = m_vRelativePos;

    if (m_pParent)
    {
        Matrix	matRot;

        matRot.Rotation(m_pParent->GetWorldRot());

        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();
    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritPos();
}

void CTransform::AddRelativePos(float x, float y, float z)
{
    m_vVelocity += Vector3(x, y, z);

    m_vRelativePos += Vector3(x, y, z);

    m_vWorldPos = m_vRelativePos;

    if (m_pParent)
    {
        Matrix	matRot;

        matRot.Rotation(m_pParent->GetWorldRot());

        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        m_vWorldPos = m_vRelativePos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();
    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritPos();
}

void CTransform::AddRelativeRotation(const Vector3& vRot)
{
    m_vVelocityRot += vRot;

    m_vRelativeRot += vRot;

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
        {
            m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
        }
        if (m_bInheritRotY)
        {
            m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
        }
        if (m_bInheritRotZ)
        {
            m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
        }
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
    m_vVelocityRot += Vector3(x, y, z);

    m_vRelativeRot += Vector3(x, y, z);

    m_vWorldRot = m_vRelativeRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
        {
            m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
        }
        if (m_bInheritRotY)
        {
            m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
        }
        if (m_bInheritRotZ)
        {
            m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
        }
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddRelativeRotationX(float x)
{
    m_vVelocityRot.x += x;

    m_vRelativeRot.x += x;

    m_vWorldRot.x = m_vRelativeRot.x;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vWorldRot.x = m_vRelativeRot.x + m_pParent->GetWorldRot().x;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddRelativeRotationY(float y)
{
    m_vVelocityRot.y += y;

    m_vRelativeRot.y += y;

    m_vWorldRot.y = m_vRelativeRot.y;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vWorldRot.y = m_vRelativeRot.y + m_pParent->GetWorldRot().y;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddRelativeRotationZ(float z)
{
    m_vVelocityRot.z += z;

    m_vRelativeRot.z += z;

    m_vWorldRot.z = m_vRelativeRot.z;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vWorldRot.z = m_vRelativeRot.z + m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

Vector3 CTransform::GetRelativeScale() const
{
    return m_vRelativeScale;
}

Vector3 CTransform::GetRelativeRot() const
{
    return m_vRelativeRot;
}

Vector3 CTransform::GetRelativePos() const
{
    return m_vRelativePos;
}

Vector3 CTransform::GetRelativeAxis(AXIS eAxis) const
{
    return m_vRelativeAxis[eAxis];
}

void CTransform::SetWorldScale(const Vector3& vScale)
{
    m_vVelocityScale += vScale - m_vWorldScale;

    m_vWorldScale = vScale;

    m_vRelativeScale = m_vWorldScale;

    //�θ� �ִ� ���� ���� WorldScale�� �����ϴ� ���̹Ƿ�
    //Relative�� ���� ����� ����Ѵ�.
    if (m_pParent && m_bInheritScale)
        m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritScale();
}

void CTransform::SetWorldScale(float x, float y, float z)
{
    m_vVelocityScale += Vector3(x, y, z) - m_vWorldScale;

    m_vWorldScale = Vector3(x, y, z);

    m_vRelativeScale = m_vWorldScale;

    //�θ� �ִ� ���� ���� WorldScale�� �����ϴ� ���̹Ƿ�
    //Relative�� ���� ����� ����Ѵ�.
    if (m_pParent && m_bInheritScale)
        m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritScale();
}

void CTransform::SetWorldRotation(const Vector3& vRot)
{
    m_vVelocityRot += vRot - m_vWorldRot;

    m_vWorldRot = vRot;

    m_vRelativeRot = m_vWorldRot;

    //�θ� �ִ� ��� Relative Setting
    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

        if (m_bInheritRotY)
            m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

        if (m_bInheritRotZ)
            m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritRot();
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
    m_vVelocityRot += Vector3(x, y, z) - m_vWorldRot;

    m_vWorldRot = Vector3(x, y, z);

    m_vRelativeRot = m_vWorldRot;

    //�θ� �ִ� ��� Relative Setting
    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

        if (m_bInheritRotY)
            m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

        if (m_bInheritRotZ)
            m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t iSize = m_vecChild.size();

    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritRot();
}

void CTransform::SetWorldRotationX(float x)
{
    m_vVelocityRot.x += x - m_vWorldRot.x;

    m_vWorldRot.x = x;

    m_vRelativeRot.x = m_vWorldRot.x;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::SetWorldRotationY(float y)
{
    m_vVelocityRot.y += y - m_vWorldRot.y;

    m_vWorldRot.y = y;

    m_vRelativeRot.y = m_vWorldRot.y;

    if (m_pParent)
    {
        if (m_bInheritRotY)
            m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::SetWorldRotationZ(float z)
{
    m_vVelocityRot.z += z - m_vWorldRot.z;

    m_vWorldRot.z = z;

    m_vRelativeRot.z = m_vWorldRot.z;

    if (m_pParent)
    {
        if (m_bInheritRotZ)
            m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::SetWorldPos(const Vector3& vPos)
{
    m_vVelocity += vPos - m_vWorldPos;

    m_vWorldPos = vPos;

    m_vRelativePos = m_vWorldPos;

    //World�� ���� Setting�ϴ� ���̹Ƿ� ������� �������ش�.
    if (m_pParent)
    {
        Matrix matRot;

        matRot.Rotation(m_pParent->GetWorldRot());
        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        matRot.Inverse();

        m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();
    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritPos();
}

void CTransform::SetWorldPos(float x, float y, float z)
{
    m_vVelocity += Vector3(x, y, z) - m_vWorldPos;

    m_vWorldPos = Vector3(x, y, z);

    m_vRelativePos = m_vWorldPos;

    //World�� ���� Setting�ϴ� ���̹Ƿ� ������� �������ش�.
    if (m_pParent)
    {
        Matrix matRot;

        matRot.Rotation(m_pParent->GetWorldRot());
        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        matRot.Inverse();

        m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t iSize = m_vecChild.size();
    for (size_t iCnt = 0; iCnt < iSize; ++iCnt)
        m_vecChild[iCnt]->InheritPos();
}

void CTransform::AddWorldScale(const Vector3& vScale)
{
    m_vVelocityScale += vScale;

    m_vWorldScale += vScale;

    m_vRelativeScale = m_vWorldScale;

    if (m_pParent && m_bInheritScale)
        m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritScale();
}

void CTransform::AddWorldScale(float x, float y, float z)
{
    m_vVelocityScale += Vector3(x, y, z);

    m_vWorldScale += Vector3(x, y, z);

    m_vRelativeScale = m_vWorldScale;

    if (m_pParent && m_bInheritScale)
        m_vRelativeScale = m_vWorldScale / m_pParent->GetWorldScale();

    m_bUpdateScale = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritScale();
}

void CTransform::AddWorldPos(const Vector3& vPos)
{
    m_vVelocity += vPos;

    m_vWorldPos += vPos;

    m_vRelativePos = m_vWorldPos;

    if (m_pParent)
    {
        Matrix	matRot;

        matRot.Rotation(m_pParent->GetWorldRot());

        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        matRot.Inverse();

        m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
    }

    m_bUpdatePos = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritPos();
}

void CTransform::AddWorldPos(float x, float y, float z)
{
    m_vVelocity += Vector3(x, y, z);

    m_vWorldPos += Vector3(x, y, z);

    m_vRelativePos = m_vWorldPos;

    if (m_pParent)
    {
        Matrix	matRot;

        matRot.Rotation(m_pParent->GetWorldRot());

        memcpy(&matRot._41, &m_pParent->GetWorldPos(), sizeof(Vector3));

        matRot.Inverse();

        m_vRelativePos = m_vWorldPos.TransformCoord(matRot);
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdatePos = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritPos();
}

void CTransform::AddWorldRotation(const Vector3& vRot)
{
    m_vVelocityRot += vRot;

    m_vWorldRot += vRot;

    m_vRelativeRot = m_vWorldRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

        if (m_bInheritRotY)
            m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

        if (m_bInheritRotZ)
            m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
    m_vVelocityRot += Vector3(x, y, z);

    m_vWorldRot += Vector3(x, y, z);

    m_vRelativeRot = m_vWorldRot;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;

        if (m_bInheritRotY)
            m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;

        if (m_bInheritRotZ)
            m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotationX(float x)
{
    m_vVelocityRot.x += x;

    m_vWorldRot.x += x;

    m_vRelativeRot.x = m_vWorldRot.x;

    if (m_pParent)
    {
        if (m_bInheritRotX)
            m_vRelativeRot.x = m_vWorldRot.x - m_pParent->GetWorldRot().x;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotationY(float y)
{
    m_vVelocityRot.y += y;

    m_vWorldRot.y += y;

    m_vRelativeRot.y = m_vWorldRot.y;

    if (m_pParent)
    {
        if (m_bInheritRotY)
            m_vRelativeRot.y = m_vWorldRot.y - m_pParent->GetWorldRot().y;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::AddWorldRotationZ(float z)
{
    m_vVelocityRot.z += z;

    m_vWorldRot.z += z;

    m_vRelativeRot.z = m_vWorldRot.z;

    if (m_pParent)
    {
        if (m_bInheritRotZ)
            m_vRelativeRot.z = m_vWorldRot.z - m_pParent->GetWorldRot().z;
    }

    Matrix	matRot;
    matRot.Rotation(m_vRelativeRot);

    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vRelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vRelativeAxis[i].Normalize();
    }

    matRot.Identity();
    matRot.Rotation(m_vWorldRot);
    for (int i = 0; i < AXIS_END; ++i)
    {
        m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
        m_vWorldAxis[i].Normalize();
    }

    m_bUpdateRot = true;

    size_t	iSize = m_vecChild.size();

    for (size_t i = 0; i < iSize; ++i)
        m_vecChild[i]->InheritRot();
}

void CTransform::SetPivot(const Vector3& vPivot)
{
    m_vPivot = vPivot;
}

void CTransform::SetPivot(float x, float y, float z)
{
    m_vPivot = Vector3(x, y, z);
}

void CTransform::SetMeshSize(const Vector3& vSize)
{
    m_vMeshSize = vSize;
}

Vector3 CTransform::GetWorldScale() const
{
    return m_vWorldScale;
}

Vector3 CTransform::GetWorldRot() const
{
    return m_vWorldRot;
}

Vector3 CTransform::GetWorldPos() const
{
    return m_vWorldPos;
}

Vector3 CTransform::GetWorldAxis(AXIS eAxis) const
{
    return m_vWorldAxis[eAxis];
}

Vector3 CTransform::GetPivot() const
{
    return m_vPivot;
}

Vector3 CTransform::GetMeshSize() const
{
    return m_vMeshSize;
}

Matrix CTransform::GetScaleMatrix() const
{
    return m_matScale;
}

Matrix CTransform::GetRotMatrix() const
{
    return m_matRot;
}

Matrix CTransform::GetTranslationMatrix() const
{
    return m_matTranslation;
}

Matrix CTransform::GetWorldMatrix() const
{
    return m_matWorld;
}

void CTransform::Start()
{
    m_vVelocityScale = Vector3::Zero;
    m_vVelocityRot = Vector3::Zero;
    m_vVelocity = Vector3::Zero;
}

void CTransform::Update(float fTime)
{
}

void CTransform::PostUpdate(float fTime)
{
    if (m_bUpdateScale)
        m_matScale.Scaling(m_vWorldScale);

    if (m_bUpdateRot)
        m_matRot.Rotation(m_vWorldRot);

    // 2D ��ü�� ��� z�� y�� ��ü���ش�.
    //���� 0~3000���̷� Setting
    //------------------------------------------
    if (m_b2D)
    {
        //Vector3 vWorldStart = m_pScene->GetWorldStart();
        //Vector3 vWorldSize = m_pScene->GetWorldSize();

        ////�θ��� z���� ����ϴ� ��� ���� ���
        //if (m_bIsUseParentZValue)
        //    m_vWorldPos.z = (m_pParent->m_vWorldPos.y - vWorldStart.y - (int)m_pOwner->GetRender_Priority()) * (3000.f / vWorldSize.y);
        //else
        //    m_vWorldPos.z = (m_vWorldPos.y - vWorldStart.y) * (3000.f / vWorldSize.y);
        Vector3	vWorldStart = m_pScene->GetWorldStart();
        Vector3	vWorldSize = m_pScene->GetWorldSize();
        m_vWorldPos.z = (m_vWorldPos.y - vWorldStart.y) * (3000.f / vWorldSize.y);
    }
    //------------------------------------------

    m_matTranslation.Translation(m_vWorldPos);

    m_matWorld = m_matScale * m_matRot * m_matTranslation;


    if (m_b2D)
        m_vWorldPos.z = 0.f;
}

void CTransform::SetTransform()
{
    CCamera* pCamera = m_pScene->GetCameraManager()->GetMainCamera();

    if(m_pOwner->GetSceneComponentType() == SCENECOMPONENT_TYPE::ST_UI)
        pCamera = m_pScene->GetCameraManager()->GetUICamera();

    m_tCBuffer.matWorld = m_matWorld;
    m_tCBuffer.matView = pCamera->GetViewMatrix();
    m_tCBuffer.matProj = pCamera->GetProjMatrix();
    m_tCBuffer.matWV = m_tCBuffer.matWorld * m_tCBuffer.matView;
    m_tCBuffer.matWVP = m_tCBuffer.matWorld * m_tCBuffer.matView * m_tCBuffer.matProj;
    m_tCBuffer.vPivot = m_vPivot;
    m_tCBuffer.vMeshSize = m_vMeshSize;

    // Shader�� ����� �ѱ涧�� �ݵ�� Transpose ���־�� �Ѵ�.
    // Transpose : ��ġ���
    m_tCBuffer.matWorld.Transpose();
    m_tCBuffer.matView.Transpose();
    m_tCBuffer.matProj.Transpose();
    m_tCBuffer.matWV.Transpose();
    m_tCBuffer.matWVP.Transpose();

    GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &m_tCBuffer);

    //�ش� �������� ������ �����ͷ� �ʱ�ȭ
    //------------------------------------
    m_vVelocityScale = Vector3::Zero;
    m_vVelocityRot = Vector3::Zero;
    m_vVelocity = Vector3::Zero;
    m_bUpdateScale = false;
    m_bUpdateRot = false;
    m_bUpdatePos = false;
    //------------------------------------
}

CTransform* CTransform::Clone()
{
    return new CTransform(*this);
}

void CTransform::Save(FILE* pFile)
{
    fwrite(&m_bInheritScale, sizeof(m_bInheritScale), 1, pFile);
    fwrite(&m_bInheritRotX, sizeof(m_bInheritRotX), 1, pFile);
    fwrite(&m_bInheritRotY, sizeof(m_bInheritRotY), 1, pFile);
    fwrite(&m_bInheritRotZ, sizeof(m_bInheritRotZ), 1, pFile);

    fwrite(&m_vRelativeScale, sizeof(m_vRelativeScale), 1, pFile);
    fwrite(&m_vRelativeRot, sizeof(m_vRelativeRot), 1, pFile);
    fwrite(&m_vRelativePos, sizeof(m_vRelativePos), 1, pFile);
    fwrite(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);

    fwrite(&m_vWorldScale, sizeof(m_vWorldScale), 1, pFile);
    fwrite(&m_vWorldRot, sizeof(m_vWorldRot), 1, pFile);
    fwrite(&m_vWorldPos, sizeof(m_vWorldPos), 1, pFile);
    fwrite(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
    fwrite(&m_vPivot, sizeof(m_vPivot), 1, pFile);
    fwrite(&m_vMeshSize, sizeof(m_vMeshSize), 1, pFile);
}

void CTransform::Load(FILE* pFile)
{
    m_bUpdateScale = true;
    m_bUpdateRot = true;
    m_bUpdatePos = true;

    fread(&m_bInheritScale, sizeof(m_bInheritScale), 1, pFile);
    fread(&m_bInheritRotX, sizeof(m_bInheritRotX), 1, pFile);
    fread(&m_bInheritRotY, sizeof(m_bInheritRotY), 1, pFile);
    fread(&m_bInheritRotZ, sizeof(m_bInheritRotZ), 1, pFile);

    fread(&m_vRelativeScale, sizeof(m_vRelativeScale), 1, pFile);
    fread(&m_vRelativeRot, sizeof(m_vRelativeRot), 1, pFile);
    fread(&m_vRelativePos, sizeof(m_vRelativePos), 1, pFile);
    fread(m_vRelativeAxis, sizeof(Vector3), AXIS_END, pFile);

    fread(&m_vWorldScale, sizeof(m_vWorldScale), 1, pFile);
    fread(&m_vWorldRot, sizeof(m_vWorldRot), 1, pFile);
    fread(&m_vWorldPos, sizeof(m_vWorldPos), 1, pFile);
    fread(m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);
    fread(&m_vPivot, sizeof(m_vPivot), 1, pFile);
    fread(&m_vMeshSize, sizeof(m_vMeshSize), 1, pFile);
}
