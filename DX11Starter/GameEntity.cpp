#include "GameEntity.h"

// For the DirectX Math library
using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh) 
{
	this->mesh = mesh;

	transVector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotVector = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleVector = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
	
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
	XMVECTOR v = { x, y, z };
	XMStoreFloat3(&transVector, v);
	isWorldMatrixChanged = true;
}

void GameEntity::SetRotation(float x, float y, float z)
{
	XMVECTOR v = { x, y, z };
	XMStoreFloat3(&rotVector, v);
	isWorldMatrixChanged = true;
}

void GameEntity::SetScale(float x, float y, float z)
{
	XMVECTOR v = {x, y, z};
	XMStoreFloat3(&scaleVector, v);
	isWorldMatrixChanged = true;
}

XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	if (isWorldMatrixChanged) {
		XMMATRIX trans = XMMatrixTranslation(transVector.x, transVector.y, transVector.z);
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotVector.x, rotVector.y, rotVector.z);
		XMMATRIX scale = XMMatrixScaling(scaleVector.x, scaleVector.y, scaleVector.z);
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(scale * rot * trans));
		isWorldMatrixChanged = false;
	}
	return worldMatrix;
}