#include "pch.h"
#include "GraphEditor/Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Libraries/yamadaLib/Resources.h"

// コンストラクタ
Graph::Graph()
    : 
    m_vertexList{},
    m_adjacencyList{},
    m_patrolRootMap{},
    m_patrolAdjacencyList{},
    m_initialVertexNumber(0)
{
}

// デストラクタ
Graph::~Graph()
{
    Clear();
}

//グラフを初期化する
void Graph::Clear()
{
    m_vertexList.clear();
    m_adjacencyList.clear();
    m_patrolRootMap.clear();
    m_patrolAdjacencyList.clear();
}

//頂点を生成する
bool Graph::CreateVertex(const int enemyIndex, const Vertex& vertex)
{
    //頂点リストに頂点を追加する
    m_vertexList.emplace_back(vertex);
    //存在する敵すべての隣接リストに頂点を追加する
    for (int number = 0; number <= enemyIndex; number++)
    {
        m_adjacencyList[number].adjacencyList.emplace_back(std::vector<int>(0, 0));
    }

    return true;
}

//頂点リストの頂点位置をスケーリングする
void Graph::ScaleVertexPositionOfVertexList(const float& scale)
{
    for (auto it = m_vertexList.begin(); it != m_vertexList.end(); it++)
    {
        it->SetPosition(it->GetPosition() * scale);
    }
}

// 頂点リストから頂点を削除する
bool Graph::RemoveVertexFromVertexList(const Vertex& vertex)
{
    // 頂点リストから頂点を削除する
    m_vertexList.erase(
        std::remove_if(
            m_vertexList.begin(),
            m_vertexList.end(),
            [&](Vertex const& vertexIndex) { return vertexIndex.GetPosition() == vertex.GetPosition(); }),
        m_vertexList.end()
    );
    // 頂点番号を再設定する
    ReNumber();
    // 正常終了
    return true;
}

// 頂点リストから頂点を削除する
bool Graph::RemoveVertexFromVertexList(const int& vertexIndex)
{
    // 頂点リストから頂点を削除する
    m_vertexList.erase(m_vertexList.begin() + vertexIndex);
    // 頂点番号を再設定する
    ReNumber();
    // 正常終了
    return true;
}


//隣接リストにエッジの頂点を追加する
bool Graph::AddVertexOfEdgeToAdjacencyList(const int& enemyNumber, const int& vertexNumber, const int& vertexNumberOfEdge)
{
    //隣接リストにエッジの頂点番号を追加する
    if (m_adjacencyList[enemyNumber].adjacencyList.empty())
        NewAdjacencyList(enemyNumber);

    m_adjacencyList[enemyNumber].adjacencyList[vertexNumber].push_back(vertexNumberOfEdge);

    return false;
}


// 隣接リストのエッジ頂点を削除する
bool Graph::RemoveVertexOfEdgeFromAdjacencyList(const int& enemyNumber, const int& vertexNumber)
{        
    // 隣接リストを削除する
    m_adjacencyList[enemyNumber].adjacencyList[vertexNumber].clear();
    // 隣接リストで削除すべき頂点番号を持つ要素を削除する
    for (int vertexIndex = 0; vertexIndex < (int)m_adjacencyList[enemyNumber].adjacencyList.size(); vertexIndex++)
    {
        // 隣接リストから頂点インデックスを削除する
        m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].erase(
            std::remove_if(
                m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].begin(),
                m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].end(),
                [&](int index) {return index == vertexNumber; }
            ),
            m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].end()
        );
    }
    // 近隣リストのエッジの頂点インデックスを調整する
    for (int vertexIndex = 0; vertexIndex < (int)m_adjacencyList[enemyNumber].adjacencyList.size(); vertexIndex++)
    {
        for (int edgeIndex = 0; edgeIndex < (int)m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].size(); edgeIndex++)
        {
            // 隣接リストの頂点インデックスが削除する頂点インデックスより大きい場合
            if (m_adjacencyList[enemyNumber].adjacencyList[vertexIndex][edgeIndex] > vertexNumber)
            {
                // 頂点インデックスを１減らす
                m_adjacencyList[enemyNumber].adjacencyList[vertexIndex][edgeIndex]--;
            }
        }
    }
    // 隣接リストの配列の要素を削除する
    m_adjacencyList[enemyNumber].adjacencyList.erase(m_adjacencyList[enemyNumber].adjacencyList.begin() + vertexNumber);

    // 正常終了
    return true;
}


// 頂点リストからインデックス番目の頂点を削除する
void Graph::RemoveVertexFromVertexListByIndex(const int& index)
{
    // 頂点リストの配列の要素を削除する
    m_vertexList.erase(m_vertexList.begin() + index);
}

// 隣接リストからインデックス番目のエッジの頂点を削除する
void Graph::RemoveVertexOfEdgeFromAdjacencyListByIndex(const int& enemyNumber, const int& index)
{
    // 隣接リストの配列の要素を削除する
    m_adjacencyList[enemyNumber].adjacencyList.erase(m_adjacencyList[enemyNumber].adjacencyList.begin() + index);
}


// 隣接リストからインデックス番目の頂点のエッジインデックス番目のエッジを削除する
void Graph::RemoveEgdeFromAdjacencyListByIndex(const int& enemyNumber, const int& index, const int& edgeIndex)
{
    // 隣接リストからインデックス番目の頂点のエッジインデックス番目のエッジを削除する
    m_adjacencyList[enemyNumber].adjacencyList[index].erase(m_adjacencyList[enemyNumber].adjacencyList[index].begin() + edgeIndex);
}

// 頂点番号を再設定する
void Graph::ReNumber()
{
    for (int index = 0; index < (int)m_vertexList.size(); index++)
    {
        // 頂点番号を再設定する
        m_vertexList[index].SetVertexNumber(index);
    }
    // 最大頂点番号を再設定する
    Vertex::ResetMaxNumber((int)m_vertexList.size());
}

// JSON形式でグラフデータを保存する
bool Graph::SaveGraphDataByJSON(int index)
{
    // 出力ファイルストリームを宣言する
    std::ofstream out;

    // 出力ファイルストリームをオープンする
    out.open(L"Resources\\EnemyRouteData\\StagePatrolRoot.json", std::ios::out);
    if (!out.is_open())
        return false;

    std::string text;
    out << "{" << std::endl;
    //敵の総数を出力する
    out << "\t\"enemy_index\": " << index << ", " << std::endl;

    // 頂点リストの文字列を出力する
    out << "\t\"rootList\": [" << std::endl;
    // 頂点リストを出力する
    for (int vertexIndex = 0; vertexIndex < (int)m_vertexList.size(); vertexIndex++)

    {
        // 頂点番号の文字列を出力する
        out << "\t\t{\"root_number\": ";
        // 頂点番号を出力する
        out << m_vertexList[vertexIndex].GetVertexNumber() << ", ";
        // 位置の文字列を出力する
        out << "\"position\": ";
        out << "{";
        // X座標を出力する
        out << "\"x\": " << m_vertexList[vertexIndex].GetPosition().x << ", ";
        // Y座標を出力する
        out << "\"y\": " << m_vertexList[vertexIndex].GetPosition().y << ", ";
        // Z座標を出力する
        out << "\"z\": " << m_vertexList[vertexIndex].GetPosition().z;

        out << "}";
        // 頂点リストのデータが継続しているかどうかを調べる
        if (vertexIndex < m_vertexList.size() - 1)
            // ブロックとコンマを出力する
            out << "}," << std::endl;
        else
            // ブロックを出力する
            out << "}" << std::endl;
    }
    out << "\t]," << std::endl;
    

    // 隣接リストの文字列を出力する
    out << "\t\"adjacency_list\": [" << std::endl;
    for (int enemyNumber = 0; enemyNumber <= index; enemyNumber++)
    {
        out << "\t\t{" << std::endl;
        // 敵番号を出力する
        out << "\t\t\t\"enemy_number\": " << enemyNumber << ", " << std::endl;

        // 敵の初期頂点番号を出力する
        out << "\t\t\t\"intial_number\": " << m_initialVertexNumber[enemyNumber][0] << ", " << std::endl;

        //隣接している頂点を出力
        out << "\t\t\t\"connected_vertices\": [" << std::endl;
        for (int vertexIndex = 0; vertexIndex < (int)m_adjacencyList[enemyNumber].adjacencyList.size(); vertexIndex++)
        {
            out << "\t\t\t\t[";
            for (int edgeIndex = 0; edgeIndex < (int)m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].size(); edgeIndex++)
            {
                // 隣接リストの頂点データを表示する
                out << m_adjacencyList[enemyNumber].adjacencyList[vertexIndex][edgeIndex];
                // 頂点リストの頂点データが継続しているかどうかを調べる
                if (edgeIndex < m_adjacencyList[enemyNumber].adjacencyList[vertexIndex].size() - 1)
                {
                    // 頂点データが継続している
                    out << ", ";
                }
            }
            // 頂点が隣接リストの最後かどうかを調べる
            if (vertexIndex < m_adjacencyList[enemyNumber].adjacencyList.size() - 1)
                // 頂点データは継続している
                out << "]," << std::endl;
            else
                // 頂点データは最後のデータ
                out << "]" << std::endl;
        }
        out << "\t\t\t]" << std::endl;
        if (enemyNumber < index)
            out << "\t\t}," << std::endl;
        else
            out << "\t\t}" << std::endl;
    }  
    out << "\t]" << std::endl;
    out << "}" << std::endl;


    // 出力ファイルストリームをクローズする
    out.close();
    // 正常終了
    return true;
}

// JSON形式でグラフデータを読み込む
bool Graph::LoadGraphDataByJSON(const int stageNumber)
{
    std::stringstream ss;
    std::string graphData;

    // 入力ファイルストリーム
    std::ifstream in(L"Resources\\EnemyRouteData\\Stage" + std::to_wstring(stageNumber) + L"PatrolRoot.json", std::ifstream::in);
    // 出力ファイルストリームをオープンする
    in.open(L"Resources\\EnemyRouteData\\Stage" + std::to_wstring(stageNumber) + L"PatrolRoot", std::ifstream::in);
    if (!in.is_open())
        return false;

    // ストリングストリーム
    ss << in.rdbuf();
    // 配列をクリアする
    m_graphData.clear();
    // 改行までの一行分の文字列を取得する
    while (std::getline(ss, graphData))
    {
        // グラフデータから不要な文字を消去する
        graphData.erase(std::remove_if(
            graphData.begin(),
            graphData.end(),
            [](char c) {
                return (
                    c == '\r' || c == '\t' || c == ' ' || c == '\n' || c == '\"' ||
                    c == '[' || c == ']' || c == '{' || c == '}'
                    );
            }),
            graphData.end()
        );
        if (!graphData.empty())
        {
            // 配列にワードを追加する
            m_graphData.push_back(graphData);
        }
    }
    // 入力ファイルストリームをクローズする
    in.close();
    // 正常終了
    return true;
}

/// JSON形式のデータをパースしオブジェクトに変換する
void Graph::ParseJSON()
{
    int position;

    //敵の総数
    int enemyIndex = 0;
    for (int index = 0; index < (int)m_graphData.size(); index++)
    {
        //敵の総数の文字列を検索する
        position = (int)m_graphData[index].find("enemy_index");
        //文字列リストが見つからなかった場合
        if (position == std::string::npos)
            continue;
        std::stringstream ss(m_graphData[index].c_str());
        //enemy_indexを削除する
        m_graphData[index].replace(position, strlen("enemy_index:"), "");
        ss.clear();
        ss.str(m_graphData[index]);
        ss >> enemyIndex;
        m_enemyIndex = enemyIndex;
        break;
    }

    // JSONデータをパースする
    for (int index = 0; index < (int)m_graphData.size(); index++)
    {
        int rootNumber;
        float x;
        float y;
        float z;
        // 書式：root_number:0,position:x:-40,y:20,
        std::stringstream ss(m_graphData[index].c_str());
        //敵の番号文字列を検索する
        position = (int)m_graphData[index].find("root_number:");
        // 文字列が見つからない場合
        if (position == std::string::npos)
            continue;
        // "root_number:"を削除する
        m_graphData[index].replace(position, strlen("root_number:"), "");
        // ",position:x:"を検索する
        position = (int)m_graphData[index].find(",position:x:");
        // ",position:x:"を空文字に置き換える
        m_graphData[index].replace(position, strlen(",position:x:"), " ");
        // ",y:"を探索する
        position = (int)m_graphData[index].find(",y:");
        // ",y:"を空文字に置き換える
        m_graphData[index].replace(position, strlen(",y:"), " ");
        // ",z:"を探索する
        position = (int)m_graphData[index].find(",z:");
        // ",z:"を空文字に置き換える
        m_graphData[index].replace(position, strlen(",z:"), " ");

        ss.clear();
        ss.str(m_graphData[index]);
        // 頂点の座標を取得する rootNumber経路番号,座標
        ss >> rootNumber >> x >> y >> z;
        //頂点情報を配列に保存する
        m_patrolRootMap.push_back(DirectX::SimpleMath::Vector3(x, y, z));
        // 頂点を生成する(VertexListに登録する)
        CreateVertex(m_enemyIndex, Vertex(DirectX::SimpleMath::Vector3(roundf(x), roundf(y), roundf(z))));
    }

    //隣接リストデータの抽出
    bool found = false;
    int enemyNumber = 0;
    int initialNumber = 0;
    int vertexIndex = 0;
    for (int index = 0; index < (int)m_graphData.size(); index++)
    {
        //初期検索
        if (!found)
        {
            position = (int)m_graphData[index].find("enemy_number:");
            if (position == std::string::npos)
                continue;
            found = true;
        }
        //敵番号を検索、connected_verticesの終わりを検知
        position = (int)m_graphData[index].find("enemy_number:");
        if (position != std::string::npos)
        {
            //敵番号の文字列を見つけた      
            m_graphData[index].replace(position, strlen("enemy_number:"), "");
            std::stringstream ss(m_graphData[index].c_str());
            ss >> enemyNumber;
            // indexがsize範囲内なら加算
            if (index < (int)m_graphData.size() - 1)
                index++;
            else
                continue;
            //初期頂点番号の文字列を見つけた
            m_graphData[index].replace(position, strlen("intial_number:"), "");
            std::stringstream ssInital(m_graphData[index].c_str());
            ssInital >> initialNumber;
            m_initialVertexNumber.resize(enemyNumber + 1);
            m_initialVertexNumber[enemyNumber].resize(1);
            m_initialVertexNumber[enemyNumber][0] = initialNumber;
            // indexがsize範囲内なら加算
            if (index < (int)m_graphData.size() - 1)
                index++;
            else
                continue;

            position = (int)m_graphData[index].find("connected_vertices:");
            m_graphData[index].replace(position, strlen("connected_vertices:"), "");
            vertexIndex = 0;
            // indexがsize範囲内なら加算
            if (index < (int)m_graphData.size() - 1)
                index++;
            else
                continue;
        }       

        // 隣接リストの行末のコンマが存在する場合は削除する
        if (m_graphData[index].at((m_graphData[index].size() - 1)) == ',')
            m_graphData[index].erase(--m_graphData[index].end());

        // ストリングストリームを生成する
        std::stringstream ssVertices(m_graphData[index].c_str());
        std::string vertexNumber;

        while (std::getline(ssVertices, vertexNumber, ','))
        {
            int vertexIndexOfEdge = std::stoi(vertexNumber);
            AddVertexOfEdgeToAdjacencyList(enemyNumber, vertexIndex, vertexIndexOfEdge);
        }
        vertexIndex++;
    }
}

void Graph::NewAdjacencyList(const int& enemyNumber)
{
    // 敵番号に対応する隣接リストが存在しない場合、頂点の数分だけリストを生成
    if (m_adjacencyList[enemyNumber].adjacencyList.empty())
    {
        for (int i = 0; i < m_vertexList.size(); i++)
        {
             m_adjacencyList[enemyNumber].adjacencyList.emplace_back(std::vector<int>(0, 0));
        }
    }
    else
    {
        for (int i = 0; i < m_vertexList.size(); i++)
        {
            if (m_adjacencyList[enemyNumber].adjacencyList.size() < m_vertexList.size())
                m_adjacencyList[enemyNumber].adjacencyList.emplace_back(std::vector<int>(0, 0));
        }
    }
}

//敵番号の隣接リストを削除する
void Graph::ClearNumberAdjacenyList(const int& enemyNumber)
{
    //敵番号の隣接リストを削除する
    m_adjacencyList.erase(enemyNumber);
    //敵の数を減らす
    m_enemyIndex--;
    std::unordered_map<int, PatrolRoute> updateList;

    for (auto it = m_adjacencyList.begin(); it != m_adjacencyList.end(); it++)
    {
        int currentEnemyNumber = it->first;
        if (currentEnemyNumber > enemyNumber)
        {
            updateList[currentEnemyNumber - 1] = it->second;
        }
        else
        {
            updateList[currentEnemyNumber] = it->second;
        }
    }
    //隣接リストを更新する
   m_adjacencyList = updateList;
   if (m_adjacencyList.empty())
   {
       NewAdjacencyList(enemyNumber);
   }
   //敵番号に対応した初期頂点番号を削除する
   m_initialVertexNumber.erase(m_initialVertexNumber.begin() + enemyNumber);
   if (m_initialVertexNumber.empty())
   {
       CreateInitailVertex(enemyNumber, m_vertexList.front().GetVertexNumber());
   }
}

//敵の初期頂点番号を作成
void Graph::CreateInitailVertex(const int& enemyNumber, const int vertexNumber)
{
    // 敵番号に対応する配列が存在しない場合は、サイズを調整
    if (m_initialVertexNumber.size() <= enemyNumber)
    {
        m_initialVertexNumber.resize(enemyNumber + 1);
        m_initialVertexNumber[enemyNumber].resize(1);
    }   
    m_initialVertexNumber[enemyNumber][0] = vertexNumber;
}
