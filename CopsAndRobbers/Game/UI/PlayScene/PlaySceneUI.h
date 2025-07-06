/*
	@file	PlaySceneUI.h
	@brief	�v���C�V�[��UI�N���X
*/
#pragma once
#ifndef PLAY_SCENE_UI_DEFINED
#define PLAY_SCENE_UI_DEFINED
#include "Libraries/yamadaLib/Graphics.h"
#include "Game/UI/PlayScene/MiniMapUI/MiniMapUI.h"
#include "Game/UI/PlayScene/KeyCountUI/KeyCountUI.h"
#include "Game/Interface/IUIGroup.h"
#include "Game/UI/PlayScene/MissionMessageUI/MissionMessageUI.h"
#include "Game/UI/PlayScene/StaminaUI/StaminaUI.h"
#include "Game/UI/PlayScene/TimerUI/TimerUI.h"
#include "Game/UI/PlayScene/ControlUI/ControlUI.h"
#include "Game/UI/PlayScene/ControlUI/GimmickControlUI.h"

// �O���錾
class CommonResources;
class Player;
class Fellow;
class Map;
class GameManager;

class PlaySceneUI : public IUIGroup
{
public:
   static constexpr DirectX::SimpleMath::Vector2 HINT_POSITION = DirectX::SimpleMath::Vector2(0.0f, 150.0f);					 //�q���g�e�N�X�`���̍��W
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_TOP_POSITION = DirectX::SimpleMath::Vector2(0.0f, -150.0f);		 //��̔w�i�e�N�X�`���̍��W
   static constexpr DirectX::SimpleMath::Vector2 BLACK_TEXTURE_BOTTOM_POSITION = DirectX::SimpleMath::Vector2(0.0f, 650.0f);	 //���̔w�i�e�N�X�`���̍��W
public:
	//�~�j�}�b�vUI���擾����
	MiniMapUI* GetMiniMap() const { return m_miniMapUI.get(); }

	//�~�b�V�������b�Z�[�WUI���擾����
	MissionMessageUI* GetMissionMessageUI() const { return m_missionMessageUI.get(); }
public:
    //�R���X�g���N�^
	PlaySceneUI(CommonResources* resoureces, Player* player, Fellow* fellow, Map* map);
	//�f�X�g���N�^
	~PlaySceneUI();

	//����������
	void Initialize() override;
	//�X�V����
	void Update(const float& elapsedTime, const float& playTime = 0) override;
	//�`�悷��
	void Render() override;
	//��n������
	void Finalize();

	//���\�[�X��ǂݍ���
	void LoadResources();

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//�O���t�B�b�N�X
	yamadalib::Graphics* m_graphics;
	//�Q�[���}�l�[�W���[
	GameManager* m_gameManager;
	//�L�����N�^�[�̃|�C���^
	Player* m_player;
	Fellow* m_fellow;
	Map* m_map;
	//�~�j�}�b�vUI
	std::unique_ptr<MiniMapUI> m_miniMapUI;
	//�~�b�V�������b�Z�[�WUI
	std::unique_ptr<MissionMessageUI> m_missionMessageUI;
	//���̐���\������UI  
	std::unique_ptr<KeyCountUI> m_keyCountUI;
	//�X�^�~�iUI
	std::unique_ptr<StaminaUI> m_staminaUI;
	//�^�C�}�[UI
	std::unique_ptr<TimerUI> m_timerUI;
	//������@��UI
	std::unique_ptr<ControlUI> m_controlUI;
	//�M�~�b�N����UI
	std::unique_ptr<GimmickControlUI> m_gimmickControlUI;

	//UI�w�i�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	//�q���g�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hintTexture;
};
#endif // PLAY_SCENE_UI_DEFINED
