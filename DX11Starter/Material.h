#pragma once

#include "SimpleShader.h"

using namespace DirectX;

class Material
{

public:
	Material();
	~Material();

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

private:
	// Buffers to hold actual geometry data
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

};