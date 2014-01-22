/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "MeshSphere.h"


MeshSphere::MeshSphere(IDirect3DDevice9 *d3dDev, NxScene *scene,IDirect3DTexture9* tex, Vector &position, Vector &velocity, float radius):
MeshObject(d3dDev, scene, tex, position, velocity, radius, radius, radius)
{
	mesh = createMappedSphere(d3dDev, radius, 20, 20);
	if(scene != NULL)
	{
		actor = PhysicsEngine::physicsSphere(scene, position, velocity, radius,5.0f);
		actor->userData = this;
	}
}


MeshSphere::~MeshSphere(void)
{
}
LPD3DXMESH MeshSphere::createMappedSphere(IDirect3DDevice9 *d3dDev,float radius,UINT slices,UINT stacks)
{
    // create the sphere
    LPD3DXMESH mesh;
    if (FAILED(D3DXCreateSphere(d3dDev,radius*0.5f,slices,stacks,&mesh,NULL)))
        return NULL;

    // create a copy of the mesh with texture coordinates,
    // since the D3DX function doesn't include them
    LPD3DXMESH texMesh;
    if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM,FVF_VERTEX,d3dDev,&texMesh)))
        // failed, return un-textured mesh
        return mesh;

    // finished with the original mesh, release it
    mesh->Release();


	int numVerts=texMesh->GetNumVertices();
    MESH_VERTEX *vertex; 

	if (SUCCEEDED(texMesh->LockVertexBuffer(0,(void**)&vertex))){
        // loop through the vertices
        for (int i=0;i<numVerts;i++) 
		{
            // calculate texture coordinates
			vertex->tu=asinf(vertex->norm.x)/D3DX_PI+0.5f;
            vertex->tv=asinf(vertex->norm.y)/D3DX_PI+0.5f;

			//next vert
			vertex++;
        }

        // unlock the vertex buffer
		texMesh->UnlockVertexBuffer();
    }
  
    // return pointer to caller
    return texMesh;
}