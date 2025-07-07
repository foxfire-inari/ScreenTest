#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include "ObjFile.h"
#include "LineVertex.h"
#include "MeshObject.h"

/// <summary>
/// キューブ状のラインを作る関数(メッシュ表示のテスト用)
/// </summary>
/// <param name="size">一辺の長さ</param>
/// <param name="center">中心座標</param>
/// <returns>メッシュで構成されたオブジェクト</returns>
MeshObject CreateCuveLines(float size, const Vector3D& center)
{
    MeshObject result;
    //centerを中心にするため
    float halfSize = size * 0.5f;

    //直方体の長点は8つ
    Vector3D vertices[8] =
    {
        //それぞれCをなぞるイメージ
        center + Vector3D{  halfSize, halfSize, halfSize},
        center + Vector3D{ -halfSize, halfSize, halfSize},
        center + Vector3D{ -halfSize,-halfSize, halfSize},
        center + Vector3D{  halfSize,-halfSize, halfSize},

        center + Vector3D{  halfSize, halfSize,-halfSize},
        center + Vector3D{ -halfSize, halfSize,-halfSize},
        center + Vector3D{ -halfSize,-halfSize,-halfSize},
        center + Vector3D{  halfSize,-halfSize,-halfSize}

    };
    int edges[12][2] =
    {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
    for (const auto& edge : edges)
    {
        LineVertex _vertexPair(vertices[edge[0]], vertices[edge[1]]);
        result.lineVertexs.push_back(_vertexPair);
    }
    return result;
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
    //カーソル非表示
    SetMouseDispFlag(false);
    // 描画先を裏画面に変更
    SetDrawScreen(DX_SCREEN_BACK);

    //要素の削除を効率よくするためlistにする
    std::list<MeshObject> worldObjects = {};

    //キューブを作成＆保存--------------メッシュ表示のテスト用
    //MeshObject cube = CreateCuveLines(50.f, { 0.0f,0.0f,50.0f });
    //worldObjects.emplace_back(cube);

    //モデルを読み込むシングルトンクラスを作成
    ObjFile::Create();
    
    //剣のモデルを読み込む--------------三角形で無いオブジェクトの描画
    //MeshObject modelSword = ObjFile::GetInstance()->LoadModel("3DModel/Sting-Sword-lowpoly.obj");
    //worldObjects.emplace_back(modelSword);
    
    //ミクさんを読み込む--------------膨大な頂点を持つオブジェクトの描画（主に読み込み速度のテスト用）
    MeshObject modelmiku = ObjFile::GetInstance()->LoadModel("3DModel/miku.obj");
    worldObjects.emplace_back(modelmiku);

    //ガチャガチャを読み込む--------------ネット上にないモデルの描画（万が一の状況がないかの確認）
    //MeshObject modelGacya = ObjFile::GetInstance()->LoadModel("3DModel/Gacya.obj");
    //worldObjects.emplace_back(modelGacya);

    //木を読み込む--------------非常に頂点が密集している状況下でのテスト用
    //MeshObject modelTree = ObjFile::GetInstance()->LoadModel("3DModel/Tree.obj");
    //worldObjects.emplace_back(modelTree);


    //読み込み終わったら削除
    ObjFile::Destroy();

    //カメラオブジェクトを生成
    Camera* camera = new Camera();

    // ウインドウが閉じられるかエスケープキーが押されるまでループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面をクリア
        ClearDrawScreen();

        //カメラの更新処理
        camera->Update();
        //描画処理
        camera->Draw(worldObjects);

        // 裏画面の内容を表画面に反映
        ScreenFlip();
    }

    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}