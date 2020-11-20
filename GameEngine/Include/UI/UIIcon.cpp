#include "UIIcon.h"
#include "../Input.h"
#include "UIObject.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"

CUIIcon::CUIIcon()
	: m_bMouseCollision(false), m_bDown(false),
	m_bPush(false), m_bUp(false), m_bDrag(false),
    m_bAttachMouse(false)
{
}

CUIIcon::CUIIcon(const CUIIcon& control)
	: CUIControl(control), m_bMouseCollision(false), m_bDown(false),
	m_bPush(false), m_bUp(false), m_bDrag(false),
    m_bAttachMouse(control.m_bAttachMouse)
{
}

CUIIcon::~CUIIcon()
{
}

bool CUIIcon::Init()
{
	if (!CUIControl::Init())
		return false;


	return true;
}

void CUIIcon::Start()
{
	CUIControl::Start();
}

void CUIIcon::Update(float fTime)
{

    CUIControl::Update(fTime);

    //Ű�� ���� ���
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        //ó�� Ŭ��
        if (!m_bDown && !m_bPush)
            m_bDown = true;

        //ó�� ���� Ŭ��
        else if (!m_bPush)
        {
            m_bDown = false;
            m_bPush = true;
        }
    }

    //Ű�� ó�� �� ���
    else if (m_bPush || m_bDown)
    {
        m_bUp = true;
        m_bPush = false;
        m_bDown = false;

        m_vPrevPos = Vector3();

        //ZOrder���� ����
        m_pOwner->SetZOrder(m_iPrevOwnerZOrder);
        m_iZOrder = m_iPrevZOrder;
    }

    //��� �� ���
    else if (m_bUp)
    {
        m_bUp = false;
        m_bDrag = false;
    }

    if (m_bMouseCollision)
    {
        if (m_bDown)
        {
            m_bDrag = true;
            m_bAttachMouse = true;
        }

        if (m_bDrag)
        {    
            //���콺�� ���� �̵�
            //--------------------------------------------------
            m_vPrevPos.x += GET_SINGLE(CInput)->GetMouseMove().x;
            m_vPrevPos.y += GET_SINGLE(CInput)->GetMouseMove().y;
            m_pOwner->SetWorldPos(m_vPrevPos);
            //--------------------------------------------------

            m_iPrevOwnerZOrder = m_pOwner->GetZOrder();
            //m_iPrevZOrder = m_iZOrder;

            //�巡�� �߿��� �׻� ������ ��� & �浹üũ ���� ����
            m_pOwner->SetZOrder(INT_MIN);
            SetZOrder(INT_MIN);
        }
    }
}

void CUIIcon::PostUpdate(float fTime)
{
    CUIControl::PostUpdate(fTime);
}

void CUIIcon::PrevRender()
{
    CUIControl::PrevRender();
}

void CUIIcon::Render()
{
    CUIControl::Render();

    m_pUITexture->SetShader(0, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
    m_pMaterial->SetMaterial();

    m_pBodyMesh->Render(0.f);
}

void CUIIcon::PostRender()
{
    CUIControl::PostRender();
}

CUIIcon* CUIIcon::Clone()
{
    return new CUIIcon(*this);
}

void CUIIcon::CollisionMouse(const Vector2& vMousePos, float fTime)
{
    m_bMouseCollision = true;

    m_vPrevPos = m_pOwner->GetWorldPos();
}

void CUIIcon::CollisionReleaseMouse(const Vector2& vMousePos, float fTime)
{
    m_bMouseCollision = false;
    m_bDrag = false;
}
