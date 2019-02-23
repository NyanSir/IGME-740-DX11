#pragma once

#include <DirectXMath.h>

#include "Mesh.h"
#include "Material.h"

using namespace DirectX;

class GameEntity
{

public:
	GameEntity(Mesh* mesh, Material* material);
	~GameEntity();

	Mesh* mesh;
	Material* material;

	void PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* rView, ID3D11SamplerState* sState);

	void SetWorldMatrix(XMFLOAT4X4 matrix) ;
	void SetTranslation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	XMFLOAT4X4 GetWorldMatrix();

private:
	XMFLOAT4X4 worldMatrix;

	XMFLOAT3 transVector;
	XMFLOAT3 scaleVector;
	XMFLOAT3 rotVector;

	bool isWorldMatrixChanged;

};