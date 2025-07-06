/*
	@file	Resources.h
	@brief	リソースクラス
*/
#include "pch.h"
#include "Libraries/yamadalib/Resources.h"
#include "Game/Screen.h"
#include <iostream>
#include <thread>

std::unique_ptr<yamadalib::Resources> yamadalib::Resources::m_resources = nullptr;

/// <summary>
/// Resourcesクラスのインスタンスを取得する
/// </summary>
/// <returns></returns>
yamadalib::Resources* const yamadalib::Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		//Resourcesクラスのインスタンスを生成する
		m_resources.reset(new Resources());
	}
	return m_resources.get();
}

/// <summary>
/// すべてのリソースを読み込む
/// </summary>
void yamadalib::Resources::LoadResources()
{
    m_loadJson = std::make_unique<LoadJson>();
	//モデルを生成する
	std::thread textureThread(&yamadalib::Resources::LoadTextureResources, this);
	std::thread modelThread(&yamadalib::Resources::LoadModelResources, this);
	
	//処理を待機
	textureThread.join();
	modelThread.join();
}

/// <summary>
/// モデルリソースを読み込む
/// </summary>
void yamadalib::Resources::LoadModelResources()
{
	//プレイヤーモデルを読み込む
	m_graphics->GetFX()->SetDirectory(L"Resources/Models");
	//モデルの名前とファイルパスを取得する
	std::unordered_map<std::string, std::string> modelList = m_loadJson->LoadResources("Model");
	//モデルを読み込む
	for (const auto& item : modelList)
	{
	   RegistryModel(item.first, item.second);
	}
}

/// <summary>
/// テクスチャのリソースを読み込む
/// </summary>
void yamadalib::Resources::LoadTextureResources()
{
   //テクスチャの名前とファイルパスを取得する
   std::unordered_map<std::string, std::string> textureList = m_loadJson->LoadResources("Texture");
   //テクスチャを読み込む
   for (const auto& item : textureList)
   {
	  RegistryTexture(item.first, item.second);
   }
}

/// <summary>
/// モデルリストを作成
/// </summary>
/// <param name="modelName">モデルの名前</param>
/// <param name="path">モデルのファイルパス</param>
void yamadalib::Resources::RegistryModel(const std::string& modelName, const std::string& path)
{
   if (path == "") return;

   std::wstring ws(path.begin(), path.end());
   const wchar_t* wpath = ws.c_str();

   m_modelList[modelName] = DirectX::Model::CreateFromCMO(m_device, wpath, *m_graphics->GetFX());

}

/// <summary>
/// テクスチャリストを作成
/// </summary>
/// <param name="textureName">テクスチャの名前</param>
/// <param name="path">テクスチャのファイルパス</param>
void yamadalib::Resources::RegistryTexture(const std::string& textureName, const std::string& path)
{
   //パスがない場合は処理しない
   if (path == "") return;

   Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

   std::wstring ws(path.begin(), path.end());
   const wchar_t* wpath = ws.c_str();

   //拡張子で判別
   if (path.size() >= ExtensionLength && path.substr(path.size() - ExtensionLength) == ".dds")
   {
	  DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		 wpath, nullptr, texture.ReleaseAndGetAddressOf()));
   }
   else if(path.size() >= ExtensionLength && path.substr(path.size() - ExtensionLength) == ".png")
   {
	  DX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		 wpath, nullptr, texture.ReleaseAndGetAddressOf()));
   }

   m_textureList[textureName] = texture;
}


/// <summary>
/// 後始末する
/// </summary>
void yamadalib::Resources::Finalize()
{
   for (auto& model : m_modelList)
   {
	  if (model.second)
	  {
		 model.second.reset();
	  }
   }
   for (auto& texture : m_textureList) 
   {
	  if (texture.second)
	  {
		 texture.second.Reset();
	  }
   }
   m_textureList.clear(); 
}