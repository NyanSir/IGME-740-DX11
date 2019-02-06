#include "DXCore.h"
#include <DirectXMath.h>

#include "Camera.h"

// For the DirectX Math library
using namespace DirectX;

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Update()
{
	XMVECTOR rotVector = XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 0);

	//(viewMatrix, XMMatrixLookToLH(cameraPosition, cameraDirection, ));
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}