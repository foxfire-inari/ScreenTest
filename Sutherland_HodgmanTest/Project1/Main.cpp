#include "Common.h"
#include "DxLib.h"

namespace
{
    const float CLIP_LEFT = 100;
    const float CLIP_RIGHT = 300;
    const float CLIP_BOTTOM = 100;
    const float CLIP_TOP = 300;
}


/// <summary>
/// 平面上の頂点
/// </summary>
struct Vec2 {
    float x, y;
};

/// <summary>
/// はみ出ている方向
/// </summary>
enum Edge {
    LEFT, RIGHT, BOTTOM, TOP
};

/// <summary>
/// 三角形の頂点
/// </summary>
struct Triangle {
    Vec2 a, b, c;
};

/// <summary>
/// ポリゴンから三角形を作成
/// </summary>
/// <param name="polygon">ポリゴン</param>
/// <returns>三角形の配列</returns>
std::vector<Triangle> Triangulate(const std::vector<Vec2>& polygon) 
{
    //三角形を複数保存
    std::vector<Triangle> triangles;

    //三角形を作る
    if (polygon.size() < 3) return triangles;

    //ポリゴンの1つ目の頂点から三角形を作成
    for (size_t i = 1; i + 1 < polygon.size(); ++i) 
    {
        triangles.push_back({ polygon[0], polygon[i], polygon[i + 1] });
    }

    return triangles;
}

/// <summary>
/// 内側にあるか
/// </summary>
/// <param name="p">線分</param>
/// <param name="edge">確認する方向</param>
/// <returns>はみ出ているかどうか</returns>
bool inside(const Vec2& p, Edge edge) {
    switch (edge) {
    case LEFT:   return p.x >= CLIP_LEFT;
    case RIGHT:  return p.x <= CLIP_RIGHT;
    case BOTTOM: return p.y >= CLIP_BOTTOM;
    case TOP:    return p.y <= CLIP_TOP;
    }
    return false;
}

/// <summary>
/// 辺（p1→p2）とクリップ線との交点を計算
/// </summary>
/// <param name="p1">線分の開始位置</param>
/// <param name="p2">線分の終了位置</param>
/// <param name="edge">対象とするクリップ線</param>
/// <returns>クリップされた点を返す</returns>
Vec2 intersect(const Vec2& p1, const Vec2& p2, Edge edge) 
{
    //線分の交点
    float x, y;
    //線形保管用の値（傾き）
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    //反応した辺ごとに交点を求める
    switch (edge)
    {
    case LEFT:
        x = CLIP_LEFT;
        y = p1.y + dy * (CLIP_LEFT - p1.x) / dx;
        break;
    case RIGHT:
        x = CLIP_RIGHT;
        y = p1.y + dy * (CLIP_RIGHT - p1.x) / dx;
        break;
    case BOTTOM:
        y = CLIP_BOTTOM;
        x = p1.x + dx * (CLIP_BOTTOM - p1.y) / dy;
        break;
    case TOP:
        y = CLIP_TOP;
        x = p1.x + dx * (CLIP_TOP - p1.y) / dy;
        break;
    }

    //x = p1.x + t * dx
    //y = p1.y + t * dy
    //x=CLIP_LEFTの場合
    //CLIP_LEFT=p1.x+t*dx
    //t=(CLIP_LEFT-p1.x)/dx


    return { x, y };
}

/// <summary>
/// ポリゴンをクリップし、新しいポリゴンを作る
/// </summary>
/// <param name="input">ポリゴン</param>
/// <param name="edge">対象とするクリップ線</param>
/// <returns>クリップ後のポリゴン</returns>
std::vector<Vec2> clipEdge(const std::vector<Vec2>& input, Edge edge) 
{
    //結果をまとめた配列
    std::vector<Vec2> output;

    //ポリゴンの辺を1つずつ計算
    for (size_t i = 0; i < input.size(); ++i) 
    {
        //ポリゴンの辺（頂点とその1つ前の頂点）を1本ずつ処理
        Vec2 current = input[i];
        Vec2 prev = input[(i + input.size() - 1) % input.size()];

        //辺の始点終点がクリップ線の内側化外側かを判定
        bool currIn = inside(current, edge);
        bool prevIn = inside(prev, edge);

        //はみ出ていないか、始点か終点がはみ出ている場合は保存する
        if (prevIn && currIn) {
            output.push_back(current);
        }
        else if (prevIn && !currIn) {
            output.push_back(intersect(prev, current, edge));
        }
        else if (!prevIn && currIn) {
            output.push_back(intersect(prev, current, edge));
            output.push_back(current);
        }
    }

    return output;
}

/// <summary>
/// ポリゴンをクリップ範囲に収める処理
/// </summary>
/// <param name="polygon">ポリゴン</param>
/// <returns>クリップ後のポリゴン</returns>
std::vector<Vec2> sutherlandHodgmanClip(const std::vector<Vec2>& polygon) 
{
    std::vector<Vec2> output = polygon;

    //各クリップ線で線分を処理
    for (Edge e : {LEFT, RIGHT, BOTTOM, TOP}) {
        output = clipEdge(output, e);
    }
    return output;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウィンドウモードに設定
    ChangeWindowMode(TRUE); 
    //ウィンドウサイズを変えられないようにする
    SetWindowSizeChangeEnableFlag(FALSE);
    //ウィンドウサイズをセット
    SetGraphMode(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT), 32);
    // ＤＸライブラリの初期化
    if (DxLib_Init() < 0)
    {
        // エラーが発生したら直ちに終了
        return -1;
    }

    Vec2 offset = { 0, 0 };
    Vec2 prevMouse = { 0, 0 };
    bool dragging = false;

    // 描画先を裏画面に変更
    SetDrawScreen(DX_SCREEN_BACK);

    //クリップ対象
    std::vector<Vec2> polygon = {
    { 50, 150 },
    { 200, 50 },
    { 350, 150 },
    { 200, 350 },
    { 50, 300 }
    };

    // ウインドウが閉じられるかエスケープキーが押されるまでループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        int mx, my;
        GetMousePoint(&mx, &my);
        Vec2 mouse = { (float)mx, (float)my };

        // 左ボタン押下
        if (GetMouseInput() & MOUSE_INPUT_LEFT) {
            if (!dragging) {
                dragging = true;
                prevMouse = mouse;
            }
            else {
                // 差分だけ動かす
                offset.x += mouse.x - prevMouse.x;
                offset.y += mouse.y - prevMouse.y;
                prevMouse = mouse;
            }
        }
        else {
            dragging = false;
        }

        // 画面をクリア
        ClearDrawScreen();

        //動いた後のポリゴン
        std::vector<Vec2> movedPolygon;

        //動いた後のポリゴンを保存
        for (const auto& p : polygon) {
            movedPolygon.push_back({ p.x + offset.x, p.y + offset.y });
        }

        //ポリゴンをクリップする
        auto clipped = sutherlandHodgmanClip(movedPolygon);
        //クリップ後のポリゴンを三角ポリゴンに変更
        auto triangles = Triangulate(clipped);


        DrawBox(CLIP_LEFT, CLIP_TOP, CLIP_RIGHT, CLIP_BOTTOM, GetColor(0, 0, 255), FALSE);

        // 元ポリゴン（赤）
        for (int i = 0; i < movedPolygon.size(); ++i) {
            auto& a = movedPolygon[i];
            auto& b = movedPolygon[(i + 1) % movedPolygon.size()];
            DrawLine(a.x, a.y, b.x, b.y, GetColor(255, 0, 0));
        }

        // クリップ後ポリゴン（三角形・青緑）
        for (const auto& tri : triangles) {
            DrawTriangle(
                static_cast<int>(tri.a.x), static_cast<int>(tri.a.y),
                static_cast<int>(tri.b.x), static_cast<int>(tri.b.y),
                static_cast<int>(tri.c.x), static_cast<int>(tri.c.y),
                GetColor(0, 255, 255), TRUE  // 塗りつぶしON
            );
        }


        // 三角形（）
        for (const auto& tri : triangles) {
            DrawLine(tri.a.x, tri.a.y, tri.b.x, tri.b.y, GetColor(255, 0, 0));
            DrawLine(tri.b.x, tri.b.y, tri.c.x, tri.c.y, GetColor(255, 0, 0));
            DrawLine(tri.c.x, tri.c.y, tri.a.x, tri.a.y, GetColor(255, 0, 0));
        }


        // 裏画面の内容を表画面に反映
        ScreenFlip();
    }


    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}