//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "Game/Screen.h"
#include "Game/Scene/SceneManager.h"
#include "Libraries/yamadaLib/Resources.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
    :
    m_deviceResources{},
    m_graphics{},
    m_timer{},
    m_commonStates{},
    m_commonResources{},
    m_debugString{},
    m_inputManager{},
    m_sceneManager{}
{
    m_graphics = yamadalib::Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_deviceResources->RegisterDeviceNotify(this);
    //m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    //m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    // デバイスリソース関連を設定する
    m_deviceResources->SetWindow(window, width, height);
    //デバイスソースを生成する
    m_deviceResources->CreateDeviceResources();
    //デバイスに依存したリソースを生成する
    CreateDeviceDependentResources();

    //ウィンドウサイスに依存したリソースを生成する
    m_deviceResources->CreateWindowSizeDependentResources();
    //ウィンドウサイズに依存したリソースを生成する
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */


    // ★追記ココから↓↓↓★

    // デバイスとコンテキストを取得する
    auto device  = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // 入力マネージャを作成する
    m_inputManager = std::make_unique<mylib::InputManager>(window);

    // コモンステートを作成する
    m_commonStates = std::make_unique<CommonStates>(device);

    // デバッグ文字列を作成する
    m_debugString = std::make_unique<mylib::DebugString>(
        device,
        context,
        L"Resources/Fonts/SegoeUI_18.spritefont"
    );
    
    //ブラックボードを作成する
    m_blackBoard = std::make_unique<mylib::BlackBoard>();

    // 共通リソースを作成する
    m_commonResources = std::make_unique<CommonResources>();

    // シーンへ渡す共通リソースを設定する
    m_commonResources->Initialize(
        &m_timer,
        m_deviceResources,
        m_commonStates.get(),
        m_debugString.get(),
        m_inputManager.get(),
        m_blackBoard.get()
    );


    //グラフィックスを初期化する
    m_graphics->Initialize(m_commonResources.get());

    //リソースをロードする
    yamadalib::Resources::GetInstance()->LoadResources();

    // シーンマネージャを初期化する
    m_sceneManager = std::make_unique<SceneManager>();
    m_sceneManager->Initialize(m_commonResources.get());

    // ★追記ココまで↑↑↑★
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.


    // ★追記ココから↓↓↓★

    // 入力マネージャを更新する
    m_inputManager->Update();

    // キーボードステートを取得する
    const auto& keyboardState = m_inputManager->GetKeyboardState();

    // 「ECS」キーで終了する
    if (keyboardState.Escape)
    {
#ifdef _DEBUG
       ExitGame();
#endif // _DEBUG
       if (MessageBox(NULL, L"本当にゲームをやめますか？", L"ゲームを終了", MB_YESNO) == IDYES)
       {
          ExitGame();
       }
    }

    // シーンマネージャを更新する
    m_sceneManager->Update(elapsedTime);

    // ★追記ココまで↑↑↑★
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.


    // ★追記ココから↓↓↓★

    UNREFERENCED_PARAMETER(context);

    // デバッグ文字列を作成する：FPS
#ifdef _DEBUG
    m_debugString->AddString("fps : %d", m_timer.GetFramesPerSecond());
#endif // DEBUG


    // シーンマネージャを描画する
    m_sceneManager->Render();

    // デバッグ文字列を描画する
    m_debugString->Render(m_commonStates.get());

    // ★追記ココまで↑↑↑★


    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);

    // フルスクリーンか調べる
    BOOL fullscreen = FALSE;
    m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);
    // フルスクリーンが解除されてしまった時の処理
    if (m_fullscreen != fullscreen)
    {
        m_fullscreen = fullscreen;
        // ResizeBuffers関数を呼び出す
        m_deviceResources->CreateWindowSizeDependentResources();
    }
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width   = Screen::WIDTH;    // ★変更::800->Screen::WIDTH
    height  = Screen::HEIGHT;   // ★変更::600->Screen::HEIGHT
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;

    // ★追記★
    UNREFERENCED_PARAMETER(device);

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

