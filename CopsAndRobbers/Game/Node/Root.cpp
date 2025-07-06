#include "pch.h"
#include "Game/Node/Root.h"
#include "Game/Character/Enemy/Enemy.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="parent">親のポインタ</param>
/// <param name="initialPosition">初期座標</param>
/// <param name="initialAngle">初期回転</param>
Root::Root(IComponent* parent,
	const DirectX::SimpleMath::Vector3& initialPosition,
	const float& initialAngle)
	:
	m_parent(parent),
	m_nodeNumber{},
	m_partNumber{},
	m_partID{},
	m_nodes{},
	m_initialPosition(initialPosition),
	m_initialAngle(initialAngle),
	m_currentPosition(DirectX::SimpleMath::Vector3::Zero),
	m_currentAngle{},
	m_targetPosition{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
Root::~Root()
{
   Finalize();
}

/// <summary>
///初期化する
/// </summary>
void Root::Initialize()
{
}

/// <summary>
/// キーボード通知する
/// </summary>
/// <param name="keys">押されたキー</param>
inline void Root::OnKeyPressed(const DirectX::Keyboard::Keys& keys)
{
	UNREFERENCED_PARAMETER(keys);
}

/// <summary>
/// 更新する
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="position">更新座標</param>
/// <param name="angle">更新回転角</param>
/// <param name="target">ターゲットの更新座標</param>
void Root::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position, const float& angle, const DirectX::SimpleMath::Vector3& target)
{
	//ノードを更新する
	for (auto& node : m_nodes)
	{
		node->Update(elapsedTime, position, angle, target);
	}
}


/// <summary>
/// パーツを追加する
/// </summary>
/// <param name="parts">追加するキャラクターパーツ</param>
void Root::Attach(std::unique_ptr<IComponent> parts)
{
	// ノードを追加する
	m_nodes.emplace_back(std::move(parts));
	// プレイヤーかどうかを判定
	if (auto* player = m_nodes.back()->AsPlayer())
	{
	   // m_player にプレイヤーのポインタを設定
	   m_player = player;
	}
	// 敵かどうかを判定
	if (auto* enemy = m_nodes.back()->AsEnemy())
	{
	   m_enemies.push_back(enemy);
	}
	// 仲間かどうかを判定
	if (auto* fellow = m_nodes.back()->AsFellow())
	{
	   m_fellows.push_back(fellow);
	}
}


/// <summary>
/// パーツを削除する
/// </summary>
/// <param name="parts">削除するパーツ</param>
void Root::Detach(std::unique_ptr<IComponent> parts)
{
	// 部品を削除する
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
		[&](const std::unique_ptr<IComponent>& parts)
		{
			return parts.get() == parts.get();
		}
	);
	// 削除する部品が見つかった場合は部品を削除する
	if (it != m_nodes.end())
	{
		m_nodes.erase(it);
	}
}

/// <summary>
/// 描画する
/// </summary>
void Root::Render()
{
	// ノードを描画する
	for (auto& node : m_nodes)
	{
		node->Render();
	}
}

/// <summary>
/// 後処理をする
/// </summary>
void Root::Finalize()
{
	//ノードをクリア
	m_nodes.clear();
	m_enemies.clear();
	m_fellows.clear();
}
