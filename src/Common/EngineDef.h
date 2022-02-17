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
	CreateVirtualEngine���صĴ���ֵ
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
	EventErrorCallBack�������
*/
#define FE_FILENOTFOUND   0x00000501L
#define FE_CANNOTOPENFILE 0x00000502L
#define FE_PHYSXINTERNAL  0x00000503L
#define FE_AUDIOUPDATE    0x00000504L
#define FE_UNKNOWN        0x00000505L

/*
	������Ϣ����
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
���ϣ����Ϣ���ᴥ��ISComponent::OnMessageBegin��ISComponent::OnMessageEnd
�ɽ������������Ϣ���ʹ��
ע�⣺�Զ�����Ϣ�ֽ�ֵ��С�𳬹�0x80
*/
#define SMSG_NONOTIFY       0x80

/*
	��ISComponent::OnMessageBegin��������
	MSGR_OVERLAP�����޸ĺ�Ĳ���ȡ��ԭ�ȵĲ���
	MSGR_IGNORE�����Դ��¼��������κ��޸�
	MSGR_SUPERCEDE����ȫȡ�����¼���Ĭ�ϵĲ���������ִ��
*/
#define MSGRESULT      HRESULT
#define MSGR_OVERLAP   0x00000000L
#define MSGR_IGNORE    0x00000001L
#define MSGR_SUPERCEDE 0x00000002L

/*
	��IAComponent::OnPreAwake,ISComponent::OnPreAwake��������
	ISR_IGNORE�����Դ��¼��������κ��޸�
	ISR_SUPERCEDE����ȫȡ�����¼���Ĭ�ϵĲ���������ִ��
*/
#define ISRESULT      HRESULT
#define ISR_IGNORE    0x00000001L
#define ISR_SUPERCEDE 0x00000002L

/*
	Light Types
	LT_DIRECTIONAL��ƽ�й�
	LT_POINT�����Դ
	LT_SPOT���۹��
*/
#define LIGHTTYPE      int
#define LT_DIRECTIONAL 1
#define LT_POINT       2
#define LT_SPOT        3

/*////////////////////////////////////////////////
	IRenderer::SendShaderDataĬ�ϵ�type����
*/////////////////////////////////////////////////

/*
	���ù��ղ���
	data1:int �����������Ϊ9��
	data2:Light*
*/
#define SD_LIGHT 0x00000001L

/*
	���ù��տ���
	data1:int �����������Ϊ9��
	data2:bool (��ʾ����)
*/
#define SD_LIGHTENABLED 0x00000002L

/*
	���ò���
	data1:Material*
	data2:null
*/
#define SD_MATERIAL 0x00000003L

/*
	��������
	data1:ID3D11ShaderResourceView*
	data2:null
*/
#define SD_TEXTURE 0x00000004L

/*
	����������
	data1:bool
	data2:null
*/
#define SD_FOGENABLED 0x00000005L

/*
	��������ʼ����
	data1:float*
	data2:null
*/
#define SD_FOGSTART 0x00000006L

/*
	��������Χ������ڿ�ʼ���룩
	data1:float*
	data2:null
*/
#define SD_FOGRANGE 0x00000007L

/*
	��������ɫ
	data1:Color*
	data2:null
*/
#define SD_FOGCOLOR 0x00000008L

/*
	���÷�ɫ����
	data1:bool
	data2:null
*/
#define SD_INVENABLED 0x00000009L

/*
	���õ�ǰ���������һ֡�ᱻ��ԭΪ���������
	data1:ICamera*
	data2:null
*/
#define SD_UPDATEMATRIX 0x0000000aL

/*
	�����ͶӰ����
	CAMERA_TYPE_PROJECTIVE��͸��ͶӰ
	CAMERA_TYPE_ORTHOGONAL������ͶӰ
*/
enum CAMERA_TYPE
{
	CAMERA_TYPE_PROJECTIVE,
	CAMERA_TYPE_ORTHOGONAL
};

/*
	IRenderer::SetWindowed scalingMode����
	ֻ��windowedΪfalseʱ��Ч
	SCALING_MODE_UNSPECIFIED���Զ�ѡ���Ƽ���
	SCALING_MODE_FIXEDASPECT���̶��ĳ���ȣ�ȫ��ʱ�ή�����ܣ�
	SCALING_MODE_SCREENASPECT����Ļ�����
*/
enum SCALING_MODE
{
	SCALING_MODE_UNSPECIFIED,
	SCALING_MODE_FIXEDASPECT,
	SCALING_MODE_SCREENASPECT
};

/*
	IRenderer::SetBlendState blendState����
	BLEND_STATE_NORMAL������
	BLEND_STATE_BRIGHTEN������
	BLEND_STATE_DARKEN���䰵
	BLEND_STATE_INVDEST����ɫ
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