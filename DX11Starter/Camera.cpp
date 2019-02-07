#include "Camera.h"
#include "Windows.h"

// For the DirectX Math library
using namespace DirectX;

Camera::Camera(float width, float height)
{
	cameraPosition = XMFLOAT3(0.0f, 0.0f, -5.0f);
	cameraDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);

	rotationX = 0;
	rotationY = 0;

	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	UpdateProjectionMatrix(width, height);
}

Camera::~Camera()
{

}

void Camera::Update(float deltaTime, float totalTime)
{
	XMVECTOR posVector = XMLoadFloat3(&cameraPosition);
	XMVECTOR dirVector = XMLoadFloat3(&cameraDirection);

	XMVECTOR rotVector = XMQuaternionRotationRollPitchYaw(rotationX, rotationY, 0);

	XMVECTOR faceVector = XMVector3Rotate(dirVector, rotVector);
	XMVECTOR upVector = XMVectorSet(0, 1, 0, 0);

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(XMMatrixLookToLH(posVector, faceVector, upVector)));

	if (GetAsyncKeyState('W') & 0x8000) 
	{
		XMVECTOR offsetVector = XMVectorSet(0, 0, deltaTime, 0);
		offsetVector = XMVector3Rotate(offsetVector, rotVector);
		XMStoreFloat3(&cameraPosition, posVector + offsetVector);
	}
	
	if (GetAsyncKeyState('S') & 0x8000) 
	{
		XMVECTOR offsetVector = XMVectorSet(0, 0, -deltaTime, 0);
		offsetVector = XMVector3Rotate(offsetVector, rotVector);
		XMStoreFloat3(&cameraPosition, posVector + offsetVector);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		XMVECTOR offsetVector = XMVector3Cross(faceVector, upVector);
		offsetVector = XMVector3Normalize(offsetVector) * deltaTime;
		XMStoreFloat3(&cameraPosition, posVector + offsetVector);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		XMVECTOR offsetVector = -XMVector3Cross(faceVector, upVector);
		offsetVector = XMVector3Normalize(offsetVector) * deltaTime;
		XMStoreFloat3(&cameraPosition, posVector + offsetVector);
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		XMVECTOR offsetVector = XMVectorSet(0, deltaTime, 0, 0);
		XMStoreFloat3(&cameraPosition, posVector + offsetVector);
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		XMVECTOR offsetVector = XMVectorSet(0, -deltaTime, 0, 0);
		XMStoreFloat3(&cameraPosition, posVector + offsetVector);
	}
}

void Camera::RotateCamera(float x, float y)
{
	rotationX += x;
	rotationY += y;
}

void Camera::UpdateProjectionMatrix(float width, float height)
{
	// Update the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		width / height,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}