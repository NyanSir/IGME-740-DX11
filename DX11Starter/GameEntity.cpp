#include "DXCore.h"
#include <DirectXMath.h>

#include "GameEntity.h"
#include "Mesh.h"

// For the DirectX Math library
using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh) 
{
	this->mesh = mesh;

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	XMStoreFloat4x4(&transMatrix, XMMatrixTranspose(W));
	XMStoreFloat4x4(&rotMatrix, XMMatrixTranspose(W));
	XMStoreFloat4x4(&scaleMatrix, XMMatrixTranspose(W));
	
	isWorldMatrixChanged = false;
}

GameEntity::~GameEntity() 
{
	
}

void GameEntity::SetWorldMatrix(XMFLOAT4X4 matrix)
{
	worldMatrix = matrix;
}

void GameEntity::SetTranslation(float x, float y, float z)
{	
	XMStoreFloat4x4(&transMatrix, XMMatrixTranslation(x, y, z));
	isWorldMatrixChanged = true;
}

void GameEntity::SetRotation(float x, float y, float z)
{
	XMStoreFloat4x4(&rotMatrix, XMMatrixTranslation(x, y, z));
	isWorldMatrixChanged = true;
}

void GameEntity::SetScale(float x, float y, float z)
{
	XMStoreFloat4x4(&scaleMatrix, XMMatrixScaling(x, y, z));
	isWorldMatrixChanged = true;
}

XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	if (isWorldMatrixChanged) {
		XMMATRIX trans = XMLoadFloat4x4(&transMatrix);
		XMMATRIX rot = XMLoadFloat4x4(&rotMatrix);
		XMMATRIX scale = XMLoadFloat4x4(&scaleMatrix);
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(trans * rot * scale));
		isWorldMatrixChanged = false;
	}
	return worldMatrix;
}