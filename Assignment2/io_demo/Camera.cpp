/*
Author: Nick Tierney
*/


#include "StdAfx.h"
#include "Camera.h"


Camera::Camera(IDirect3DDevice9 *d3dDev, int wWidth, int wHeight)
	:eye(NULL), lookAt(NULL), up(NULL)
{
	md3dDev = d3dDev;
	windowWidth = wWidth;
	windowHeight = wHeight;

	//eye, lookat, up
	setViewMatrix(D3DXVECTOR3(-6.0f, 5.0f,-6.0f), D3DXVECTOR3(16.0f, 0.0f, 16.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	setProjectionMatrix();
}


Camera::~Camera(void)
{
	delete eye;
	delete lookAt;
	delete up;
}



void Camera::setViewMatrix(D3DXVECTOR3& eye, D3DXVECTOR3& lookAt, D3DXVECTOR3& up) {
	this->eye = &eye;
	this->lookAt = &lookAt;
	this->up = &up;
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &lookAt, &up);
	md3dDev->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void Camera::setProjectionMatrix(float fov, float aspectRatio, float zClose, float zFar) {
	D3DXMATRIXA16 projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, aspectRatio, zClose, zFar);
	md3dDev->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void  Camera::setViewMatrix(D3DXVECTOR3 vector[3]) { 
	setViewMatrix(vector[0], vector[1], vector[2]); 
}

void  Camera::setProjectionMatrix(float fov, float zClose, float zFar) 
{
	setProjectionMatrix(fov, windowWidth / (float)windowHeight, zClose, zFar);
}
void  Camera::setProjectionMatrix(float zClose, float zFar) 
{
	setProjectionMatrix(D3DX_PI / 4.0f, windowWidth / (float)windowHeight, zClose, zFar);
}
void  Camera::setProjectionMatrix()
{
	setProjectionMatrix(D3DX_PI / 4.0f, windowWidth/ (float)windowHeight, 0.01f, 100.0f);
}