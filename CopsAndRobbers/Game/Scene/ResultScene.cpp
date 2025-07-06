/*
	@file	ResultScene.cpp
	@brief	���U���g�V�[���N���X
*/
#include "pch.h"
#include "ResultScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/BlackBoard.h"
#include "Libraries/yamadaLib/Resources.h"
#include <cassert>

/// <summary>
/// �R���X�g���N�^
/// </summary>
ResultScene::ResultScene()
	:
	m_graphics(yamadalib::Graphics::GetInstance()),
	m_commonResources{},
	m_spriteFont{},
    m_resultTexture{},
    m_alpha(1.0f),
	m_isChangeScene{},
    m_clearTimeTextPosition(CLEAR_TIME_TEXT_POSITION)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ResultScene::~ResultScene()
{
	// do nothing.
}

/// <summary>
/// ����������
/// </summary>
/// <param name="resources">���ʃ��\�[�X</param>
void ResultScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	//�N���A�^�C�����擾����
	m_clearTime = m_commonResources->GetBlackBoard()->GetTime();

	//���\�[�X��ǂݍ���
	LoadResources();

	//�~�`�t�F�[�h���쐬
	m_circularFade = std::make_unique<CircularFade>();
	m_circularFade->Create(m_commonResources->GetDeviceResources());
	m_circularFade->SetTime(1.0f);
	m_circularFade->SetFadeOut(true);

	// �V�[���ύX�t���O������������
	m_isChangeScene = false;

	// �X�e�[�W�ԍ������Z�b�g����
	m_commonResources->GetBlackBoard()->SetStageNumber(0);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void ResultScene::Update(float elapsedTime)
{
	// �錾���������A���ۂ͎g�p���Ă��Ȃ��ϐ�
	UNREFERENCED_PARAMETER(elapsedTime);

	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// �X�y�[�X�L�[�������ꂽ��
	if (kbTracker->pressed.Space)
	{
		m_isChangeScene = true;
	}
	FadeTexture(elapsedTime);
}


/// <summary>
/// �`�悷��
/// </summary>
void ResultScene::Render()
{
    //�R�����X�e�[�g���擾����
	auto states = m_commonResources->GetCommonStates();
	//�X�v���C�g�o�b�`�ŉ摜��`��
	m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
	m_graphics->GetSpriteBatch()->Draw(m_backTexture.Get(), BACK_TEXTURE_POSITION);
	m_graphics->GetSpriteBatch()->Draw(m_resultBoardTexture.Get(), RESULT_BOARD_POSITION);
	m_graphics->GetSpriteBatch()->Draw(m_resultTexture.Get(), TEXT_POSITION);
	m_graphics->GetSpriteBatch()->Draw(m_pushKeyTextTexture.Get(), PUSHKEY_TEXT_POSITION, nullptr, DirectX::Colors::White * m_alpha);
	m_graphics->GetSpriteBatch()->End();
	//�N���A�^�C����`�悷��
	DrawClearTime();

	if (!m_circularFade->GetFade())
	{
	   m_circularFade->Render();
	}
}

/// <summary>
/// ��n������
/// </summary>
void ResultScene::Finalize()
{
}

/// <summary>
/// ���̃V�[��ID���擾����
/// </summary>
/// <returns></returns>
IScene::SceneID ResultScene::GetNextSceneID() const
{
	// �V�[���ύX������ꍇ
	if (m_isChangeScene)
	{
		return IScene::SceneID::TITLE;
	}

	// �V�[���ύX���Ȃ��ꍇ
	return IScene::SceneID::NONE;
}


/// <summary>
/// ���\�[�X��ǂݍ���
/// </summary>
void ResultScene::LoadResources()
{
	m_resultTexture = yamadalib::Resources::GetInstance()->GetTexture("ResultText");
	m_backTexture = yamadalib::Resources::GetInstance()->GetTexture("ResultBack");
	m_resultBoardTexture = yamadalib::Resources::GetInstance()->GetTexture("ResultBoard");
	m_pushKeyTextTexture = yamadalib::Resources::GetInstance()->GetTexture("PushKey");
	m_timerTexture = yamadalib::Resources::GetInstance()->GetTexture("BackTimer");
	m_clearTimeTextTexture = yamadalib::Resources::GetInstance()->GetTexture("ClearTimer");
}

/// <summary>
/// �N���A�^�C��
/// </summary>
void ResultScene::DrawClearTime()
{
   // �X�v���C�g�o�b�`�̊J�n
	m_graphics->GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_graphics->GetCommonStates()->NonPremultiplied());

	// �����b�ɕϊ�
	int totalSeconds = static_cast<int>(m_clearTime);

	// ���ƕb���v�Z
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	// ���݂̕`��ʒu��������
	DirectX::SimpleMath::Vector2 drawPosition = TIMER_POSITION;

	// ������`�悷�邽�߂̈ʒu��ݒ�
	auto drawDigit = [&](int digit) {
		// RECT�̐ݒ�
		RECT rect;
		rect.top = 0;
		rect.bottom = 128;  // ������ݒ�
		rect.left = static_cast<LONG>(DRAW_TIME_OFSET * digit);  // ���݂̐����̍��[
		rect.right = rect.left + DRAW_TIME_OFSET;  // ���݂̐����̉E�[

		// �e�N�X�`���̕`��
		m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &rect);

		// ���̐����̕`��ʒu���E�Ɉړ�
		drawPosition.x += DRAW_TIME_OFSET;  // �e�����̕`�敝�ɍ��킹�Ĉړ�
	};

	// ���̏\�̈ʂƈ�̈ʂ�`��
	drawPosition.x += DRAW_TIME_OFSET;  // ���Ɉړ�����00:00�`���ɍ��킹��
	drawDigit(minutes / 10);
	drawDigit(minutes % 10);

	// ���ƕb�̊ԂɃR������`��
	drawPosition.x += DRAW_TIME_OFSET;  // �R�����̃X�y�[�X���ړ�
	m_graphics->GetSpriteBatch()->Draw(m_timerTexture.Get(), drawPosition, &COLON_RECT);
	drawPosition.x += DRAW_TIME_OFSET;  // �R�����̃X�y�[�X���ړ�

	// �b�̏\�̈ʂƈ�̈ʂ�`��
	drawDigit(seconds / 10);
	drawDigit(seconds % 10);

	//�N���A�^�C���e�L�X�g��`�悷��
	m_graphics->GetSpriteBatch()->Draw(m_clearTimeTextTexture.Get(), m_clearTimeTextPosition);

	// �X�v���C�g�o�b�`�̏I���
	m_graphics->GetSpriteBatch()->End();
}



/// <summary>
/// �t�F�[�h
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
void ResultScene::FadeTexture(float elapsedTime)
{
   if (m_fadeIn) //�t�F�[�h�C��
   {
	  m_alpha += FADE_SPEED * elapsedTime;
	  if (m_alpha >= 1.0f)
	  {
		 m_alpha = 1.0f;
		 m_fadeIn = false;
	  }
   }
   else   //�t�F�[�h�A�E�g
   {
	  m_alpha -= FADE_SPEED * elapsedTime;
	  if (m_alpha <= 0.3f)
	  {
		 m_alpha = 0.3f;
		 m_fadeIn = true;
	  }
   }
}
