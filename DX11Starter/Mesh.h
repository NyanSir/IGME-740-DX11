#pragma once

#include "DXCore.h"
#include <DirectXMath.h>

#include "Vertex.h"

class Mesh
{

public:
	Mesh(Vertex* vertices, int vCount, int* indices, int iCount, ID3D11Device* device);
	~Mesh();
	
	void Draw(ID3D11DeviceContext* context);

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

private:
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	int vertexCount;
	int indexCount;
};