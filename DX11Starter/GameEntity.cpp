#include "GameEntity.h"

// For the DirectX Math library
using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;

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

void GameEntity::PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* rView, ID3D11SamplerState* sState)
{
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	material->GetVertexShader()->SetMatrix4x4("world", GetWorldMatrix());
	material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	material->GetVertexShader()->CopyAllBufferData();
	material->GetPixelShader()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

	material->GetPixelShader()->SetSamplerState("BasicSampler", sState);
	material->GetPixelShader()->SetShaderResourceView("DiffuseTexture", rView);
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