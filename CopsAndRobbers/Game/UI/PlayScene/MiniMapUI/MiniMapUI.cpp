/*
	@file	MiniMapUI.cpp
	@brief	�~�j�}�b�vUI�N���X
*/
#include "pch.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Game/Screen.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Libraries/yamadaLib/Resources.h"
#include "Libraries/yamadaLib/Math.h"
#include <Libraries/Microsoft/ReadData.h>
#include <Game/Particle/BinaryFile.h>
#include "Libraries/MyLib/BlackBoard.h"
#include "Game/Collision/CollisionObject.h"
#include "Libraries/yamadaLib/GameParameter.h"


///	<summary>
///	�C���v�b�g���C�A�E�g
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> MiniMapUI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


/// <summary>
///  �R���X�g���N�^
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
/// <param name="player">�v���C���[</param>
/// <param name="fellow">����</param>
/// <param name="map">�}�b�v</param>
MiniMapUI::MiniMapUI(CommonResources* resources, Player* player, Fellow* fellow, Map* map)
   :
   m_commonResources(resources),
   m_player(player),
   m_fellow(fellow),
   m_map(map),
   m_graphics(yamadalib::Graphics::GetInstance()),
   m_backTexturePosition(BACK_TEXTURE_POSITION),
   m_playerTexturePosition(PLAYER_TEXTURE_POSITION),
   m_isCompositionTextureInitialized(false)
{
}


/// <summary>
/// �f�X�g���N�^
/// </summary>
MiniMapUI::~MiniMapUI()
{
   //��n������
   Finalize();
}

/// <summary>
/// ����������
/// </summary>
void MiniMapUI::Initialize()
{
	//UI�e�N�X�`����ǂݍ���
    m_miniMapBackTexture = yamadalib::Resources::GetInstance()->GetTexture("MiniMapBack");
	m_circleTexture = yamadalib::Resources::GetInstance()->GetTexture("MaskCircle");

	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);				//�X�v���C�g�o�b�`�𐶐�����
	
	// �����_�[�e�N�X�`���̃T�C�Y�ݒ�i�~�j�}�b�v�w�i�T�C�Y�j
	RECT miniMapRect = { 0, 0, BACK_RECT.right, BACK_RECT.bottom };

	//�}�X�N�����p�����_�[�e�N�X�`���̍쐬
	for (int i = 1; i <= GameParameters::MAX_STAGE_NUM; i++)
	{
	   m_renderTexture[i] = std::make_unique<DX::RenderTexture>(m_commonResources->GetDeviceResources()->GetBackBufferFormat());
	   m_renderTexture[i]->SetDevice(device);
	   m_renderTexture[i]->SetWindow(miniMapRect);
	}

	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
	m_states = std::make_unique<DirectX::CommonStates>(device);

	//�V�F�[�_�[���쐬
	CreateShader();

	//�~�j�}�b�v�p�̃r���[�|�[�g
	m_miniMapViewpor = {
	0.0f, 0.0f,
	RENDER_TEXTURE_SIZE.x, RENDER_TEXTURE_SIZE.y,
	0.0f, 1.0f
	};

	//���C����ʂ̃r���[�|�[�g
	m_mainViewport = {
	0.0f, 0.0f,
	static_cast<float>(m_commonResources->GetDeviceResources()->GetOutputSize().right),
	static_cast<float>(m_commonResources->GetDeviceResources()->GetOutputSize().bottom),
	0.0f, 1.0f
	};
}


/// <summary>
/// �X�V����
/// </summary>
void MiniMapUI::Update()
{
   //���݂���X�e�[�W�ԍ����擾����
   m_currentStageNumber = m_commonResources->GetBlackBoard()->GetStageNumber();
   //�w�肵���X�e�[�W�̒��S���W���擾����
   m_miniMapCenter = m_map->GetStageCenterPosition(m_commonResources->GetBlackBoard()->GetStageNumber());

   //�~�`�e�N�X�`���̍��W�X�V
   m_circlePosition = WorldToMiniMap(m_player->GetPosition(), m_miniMapCenter, MAP_SPACE) + CIRCLE_POSITION_OFSET;
}


/// <summary>
/// �`�悷��
/// </summary>
void MiniMapUI::Render()
{   
   // �~�j�}�b�v�ƃ}�b�v�I�u�W�F�N�g�̕`��
   DrawMiniMapBack();
   DrawMiniMapObject();

   //�}�X�N�p�̃e�N�X�`���`��ƃ}�X�N�e�N�X�`���̕`��
   UpdateCompositionTexture();
   RenderShader();
}


/// <summary>
/// ��n������
/// </summary>
void MiniMapUI::Finalize()
{
	// �f�o�C�X���\�[�X�̉��
   for (int i = 1; i <= GameParameters::MAX_STAGE_NUM; i++)
   {
	  m_renderTexture[i]->ReleaseDevice();
   }
   m_spriteBatch.reset();
}


/// <summary>
/// �V�F�[�_�[���쐬����
/// </summary>
void MiniMapUI::CreateShader()
{
   ID3D11Device1* device = m_commonResources->GetDeviceResources()->GetD3DDevice();

   //	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
   BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/MiniMapShader/MiniMapVS.cso");
   BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/MiniMapShader/MiniMapGS.cso");
   BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/MiniMapShader/MiniMapPS.cso");

   //	�C���v�b�g���C�A�E�g�̍쐬
   device->CreateInputLayout(&INPUT_LAYOUT[0],
	  static_cast<UINT>(INPUT_LAYOUT.size()),
	  VSData.GetData(), VSData.GetSize(),
	  m_inputLayout.GetAddressOf());

   //	���_�V�F�[�_�쐬
   if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
   {//	�G���[
	  MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
	  return;
   }
   //	�W�I���g���V�F�[�_�쐬
   if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
   {//	�G���[
	  MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
	  return;
   }
   //	�s�N�Z���V�F�[�_�쐬
   if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
   {//	�G���[
	  MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
	  return;
   }

   //	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
   D3D11_BUFFER_DESC bd;
   ZeroMemory(&bd, sizeof(bd));
   bd.Usage = D3D11_USAGE_DEFAULT;
   bd.ByteWidth = sizeof(ConstBuffer);
   bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   bd.CPUAccessFlags = 0;
   device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}


/// <summary>
/// ���[���h���W����~�j�}�b�v���W�ɕϊ�����
/// </summary>
/// <param name="worldPos">���[���h���W</param>
/// <param name="mapCenter">�}�b�v�̒��S</param>
/// <param name="mapScale">�}�b�v�̃T�C�Y</param>
/// <returns></returns>
DirectX::SimpleMath::Vector2 MiniMapUI::WorldToMiniMap(const DirectX::SimpleMath::Vector3& worldPos, const DirectX::SimpleMath::Vector3& mapCenter, float mapScale)
{
   //���[���h���W����~�j�}�b�v��̑��Έʒu���v�Z
   DirectX::SimpleMath::Vector3 offset = worldPos - mapCenter;

   return DirectX::SimpleMath::Vector2(offset.x * mapScale, offset.z * mapScale);
} 


/// <summary>
/// �~�j�}�b�v�w�i�`��
/// </summary>
void MiniMapUI::DrawMiniMapBack()
{
   //���ƍ�����ݒ肷��
   float width = static_cast<float>(BACK_RECT.right - BACK_RECT.left);
   float height = static_cast<float>(BACK_RECT.bottom - BACK_RECT.top);
   //�~�j�}�b�v�̔w�i�e�N�X�`����`�悷��
   m_graphics->DrawTexture(m_miniMapBackTexture, m_backTexturePosition, &BACK_RECT,
	  DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(width / 2 , height / 2), MINIMAP_SCALE);
}


/// <summary>
/// �~�j�}�b�v�I�u�W�F�N�g�`��
/// </summary>
void MiniMapUI::DrawMiniMapObject()
{
   //���ƍ�����ݒ肷��
   float width = static_cast<float>(OBJECT_RECT.right - OBJECT_RECT.left);
   float height = static_cast<float>(OBJECT_RECT.bottom - OBJECT_RECT.top);

   // ����X�e�[�W�̂ݎQ�Ƃ���ꍇ
   const std::vector<std::unique_ptr<IMapObject>>& stageObjs = m_map->GetStageObjects(m_currentStageNumber);
   for (int i = 0; i < stageObjs.size(); i++)
   {
	  IMapObject* obj = stageObjs[i].get();
	  // obj���Q��
	  CollisionObject* collisionObj = obj->GetCollisionObject();
	  if (collisionObj)
	  {
		 //�p�����[�^���擾����
		 const auto& param = collisionObj->GetCollisionParameter();
		 if (param.objectID == CollisionManager::ObjectType::FLOOR) continue;

		 //�I�u�W�F�N�g�̋��E�{�b�N�X������W���擾
		 DirectX::SimpleMath::Vector3 objectPos = param.collision.box.Center;
		 //���[���h���W����~�j�}�b�v���W�ɕϊ�����
		 m_mapTexturePosition = WorldToMiniMap(objectPos, m_miniMapCenter, MAP_SPACE) + m_backTexturePosition;
		 //�X�P�[������
		 DirectX::SimpleMath::Vector2 scale =
			DirectX::SimpleMath::Vector2(param.collision.box.Extents.x * MAP_SPACE / MAP_SCALE_X_DIVISOR,
			   (param.collision.box.Extents.z) * MAP_SPACE / MAP_SCALE_Z_DIVISOR);
		 //�X�P�[�����ُ�l�ɂȂ�Ȃ��悤�ɐ�������
		 scale.x = yamadalib::Math::Clamp(scale.x, MIN_ICON_SCALE, MAX_ICON_SCALE);
		 scale.y = yamadalib::Math::Clamp(scale.y, MIN_ICON_SCALE, MAX_ICON_SCALE);
		 //�I�u�W�F�N�g���~�j�}�b�v�ɕ`�悷��
		 m_graphics->DrawTexture(m_miniMapBackTexture, m_mapTexturePosition, &OBJECT_RECT, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(width / 2, height / 2), scale);
	  }
   }
   RECT charaRect = { 317, 76 , 338, 106 };

   //�v���C���[�A�C�R���̕`��
   m_playerTexturePosition = WorldToMiniMap(m_player->GetPosition(), m_miniMapCenter, MAP_SPACE) + m_backTexturePosition;
   m_graphics->DrawTexture(m_miniMapBackTexture, m_playerTexturePosition, &charaRect, DirectX::Colors::White, m_player->GetAngle(), DirectX::SimpleMath::Vector2(width / 2, height / 2), PLAYER_FELLOW_ICON_SCALE);

   //���Ԃ̃A�C�R���̕`��
   //m_fellowTexturePosition = WorldToMiniMap(m_fellow->GetPosition(), m_miniMapCenter, MAP_SPACE) + m_backTexturePosition;
   //m_graphics->DrawTexture(m_miniMapBackTexture, m_fellowTexturePosition, &FELLOW_RECT, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(width / 2, height / 2), PLAYER_FELLOW_ICON_SCALE);
}

/// <summary>
/// �ݐσe�N�X�`�����쐬����
/// </summary>
void MiniMapUI::UpdateCompositionTexture()
{
   //�f�o�C�X�R���e�L�X�g���擾����
   auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

   //�����_�[�^�[�Q�b�g�̐؂�ւ�
   ID3D11RenderTargetView* oldRTV = nullptr;
   ID3D11DepthStencilView* oldDSV = nullptr;
   context->OMGetRenderTargets(1, &oldRTV, &oldDSV);

   //�����_�[�e�N�X�`�����A�N�e�B�u�ɐݒ�
   ID3D11RenderTargetView* rtv = m_renderTexture[m_currentStageNumber]->GetRenderTargetView();
   context->OMSetRenderTargets(1, &rtv, nullptr);
   //�r���[�|�[�g�ݒ�
   context->RSSetViewports(1, &m_miniMapViewpor);

   //�����_�[�e�N�X�`��������������Ă��Ȃ���Ώ���������
   if (!m_isCompositionTextureInitialized)
   {
	  InitializeCompositionTexture(context, rtv, m_miniMapViewpor);
	  m_isCompositionTextureInitialized = true;
   }

   //�~�`�e�N�X�`����`��
   m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());  // �A���t�@�u�����h��L���ɂ���
   m_spriteBatch->Draw(m_circleTexture.Get(), m_circlePosition, nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, CIRCLE_SCALE);
   m_spriteBatch->End();

   //�����_�[�^�[�Q�b�g�̌��ʂ����ɖ߂�
   context->OMSetRenderTargets(1, &oldRTV, oldDSV);
   //���C�������_�[�^�[�Q�b�g�̃r���[�|�[�g��ݒ�
   context->RSSetViewports(1, &m_mainViewport);
}


/// <summary>
///  �ݐσe�N�X�`��������������
/// </summary>
/// <param name="context">�f�o�C�X�R���e�L�X�g</param>
/// <param name="rtv">�����_�[�^�[�Q�b�g�r���[</param>
/// <param name="viewport">�r���[�|�[�g</param>
void MiniMapUI::InitializeCompositionTexture(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, const D3D11_VIEWPORT& viewport)
{
   //�����_�[�^�[�Q�b�g�� m_compositionTexture �ɐݒ�
   context->OMSetRenderTargets(1, &rtv, nullptr);
   //�r���[�|�[�g�̐ݒ�
   context->RSSetViewports(1, &viewport);
   //�`��Ɏg�p����F��ݒ� (�����ł͓����F�ŏ�����)
   float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // ����
   //�����_�[�^�[�Q�b�g���N���A
   context->ClearRenderTargetView(rtv, clearColor);

   //�����_�[�^�[�Q�b�g�����ɖ߂� (�K�v�ɉ�����)
   ID3D11RenderTargetView* nullRTV[] = { nullptr };
   context->OMSetRenderTargets(1, nullRTV, nullptr);
}

/// <summary>
/// �V�F�[�_�[��`�悷��
/// </summary>
void MiniMapUI::RenderShader()
{
   //�f�o�C�X�R���e�L�X�g���擾����
   auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

   //�s�N�Z���V�F�[�_�[�̌��ʂ��~�j�}�b�v�r���[�|�[�g���ɕ`��
   context->IASetInputLayout(m_inputLayout.Get());

   //���_���W���r���[�|�[�g�͈͓��ɒ���
   DirectX::VertexPositionColorTexture vertex[4] =
   {
	   DirectX::VertexPositionColorTexture(MINIMAP_MASK_VERTEX, DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
   };

   //�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
   ConstBuffer cbuff;
   cbuff.matView = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matProj = DirectX::SimpleMath::Matrix::Identity;
   cbuff.matWorld = DirectX::SimpleMath::Matrix::CreateScale(MINIMAP_MASK_SCALE);
   cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

   //�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
   context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

   //�V�F�[�_�[�Ƀo�b�t�@��n��
   ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
   context->VSSetConstantBuffers(0, 1, cb);
   context->GSSetConstantBuffers(0, 1, cb);
   context->PSSetConstantBuffers(0, 1, cb);

   //�摜�p�T���v���[�̓o�^
   ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
   context->PSSetSamplers(0, 1, sampler);

   //�������`��w��
   ID3D11BlendState* blendstate = m_states->NonPremultiplied();
   //�������菈��
   context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
   //�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
   context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
   //�J�����O�͍�����
   context->RSSetState(m_states->CullNone());

   //�V�F�[�_���Z�b�g����
   context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
   context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
   context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

   //�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
   ID3D11ShaderResourceView* textures[] = {
	   m_renderTexture[m_currentStageNumber]->GetShaderResourceView()
   };
   context->PSSetShaderResources(0, 1, textures);

   //�`��
   m_batch->Begin();
   m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
   m_batch->End();

   //�V�F�[�_�̓o�^���������Ă���
   context->VSSetShader(nullptr, nullptr, 0);
   context->GSSetShader(nullptr, nullptr, 0);
   context->PSSetShader(nullptr, nullptr, 0);
}