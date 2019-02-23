#include "Material.h"

// For the DirectX Math library
using namespace DirectX;

Material::Material(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11ShaderResourceView* rView, ID3D11SamplerState* sState)
{
	vertexShader = vShader;
	pixelShader = pShader;
	resourceView = rView;
	samplerState = sState;
}

Material::~Material()
{
	
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}