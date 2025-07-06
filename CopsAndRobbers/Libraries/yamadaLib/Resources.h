/*
	@file	Resources.h
	@brief	リソースクラス
*/
#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Game/CommonResources.h"
#include "Libraries/yamadalib/Graphics.h"
#include "Libraries/yamadaLib/ResourcesList.h"
#include "Libraries/yamadaLib/LoadJson.h"
#include <unordered_map>

namespace yamadalib
{
	class Resources
	{
	public:
	   static constexpr size_t ExtensionLength = 4; // "識別する文字数
	public:
	    //モデルリストからモデルを取得する
	   DirectX::Model* GetModel(const std::string& name) 
	   {
		  auto it = m_modelList.find(name);
		  if (it != m_modelList.end())
		  {
			 return it->second.get();
		  }
		  return nullptr; 
	   }

		//リストの名前からテクスチャを取得する
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string& name) 
		{
		   auto it = m_textureList.find(name);
		   if (it != m_textureList.end())
		   {
			  return it->second;
		   }
		   return nullptr;
		}

	public:
		Resources(Resources&&) = default;
		Resources& operator= (Resources&&) = default;
		Resources& operator= (Resources const&) = delete;
		//デストラクタ
		~Resources() =default;
		//Resourcesクラスのインスタンスを取得する
		static Resources* const GetInstance();
		//リソースを読み込む
		void LoadResources();
		//モデルを読み込む
		void LoadModelResources();
		//テクスチャを読み込む
		void LoadTextureResources();
		//モデルリストを作成
		void RegistryModel(const std::string& modelName, const std::string& path);
		//テクスチャリストを作成
		void RegistryTexture(const std::string& textureName, const std::string& path);

		void Finalize();

	private:
		// コンストラクタ
		Resources() noexcept
			:
			m_graphics(nullptr),
			m_device(nullptr)
		{
			m_graphics = yamadalib::Graphics::GetInstance();
			m_device = yamadalib::Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
		}
	private:
		//リソース
		static std::unique_ptr<yamadalib::Resources> m_resources;

		std::unique_ptr<LoadJson> m_loadJson;	 //Jsonファイルロード

		//グラフィックス
		yamadalib::Graphics* m_graphics = yamadalib::Graphics::GetInstance();
		//デバイス
		ID3D11Device1* m_device = m_graphics->GetDeviceResources()->GetD3DDevice();

		//モデルのリスト
		std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_modelList;

		//テクスチャのリスト
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureList;
	};
}
#endif // RESOURCES_DEFINED
