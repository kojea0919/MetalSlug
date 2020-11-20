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

    //키가 눌린 경우
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        //처음 클릭
        if (!m_bDown && !m_bPush)
            m_bDown = true;

        //처음 이후 클릭
        else if (!m_bPush)
        {
            m_bDown = false;
            m_bPush = true;
        }
    }

    //키를 처음 뗀 경우
    else if (m_bPush || m_bDown)
    {
        m_bUp = true;
        m_bPush = false;
        m_bDown = false;

        m_vPrevPos = Vector3();

        //ZOrder원상 복구
        m_pOwner->SetZOrder(m_iPrevOwnerZOrder);
        m_iZOrder = m_iPrevZOrder;
    }

    //계속 뗀 경우
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
            //마우스를 따라서 이동
            //--------------------------------------------------
            m_vPrevPos.x += GET_SINGLE(CInput)->GetMouseMove().x;
            m_vPrevPos.y += GET_SINGLE(CInput)->GetMouseMove().y;
            m_pOwner->SetWorldPos(m_vPrevPos);
            //--------------------------------------------------

            m_iPrevOwnerZOrder = m_pOwner->GetZOrder();
            //m_iPrevZOrder = m_iZOrder;

            //드래그 중에는 항상 맨위에 출력 & 충돌체크 가장 먼저
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
