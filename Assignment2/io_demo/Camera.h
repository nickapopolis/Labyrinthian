#pragma once
#include "stdafx.h"

class Camera
{
public:
	Camera(IDirect3DDevice9 *d3dDev, int wWidth, int wHeight);
	~Camera(void);
	
	void setViewMatrix(D3DXVECTOR3 vector[3]);
	void setViewMatrix(D3DXVECTOR3& eye, D3DXVECTOR3& lookAt, D3DXVECTOR3& up);

	void setProjectionMatrix(float fov, float aspectRatio, float zClose, float zFar);
	void setProjectionMatrix(float fov, float zClose, float zFar);
	void setProjectionMatrix(float zClose, float zFar);
	void setProjectionMatrix();

protected:
	IDirect3DDevice9 *md3dDev;
	D3DXVECTOR3 *eye; 
	D3DXVECTOR3 *lookAt; 
	D3DXVECTOR3 *up;
	int windowWidth;
	int windowHeight;

};

