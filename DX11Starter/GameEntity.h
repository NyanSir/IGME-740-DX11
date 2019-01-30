#pragma once

#include "DXCore.h"
#include <DirectXMath.h>

#include "Mesh.h"

class GameEntity
{

public:
	GameEntity(Mesh* mesh);
	~GameEntity();

	Mesh* mesh;

	void SetWorldMatrix(XMFLOAT4X4 matrix);
	void SetTranslation(XMFLOAT3 translation);
	void SetScale(XMFLOAT3 scale);

	XMFLOAT4X4 GetWorldMatrix();

private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 positionVector;
	XMFLOAT3 scaleVector;
	XMFLOAT3 rotationQuat;

};