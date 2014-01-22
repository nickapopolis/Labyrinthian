/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "MeshModel.h"


MeshModel::MeshModel(IDirect3DDevice9 *d3dDev, NxScene *scene, Vector &position, char* dir, char* meshFileName, float width, float height, float depth, bool collision)
	:MeshObject(d3dDev, scene, NULL, position, Vector(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f)
{
	createMesh(d3dDev, dir, meshFileName);
	if(collision)
	{
		//pos = position;
		actor = PhysicsEngine::physicsSphere(scene, position, Vector(0,0,0), width, 5.0f);
		actor->userData = this;
		actor->raiseBodyFlag(NX_BF_KINEMATIC);
	}
	else 
	{
		pos = position;
		actor = NULL;
	}
	scaleX = 1.0f;
	scaleY= 1.0f;
	scaleZ= 1.0f;

	rotX= 0.0f;
	rotY= 0.0f;
	rotZ= 0.0f;
}

MeshModel::~MeshModel(void)
{
	if(m_pMeshTexturesBuf != NULL)
		delete[] m_pMeshTexturesBuf;
	if(m_pMeshMaterials != NULL)
		delete m_pMeshMaterials;
}

/***********************************************************************************/

// load a mesh from files.
// provide the directory name and the file name;

int MeshModel::createMesh(IDirect3DDevice9 *d3dDev, char *dir, char * meshFileName)
{
	// similar to MS tutorial
	int rc = 0;
	int i;
	int count;
	char  fileName[1024] = "";
	LPD3DXBUFFER pMaterialBuf;
	//int numMaterials;

    // Load the mesh from the specified file
	count = strlen(dir)+strlen(meshFileName);
	if (count >1020) {
		rc = -1;
		goto err;
	}
	strcpy_s(fileName,dir);
	strcat_s(fileName,"\\");
	strcat_s(fileName,meshFileName);

    rc = D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, d3dDev, NULL,
		&pMaterialBuf, NULL,&mNumMaterials, &mesh);

	if (rc != D3D_OK) {
		rc = 1;
		MessageBox( NULL, "Could not load mesh", "initGeom", MB_OK );
		goto err;
	}

    // Extract the material properties and texture names stored in the mesh file (pMaterialBuf)
    D3DXMATERIAL* pMaterials = (D3DXMATERIAL *)pMaterialBuf->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[mNumMaterials];
	if( m_pMeshMaterials == NULL ) {
		rc = 2;
		goto err;
	}

    m_pMeshTexturesBuf = new LPDIRECT3DTEXTURE9[mNumMaterials];
	if(m_pMeshTexturesBuf == NULL ) {
		rc = 3;
		goto err;
	}

    for(i = 0; i < (int)mNumMaterials; i++ )   {
		// Copy the material
        m_pMeshMaterials[i] = pMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        m_pMeshTexturesBuf[i] = NULL;
        if (pMaterials[i].pTextureFilename != NULL && lstrlenA(pMaterials[i].pTextureFilename)>0){
			// create the file name
			count = strlen(dir)+strlen(pMaterials[i].pTextureFilename);
			if (count >1022) {
				rc = -1;
				goto err;
			}
			strcpy_s(fileName,dir);
			strcat_s(fileName,"\\");
			strcat_s(fileName,pMaterials[i].pTextureFilename);
				
			// Create the texture
			rc = D3DXCreateTextureFromFileA(d3dDev, fileName, &m_pMeshTexturesBuf[i]);
			if (rc != 0) {
				// If texture is not in current folder, try parent folder
				MessageBox( NULL, "Mesh texture not found!", "Load Mesh", MB_OK );
				rc = 4;
				goto err;
			}
		}
	}

    // Done with the material buffer
	if(pMaterialBuf != NULL)
		pMaterialBuf->Release();

err:
	return(rc);
}
Vector MeshModel::getPos()
{
	if(actor ==NULL)
		return pos;
	else return MeshObject::getPos();
}
void MeshModel::drawObject(IDirect3DDevice9 *d3dDev)
{
	int i;
	D3DXMATRIX worldMat, matScale, matRotation, matTranslate;
	//scaling
	D3DXMatrixScaling(&matScale,scaleX, scaleY, scaleZ);
	worldMat = matScale;

	//rotation
	D3DXMatrixRotationX(&matRotation, rotX);

	worldMat *= matRotation;

	//translation
	Vector position = getPos();
	D3DXMatrixTranslation(&matTranslate, pos.x, pos.y, pos.z);
	worldMat *= matTranslate;

	//final matrix = ISROT, identity * scale * rotation * orbit * translation
	d3dDev->SetTransform(D3DTS_WORLD, &worldMat);
	//set texture
	d3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	d3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//draw object
	
	for(i = 0; i < (int)mNumMaterials; i++ )   
	{
		// Set the material and texture for this subset
		d3dDev->SetMaterial(&m_pMeshMaterials[i]);
		d3dDev->SetTexture(0, m_pMeshTexturesBuf[i]);  // problem with texture

        // Draw the mesh subset
        mesh->DrawSubset(i);
	}
}
