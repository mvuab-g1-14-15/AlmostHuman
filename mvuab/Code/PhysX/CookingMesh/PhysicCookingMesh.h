//----------------------------------------------------------------------------------
// CPhysicCookingMesh class
// Author: Enric Vergara
//
// Description:
// Clase encargarda de gestionar las mallas triangulares utilizadas para la física
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_PHYSIC_COOKING_MESH_H_
#define INC_PHYSIC_COOKING_MESH_H_

#include <map>
#include <vector>
#include "Math\Vector3.h"
#include "Utils\MapManager.h"

//---Forward Declarations---
class NxPhysicsSDK;
class CPhysicUserAllocator;
class NxCookingInterface;
class NxTriangleMesh;
//Cloth
class NxStream;
class NxClothMeshDesc;
//--------------------------

using namespace std;
typedef std::map<std::string, NxTriangleMesh*> VecMeshes;

class CPhysicCookingMesh
{
public:
  

public:
  //--- Init and End protocols
  CPhysicCookingMesh( void );
  ~CPhysicCookingMesh( void )
  {
    Done();
  }

  bool            Init( NxPhysicsSDK* physicSDK, CPhysicUserAllocator* myAllocator );
  void            Done( void );
  bool            IsOk( void ) const
  {
    return m_bIsOk;
  }


  NxTriangleMesh*       GetPhysicMesh( const std::string& name );
  //const VecMeshes		&GetMeshes() { return m_TriangleMeshes; }
  const VecMeshes		&GetMeshes(std::string _NameRoom) { return m_TriangleMeshes.find(_NameRoom)->second; }


  bool            CreatePhysicMesh(const std::string& _BinFilename, const std::string& _NameMesh );
  NxTriangleMesh *CreatePhysicMesh( const std::vector<Math::Vect3f> &l_VB, const std::vector<uint32> &l_IB);


  bool            CreatePhysicMesh( std::string _NameMesh, std::vector<std::vector<Math::Vect3f>> &_Vertices, std::vector<std::vector<unsigned int>> &_Faces, std::string _NameRoom  );
  bool            CreatePhysicMesh( std::string _NameMesh, std::vector<Math::Vect3f>& _Vertices, std::vector<unsigned int>& _Faces, std::string _NameRoom, VecMeshes& lRoomVecMesh );

  bool            SavePhysicMesh( const std::vector<Math::Vect3f>& _Vertices, const std::vector<uint32>& _Faces, const std::string& _BinFilename );
  bool			  CreateMeshFromASE( std::string _FileName, std::string _Name,  std::string _RoomName);
 // bool            CreateMeshFromASE( std::string _FileName, std::string _Name );
  bool            CookClothMesh( const NxClothMeshDesc& _Desc, NxStream& _Stream );

  void            Release( void );
  bool            ReadMeshFromASE( std::string _FileName, std::vector<std::vector<Math::Vect3f>> &_Vertices, std::vector<std::vector<unsigned int>> &_Faces );
  void            ReleasePhysicMesh( const std::string& name );

private:

  bool						m_bIsOk;

  NxPhysicsSDK*				m_pPhysicSDK;
  CPhysicUserAllocator*		m_pMyAllocator;

  NxCookingInterface*		m_pCooking;
  std::map<std::string, VecMeshes>	m_TriangleMeshes;
};

#endif // INC_PHYSIC_COOKING_MESH_H_