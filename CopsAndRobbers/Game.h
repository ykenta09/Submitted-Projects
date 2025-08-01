//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

// ★includeの追記★
#include "Game/CommonResources.h"
#include "Game/Interface/IScene.h"
#include "Game/Scene/SceneManager.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/BlackBoard.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;
    void SetFullscreenState(BOOL value)
    {
        m_fullscreen = value;
        m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
        if (value) m_deviceResources->CreateWindowSizeDependentResources();
    }
private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    //std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    DX::DeviceResources*    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;


    // ★追記ココから↓↓↓★
    BOOL m_fullscreen;
    //グラフィックス
    yamadalib::Graphics* m_graphics;
    // コモンステート
    std::unique_ptr<DirectX::CommonStates>  m_commonStates;

    // コモンリソース
    std::unique_ptr<CommonResources>        m_commonResources;

    // デバッグストリング
    std::unique_ptr<mylib::DebugString>     m_debugString;

    // 入力マネージャ
    std::unique_ptr<mylib::InputManager>    m_inputManager;

    // シーンマネージャ
    std::unique_ptr<SceneManager>           m_sceneManager;

    //ブラックボード
    std::unique_ptr<mylib::BlackBoard>      m_blackBoard;

    // ★追記ココまで↑↑↑★
};
