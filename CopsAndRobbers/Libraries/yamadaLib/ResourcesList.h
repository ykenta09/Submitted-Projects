#pragma once


/// <summary>
/// リソースパラメータ
/// </summary>
struct ResourcesParameter
{
   std::string name;
   std::string path;
};

/// <summary>
/// テクスチャリスト
/// </summary>
enum class TextureList
{
   Stamina,
   Shadow,
   Detection,
   Timer,
   BackTimer,
   ClearTimer,
   PushKey,
   ResultBack,
   ResultBoard,
   MissionBack,
   Title,
   Text,
   Menu,
   Cursor,
   TextBack,
   BackTexture,
   Warning,
   MiniMapBack,
   MissionText,
   ControlsText,
   StageSelect,
   Stage,
   Hint,
   MaskCircle,
   ResultText,
   Floor01_DDS,
   Floor02_DDS,
   Floor03_DDS,
   Wall01_DDS,
   Wall03_DDS,
   Wall04_DDS,
   Key_DDS
};


/// <summary>
/// モデルリスト
/// </summary>
enum  class ModelList
{
   PlayerHead,			//プレイヤーの頭モデル
   PlayerBody,			//プレイヤーの胴体モデル
   PlayerArm,			//プレイヤーの腕モデル
   PlayerFoot,			//プレイヤーの足モデル
   FellowHead,			//仲間の頭モデル
   FellowBody,			//仲間の胴体モデル
   FellowArm,			//仲間の腕モデル
   FellowFoot,			//仲間の足モデル
   EnemyHead,			//敵の頭モデル
   EnemyBody,			//敵の胴体モデル
   EnemyArm,			//敵の腕モデル
   EnemyFoot,			//敵の足モデル
   StageBase,			//ステージのベースモデル
   CelestialSphere,		//天球モデル
   Key,					//鍵のモデル
   PrisonRoom,			//牢屋の部屋モデル
   PrisonDoar,			//牢屋のドアモデル
   GateFrame,			//門のフレームモデル
   Gate,				//門の扉モデル
   Door,				//ステージ間移動ドアモデル
   Floor01,				//床モデル
   Floor02,
   Floor03,
   MovingFloor,			//動く床モデル
   Wall01,				//壁モデル
   Wall02,
   Wall03,
   Wall04,	   
   Room,				//ステージセレクトの部屋
   Lever01_Stand,				//レバー装置
   Lever01_Handle				//レバー装置
};