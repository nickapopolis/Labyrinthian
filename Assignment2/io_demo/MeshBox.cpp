/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "MeshBox.h"


MeshBox::MeshBox(IDirect3DDevice9 *d3dDev, NxScene *scene,IDirect3DTexture9* tex, Vector &position, Vector &velocity, float width, float height, float depth) :
MeshObject(d3dDev, scene, tex, position, velocity, width, height, depth)
{
	mesh = createMappedBox(d3dDev, width, height, depth);
	actor = PhysicsEngine::physicsBox(scene, position, velocity, width, height, depth,5.0f);
	actor->userData = this;
}


MeshBox::~MeshBox(void)
{
}
LPD3DXMESH MeshBox::createMappedBox(IDirect3DDevice9 *d3dDev,float width, float height,float depth)
{
	// create the sphere
    LPD3DXMESH mesh;
    if (FAILED(D3DXCreateBox(d3dDev,width, height, depth, &mesh, NULL)))
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
        for (int i=0;i<numVerts;i+=4) 
		{ 
            // calculate texture coordinates
			
			vertex->tu= 0.0f;
            vertex->tv= 0.0f;
			vertex++;

			vertex->tu= 1.0f;
            vertex->tv= 0.0f;
			vertex++;

			vertex->tu= 1.0f;
            vertex->tv= 1.0f;
			vertex++;

			vertex->tu= 0.0f;
            vertex->tv= 1.0f;
			vertex++;
        }



        // unlock the vertex buffer
		texMesh->UnlockVertexBuffer();
    }
  
    // return pointer to caller
    return texMesh;

}
