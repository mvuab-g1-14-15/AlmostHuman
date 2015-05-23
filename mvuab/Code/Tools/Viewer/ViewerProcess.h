#ifndef MESH_PROCESS_H
#define MESH_PROCESS_H

#include "Process.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

#include <vector>

class CGraphicsManager;
class CFontManager;
class CObject3D;
class CAnimatedInstanceModel;

typedef enum EMenuOption
{
    eOptionScene = 1,
    eOptionMesh,
    eOptionParticles,
    eOptionLights,
    eOptionAnimatedModels,
    eOptionExit
};

class CViewerProcess: public CProcess
{
    public:
        CViewerProcess( void );
        ~CViewerProcess( void );

        virtual void Init();
        virtual void Update();
        virtual void Render();

        void OnClickedMenuOption( EMenuOption aMenuOption );

        void CreateViewerCameras();

        void  DeInit();

        void  RenderScene( CGraphicsManager* GM, float ElapsedTime );
        void  RenderScene2D( CGraphicsManager* GM, CFontManager* FM, float ElapsedTime, float FPS );
        uint32  RenderDebugInfo( CGraphicsManager* GM, CFontManager* FM, float l_Fps );
        std::string OpenPicker( std::string Extension );
        void RenderModel( int wParam );
        std::string OpenFolder();
        void CreateLight( Math::Vect3f Position );
        void CreateCamera( Math::Vect3f Position , std::string CameraName );
        void Clear();
    private:
        std::string m_FileName;
};

#endif