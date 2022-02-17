#pragma once

#include "Common/EngineCommon.h"
#include "Common/EngineDef.h"

#include "EasyDevelop/GameObject.h"
#include "EasyDevelop/ActorCom.h"
#include "EasyDevelop/SceneCom.h"
#include "EasyDevelop/Camera.h"
#include "EasyDevelop/EasyDef.h"

#include "Extend/AComponents.h"
#include "Extend/LinearMath.h"
#include "Extend/ResourceLoader.h"
#include "Extend/EngineDefault.h"

#include "Interface/IActor.h"
#include "Interface/IAComponent.h"
#include "Interface/IAudio.h"
#include "Interface/IAudioInstance.h"
#include "Interface/IAudioPlayer.h"
#include "Interface/IAContainer.h"
#include "Interface/IDelegate.h"
#include "Interface/IDynamicAudioInstance.h"
#include "Interface/IVirtualBase.h"
#include "Interface/IVirtualEngine.h"
#include "Interface/IInput.h"
#include "Interface/IPhysicsActor.h"
#include "Interface/IPhysicsManager.h"
#include "Interface/IPhysicsScene.h"
#include "Interface/IRenderable.h"
#include "Interface/IRenderer.h"
#include "Interface/IRenderTarget.h"
#include "Interface/IRenderTargetTreeNode.h"
#include "Interface/IRenderTargetTreeView.h"
#include "Interface/IScene.h"
#include "Interface/ISComponent.h"
#include "Interface/ITexture.h"
#include "Interface/ITimer.h"

#include "PhysX/include/PxPhysicsAPI.h"

#include "DXMaster/Include/WICTextureLoader.h"
#include "FXMaster/Include/d3dx11effect.h"

#include <dinput.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dsound.h>
#include <D3D11SDKLayers.h>
#include <dxgi.h>
#include <D3D11Shader.h>
#include <DCommon.h>
#include <D3DCommon.h>
#include <D3Dcompiler.h>
#include <dxgi1_6.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>

using namespace std;
using namespace physx;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace D2D1;

HRESULT CreateVirtualEngine(DWORD sdkVersion, EngineDescription& des, IVirtualEngine** ppEngine);