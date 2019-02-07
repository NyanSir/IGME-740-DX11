#pragma once

#include "DXCore.h"
#include <DirectXMath.h>

#include "Mesh.h"

using namespace DirectX;

class Camera
{

public:
	Camera(float width, float height);
	~Camera();

	void Update(float deltaTime, float totalTime);

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();

private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	XMFLOAT3 cameraPosition;
	XMFLOAT3 cameraDirection;
	float rotationX;
	float rotationY;

};