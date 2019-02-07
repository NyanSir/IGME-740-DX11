#include "Material.h"

// For the DirectX Math library
using namespace DirectX;

Material::Material(SimpleVertexShader* vShader, SimplePixelShader* pShader)
{
	vertexShader = vShader;
	pixelShader = pShader;
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