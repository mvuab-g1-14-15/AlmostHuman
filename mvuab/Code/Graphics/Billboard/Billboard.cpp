#include "Billboard.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Core.h"
#include "Cameras\CameraManager.h"
#include "Cameras\CameraFPShooter.h"
#include "GraphicsManager.h"
#include "Math\Matrix44.h"
#include "RenderableVertex\VertexTypes.h"

CBillboard::CBillboard() : CName()
{

}

CBillboard::~CBillboard()
{
}


void CBillboard::Init(Math::Vect3f Position, float Size, std::string Texture)
{
    m_Position=Position;
    m_Size = Size;
    m_Active = true;
    m_Texture = CoreInstance->GetTextureManager()->GetTexture(Texture);

	/*
	unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
	TT1_VERTEX v[4] =
	{
		{ ul.x,        ul.y,        ul.z, 0,1},
		{ dl.x,        dl.y,        dl.z, 1,1},
		{ ur.x,        ur.y,        ur.z, 1,0},
		{ dr.x,        dr.y,        dr.z, 0,0}
	};
	void* lIdxAddress = malloc( sizeof( unsigned short int ) * 6 );
	void* lVtxsAddress = ( void* ) malloc( 4 * sizeof(TT1_VERTEX) );

	// TODO Copy the  indices and de v into the idxaddres and vtx adress

	mRV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_DIFF_VERTEX>( CGraphicsManager::GetSingletonPtr(),
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
	*/
}

void CBillboard::Update()
{
        if(m_Active)
        {
                /*Calcular A,B,C,D del rectangulo.
                orientar a la camara (producto vectorial direccion i sumar VectUP) [normalizados]
                N_VectRight=VectUp^VectDir
                a=pos + n_VectorUP*size/2 -N_VectRight*size/2;*/
				CCamera *l_Camera = CCore::GetSingletonPtr()->GetCameraManager()->GetCurrentCamera();
                Math::Vect3f l_vRight = l_Camera->GetDirection().Normalize()^l_Camera->GetVecUp().Normalize();
                l_vRight.Normalize();
                float halfSize = m_Size/2;
                Math::Vect3f l_newUP(l_Camera->GetVecUp().Normalize()*halfSize);
                Math::Vect3f l_newRight(l_vRight*halfSize);
                m_PosA = m_Position + l_newUP - l_newRight;
                m_PosB = m_Position + l_newUP + l_newRight;
                m_PosC = m_Position - l_newUP - l_newRight;
                m_PosD = m_Position - l_newUP + l_newRight;
        }
}
void CBillboard::Render()
{
        if(m_Active)
        {
				//GraphicsInstance->EnableAlphaBlend();
                TT1_VERTEX l_Points[4];
                u_short l_Indexes[6]={0,2,1,1,2,3};

                l_Points[0].x=m_PosA.x;
                l_Points[0].y=m_PosA.y;
                l_Points[0].z=m_PosA.z;
                l_Points[0].tu=0.0f;
                l_Points[0].tv=1.0f;

                l_Points[1].x=m_PosB.x;
                l_Points[1].y=m_PosB.y;
                l_Points[1].z=m_PosB.z;
                l_Points[1].tu=1.0f;
                l_Points[1].tv=1.0f;
        
                l_Points[2].x=m_PosC.x;
                l_Points[2].y=m_PosC.y;
                l_Points[2].z=m_PosC.z;
                l_Points[2].tu=0.0f;
                l_Points[2].tv=0.0f;

                l_Points[3].x=m_PosD.x;
                l_Points[3].y=m_PosD.y;
                l_Points[3].z=m_PosD.z;
                l_Points[3].tu=1.0f;
                l_Points[3].tv=0.0f;

                Math::Mat44f l_Mat;
                l_Mat.SetIdentity(); 
				GraphicsInstance->SetTransform(l_Mat);

                GraphicsInstance->GetDevice()->SetTexture(0,m_Texture->GetDXTexture());
                GraphicsInstance->GetDevice()->SetFVF(TT1_VERTEX::GetFVF());

				GraphicsInstance->DrawQuad3D(m_PosA, m_PosB, m_PosC, m_PosD);

                //GraphicsInstance->GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 6, 2,l_Indexes,D3DFMT_INDEX16, l_Points, sizeof( TT1_VERTEX ) );
                //GraphicsInstance->DisableAlphaBlend();
        }
}

void CBillboard::SetTexture(std::string Texture)
{
        m_Texture = CoreInstance->GetTextureManager()->GetResource(Texture);
}
CTexture* CBillboard::GetTexture()
{
        return m_Texture;
}