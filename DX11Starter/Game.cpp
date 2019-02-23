#include "Game.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields;
	vertexShader = 0;
	pixelShader = 0;

	triangle = 0;
	square = 0;
	hexagon = 0;

	for (int i = 0; i < 6; i++) {
		models[i] = 0;
	}

	for (int i = 0; i < 5; i++) {
		gameEntities[i] = 0;
	}

	camera = new Camera((float)width, (float)height);

	directionalLight_1 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0, 0, 1, 1), XMFLOAT3(1, -1, 0) };
	directionalLight_2 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(1, 0, 0, 1), XMFLOAT3(-1, 1, 0) };

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete triangle;
	delete square;
	delete hexagon;

	for (int i = 0; i < 6; i++)
	{
		delete models[i];
	}

	for (int i = 0; i < 5; i++) 
	{
		delete gameEntities[i];
	}

	delete camera;

	delete defaultMaterial;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelShader->SetData(
		"light_1",					// The name of the (eventual) variable in the shader
		&directionalLight_1,		// The address of the data to copy
		sizeof(DirectionalLight));	// The size of the data to copy

	pixelShader->SetData(
		"light_2",					// The name of the (eventual) variable in the shader
		&directionalLight_2,		// The address of the data to copy
		sizeof(DirectionalLight));	// The size of the data to copy


	// Load some textures
	CreateWICTextureFromFile(
		device,									// The Direct3D device for resource creation
		context,								// Rendering context (this will auto-generate mip maps!!!)
		L"../../Assets/Textures/SmoothRock.jpg",// Path to the file ("L" means wide characters)
		0,										// Texture ref?  No thanks!  (0 means we don't want an extra ref)
		&defaultSRV);							// Actual SRV for use with shaders

	// Create a sampler state for sampling options
	D3D11_SAMPLER_DESC sampDesc = {};					// " = {}" fills the whole struct with zeros!
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// Tri-linear filtering
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;				// This will ensure mip maps are used!
	//sampDesc.MaxAnisotropy = 16;						// Must be set for anisotropic filtering

	device->CreateSamplerState(&sampDesc, &sampler);

	//Create a default material
	defaultMaterial = new Material(vertexShader, pixelShader, defaultSRV, sampler);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	DirectX::XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	DirectX::XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	XMFLOAT3 normal = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uv = XMFLOAT2(0, 0);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex triangleVertices[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.5, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), normal, uv },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	UINT triangleIndices[] = { 0, 1, 2 };

	triangle = new Mesh(triangleVertices, 3, triangleIndices, 3, device);

	Vertex squareVertices[] =
	{
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), normal, uv },
	};
	UINT squareIndices[] = { 0, 1, 2, 0, 2, 3 };
	square = new Mesh(squareVertices, 4, squareIndices, 6, device);

	Vertex hexagonVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.5f, +1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+1.0f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.5f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.5f, -1.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-1.0f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.5f, +1.0f, +0.0f), normal, uv },
	};
	UINT hexagonIndices[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 1 };
	hexagon = new Mesh(hexagonVertices, 7, hexagonIndices, 18, device);

	//Import models
	models[0] = new Mesh("../../Assets/Models/torus.obj", device);
	models[1] = new Mesh("../../Assets/Models/cube.obj", device);
	models[2] = new Mesh("../../Assets/Models/cone.obj", device);
	models[3] = new Mesh("../../Assets/Models/cylinder.obj", device);
	models[4] = new Mesh("../../Assets/Models/helix.obj", device);
	models[5] = new Mesh("../../Assets/Models/torus.obj", device);
	
	//gameEntities[0] = new GameEntity(hexagon, defaultMaterial);
	//gameEntities[1] = new GameEntity(hexagon, defaultMaterial);
	//gameEntities[2] = new GameEntity(triangle, defaultMaterial);
	//gameEntities[3] = new GameEntity(square, defaultMaterial);
	//gameEntities[4] = new GameEntity(square, defaultMaterial);

	gameEntities[0] = new GameEntity(models[0], defaultMaterial);
	gameEntities[1] = new GameEntity(models[1], defaultMaterial);
	gameEntities[2] = new GameEntity(models[2], defaultMaterial);
	gameEntities[3] = new GameEntity(models[3], defaultMaterial);
	gameEntities[4] = new GameEntity(models[4], defaultMaterial);

	gameEntities[0]->SetTranslation(0, -0.5f, -2);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->UpdateProjectionMatrix((float)width, (float)height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//float sinTime = (sin(totalTime * 2.0f) + 5.0f) / 10.0f;

	//gameEntities[0]->SetTranslation(sin(totalTime), sin(totalTime), 0);
	//gameEntities[0]->SetRotation(0, 0, totalTime);
	//gameEntities[0]->SetScale(sinTime, sinTime, sinTime);

	//gameEntities[1]->SetTranslation(-totalTime / 2.0f, 0, 0);

	//gameEntities[2]->SetRotation(0, 0, totalTime);
	//gameEntities[2]->SetScale(0.5f, 0.5f, 0.5f);

	//gameEntities[3]->SetTranslation(2, 0, 0);
	//gameEntities[3]->SetScale(sinTime, sinTime, sinTime);

	//gameEntities[4]->SetTranslation(0, sin(totalTime), 0);

	//Rotate
	gameEntities[0]->SetRotation(0, totalTime, 0);

	camera->Update(deltaTime, totalTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	
	for (int i = 0; i < 1; i++) 
	{
		gameEntities[i]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* vBuffer = gameEntities[i]->mesh->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
		context->IASetIndexBuffer(gameEntities[i]->mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		
		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			gameEntities[i]->mesh->GetIndexCount(),		// The number of indices to use (we could draw a subset if we wanted)
			0,											// Offset to the first index we want to use
			0);											// Offset to add to each index when looking up vertices
	}


	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0002)
	{ /* Right button is down */
		camera->RotateCamera((y - prevMousePos.y) * 0.002f, (x - prevMousePos.x) * 0.002f);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion