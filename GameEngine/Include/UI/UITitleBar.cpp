#include "UITitleBar.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
#include "../Input.h"
#include "UIObject.h"

CUITitleBar::CUITitleBar()
    : m_bMouseCollision(false),m_bDown(false),
    m_bPush(false),m_bUp(false),m_bDrag(false)
{
}

CUITitleBar::CUITitleBar(const CUITitleBar& control)
    : CUIControl(control), m_bMouseCollision(false), m_bDown(false),
    m_bPush(false), m_bUp(false), m_bDrag(false)
{
}

CUITitleBar::~CUITitleBar()
{
}

bool CUITitleBar::Init()
{
    if (!CUIControl::Init())
        return false;

    return true;
}

void CUITitleBar::Start()
{
    CUIControl::Start();
}

void CUITitleBar::Update(float fTime)
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
            m_bDrag = true;

        if (m_bDrag)
        {
            m_vPrevPos.x += GET_SINGLE(CInput)->GetMouseMove().x;
            m_vPrevPos.y += GET_SINGLE(CInput)->GetMouseMove().y;

            m_pOwner->SetWorldPos(m_vPrevPos);
        }
    }
}

void CUITitleBar::PostUpdate(float fTime)
{
    CUIControl::PostUpdate(fTime);
}

void CUITitleBar::PrevRender()
{
    CUIControl::PrevRender();
}

void CUITitleBar::Render()
{
    CUIControl::Render();

    m_pUITexture->SetShader(0, (int)CBUFFER_SHADER_TYPE::CBUFFER_PIXEL);
    m_pMaterial->SetMaterial();

    m_pBodyMesh->Render(0.f);
}

void CUITitleBar::PostRender()
{
    CUIControl::PostRender();
}

CUITitleBar* CUITitleBar::Clone()
{
    return new CUITitleBar(*this);
}

void CUITitleBar::CollisionMouse(const Vector2& vMousePos, float fTime)
{
    m_bMouseCollision = true;

    m_vPrevPos = m_pOwner->GetWorldPos();
}

void CUITitleBar::CollisionReleaseMouse(const Vector2& vMousePos, float fTime)
{
    m_bMouseCollision = false;
    m_bDrag = false;
}
