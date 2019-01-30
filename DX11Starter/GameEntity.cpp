#include "GameEntity.h"
#include "Mesh.h"
#include <DirectXMath.h>

// For the DirectX Math library
using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh) 
{
	this->mesh = mesh;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	
}

GameEntity::~GameEntity() 
{
	
}

void GameEntity::SetWorldMatrix(XMFLOAT4X4 matrix)
{
	worldMatrix = matrix;
}

void GameEntity::SetTranslation(XMFLOAT3 translation)
{
	
}

void GameEntity::SetScale(XMFLOAT3 scale)
{
	
}

XMFLOAT4X4 GameEntity::GetWorldMatrix()
{
	return worldMatrix;
}