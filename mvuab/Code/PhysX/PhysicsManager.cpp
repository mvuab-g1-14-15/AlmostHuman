#define __DONT_INCLUDE_MEM_LEAKS__

#include "PhysicsManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Defines.h"
////#include "Utils\Base.h"

#include "Physics.h"

#include "Actor\PhysicActor.h"
#include "Reports\PhysicCollisionReport.h"
#include "Actor\PhysicController.h"
#include "CookingMesh\PhysicCookingMesh.h"
#include "Joints\PhysicFixedJoint.h"
#include "Joints\PhysicRevoluteJoint.h"
#include "Joints\PhysicSphericalJoint.h"
#include "Reports\PhysicTriggerReport.h"
#include "Utils\PhysicUserAllocator.h"
#include "Utils\PhysicUserData.h"
//#include "Particles\SphereEmitter.h"
#include "Particles\ParticleManager.h"
////--------------------------------

#include "Utils\Defines.h"
#include "Timer\Timer.h"
#include "GraphicsManager.h"
//#include "Utils\Exception.h"
#include "Logger\Logger.h"
////#include "Utils\Base.h"
#include "EngineManagers.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//      CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

using namespace Math;

CPhysicsManager::CPhysicsManager( void )
    : m_szConfigFileName( "" )
    , m_bIsOk( false )
    , m_bDebugRenderMode( true )
    , m_pPhysicsSDK( NULL )
    , m_pScene( NULL )
    , mControllerManager( NULL )
    , m_pMyAllocator( NULL )
    , m_pCookingMesh( NULL )
    , m_InitParams( )
    , m_LoadASE( true )
    , CManager()
{
}

CPhysicsManager::CPhysicsManager( CXMLTreeNode& atts )
    : m_szConfigFileName( "" )
    , m_bIsOk( false )
    , m_bDebugRenderMode( true )
    , m_pPhysicsSDK( NULL )
    , m_pScene( NULL )
    , mControllerManager( NULL )
    , m_pMyAllocator( NULL )
    , m_pCookingMesh( NULL )
    , m_InitParams( )
    , m_LoadASE( true )
    , CManager( atts )
{
}

// -----------------------------------------
//        Métodos principales
// -----------------------------------------
//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
void CPhysicsManager::Init()
{
    m_pMyAllocator = new CPhysicUserAllocator;
    LOG_INFO_APPLICATION( "PhysicsManager:: Inicializando la libreria PhysX" );
    m_bIsOk = ( m_pMyAllocator != NULL );

	m_LoadASE = true;

    if ( m_bIsOk )
    {
        // Initialize PhysicsSDK
        NxPhysicsSDKDesc l_SDK_Desc;
        NxSDKCreateError errorCode = NXCE_NO_ERROR;
        m_pPhysicsSDK = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, m_pMyAllocator, NULL, l_SDK_Desc, &errorCode );
        m_bIsOk = ( m_pPhysicsSDK != NULL );

        if ( m_bIsOk )
        {
            LOG_INFO_APPLICATION( "PhysicsManager:: Creado el PhysXSDK" );
            LOG_INFO_APPLICATION( "PhysicsManager:: -------PhsX Settings---" );
            LOG_INFO_APPLICATION( "PhysicsManager:: El valor del SkinWidth es: %f", m_InitParams.m_fSkinWidth );
            m_pPhysicsSDK->setParameter( NX_SKIN_WIDTH, m_InitParams.m_fSkinWidth );
            //Código para pintar la información de los Joints
            m_pPhysicsSDK->setParameter( NX_VISUALIZE_ACTOR_AXES, 1 );
            m_pPhysicsSDK->setParameter( NX_VISUALIZE_JOINT_LIMITS, 1 );
            m_pPhysicsSDK->setParameter( NX_VISUALIZE_JOINT_LOCAL_AXES, 1 );
            m_pPhysicsSDK->setParameter( NX_VISUALIZE_JOINT_WORLD_AXES, 1 );
            m_pPhysicsSDK->setParameter( NX_CONTINUOUS_CD, 1 );
            // Create a scene
            LOG_INFO_APPLICATION( "PhysicsManager::Init-> El valor de la gravedad es: %f", m_InitParams.m_fGravity );
            NxSceneDesc sceneDesc;
            sceneDesc.gravity = NxVec3( 0.0f, m_InitParams.m_fGravity, 0.0f );
            sceneDesc.simType = NX_SIMULATION_HW;
            m_pScene = m_pPhysicsSDK->createScene( sceneDesc );

            // Si no va por Hardware busco por software
            if ( !m_pScene )
            {
                sceneDesc.simType = NX_SIMULATION_SW;
                m_pScene = m_pPhysicsSDK->createScene( sceneDesc );
            }

            m_bIsOk = ( m_pScene != NULL );

            if ( m_bIsOk )
            {
                LOG_INFO_APPLICATION( "PhysicsManager::Init-> Solo hay un material, con los siguientes params" );
                LOG_INFO_APPLICATION( "PhysicsManager::Init-> DefaultMaterial->Restitution %f:", m_InitParams.m_Restitution_DefMat );
                LOG_INFO_APPLICATION( "PhysicsManager::Init-> DefaultMaterial->StaticFriction %f:",
                                      m_InitParams.m_StaticFriction_DefMat );
                LOG_INFO_APPLICATION( "PhysicsManager::Init-> DefaultMaterial->DynamicFriction %f:",
                                      m_InitParams.m_DynamicFriction_DefMat );
                LOG_INFO_APPLICATION( "PhysicsManager::Init-> ----END PhsX Settings---" );
                NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex( 0 );
                defaultMaterial->setRestitution( m_InitParams.m_Restitution_DefMat );
                defaultMaterial->setStaticFriction( m_InitParams.m_StaticFriction_DefMat );
                defaultMaterial->setDynamicFriction( m_InitParams.m_DynamicFriction_DefMat );
                // Create a controllerManager
                mControllerManager = NxCreateControllerManager( m_pMyAllocator );
                m_bIsOk = ( mControllerManager != NULL );

                if ( m_bIsOk )
                {
                    LOG_INFO_APPLICATION( "PhysicsManager::Init-> Creado el controlador de caracteres" );
                    m_pCookingMesh = new CPhysicCookingMesh();
                    assert( m_pCookingMesh );
                    m_bIsOk = m_pCookingMesh->Init( m_pPhysicsSDK, m_pMyAllocator );

                    if ( m_bIsOk )
                        LOG_INFO_APPLICATION( "PhysicsManager::Init-> Creado el CookingMesh" );
                }// isOk mControllerManager?
            }//isOk m_pScene?
        }//isOk m_pPhysicsSDK ?
    }//isOk m_pMyAllocator ?

    if ( !m_bIsOk )
    {
        std::string msg_error = "PhysicsManager::Init-> Error en la inicializacion de PhysX";
        LOG_ERROR_APPLICATION( msg_error.c_str() );
        Release();
    }

    /*Precompilation Directives*/
#if defined( _DEBUG )
    //#define USE_DEBUGGER
    //#ifdef USE_DEBUGGER
    m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect( "127.0.0.1" );
    //#endif
#endif
    //return m_bIsOk;

#ifndef _USING_MESH_FOR_PHYSX
	m_pCookingMesh->CreateMeshFromASE("Data/PhysXMeshes.ase", "PhysXMeshes");
#endif
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicsManager::Done()
{
    if ( IsOk() )
    {
        Release();
        /* By XMA */
        CMapManager<CPhysicActor>::Destroy();
        CMapManager<CPhysicController>::Destroy();
        ReleaseVectors();
        /**********/
        m_bIsOk = false;
    }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicsManager::Release( void )
{
    ReleaseAllActors();
    CHECKED_DELETE( m_pCookingMesh );

    if ( mControllerManager != NULL )
    {
        mControllerManager->purgeControllers();
        NxReleaseControllerManager( mControllerManager );
    }

    if ( m_pScene != NULL )
    {
        m_pPhysicsSDK->releaseScene( *m_pScene );
        m_pScene = NULL;
    }

    if ( m_pPhysicsSDK != NULL )
    {
        NxReleasePhysicsSDK( m_pPhysicsSDK );
        m_pPhysicsSDK = NULL;
    }

    CHECKED_DELETE( m_pMyAllocator );
}

void CPhysicsManager::ReleaseVectors()
{
    // UsersData & Actors del CreateMeshFromXML
    for ( size_t i = 0; i < m_vUsersData.size(); ++i )
        CHECKED_DELETE( m_vUsersData[i] );

    m_vUsersData.clear();

    for ( size_t i = 0; i < m_vActors.size(); ++i )
        CHECKED_DELETE( m_vActors[i] );

    m_vActors.clear();

    for ( size_t i = 0; i < m_vUD.size(); ++i )
        CHECKED_DELETE( m_vUD[i] );

    m_vUD.clear();
}

void CPhysicsManager::ReleaseToReload()
{
    GetCookingMesh()->Release();

    for ( size_t i = 0; i < m_vActors.size(); ++i )
        ReleasePhysicActor( m_vActors[i] );
}

void CPhysicsManager::ReleaseElement( const std::string& _ase )
{
    unsigned int l_id;
    std::map<std::string, unsigned int>::iterator it = m_vIds.find( _ase );

    if ( it != m_vIds.end() )
        l_id = it->second;

    GetCookingMesh()->ReleasePhysicMesh( _ase );
    ReleasePhysicActor( m_vActors[l_id] );
}


//----------------------------------------------------------------------------
// Load : Para cargar un archivo XML con todos los scripts
//----------------------------------------------------------------------------
bool CPhysicsManager::Load( const std::string& _PhysXConfig )
{
    LOG_INFO_APPLICATION(
        "CPhysicsManager::Load-->Loading physics." );
    m_szConfigFileName = _PhysXConfig;
    Init();
    return LoadXML();
}

//----------------------------------------------------------------------------
// Reload : Para recargar el XML
//----------------------------------------------------------------------------
bool CPhysicsManager::Reload( void )
{
    LOG_INFO_APPLICATION(
        "CPhysicsManager::Reload-> Reloading physics." );
    /* By XMA */
    ReleaseToReload();
    ReleaseVectors();
    Init();
    return CreateMeshFromXML( m_FileName );
}

bool CPhysicsManager::ReloadXML( const std::string& FileName )
{
    LOG_INFO_APPLICATION( "CPhysicsManager::Reload-> Reloading physics." );
    Release();
    //  return LoadXML ();
    /* By XMA */
    Init();
    return CreateMeshFromXML( FileName );
}

//----------------------------------------------------------------------------
// LoadXML : Carga el XML
//----------------------------------------------------------------------------
bool CPhysicsManager::LoadXML( void )
{
    LOG_INFO_APPLICATION( "CPhysicsManager::LoadXML --> Loading physic Files..." );
    /*  CXMLTreeNode l_File;

        if ( l_File.LoadFile ( m_FileName.c_str () ) )
        {
        CXMLTreeNode l_Scripts = l_File["scripts"];
        if ( l_Scripts.Exists ( ) )
        {
        unsigned int l_NumChilds = l_Scripts.GetNumChildren ( );
        for ( unsigned int i = 0; i < l_NumChilds; i ++ )
        {
          std::string l_Type = l_Scripts(i).GetName();
          if ( l_Type == "script" )
          {
            std::string l_FileName = l_Scripts(i).GetAttribute<std::string> ( "filename" );    // Obtenemos la ruta del fichero lua
            RunFile ( l_FileName );                           // Ejecutamos el fichero lua
          }
          else
          {
            CLogger::GetSingletonPtr()->AddNewLog ( ELL_WARNING, "CPhysicsManager::LoadXML --> Error loading file %s. The file doesn't contain any tag different form <SCRIPT>.", m_FileName );
          }
        }
        }
        else
        {
        CLogger::GetSingletonPtr()->AddNewLog ( ELL_ERROR, "CPhysicsManager::LoadXML --> Error loading file %s. The file doesn't contain tag <SCRIPTS>.", m_FileName );
        return false;
        }
        }
        else
        {
        CLogger::GetSingletonPtr()->AddNewLog ( ELL_ERROR, "CPhysicsManager::LoadXML --> Error loading file %s. The file doesn't exist or contain sintaxis errors.", m_FileName );
        return false;
        }
    */
    return true;
}


// -----------------------------------------
//          MÉTODOS
// -----------------------------------------

//----------------------------------------------------------------------------
// Update : Para actualizar la escena y realizar las físicas y simulaciones
//----------------------------------------------------------------------------
void CPhysicsManager::Update()
{
    assert( m_pScene != NULL );
    assert( mControllerManager != NULL );
    // Start simulation (non blocking)
    m_pScene->simulate( deltaTimeMacro );
    // Fetch simulation results
    m_pScene->flushStream( );
    m_pScene->fetchResults( NX_RIGID_BODY_FINISHED,  true );
}

void CPhysicsManager::WaitForSimulation( void )
{
    m_pScene->fetchResults( NX_RIGID_BODY_FINISHED,  true );
    NxReal l_MaxTimestep;
    NxTimeStepMethod l_Method;
    NxU32 l_MaxIter;
    NxU32 l_NumSubSteps;
    m_pScene->getTiming( l_MaxTimestep, l_MaxIter, l_Method, &l_NumSubSteps );

    if ( l_NumSubSteps )
        mControllerManager->updateControllers();
}

void CPhysicsManager::AddGravity( Math::Vect3f g )
{
    NxVec3 actual_g;
    m_pScene->getGravity( actual_g );
    m_pScene->setGravity( actual_g + NxVec3( g.x, g.y, g.z ) );
}

//----------------------------------------------------------------------------------------
// Debug Render : dibuja cada uno de los actores encontrados en la escena si estamos en modo debug
//----------------------------------------------------------------------------------------
void CPhysicsManager::Render()
{
    assert( m_pScene != NULL );

    if ( !m_bDebugRenderMode )
        return;

    // Render all actors in the scene
    int nbActors = m_pScene->getNbActors();
    NxActor** l_pActors = m_pScene->getActors();

    while ( nbActors-- )
    {
        NxActor* l_pActor = *l_pActors++;
        DrawActor( l_pActor, GraphicsInstance );
    }
}

void CPhysicsManager::DrawActor( NxActor* _pActor, CGraphicsManager* _RM )
{
    CPhysicUserData* physicUserData = ( CPhysicUserData* ) _pActor->userData;
    ASSERT( physicUserData, "Registering a phyxs object without user data" );

    if ( !physicUserData->GetPaint() || physicUserData->GetName() == "CharacterController" )
        return;

    NxShape* const* shapes  = _pActor->getShapes();
    NxU32 nShapes           = _pActor->getNbShapes();
    Mat44f m;
    Math::CColor color = physicUserData->GetColor();

    while ( nShapes-- )
    {
        switch ( shapes[nShapes]->getType() )
        {
        case NX_SHAPE_PLANE:
        {
            float distance  = shapes[nShapes]->isPlane()->getPlane().d;
            NxVec3 normal   = shapes[nShapes]->isPlane()->getPlane().normal;
            Math::Vect3f n( normal.x, normal.y, normal.z );
            _RM->DrawPlane( 100.f, n, distance, color, 40, 40 );
        }
        break;

        case NX_SHAPE_BOX:
        {
            NxVec3 pos = shapes[nShapes]->getGlobalPosition();
            NxF32 m_aux[16];
            shapes[nShapes]->getGlobalPose().getColumnMajor44( m_aux );
            m = ConvertNxF32ToMat44( m_aux );
            NxVec3 boxDim = shapes[nShapes]->isBox()->getDimensions();
            _RM->DrawBox( m, boxDim.x * 2, boxDim.y * 2, boxDim.z * 2, color );
        }
        break;

        case NX_SHAPE_SPHERE:
        {
            NxF32 m_aux[16];
            shapes[nShapes]->getGlobalPose().getColumnMajor44( m_aux );
            m = ConvertNxF32ToMat44( m_aux );
            NxReal radius = shapes[nShapes]->isSphere()->getRadius();
            Math::CColor color = physicUserData->GetColor();
            _RM->DrawSphere( m, radius, color, MAX_ARISTAS );
        }
        break;

        case NX_SHAPE_CAPSULE:
        {
            NxF32 m_aux[16];
            shapes[nShapes]->getGlobalPose().getColumnMajor44( m_aux );
            m = ConvertNxF32ToMat44( m_aux );
            _RM->SetTransform( m );
            const NxReal& radius = shapes[nShapes]->isCapsule()->getRadius();
            const NxReal& height = shapes[nShapes]->isCapsule()->getHeight();
            Math::CColor color = physicUserData->GetColor();
            Mat44f t;
            t.RotByAngleX( 3.1415f / 2 );
            _RM->DrawCylinder( m * t, radius, radius, height, MAX_ARISTAS, color, false );
            t.SetIdentity();
            t.Translate( Math::Vect3f( 0, height * 0.5f, 0 ) );
            _RM->DrawSphere( m * t, radius, color, MAX_ARISTAS );
            t.SetIdentity();
            t.Translate( Math::Vect3f( 0, -height * 0.5f, 0 ) );
            _RM->SetTransform( m * t );
            _RM->DrawSphere( m * t, radius, color, MAX_ARISTAS );
            t.SetIdentity();
            _RM->SetTransform( m * t );
        }
        break;

        case NX_SHAPE_CONVEX:
            break;

        case NX_SHAPE_MESH:
        {
            NxShape* mesh = shapes[nShapes];
            NxTriangleMeshDesc meshDesc;
            mesh->isTriangleMesh()->getTriangleMesh().saveToDesc( meshDesc );
            typedef NxVec3 Point;
            typedef struct _Triangle
            {
                NxU32 p0;
                NxU32 p1;
                NxU32 p2;
            } Triangle;
            NxU32 nbVerts       = meshDesc.numVertices;
            NxU32 nbTriangles   = meshDesc.numTriangles;
            Point* points       = ( Point* )meshDesc.points;
            Triangle* triangles = ( Triangle* )meshDesc.triangles;
            Math::CColor color  = physicUserData->GetColor();
            NxF32 m_aux[16];
            mesh->getGlobalPose().getColumnMajor44( m_aux );
            m = ConvertNxF32ToMat44( m_aux );
            _RM->SetTransform( m );
            Math::Vect3f a, b, c;

            while ( nbTriangles-- )
            {
                a = Math::Vect3f( points[triangles->p0].x, points[triangles->p0].y, points[triangles->p0].z );
                b = Math::Vect3f( points[triangles->p1].x, points[triangles->p1].y, points[triangles->p1].z );
                c = Math::Vect3f( points[triangles->p2].x, points[triangles->p2].y, points[triangles->p2].z );
                _RM->DrawLine( a, b, color );
                _RM->DrawLine( b, c, color );
                _RM->DrawLine( c, a, color );
                triangles++;
            }
        }
        break;

        case NX_SHAPE_WHEEL:
        {
            //TODO...
        }
        break;

        default:
        {
            //TODO...
        }
        break;
        }
    }

    Math::Mat44f identity = Math::m44fIDENTITY;
    _RM->SetTransform( identity );
}

//Convierte una un vector de 16 NxF32 a matriz de 4x4
Math::Mat44f CPhysicsManager::ConvertNxF32ToMat44( NxF32 m[16] )
{
    return Math::Mat44f( m[0], m[4], m[8], m[12],
                         m[1], m[5], m[9], m[13],
                         m[2], m[6], m[10], m[14],
                         m[3], m[7], m[11], m[15] );
}

//----------------------------------------------------------------------------
// AddPhysicActor : Añade un actor en la escena de PhysX
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicActor( CPhysicActor* _pActor )
{
	bool lOk = false;
	if(_pActor && m_pScene )
	{
		NxActorDesc* l_pActorDesc = _pActor->GetActorDesc();
		ASSERT( l_pActorDesc, "Null actor descriptor" );
		NxActor* nxActor = m_pScene->createActor( *l_pActorDesc );

		if(nxActor)
		{
			nxActor->userData = _pActor->GetUserData();
			_pActor->CreateActor( nxActor );
			_pActor->SetCollisionGroup( _pActor->GetColisionGroup() );
			ASSERT( _pActor->GetUserData(), "Null actor user data" );
			CMapManager<CPhysicActor>::AddResource( _pActor->GetUserData()->GetName(), _pActor );
			lOk = true;
		}
	}

    return lOk;
}

//----------------------------------------------------------------------------
// ReleasePhysicActor : Liberamos un actor de la escena de PhysX
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleasePhysicActor( CPhysicActor* _pActor )
{
    assert( _pActor != NULL );
    assert( m_pScene != NULL );
    assert( m_pPhysicsSDK != NULL );
    bool isOk = false;
    NxActor* nxActor = _pActor->GetPhXActor();

    if ( nxActor != 0 )
    {
        NxArray<NxCCDSkeleton*> skeletons;

        for ( NxU32 i = 0; i < nxActor->getNbShapes(); i++ )
        {
            NxShape* shape = nxActor->getShapes()[i];

            if ( shape->getCCDSkeleton() != NULL )
                skeletons.pushBack( shape->getCCDSkeleton() );
        }

        for ( NxU32 i = 0; i < skeletons.size(); i++ )
            m_pPhysicsSDK->releaseCCDSkeleton( *skeletons[i] );

        m_pScene->releaseActor( *nxActor );
        nxActor = 0;
        isOk = true;
    }

    return isOk;
}

//----------------------------------------------------------------------------
// ReleaseAllActors : Liberamos todos los actores de la escena de PhysX
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleaseAllActors( void )  //EUserDataFlag _eFlags )
{
    assert( m_pScene != NULL );
    assert( m_pPhysicsSDK != NULL );
    bool isOk = true;
    NxActor** l_ppActorList   = m_pScene->getActors();
    NxU32     l_TotalActors   = m_pScene->getNbActors();

    while ( l_TotalActors -- )
    {
        NxActor* nxActor = *l_ppActorList;

        if ( nxActor != 0 )
        {
            NxArray<NxCCDSkeleton*> skeletons;

            for ( NxU32 i = 0; i < nxActor->getNbShapes(); i++ )
            {
                NxShape* shape = nxActor->getShapes()[i];

                if ( shape->getCCDSkeleton() != NULL )
                    skeletons.pushBack( shape->getCCDSkeleton() );
            }

            for ( NxU32 i = 0; i < skeletons.size(); i++ )
                m_pPhysicsSDK->releaseCCDSkeleton( *skeletons[i] );

            m_pScene->releaseActor( *nxActor );
            nxActor = 0;
        }
    }

    return isOk;
}

//----------------------------------------------------------------------------
// AddPhysicSphericalJoint : Añadimos un joint esférico en la escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicSphericalJoint( CPhysicSphericalJoint* _Joint )
{
    assert( _Joint != NULL );
    assert( m_pScene != NULL );
    bool l_IsOk = false;
    NxJoint* l_NxJoint;
    NxSphericalJointDesc* l_JointDesc = _Joint->GetPhXDescJoint();
    assert( l_JointDesc != NULL );
    l_NxJoint = m_pScene->createJoint( *l_JointDesc );

    if ( l_NxJoint != NULL )
    {
        _Joint->CreateJoint( l_NxJoint );
        l_IsOk = true;
    }

    return l_IsOk;
}

//----------------------------------------------------------------------------
// ReleasePhysicSphericalJoint : Liberamos un joint esférica de la escena
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleasePhysicSphericalJoint( CPhysicSphericalJoint* _Joint )
{
    assert( _Joint );
    assert( m_pScene );
    NxJoint* l_PhXJoint = _Joint->GetPhXJoint();
    m_pScene->releaseJoint( *l_PhXJoint );
    l_PhXJoint = 0;
    return true;
}

//----------------------------------------------------------------------------
// AddPhysicRevoluteJoint : Añadimos un joint de tipo bisagras de puerta a la escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicRevoluteJoint( CPhysicRevoluteJoint* _pJoint )
{
    assert( _pJoint != NULL );
    assert( m_pScene != NULL );
    bool l_IsOk = false;
    NxJoint* nxJoint = 0;
    NxRevoluteJointDesc* l_JointDesc = _pJoint->GetPhXDescJoint();
    assert( l_JointDesc != NULL );
    nxJoint = m_pScene->createJoint( *l_JointDesc );

    if ( nxJoint != NULL )
    {
        _pJoint->CreateJoint( nxJoint );
        l_IsOk = true;
    }

    return l_IsOk;
}

//----------------------------------------------------------------------------
// ReleasePhysicRevoluteJoint : Liberamos un joint de bisagra de la escena
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleasePhysicRevoluteJoint( CPhysicRevoluteJoint* _Joint )
{
    assert( _Joint );
    assert( m_pScene );
    NxJoint* l_PhXJoint = _Joint->GetPhXJoint();
    m_pScene->releaseJoint( *l_PhXJoint );
    l_PhXJoint = 0;
    return true;
}

//----------------------------------------------------------------------------
// AddPhysicFixedJoint : Añadimos un joint de prensa, como un amortiguador en la escena
//----------------------------------------------------------------------------
bool CPhysicsManager::AddPhysicFixedJoint( CPhysicFixedJoint* _pJoint )
{
    assert( _pJoint != NULL );
    assert( m_pScene != NULL );
    bool l_bIsOk = false;
    NxJoint* l_NxJoint = 0;
    NxFixedJointDesc* l_pJointDesc = _pJoint->GetPhXDescJoint();
    assert( l_pJointDesc != NULL );
    l_NxJoint = m_pScene->createJoint( *l_pJointDesc );

    if ( l_NxJoint != NULL )
    {
        _pJoint->CreateJoint( l_NxJoint );
        l_bIsOk = true;
    }

    return l_bIsOk;
}

//----------------------------------------------------------------------------
// ReleasePhysicFixedJoint : Liberamos un joint fijo de la escena
//----------------------------------------------------------------------------
bool CPhysicsManager::ReleasePhysicFixedJoint( CPhysicFixedJoint* _pJoint )
{
    assert( _pJoint );
    assert( m_pScene );
    NxJoint* l_PhXJoint = _pJoint->GetPhXJoint();
    m_pScene->releaseJoint( *l_PhXJoint );
    l_PhXJoint = 0;
    return true;
}

bool CPhysicsManager::AddPhysicController( CPhysicController* _pController, EControleType _Tipus,
        ECollisionGroup _Group )
{
    assert( _pController != NULL );
    assert( m_pScene != NULL );
    assert( mControllerManager != NULL );
    bool l_bIsOK = false;
    NxController* l_NxController = _pController->GetPhXController();
    assert( l_NxController == NULL );  //Nos aseguramos que no hayan registrado ya un actor en la escena

    switch ( _pController->GetType() )
    {
    case BOX:
    {
        NxControllerDesc* l_NxControllerDesc = NULL;
        l_NxControllerDesc = _pController->GetPhXControllerDesc();
        assert( l_NxControllerDesc != NULL );
        l_NxController = mControllerManager->createController( m_pScene, *l_NxControllerDesc );
        break;
    }

    case CAPSULE:
    {
        NxControllerDesc* l_NxControllerDesc = NULL;
        l_NxControllerDesc = _pController->GetPhXControllerDesc();
        assert( l_NxControllerDesc != NULL );
        l_NxController = mControllerManager->createController( m_pScene, *l_NxControllerDesc );
        break;
    }
    }

    if ( mControllerManager != NULL )
    {
        _pController->CreateController( l_NxController, m_pScene );
        l_NxController->getActor()->userData = _pController->GetUserData();
        //NxShape*const* shape = nxController->getActor()->getShapes();
        //shape[0]->setGroup(controller->);
        l_NxController->getActor()->getShapes()[0]->setGroup( _Group );
        l_bIsOK = true;
    }

    if ( l_bIsOK )
        _pController->SetGroup( _pController->GetColisionGroup() );

    return l_bIsOK;
}

bool CPhysicsManager::ReleasePhysicController( CPhysicController* _pController )
{
    assert( _pController != NULL );
    assert( mControllerManager != NULL );

    bool l_bIsOk = false;
    NxController* l_NxController = _pController->GetPhXController();

    if ( l_NxController != NULL )
    {
		CMapManager<CPhysicController>::RemoveResource( _pController->GetUserData()->GetName() );
        mControllerManager->releaseController( *l_NxController );
        l_bIsOk = true;
    }

    return l_bIsOk;
}

NxCCDSkeleton* CPhysicsManager::CreateCCDSkeleton( float size )
{
    assert( m_pPhysicsSDK != NULL );
    NxU32 triangles[3 * 12] = { 0, 1, 3,  0, 3, 2, 3, 7, 6, 3, 6, 2, 1, 5, 7, 1, 7, 3, 4, 6, 7, 4, 7, 5, 1, 0, 4,
                                5, 1, 4,  4, 0, 2,  4, 2, 6
                              };
    NxVec3 points[8];
    //static mesh
    points[0].set( size, -size, -size );
    points[1].set( size, -size,  size );
    points[2].set( size,  size, -size );
    points[3].set( size,  size,  size );
    points[4].set( -size, -size, -size );
    points[5].set( -size, -size,  size );
    points[6].set( -size,  size, -size );
    points[7].set( -size,  size,  size );
    NxSimpleTriangleMesh stm;
    stm.numVertices   = 8;
    stm.numTriangles  = 6 * 2;
    stm.pointStrideBytes    = sizeof( NxVec3 );
    stm.triangleStrideBytes = sizeof( NxU32 ) * 3;
    stm.points    = points;
    stm.triangles = triangles;
    stm.flags     |= NX_MF_FLIPNORMALS;
    return m_pPhysicsSDK->createCCDSkeleton( stm );
}

//CPhysicUserData* CPhysicsManager::RaycastClosestActor ( const Math::Vect3f _vPosRay, const Math::Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info )
//{
//  //NxUserRaycastReport::ALL_SHAPES
//  assert(m_pScene != NULL);
//
//  NxRay ray;
//  ray.dir =  NxVec3 ( _vDirRay.x, _vDirRay.y, _vDirRay.z );
//  ray.orig = NxVec3 ( _vPosRay.x, _vPosRay.y, _vPosRay.z );
//
//  NxRaycastHit hit;
//  NxShape* closestShape = NULL;
//
//  //closestShape = m_pScene->raycastClosestShape ( ray, NX_ALL_SHAPES, hit, _uiImpactMask,  NX_MAX_F32, _uiImpactMask );
//  //closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, 0xffffffff, NX_MAX_F32, 0xffffffff, NULL, NULL );
//  closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, _uiImpactMask );
//  if (!closestShape)
//  {
//    //No hemos tocado a ningún objeto físico de la escena.
//    return NULL;
//  }
//  NxActor* actor = &closestShape->getActor();
//  CPhysicUserData* impactObject =(CPhysicUserData*)actor->userData;
//  //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
//  assert(impactObject);
//
//  _Info.m_fDistance   = hit.distance;
//  _Info.m_Normal      = Math::Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z );
//  _Info.m_CollisionPoint  = Math::Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z );
//
//  return impactObject;
//}

CPhysicUserData* CPhysicsManager::RaycastClosestActor( const Math::Vect3f _vPosRay,
        const Math::Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info, float _uiMaxDistance )
{
    //NxUserRaycastReport::ALL_SHAPES
    assert( m_pScene != NULL );
    NxRay ray;
    ray.dir   = NxVec3( _vDirRay.x, _vDirRay.y, _vDirRay.z );
    ray.orig  = NxVec3( _vPosRay.x, _vPosRay.y, _vPosRay.z );
    NxRaycastHit  hit;
    NxShape*      closestShape = NULL;
    //closestShape = m_pScene->raycastClosestShape ( ray, NX_ALL_SHAPES, hit, _uiImpactMask,  NX_MAX_F32, _uiImpactMask );
    //closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, 0xffffffff, NX_MAX_F32, 0xffffffff, NULL, NULL );
    NxReal l_Distance = ( NxReal ) _uiMaxDistance;
    // --- Jordi : Provisional. Cal deixar aquesta linia i modificar la col·lisió de càmera
    closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, _uiImpactMask, l_Distance );

    if ( !closestShape )
    {
        //No hemos tocado a ningún objeto físico de la escena.
        return NULL;
    }

    NxActor*          actor         = &closestShape->getActor();
    CPhysicUserData*  impactObject  = ( CPhysicUserData* )actor->userData;
    //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
    assert( impactObject );
    _Info.m_fDistance       = hit.distance;
    _Info.m_Normal          = Math::Vect3f( hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z );
    _Info.m_CollisionPoint  = Math::Vect3f( hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z );
    return impactObject;
}

CPhysicUserData* CPhysicsManager::RaycastClosestActorShoot( const Math::Vect3f _vPosRay,
        const Math::Vect3f& _vDirRay, uint32 _uiImpactMask, SCollisionInfo& _Info, float _fPower )
{
    //NxUserRaycastReport::ALL_SHAPES
    assert( m_pScene != NULL );
    NxRay ray;
    ray.dir  = NxVec3( _vDirRay.x, _vDirRay.y, _vDirRay.z );
    ray.orig = NxVec3( _vPosRay.x, _vPosRay.y, _vPosRay.z );
    NxRaycastHit  hit;
    NxShape*      closestShape = NULL;
    closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, _uiImpactMask );

    if ( !closestShape )
    {
        //No hemos tocado a ningún objeto físico de la escena.
        return NULL;
    }

    NxActor*          actor         = &closestShape->getActor();
    CPhysicUserData*  impactObject  = ( CPhysicUserData* )actor->userData;
    //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
    assert( impactObject );
    _Info.m_fDistance       = hit.distance;
    _Info.m_Normal          = Math::Vect3f( hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z );
    _Info.m_CollisionPoint  = Math::Vect3f( hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z );
    Math::Vect3f l_vDirection( _vDirRay.x - _vPosRay.x, _vDirRay.y - _vPosRay.y,
                               _vDirRay.z - _vPosRay.z );
    l_vDirection.Normalize();
    NxVec3 l_vDirectionVec( _vDirRay.x, _vDirRay.y, _vDirRay.z );
    NxF32 coeff = actor->getMass() * _fPower;
    actor->addForceAtLocalPos( l_vDirectionVec * coeff, NxVec3( 0, 0, 0 ), NX_IMPULSE, true );
    return impactObject;
}


std::string CPhysicsManager::RaycastClosestActorName( const Math::Vect3f oriRay, const Math::Vect3f& dirRay,
        uint32 impactMask )
{
    //NxUserRaycastReport::ALL_SHAPES
    ASSERT( m_pScene != NULL , "NULL SCENE" );
    NxRay ray;
    ray.dir =  NxVec3( dirRay.x, dirRay.y, dirRay.z );
    ray.orig = NxVec3( oriRay.x, oriRay.y, oriRay.z );
    NxRaycastHit hit;
    NxShape* closestShape = NULL;
    closestShape = m_pScene->raycastClosestShape( ray, NX_ALL_SHAPES, hit, impactMask, ( NxReal ) FLT_MAX );

    if ( !closestShape )
    {
        //No hemos tocado a ningún objeto físico de la escena.
        return std::string( "" );
    }

    NxActor*          actor         = &closestShape->getActor();
    CPhysicUserData*  impactObject  = ( CPhysicUserData* )actor->userData;

    ASSERT( impactObject, "NO IMPACTOBJECT" );
    return impactObject->GetName();
}


std::set<CPhysicUserData*> CPhysicsManager::OverlapSphere( float radiusSphere, const Math::Vect3f& posSphere,
        EShapesType shapeType /* = NX_ALL_SHAPES  */, uint32 impactMask/* =0xffffffff */ )
{
    // Check the scene
    assert( m_pScene );
    std::set<CPhysicUserData*> l_ImpactObjects;
    NxSphere l_CollisionSphere( NxVec3( posSphere.x, posSphere.y, posSphere.z ), radiusSphere );
    // Get the total of shapes in the scene
    NxU32 nbShapes = m_pScene->getTotalNbShapes();
    // Create a buffer at least the size of NxShape * nbShapes and clear the buffer
    NxShape** l_CollisionShapes = new NxShape* [nbShapes];
    memset( l_CollisionShapes, 0, sizeof( NxShape* ) * nbShapes );
    NxU32 l_NumShapesCollision = m_pScene->overlapSphereShapes( l_CollisionSphere,
                                 ( NxShapesType )shapeType, nbShapes, l_CollisionShapes, NULL, impactMask );

    for ( NxU32 i = 0; i < l_NumShapesCollision; ++i )
    {
        if ( l_CollisionShapes[i] )
        {
            NxActor* l_pActor = &( l_CollisionShapes[i]->getActor() );
            CPhysicUserData* l_pPhysicObject = ( CPhysicUserData* ) l_pActor->userData;
            assert( l_pPhysicObject );  // A PhyX object has been registered without ID
            l_ImpactObjects.insert( l_pPhysicObject );
        }
    }

    delete []l_CollisionShapes;
    return l_ImpactObjects;
}


std::vector<CPhysicUserData*> CPhysicsManager::OverlapSphereActor( float _fRadiusSphere,
        const Math::Vect3f& _vPosSphere, uint32 _uiImpactMask )
{
    assert( m_pScene );
    NxSphere l_WorldSphere( NxVec3( _vPosSphere.x, _vPosSphere.y, _vPosSphere.z ), _fRadiusSphere );
    //NxU32 nbShapes = m_pScene->getNbActors();
    // Hardcoded to get all the possible objects
    NxU32 nbShapes( 1024 );
    NxShape** shapes = new NxShape* [nbShapes];

    for ( NxU32 i = 0; i < nbShapes; i++ )
        shapes[i] = NULL;

    NxU32 l_NumShapesCollision = m_pScene->overlapSphereShapes( l_WorldSphere, NX_ALL_SHAPES, nbShapes, shapes, NULL,
                                 _uiImpactMask, NULL, true );
    std::vector<CPhysicUserData*> _ImpactObjects;

    for ( NxU32 i = 0; i < nbShapes; ++i )
    {
        if ( shapes[i] != NULL )
        {
            NxActor*          l_pActor        = &shapes[i]->getActor();
            CPhysicUserData*  l_pPhysicObject = ( CPhysicUserData* ) l_pActor->userData;
            //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle ID
            assert( l_pPhysicObject );
            //Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
            std::vector<CPhysicUserData*>::iterator it( _ImpactObjects.begin() ), itEnd( _ImpactObjects.end() );
            bool find = false;

            while ( it != itEnd )
            {
                CPhysicUserData* id = *it;

                if ( id == l_pPhysicObject )
                    find = true;

                ++it;
            }

            if ( !find )
                _ImpactObjects.push_back( l_pPhysicObject );
        }
    }

    delete shapes;
    return _ImpactObjects;
}

std::vector<CPhysicUserData*> CPhysicsManager::OverlapSphereController( float _fRadiusSphere,
        const Math::Vect3f& _vPosSphere, uint32 _uiImpactMask )
{
    std::vector<CPhysicUserData*> l_Result;
    std::vector<CPhysicUserData*> l_OverlapSphere = OverlapSphereActor( _fRadiusSphere, _vPosSphere, _uiImpactMask );

    std::vector<CPhysicUserData*>::iterator it      = l_OverlapSphere.begin(),
                                            it_end  = l_OverlapSphere.end();

    for ( ; it != it_end; ++it )
    {
        CPhysicUserData* l_UserData = *it;
        CPhysicController* l_Controller = l_UserData->GetController();

        if ( l_Controller )
            l_Result.push_back( l_UserData );

    }

    return l_Result;
}

std::vector<CPhysicUserData*> CPhysicsManager::OverlapConeActor( float _Distance, float _Angle,
        const Math::Vect3f& _Position, const Math::Vect3f& _Direction, uint32 _uiImpactMask )
{
    std::vector<CPhysicUserData*> l_Result;
    std::vector<CPhysicUserData*> l_OverlapSphere = OverlapSphereActor( _Distance, _Position, _uiImpactMask );

    float lenSq2 = _Direction.x * _Direction.x + _Direction.y * _Direction.y + _Direction.z * _Direction.z;

    std::vector<CPhysicUserData*>::iterator it      = l_OverlapSphere.begin(),
                                            it_end  = l_OverlapSphere.end();

    for ( ; it != it_end; ++it )
    {
        CPhysicUserData*    l_UserData    = *it;
        CPhysicController*  l_Controller  = l_UserData->GetController();
        CPhysicActor*       l_Actor       = l_UserData->GetActor();

        Math::Vect3f l_ActorPos;

        if ( l_Controller )
            l_ActorPos = l_Controller->GetPosition();
        else if ( l_Actor )
            l_ActorPos = l_Actor->GetPosition();
        else
            continue;

        Math::Vect3f l_VectToActor = l_ActorPos - _Position;
        l_VectToActor.Normalize();

        float dot     = l_VectToActor.DotProduct( _Direction );
        float lenSq1  = l_VectToActor.x * l_VectToActor.x + l_VectToActor.y * l_VectToActor.y + l_VectToActor.z *
                        l_VectToActor.z;
        float angle   = Math::Utils::Rad2Deg( acos( dot / sqrt( lenSq1 * lenSq2 ) ) );

        if ( angle < _Angle )
            l_Result.push_back( l_UserData );
    }

    return l_Result;
}

void CPhysicsManager::OverlapSphereActorGrenade( float radiusSphere, const Math::Vect3f& posSphere,
        std::vector<CPhysicUserData*> impactObjects, float _fPower )
{
    assert( m_pScene );
    NxSphere  worldSphere( NxVec3( posSphere.x, posSphere.y, posSphere.z ), radiusSphere );
    NxU32     nbShapes  = m_pScene->getNbDynamicShapes();
    NxShape** shapes    = new NxShape* [nbShapes];

    for ( NxU32 i = 0; i < nbShapes; i++ )
        shapes[i] = NULL;

    //NX_DYNAMIC_SHAPES
    m_pScene->overlapSphereShapes( worldSphere, NX_DYNAMIC_SHAPES, nbShapes, shapes, NULL );

    for ( NxU32 i = 0; i < nbShapes; i++ )
    {
        if ( shapes[i] != NULL )
        {
            NxActor*          actor         = &shapes[i]->getActor();
            CPhysicUserData*  physicObject  = ( CPhysicUserData* )actor->userData;
            //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle ID
            assert( physicObject );
            //Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
            std::vector<CPhysicUserData*>::iterator it( impactObjects.begin() ),
                itEnd( impactObjects.end() );
            bool find = false;

            while ( it != itEnd )
            {
                CPhysicUserData* id = *it;

                if ( id == physicObject )
                    find = true;

                ++it;
            }

            if ( !find )
            {
                impactObjects.push_back( physicObject );
                physicObject->SetColor( colRED );
                ApplyExplosion( actor, posSphere, radiusSphere, _fPower );
            }
        }

        //delete &shapes[i];
    }

    delete shapes;
    /*  for (NxU32 i = 0; i < nbShapes; i++)
        {
        delete &shapes[i];
        }*/
}

void CPhysicsManager::ApplyExplosion( NxActor* _pActor, const Math::Vect3f& _vPosSphere,
                                      float _fEffectRadius, float _fPower )
{
    NxVec3 l_vPos                       = _pActor->getGlobalPosition();
    Math::Vect3f l_vActorPosition       = Math::Vect3f( l_vPos.x, l_vPos.y, l_vPos.z );
    Math::Vect3f l_vVelocityDirection   = l_vActorPosition - _vPosSphere;
    l_vVelocityDirection.Normalize();

    float l_fDistance   = _vPosSphere.Distance( l_vActorPosition );
    float l_fTotalPower = _fPower * ( ( _fEffectRadius - l_fDistance ) / _fEffectRadius );

    NxF32 coeff         = _pActor->getMass() * l_fTotalPower;
    NxVec3 l_vDirection( l_vVelocityDirection.x, l_vVelocityDirection.y, l_vVelocityDirection.z );
    _pActor->addForceAtLocalPos( l_vDirection * coeff, NxVec3( 0, 0, 0 ), NX_IMPULSE, true );
}

// -----------------------------------------
//        PROPIEDADES
// -----------------------------------------

void CPhysicsManager::SetTriggerReport( CPhysicTriggerReport* _Report )
{
    assert( m_pScene );
    assert( _Report );
    NxUserTriggerReport* nxUserTriggerReport = ( NxUserTriggerReport* ) _Report;
    m_pScene->setUserTriggerReport( nxUserTriggerReport );
}

void CPhysicsManager::SetCollisionReport( CPhysicCollisionReport* _Report )
{
    assert( m_pScene );
    assert( _Report );
    NxUserContactReport* nxContactReport = ( NxUserContactReport* ) _Report;
    m_pScene->setUserContactReport( nxContactReport );
}


// TODO!!
CPhysicActor* CPhysicsManager::GetActor( std::string _ActorName )
{
    assert( m_pScene );
    //NxActor** l_ppActorList = m_pScene->getActors();
    //NxU32 l_TotalActors   = m_pScene->getNbActors();
    //
    CPhysicActor* l_pActor  = NULL;
    int           nbActors  = m_pScene->getNbActors();
    NxActor**     l_pActors = m_pScene->getActors();

    while ( nbActors-- )
    {
        NxActor*          l_pPhysicActor    = *l_pActors++;
        CPhysicUserData*  l_pPhysicUserData = NULL;
        l_pPhysicUserData = ( CPhysicUserData* ) l_pPhysicActor->userData;
        //Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
        assert( l_pPhysicUserData );

        if ( 0 == strcmp( l_pPhysicUserData->GetName().c_str() , _ActorName.c_str() ) )
        {
            //  l_pActor = new CPhysicActor(l_pPhysicActor);
            l_pActor = l_pPhysicUserData->GetActor(); // Linea de código añadida por XMA
            break;
        }
    }

    return l_pActor;
}

int CPhysicsManager::GetCollisionGroup( const std::string& _szGroup )
{
    if ( _szGroup.compare( "ECG_ESCENE" ) == 0 )
        return ECG_ESCENE;
    else if ( _szGroup.compare( "ECG_PLAYER" ) == 0 )
        return ECG_PLAYER;
    else if ( _szGroup.compare( "ECG_ENEMY" ) == 0 )
        return ECG_ENEMY;
    else if ( _szGroup.compare( "ECG_TRIGGERS" ) == 0 )
        return ECG_TRIGGERS;
    else if ( _szGroup.compare( "ECG_MALGLOBAL" ) == 0 )
        return ECG_MALGLOBAL;
    else if ( _szGroup.compare( "ECG_COBERTURES" ) == 0 )
        return ECG_COBERTURES;
    else if ( _szGroup.compare( "ECG_DYNAMIC_OBJECTS" ) == 0 )
        return ECG_DYNAMIC_OBJECTS;
    else if ( _szGroup.compare( "ECG_EXPLOSIONS" ) == 0 )
        return ECG_EXPLOSIONS;
    else if ( _szGroup.compare( "ECG_RAY_SHOOT" ) == 0 )
        return ECG_RAY_SHOOT;
    else if ( _szGroup.compare( "ECG_RAY_SHOOT_PLAYER" ) == 0 )
        return ECG_RAY_SHOOT_PLAYER;
    else if ( _szGroup.compare( "ECG_FORCE" ) == 0 )
        return ECG_FORCE;
    else if ( _szGroup.compare( "ECG_CAMERA" ) == 0 )
        return ECG_CAMERA;
    else if ( _szGroup.compare( "ECG_RAGDOLL" ) == 0 )
        return ECG_RAGDOLL;
    else if ( _szGroup.compare( "ECG_RAGDOLL_PLAYER" ) == 0 )
        return ECG_RAGDOLL_PLAYER;
    else if ( _szGroup.compare( "ECG_RAY_IA_GRAPH" ) == 0 )
        return ECG_RAY_IA_GRAPH;
    else if ( _szGroup.compare( "ECG_VIGIA" ) == 0 )
        return ECG_VIGIA;
    else if ( _szGroup.compare( "ECG_LIMITS" ) == 0 )
        return ECG_LIMITS;
    else if ( _szGroup.compare( "ECG_STATIC_OBJECTS" ) == 0 )
        return ECG_STATIC_OBJECTS;
    else
        return 0;
}

ECollisionGroup CPhysicsManager::GetCollisionGroup( const int _szGroup )
{
    if ( _szGroup == 0 )
        return ECG_ESCENE;
    else if ( _szGroup == 1 )
        return ECG_PLAYER;
    else if ( _szGroup == 2 )
        return ECG_ENEMY;
    else if ( _szGroup == 3 )
        return ECG_TRIGGERS;
    else if ( _szGroup == 4 )
        return ECG_MALGLOBAL;
    else if ( _szGroup == 5 )
        return ECG_COBERTURES;
    else if ( _szGroup == 6 )
        return ECG_DYNAMIC_OBJECTS;
    else if ( _szGroup == 7 )
        return ECG_EXPLOSIONS;
    else if ( _szGroup == 8 )
        return ECG_RAY_SHOOT;
    else if ( _szGroup == 9 )
        return ECG_RAY_SHOOT_PLAYER;
    else if ( _szGroup == 10 )
        return ECG_FORCE;
    else if ( _szGroup == 11 )
        return ECG_CAMERA;
    else if ( _szGroup == 12 )
        return ECG_RAGDOLL;
    else if ( _szGroup == 13 )
        return ECG_RAGDOLL_PLAYER;
    else if ( _szGroup == 14 )
        return ECG_RAY_IA_GRAPH;
    else if ( _szGroup == 15 )
        return ECG_VIGIA;
    else if ( _szGroup == 16 )
        return ECG_LIMITS;
    else if ( _szGroup == 17 )
        return ECG_STATIC_OBJECTS;
    else
        return ECG_ESCENE;
}

bool CPhysicsManager::CompareUserDatas( CPhysicUserData* _pUserData1, CPhysicUserData* _pUserData2 )
{
    if ( _pUserData1 == _pUserData2 )
        return true;

    return false;
}

bool CPhysicsManager::CreateMeshFromXML( const std::string& FileName )
{
    m_FileName = FileName;
    CXMLTreeNode newFile;

    if ( !newFile.LoadFile( FileName.c_str() ) )
    {
        LOG_WARNING_APPLICATION( "ERROR loading the file %s.\n", FileName );
        return false;
    }

    CXMLTreeNode  m = newFile["PhyX"];

    if ( m.Exists() )
    {
        std::string folder  =  m( 0 ).GetAttribute<std::string>( "folder" , "" );
        std::string path    = "./assets/PhX/" + folder + "/";
        unsigned int count  = ( unsigned int )m.GetNumChildren();

        for ( size_t i = 1; i < count; ++i )
        {
            //path viejo - "./assets/data/ases/barrioRico/"
            //path nuevo - "./assets/PhX/Fisicas_arturo/"
            //std::string path = "./assets/PhX/Fisicas_arturo/";
            std::string name = m( i ).GetAttribute<std::string>( "name", "" );
            std::string file = path + name + ".ASE";
            // Guardar id elemento
            m_vIds.insert( std::pair<std::string, unsigned int>( name, i - 1 ) );
            /********************/
            CPhysicUserData* l_pUserData = new CPhysicUserData( name );
            l_pUserData->SetName( name );
            l_pUserData->SetPaint( true );
            l_pUserData->SetColor( colGREEN );
            l_pUserData->SetGroup( ECG_ESCENE );
            m_vUsersData.push_back( l_pUserData );
            CPhysicActor* l_pSceneMesh = new CPhysicActor( l_pUserData );
            m_vActors.push_back( l_pSceneMesh );
            GetCookingMesh()->CreateMeshFromASE( file, name );
            l_pSceneMesh->AddMeshShape( GetCookingMesh()->GetPhysicMesh( name ) );
            AddPhysicActor( l_pSceneMesh );
        }
    }

    return true;
}

int CPhysicsManager::AddMaterial( float restitution, float staticFriction, float dynamicFriction )
{
    NxMaterialDesc l_MatDesc;
    l_MatDesc.restitution             = restitution;
    l_MatDesc.staticFriction          = staticFriction;
    l_MatDesc.dynamicFriction         = dynamicFriction;
    l_MatDesc.restitutionCombineMode  = NxCombineMode( 0 );
    return m_pScene->createMaterial( l_MatDesc )->getMaterialIndex();
}

CPhysicUserData* CPhysicsManager::GetUserData( const std::string& name )
{
    std::vector<CPhysicUserData*>::iterator it = m_vUD.begin();

    for ( ; it != m_vUD.end(); ++it )
        if ( ( *it )->GetName() == name )
            return *it;

    return 0;
}

std::set<CPhysicUserData*> CPhysicsManager::OverlapSphereHardcoded( float radiusSphere,
        const Math::Vect3f& posSphere )
{
    EShapesType shapeType   = ALL_SHAPES;
    uint32      impactMask  = 1 << ECG_ESCENE;
    // Check the scene
    assert( m_pScene );
    std::set<CPhysicUserData*> l_ImpactObjects;
    NxSphere l_CollisionSphere( NxVec3( posSphere.x, posSphere.y, posSphere.z ), radiusSphere );
    // Get the total of shapes in the scene
    NxU32 nbShapes = m_pScene->getTotalNbShapes();
    // Create a buffer at least the size of NxShape * nbShapes and clear the buffer
    NxShape** l_CollisionShapes = new NxShape* [nbShapes];
    memset( l_CollisionShapes, 0, sizeof( NxShape* ) * nbShapes );
    NxU32 l_NumShapesCollision = m_pScene->overlapSphereShapes( l_CollisionSphere,
                                 ( NxShapesType )shapeType, nbShapes, l_CollisionShapes, NULL, impactMask, NULL, true );

    for ( NxU32 i = 0; i < l_NumShapesCollision; ++i )
    {
        if ( l_CollisionShapes[i] )
        {
            NxActor* l_pActor = &( l_CollisionShapes[i]->getActor() );
            CPhysicUserData* l_pPhysicObject = ( CPhysicUserData* ) l_pActor->userData;
            assert( l_pPhysicObject );  // A PhyX object has been registered without ID
            l_ImpactObjects.insert( l_pPhysicObject );
        }
    }

    delete []l_CollisionShapes;
    return l_ImpactObjects;
}

bool CPhysicsManager::AddActor( const std::string& Name, const std::string& Type, const Math::Vect3f& _vDimension,
                                const Math::CColor& Color,
                                bool Paint, const Math::Vect3f& _vGlobalPos, const Math::Vect3f& _vLocalPos, const Math::Vect3f& rotation,
                                NxCCDSkeleton* _pSkeleton, uint32 _uiGroup )
{
    CPhysicUserData*  l_UserData = new CPhysicUserData( Name );
    l_UserData->SetColor( Color );
    l_UserData->SetPaint( Paint );
    CPhysicActor*     l_Actor    = new CPhysicActor( l_UserData );

    if ( Type == "Box" )
        l_Actor->AddBoxShape( _vDimension, _vGlobalPos, _vLocalPos, rotation, _pSkeleton, _uiGroup );
    else if ( Type == "Sphere" )
        l_Actor->AddSphereShape( _vDimension.x, _vGlobalPos, _vLocalPos, _pSkeleton, _uiGroup );
    else if ( Type == "Capsule" )
        l_Actor->AddCapsuleShape( _vDimension.x, _vDimension.y, _vGlobalPos, _vLocalPos, _pSkeleton, _uiGroup );
    else if ( Type == "Plane" )
        l_Actor->AddPlaneShape( _vGlobalPos, _vDimension.x, _uiGroup );

    if ( !CMapManager<CPhysicActor>::GetResource( Name ) )
    {
        if ( AddPhysicActor( l_Actor ) )
        {
            CMapManager<CPhysicActor>::AddResource( Name, l_Actor );
            m_vUD.push_back( l_UserData );
            return true;
        }
        else
        {
            CHECKED_DELETE( l_UserData );
            CHECKED_DELETE( l_Actor );
        }
    }

    return false;
}

bool CPhysicsManager::AddActorCapsule( const std::string& aName, float aRadius, float aHeight )
{
	return AddActor( aName, "Capsule", Vect3f(aRadius, aHeight, 0.0f));
}

bool CPhysicsManager::AddController( const std::string& Name, float radius, float height, float slope, float skin_width,
                                     float step,
                                     Math::Vect3f pos,
                                     ECollisionGroup ColliusionGroup, float gravity )
{
    CPhysicUserData*    l_UserData    = new CPhysicUserData( Name );
    CPhysicController*  l_Controller  = new CPhysicController( radius, height, slope, skin_width, step, ColliusionGroup,
            l_UserData, pos, gravity );

    if ( !CMapManager<CPhysicController>::GetResource( Name ) )
    {
        if ( AddPhysicController( l_Controller, l_Controller->GetType(), l_Controller->GetColisionGroup() ) )
        {
            CMapManager<CPhysicController>::AddResource( Name, l_Controller );
            m_vUD.push_back( l_UserData );
            return true;
        }
        else
        {
            CHECKED_DELETE( l_UserData );
            CHECKED_DELETE( l_Controller );
        }
    }
    else
    {
        CHECKED_DELETE( l_UserData );
        CHECKED_DELETE( l_Controller );
    }

    return false;
}

bool CPhysicsManager::AddMesh( const std::string& Path, const std::string& Name )
{
    if ( m_pCookingMesh->CreateMeshFromASE( Path, Name ) )
    {
        CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData( Name );
        l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );

        CPhysicActor* l_AseMeshActor  = new CPhysicActor( l_pPhysicUserDataASEMesh );
        VecMeshes     l_CookMeshes    = m_pCookingMesh->GetMeshes();

        for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
            l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );

        if ( !CMapManager<CPhysicActor>::GetResource( Name ) )
        {
            if ( AddPhysicActor( l_AseMeshActor ) )
            {
                CMapManager<CPhysicActor>::AddResource( Name, l_AseMeshActor );
                m_vUD.push_back( l_pPhysicUserDataASEMesh );
                return true;
            }
            else
            {
                CHECKED_DELETE( l_pPhysicUserDataASEMesh );
                CHECKED_DELETE( l_AseMeshActor );
            }
        }
    }

    return false;
}