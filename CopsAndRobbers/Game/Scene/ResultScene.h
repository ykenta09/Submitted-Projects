/*
	@file	ResultScene.h
	@brief	���U���g�V�[���N���X
*/
#pragma once
#ifndef RESULT_SCENE_DEFINED
#define RESULT_SCENE_DEFINED
#include "Game/Interface/IScene.h"
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/Particle/CircularFade.h"

// �O���錾
class CommonResources;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
}

class ResultScene final : public IScene
{
public:
   static constexpr DirectX::SimpleMath::Vector2 BACK_TEXTURE_POSITION = { 0.0f, 0.0f };			  //�w�i�e�N�X�`���̕`����W
   static constexpr DirectX::SimpleMath::Vector2 TIMER_POSITION = { 590.0f, 305.0f };				  //�N���A���Ԃ̕`����W
   static constexpr DirectX::SimpleMath::Vector2 CLEAR_TIME_TEXT_POSITION = { 190.0f, 280.0f };		  //�^�C���e�L�X�g�e�N�X�`���`����W
   static constexpr DirectX::SimpleMath::Vector2 PUSHKEY_TEXT_POSITION = { 170.0f, 440.0f };		  //�v�b�V���L�[�e�L�X�g�e�N�X�`���`����W
   static constexpr DirectX::SimpleMath::Vector2 RESULT_BOARD_POSITION = { 90.0f, 70.0f };			  //���U���g�{�[�h�e�N�X�`���̕`����W
   static constexpr DirectX::SimpleMath::Vector2 TEXT_POSITION = { 300.0f, 50.0f };					  //���U���g�e�L�X�g�̕`����W
   static constexpr RECT COLON_RECT = { 480, 0, 528, 128 };			 //�R�����̃e�N�X�`�����W
   static constexpr int DRAW_TIME_OFSET = 48;						 //���Ԃ̕`��␳
   static constexpr float FADE_SPEED = 0.5f;						 //�t�F�[�h�̑��x

public:
    //�R���X�g���N�^
	ResultScene();
	//�f�X�g���N�^
    ~ResultScene() override;

	//����������
    void Initialize(CommonResources* resources) override;
	//�X�V����
    void Update(float elapsedTime)override;
	//�`�悷��
    void Render() override;
	//��n������
    void Finalize() override;
	//���̃V�[��ID���擾����
    SceneID GetNextSceneID() const;

	//���\�[�X��ǂݍ���
	void LoadResources();
	//�N���A�^�C����`�悷��
	void DrawClearTime();
	//�e�N�X�`�����t�F�[�h����
	void FadeTexture(float elapsedTime);
private:
	CommonResources* m_commonResources;	// ���ʃ��\�[�X
	yamadalib::Graphics* m_graphics;	//�O���t�B�b�N�X

	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;	// �X�v���C�g�t�H���g
	std::unique_ptr<CircularFade> m_circularFade;		//�~�`�̃t�F�[�h

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;				 //�w�i�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultTexture;			 //���U���g�e�L�X�g�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;			 //�^�C�}�[�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTimeTextTexture;	 //�N���A�^�C���e�L�X�g�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushKeyTextTexture;		 //�v�b�V���L�[�e�L�X�g�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resultBoardTexture;		 //���U���g�{�[�h�摜

	DirectX::SimpleMath::Vector2 m_textPosition;			//���U���g�e�L�X�g���W
	DirectX::SimpleMath::Vector2 m_timerPosition;	//�N���A�^�C����`�悷����W
	DirectX::SimpleMath::Vector2 m_clearTimeTextPosition;	//�N���A�^�C���e�L�X�g��`�悷����W
	DirectX::SimpleMath::Vector2 m_resultTextPosition;	//���U���g�e�L�X�g��`�悷����W
	DirectX::SimpleMath::Vector2 m_resultBoardPosition;	//���U���g�{�[�h��`�悷����W

	float m_timer;	//�^�C�}�[
	float m_clearTime;	//�N���A�^�C��
	bool m_fadeIn;		//�t�F�[�h�C���t���O
	float m_alpha;		//�A���t�@�l

	bool m_isChangeScene;	// �V�[���`�F���W�t���O
};
#endif // RESULT_SCENE_DEFINED
