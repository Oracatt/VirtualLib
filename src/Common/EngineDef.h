#pragma once
#include <iostream>
#include "objbase.h"
#include <vector>
#include <wrl/client.h>

using namespace Microsoft::WRL;

typedef ComPtr<class IActor> LPACTOR;
typedef ComPtr<class IAComponent> LPACOM;
typedef std::vector<LPACOM>::iterator LPACOMITERATOR;
typedef ComPtr<class ISComponent> LPSCOM;
typedef std::vector<LPSCOM>::iterator LPSCOMITERATOR;

DEFINE_GUID(IID_IVirtualBase, 0x0d619ced, 0x5034, 0x4804, 0x9e, 0xc6, 0x6f, 0xb7, 0x8d, 0x1c, 0xa0, 0x4c);
DEFINE_GUID(IID_IVirtualEngine, 0x2e771471, 0x330b, 0xed21, 0x7d, 0xe2, 0x7e, 0xf8, 0xc8, 0x8f, 0x4a, 0x1a);
DEFINE_GUID(IID_IScene, 0x50cc9f37, 0x82af, 0xf15c, 0x7a, 0x65, 0x9d, 0x84, 0x26, 0x9d, 0xe8, 0x8a);
DEFINE_GUID(IID_IRenderer, 0xfe2a9aa0, 0xfccd, 0x8628, 0x00, 0x57, 0x79, 0xc5, 0x51, 0x5c, 0x45, 0x5c);
DEFINE_GUID(IID_IInput, 0xd8043428, 0x4271, 0x4dcb, 0x98, 0xd3, 0x2c, 0x51, 0x02, 0x6f, 0xee, 0x97);
DEFINE_GUID(IID_IAudioPlayer, 0x472fe509, 0xe04f, 0x4f4c, 0x86, 0xfa, 0xcf, 0xee, 0xd4, 0x5e, 0x74, 0xd3);
DEFINE_GUID(IID_IAudio, 0xb3a1d2d1, 0xe883, 0x43fd, 0x89, 0xcb, 0x8c, 0x97, 0xf0, 0x2c, 0x2d, 0xa0);
DEFINE_GUID(IID_IActor, 0x63bd739b, 0xaaa1, 0x4ee2, 0xa5, 0x71, 0xda, 0x17, 0x86, 0x36, 0x46, 0xab);
DEFINE_GUID(IID_IAComponent, 0x7437872e, 0x0405, 0x45e3, 0xa3, 0x66, 0x18, 0x58, 0xb4, 0x88, 0xd2, 0xf8);
DEFINE_GUID(IID_ITimer, 0x7083b2d0, 0xaff6, 0x41a4, 0xa3, 0x55, 0xed, 0x71, 0xad, 0xae, 0xa9, 0xfb);
DEFINE_GUID(IID_ISComponent, 0x090c3c77, 0x054d, 0x4760, 0xa8, 0x70, 0xe8, 0x1f, 0x71, 0x04, 0x26, 0xd0);
DEFINE_GUID(IID_IPhysicsManager, 0x16aad442, 0xc7c3, 0x4400, 0xaf, 0x6a, 0x96, 0xf4, 0xd1, 0x3f, 0x71, 0x1d);
DEFINE_GUID(IID_IPhysicsScene, 0xb1235a28, 0x49f3, 0x4bdd, 0x96, 0x12, 0x62, 0xdc, 0xfc, 0x3f, 0xc0, 0xf3);
DEFINE_GUID(IID_IRenderable, 0xe72b4ce5, 0xbdc5, 0x4df5, 0x8f, 0x84, 0x0b, 0x52, 0xfc, 0x16, 0xf7, 0xda);
DEFINE_GUID(IID_IPhysicsActor, 0x58db42ad, 0x9fb8, 0x4f8d, 0x97, 0xb9, 0x69, 0x25, 0xe3, 0x1e, 0x1c, 0xfb);
DEFINE_GUID(IID_IRenderTarget, 0x635d44da, 0x448f, 0x4523, 0x93, 0x74, 0x88, 0x46, 0x15, 0x53, 0xa4, 0xce);
DEFINE_GUID(IID_ITexture, 0x9b21cb7c, 0xcd6c, 0x4eb7, 0x8c, 0x95, 0x71, 0x7b, 0xa7, 0x72, 0x64, 0x9c);
DEFINE_GUID(IID_ITextFormat, 0x9accfe1b, 0x28f8, 0x4c2d, 0xa5, 0x52, 0xa6, 0x23, 0xd3, 0x47, 0x41, 0x55);
DEFINE_GUID(IID_IRenderTargetTreeView, 0xf48a6e90, 0x5742, 0x4c07, 0xbd, 0x16, 0xc4, 0xbf, 0xe2, 0x4f, 0xe8, 0x9e);
DEFINE_GUID(IID_IRenderTargetTreeNode, 0x7b5c1601, 0x21cb, 0x4f43, 0xaf, 0xc7, 0xa2, 0x80, 0x51, 0x3c, 0xe0, 0x10);
DEFINE_GUID(IID_IProvideRenderTarget, 0x8b08ac67, 0x1492, 0x43ba, 0xa4, 0xfd, 0x62, 0x95, 0x89, 0x28, 0x9a, 0xde);
DEFINE_GUID(IID_IDelegate, 0xe2391c20, 0x4c1c, 0x4936, 0x8c, 0x4c, 0x4e, 0x50, 0xe9, 0x5d, 0xda, 0xa5);
DEFINE_GUID(IID_IAContainer, 0x86d01e77, 0xcea1, 0x483d, 0xa1, 0xb6, 0xa2, 0xcc, 0xf0, 0xeb, 0x9c, 0x15);
DEFINE_GUID(IID_ICamera, 0x7437872e, 0x0405, 0x45e3, 0xa3, 0x66, 0x18, 0x58, 0xb4, 0x88, 0xd2, 0xf8);
DEFINE_GUID(IID_IAudioInstance, 0xf917a3e0, 0x43c3, 0x4767, 0xb4, 0x4b, 0xc8, 0xdf, 0x3e, 0xde, 0x98, 0x1d);
DEFINE_GUID(IID_IDynamicAudioInstance, 0x48a79cfc, 0x3c7f, 0x4e64, 0xb1, 0x91, 0x37, 0xf3, 0x07, 0xb5, 0xd4, 0x6f);
DEFINE_GUID(IID_IBuffer, 0x993376cd, 0x43a8, 0x440e, 0xb9, 0x87, 0x8c, 0x57, 0x32, 0x75, 0xef, 0x83);

#define MAKE_VERSION(a,b,c) (((a)<<6)|((b)<<4)|((c)<<2)|0xff)

#define VENGINE_SDK_VERSION MAKE_VERSION(1,0,0)

#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)

#define SUCCEED_OPERATE 0

#define ERR_OUTOFMEMORY 0x80010301L
#define ERR_INVALIDARG  0x80010302L
#define ERR_ACCESS      0x80010303L
#define ERR_POINTER     0x80010304L
#define ERR_UNEXPECTED  0x80010305L
#define ERR_UNKNOWN     0x800103ffL

/*
	CreateVirtualEngine返回的错误值
*/
#define ERR_INVALIDVERSION  0x80000001L
#define ERR_CREATEWINDOW    0x80000002L
#define ERR_CREATED3D       0x80000003L
#define ERR_D3DNOTSUPPORT   0x80000004L
#define ERR_D3DCHECKMSAA    0x80000005L
#define ERR_INITINPUT       0x80000006L
#define ERR_INITAUDIOPLAYER 0x80000007L
#define ERR_INITPHYSX       0x80000008L
#define ERR_CREATED2D       0x80000009L
#define ERR_COINITIALIZE    0x8000000aL

/*
	EventErrorCallBack错误代码
*/
#define FE_FILENOTFOUND   0x00000501L
#define FE_CANNOTOPENFILE 0x00000502L
#define FE_PHYSXINTERNAL  0x00000503L
#define FE_AUDIOUPDATE    0x00000504L
#define FE_UNKNOWN        0x00000505L

/*
	场景消息类型
*/
#define SMSG_ATTACHACTOR	0x01
#define SMSG_DESTROYACTOR	0x02
#define SMSG_INSERTAFTER	0x03
#define SMSG_INSERTBEFORE	0x04
#define SMSG_SENDTOFIRST	0x05
#define SMSG_SENDTOLAST		0x06
#define SMSG_DESTROYSCENE   0x07
#define SMSG_CHANGESCENE    0x08
#define SMSG_SCENEAWAKE     0x09

/*
如果希望消息不会触发ISComponent::OnMessageBegin和ISComponent::OnMessageEnd
可将此与上面的消息组合使用
注意：自定义消息字节值大小勿超过0x80
*/
#define SMSG_NONOTIFY       0x80

/*
	由ISComponent::OnMessageBegin主动返回
	MSGR_OVERLAP：用修改后的参数取代原先的参数
	MSGR_IGNORE：忽略此事件，不做任何修改
	MSGR_SUPERCEDE：完全取代此事件，默认的操作将不会执行
*/
#define MSGRESULT      HRESULT
#define MSGR_OVERLAP   0x00000000L
#define MSGR_IGNORE    0x00000001L
#define MSGR_SUPERCEDE 0x00000002L

/*
	由IAComponent::OnPreAwake,ISComponent::OnPreAwake主动返回
	ISR_IGNORE：忽略此事件，不做任何修改
	ISR_SUPERCEDE：完全取代此事件，默认的操作将不会执行
*/
#define ISRESULT      HRESULT
#define ISR_IGNORE    0x00000001L
#define ISR_SUPERCEDE 0x00000002L

/*
	Light Types
	LT_DIRECTIONAL：平行光
	LT_POINT：点光源
	LT_SPOT：聚光灯
*/
#define LIGHTTYPE      int
#define LT_DIRECTIONAL 1
#define LT_POINT       2
#define LT_SPOT        3

/*////////////////////////////////////////////////
	IRenderer::SendShaderData默认的type参数
*/////////////////////////////////////////////////

/*
	设置光照参数
	data1:int （索引，最大为9）
	data2:Light*
*/
#define SD_LIGHT 0x00000001L

/*
	设置光照开关
	data1:int （索引，最大为9）
	data2:bool (表示开关)
*/
#define SD_LIGHTENABLED 0x00000002L

/*
	设置材质
	data1:Material*
	data2:null
*/
#define SD_MATERIAL 0x00000003L

/*
	设置纹理
	data1:ID3D11ShaderResourceView*
	data2:null
*/
#define SD_TEXTURE 0x00000004L

/*
	设置雾化开关
	data1:bool
	data2:null
*/
#define SD_FOGENABLED 0x00000005L

/*
	设置雾化开始距离
	data1:float*
	data2:null
*/
#define SD_FOGSTART 0x00000006L

/*
	设置雾化范围（需大于开始距离）
	data1:float*
	data2:null
*/
#define SD_FOGRANGE 0x00000007L

/*
	设置雾化颜色
	data1:Color*
	data2:null
*/
#define SD_FOGCOLOR 0x00000008L

/*
	设置反色开关
	data1:bool
	data2:null
*/
#define SD_INVENABLED 0x00000009L

/*
	设置当前摄像机（下一帧会被还原为主摄像机）
	data1:ICamera*
	data2:null
*/
#define SD_UPDATEMATRIX 0x0000000aL

/*
	摄像机投影类型
	CAMERA_TYPE_PROJECTIVE：透视投影
	CAMERA_TYPE_ORTHOGONAL：正交投影
*/
enum CAMERA_TYPE
{
	CAMERA_TYPE_PROJECTIVE,
	CAMERA_TYPE_ORTHOGONAL
};

/*
	IRenderer::SetWindowed scalingMode参数
	只在windowed为false时生效
	SCALING_MODE_UNSPECIFIED：自动选择（推荐）
	SCALING_MODE_FIXEDASPECT：固定的长宽比（全屏时会降低性能）
	SCALING_MODE_SCREENASPECT：屏幕长宽比
*/
enum SCALING_MODE
{
	SCALING_MODE_UNSPECIFIED,
	SCALING_MODE_FIXEDASPECT,
	SCALING_MODE_SCREENASPECT
};

/*
	IRenderer::SetBlendState blendState参数
	BLEND_STATE_NORMAL：正常
	BLEND_STATE_BRIGHTEN：发光
	BLEND_STATE_DARKEN：变暗
	BLEND_STATE_INVDEST：反色
*/
enum BLEND_STATE
{
	BLEND_STATE_NORMAL,
	BLEND_STATE_BRIGHTEN,
	BLEND_STATE_DARKEN,
	BLEND_STATE_INVDEST
};

enum ALIGNMENT_TYPE
{
	ALIGNMENT_TYPE_CENTER = 0,
	ALIGNMENT_TYPE_LEFT = 1,
	ALIGNMENT_TYPE_RIGHT = 2,
	ALIGNMENT_TYPE_TOP = 4,
	ALIGNMENT_TYPE_BOTTOM = 8
};