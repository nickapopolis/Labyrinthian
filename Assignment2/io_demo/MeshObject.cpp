/*
Author: Nick Tierney
*/

#include "StdAfx.h"
#include "MeshObject.h"


MeshObject::MeshObject(IDirect3DDevice9 *d3dDev, NxScene *scene,IDirect3DTexture9* tex, Vector &position, Vector &velocity, float width, float height, float depth)
{
	actor = NULL;
	texture = tex;
}


MeshObject::~MeshObject(void)
{
	release();
}

float MeshObject::getX()
{
	return actor->getGlobalPose().t.x;
}
float MeshObject::getY()
{
	return actor->getGlobalPose().t.y;
}
float MeshObject::getZ()
{
	return actor->getGlobalPose().t.z;
}
float MeshObject::getRotX()
{
	float rotX = 0;
	actor->getGlobalOrientation().rotX(rotX);
	return rotX;
}
float MeshObject::getRotY()
{
	float rotY = 0;
	actor->getGlobalOrientation().rotY(rotY);
	return rotY;
}
float MeshObject::getRotZ()
{
	float rotZ = 0;
	actor->getGlobalOrientation().rotZ(rotZ);
	return rotZ;
}
Vector MeshObject::getPos()
{
	return Vector(getX(), getY(), getZ());
}
string MeshObject::toString()
{
	string objectAsString = "";
	stringstream ss (stringstream::in | stringstream::out);
	ss << getX();
	objectAsString += "("+ ss.str() +",";
	ss.str("");
	ss.clear();
	ss <<getY();
	objectAsString += ss.str() + ",";
	ss.str("");
	ss.clear();
	ss <<getZ();
	objectAsString += ss.str() + ")";
	
	return objectAsString;
}

void MeshObject::drawObject(IDirect3DDevice9 *d3dDev)
{
	D3DXMATRIX worldMat, viewMat, matTransform, matProjection, matScale, matTranslate,  matRotation;

	if(actor == NULL)
	{
		return;
	}
	d3dDev->SetTexture(0, texture);
	D3DMATERIAL9 material;
	d3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    d3dDev->SetMaterial(&material);

	D3DXMATRIX d3dMat;
	actor->getGlobalPose().getColumnMajor44( d3dMat );
	d3dDev->SetTransform( D3DTS_WORLD, &d3dMat);
	mesh->DrawSubset(0);
}
void MeshObject::drawObjectShader(IDirect3DDevice9 *d3dDev)
{
	
	int rc;
    static int angle = 0;

	static float step = 1;
	float rad = 0;
	float fTime;
	int counter = 0;
	static float earthAngle = 0.0;

	int i=0, j=0, k=0;
	unsigned int numPasses = 0;		// containes the number of passes of the technique

	d3dDev->SetTexture(0, texture);

	D3DXMATRIX d3dMat, worldMat, viewMat, projMat, worldInverseTransposeMat;
	actor->getGlobalPose().getColumnMajor44( d3dMat );
	d3dDev->SetTransform( D3DTS_WORLD, &d3dMat);

	//get transformation matrices to pass into shader
	d3dDev->GetTransform(D3DTS_WORLD, &worldMat);
	d3dDev->GetTransform(D3DTS_PROJECTION, &projMat);
	d3dDev->GetTransform(D3DTS_VIEW, &viewMat);

	// get the inverse transpose
	D3DXMatrixInverse(&worldInverseTransposeMat, NULL, &worldMat);
	D3DXMatrixTranspose(&worldInverseTransposeMat, &worldInverseTransposeMat);

	// set the technique
	rc = shaderEffect->SetTechnique(0);

	// set the global matrix
	shaderEffect->SetMatrix(hWorldMat, &worldMat);
	shaderEffect->SetMatrix(hWorldInverseTransposeMat, &worldInverseTransposeMat);
	shaderEffect->SetMatrix(hViewMat, &viewMat);
	shaderEffect->SetMatrix(hProjMat, &projMat);
	shaderEffect-> SetTexture(hTexture, texture);

	// start the effect
	shaderEffect->Begin(&numPasses, 0);		// 0 means save and restore the device states

	// start the technique passess
	for (i = 0; i < numPasses; i++) {

		shaderEffect->BeginPass(i);

		// draw the mesh
		mesh->DrawSubset(0);

		shaderEffect->EndPass();
	}

	shaderEffect->End();

   
}

void MeshObject::release()
{
	mesh->Release();
}