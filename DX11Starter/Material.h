#pragma once

#include "SimpleShader.h"

using namespace DirectX;

class Material
{

public:
	Material(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11ShaderResourceView* rView, ID3D11SamplerState* sState);
	~Material();

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

private:
	// Buffers to hold actual geometry data
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	//Textures
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* samplerState;

};