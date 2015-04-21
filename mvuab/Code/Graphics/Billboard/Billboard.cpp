#include "Billboard.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "Cameras\CameraFPShooter.h"
#include "Cameras\CameraManager.h"
#include "GraphicsManager.h"

#include "RenderableVertex\VertexTypes.h"
#include "Effects\EffectManager.h"
#include "EngineManagers.h"
#include "EngineConfig.h"

CBillboard::CBillboard() : CName(), m_Position(Math::Vect3f()), m_SizeX(0.1f), m_SizeY(0.1f), m_Active(true), CManager()
{
}

CBillboard::CBillboard(CXMLTreeNode& atts) : CName(), m_Position(Math::Vect3f()), m_SizeX(0.1f), m_SizeY(0.1f), m_Active(true), CManager(atts)
{
}

CBillboard::~CBillboard()
{
}

void CBillboard::Init()
{
    m_Texture = TextureMInstance->GetTexture(mConfigPath);
}


void CBillboard::Update()
{
    if(!m_Active) { return; }

    /*
        Calcular A,B,C,D del rectangulo.
        orientar a la camara (producto vectorial direccion i sumar VectUP) [normalizados]
        N_VectRight=VectUp^VectDir
        a=pos + n_VectorUP*size/2 -N_VectRight*size/2;
    */

    CCamera* l_Camera = CameraMInstance->GetCurrentCamera();

    Math::Vect3f l_Pos2Cam = l_Camera->GetPosition() - m_Position;
    Math::Vect3f l_vRight = ((-l_Pos2Cam.Normalize()) ^ l_Camera->GetVecUp().Normalize()).Normalize();

    float halfSizeY = m_SizeY * 0.5f;
    float halfSizeX = m_SizeX * 0.5f;

    Math::Vect3f l_newRight(l_vRight * halfSizeX);
    Math::Vect3f l_newUP(l_Camera->GetVecUp().Normalize() * halfSizeY);

    m_PosA = m_Position + l_newUP - l_newRight;
    m_PosB = m_Position + l_newUP + l_newRight;
    m_PosC = m_Position - l_newUP - l_newRight;
    m_PosD = m_Position - l_newUP + l_newRight;
}

void CBillboard::Render()
{
    if(!m_Active) { return; }

    Math::Mat44f l_Mat;
    l_Mat.SetIdentity();
    GraphicsInstance->SetTransform(l_Mat);

    CCamera* l_Camera = CameraMInstance->GetCurrentCamera();
    Math::Vect3f l_Pos2Cam = l_Camera->GetPosition() - m_Position;

    CEffectTechnique* EffectTechnique = EffectManagerInstance->GetResource("GenerateGBufferTechnique");
    GraphicsInstance->DrawQuad3DWithTechnique(m_PosA, m_PosB, m_PosC, m_PosD, l_Pos2Cam.Normalize(), EffectTechnique, m_Texture);
}

void CBillboard::SetSize(float sx, float sy)
{
    m_SizeX = sx;
    m_SizeY = sy;
}

void CBillboard::SetPosition(Math::Vect3f l_Pos)
{
    m_Position = l_Pos;
}

void CBillboard::SetTexture(std::string Texture)
{
    m_Texture = TextureMInstance->GetTexture(Texture);
}

CTexture *CBillboard::GetTexture()
{
    return m_Texture;
}
