#pragma once
#include "Scene.h"
#include "Game.h"
#include <string>
#include <vector>

class SelectMenuBase
{
public:
    // コンストラクタ
    SelectMenuBase();
    // デストラクタ
    virtual ~SelectMenuBase();
    // 初期化
    void Init(bool isMouse);
    // 更新処理
    void Update(const InputState& input);
    // 描画
    void Draw();
    
    // 選択項目追加
    void AddSelectItem(std::string itemName);

    // 選択中の項目値取得
    int GetSelectedNum() { return m_selectedPos; }

    // メニュー項目描画位置設定
    void SetDrawPos(float x, float y) { m_drawPos.x = x; m_drawPos.y = y; }
    void SetTitleDrawPos(float x, float y) { m_titleDrawPos.x = x; m_titleDrawPos.y = y; }
    // 文字カラーの設定
    void SetItemColor(int color) { m_itemColor = color; }
    void SetSelectedItemColor(int color) { m_selectedItemColor = color; }
    // シーン文字列の設定
    void SetTitleTextAndPos(std::string sceneText, float x, float y) 
    { m_sceneText = std::make_shared<std::string>(sceneText); m_titleDrawPos.x = x; m_titleDrawPos.y = y; }

private: // プライベート関数
    // 文字列の更新
    void DrawMenuText();
    // シーンタイトル描画
    void DrawSceneText();

    // メンバ関数ポインタ(更新)
    using m_tUpdateFunc = void (SelectMenuBase::*) (const InputState& input);
    m_tUpdateFunc m_updateFunc = nullptr;
    // カーソル更新処理
    void CursorUpdate(const InputState& input);
    // 通常選択更新処理
    void NormalUpdate(const InputState& input);

private:
    struct SelectItemState
    {
        std::string itemName = "EMPTY";
    };

private: // シーン装飾の管理変数
    // 選択中の項目
    int m_selectedPos;
    // 項目数
    std::vector<SelectItemState> m_selectItems;
    // タイトル文字
    std::shared_ptr<std::string> m_sceneText;

    // カーソル位置
    MouseInputState m_mouseState;
    // カーソルが枠内にあるかどうか
    bool m_isCursorRanged;

    // 項目描画位置
    VECTOR m_drawPos;
    // シーン文字列描画位置
    VECTOR m_titleDrawPos;
    // 選択項目のカラー
    int m_itemColor;
    int m_selectedItemColor;
    // 毎フレームカウント
	int m_countFrame;
};
