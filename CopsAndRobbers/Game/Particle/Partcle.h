//--------------------------------------------------------------------------------------
//	File: Particle.h
//
//	�p�[�e�B�N���N���X
//	���g���̏ꍇ��Position���̂�
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class Particle
{
public:
   //	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
   struct ConstBuffer
   {
	  DirectX::SimpleMath::Matrix		matWorld;
	  DirectX::SimpleMath::Matrix		matView;
	  DirectX::SimpleMath::Matrix		matProj;
	  DirectX::SimpleMath::Vector4		Diffuse;
	  DirectX::SimpleMath::Vector4		time;
   };


public:
   //	�֐�
   static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:
   bool GetFade() const { return m_isFadeEnd; }
public:
   Particle();
   ~Particle();

   void LoadTexture(const wchar_t* path);

   void Create(DX::DeviceResources* pDR);

   void Render();
private:

   void CreateShader();
private:
   //	�ϐ�
   DX::DeviceResources* m_pDR;

   Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

   DX::StepTimer                           m_timer;
   //	���̓��C�A�E�g
   Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

   //	�v���~�e�B�u�o�b�`
   std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
   //	�R�����X�e�[�g
   std::unique_ptr<DirectX::CommonStates> m_states;
   //	�e�N�X�`���n���h��
   std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
   //	�e�N�X�`���n���h��
   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
   //	���_�V�F�[�_
   Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
   //	�s�N�Z���V�F�[�_
   Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
   //	�W�I���g���V�F�[�_
   Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

   DirectX::SimpleMath::Matrix m_world;
   DirectX::SimpleMath::Matrix m_view;
   DirectX::SimpleMath::Matrix m_proj;

   float time;

   bool m_isFadeEnd;

};
