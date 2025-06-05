#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include "ObjFile.h"
#include "FaceVertex.h"
#include "MeshObject.h"

/// <summary>
/// キューブ状のオブジェクトを作る関数
/// </summary>
/// <param name="size"></param>
/// <param name="center"></param>
/// <returns></returns>
//MeshObject CreateCuveObject(float size, const Vector3D& center)
//{
//    MeshObject result;
//    //centerを中心にするため
//    float halfSize = size * 0.5f;
//
//    //直方体の長点は8つ
//    Vector3D vertices[8] =
//    {
//        //それぞれCをなぞるイメージ
//        center + Vector3D{  halfSize, halfSize, halfSize},
//        center + Vector3D{ -halfSize, halfSize, halfSize},
//        center + Vector3D{ -halfSize,-halfSize, halfSize},
//        center + Vector3D{  halfSize,-halfSize, halfSize},
//
//        center + Vector3D{  halfSize, halfSize,-halfSize},
//        center + Vector3D{ -halfSize, halfSize,-halfSize},
//        center + Vector3D{ -halfSize,-halfSize,-halfSize},
//        center + Vector3D{  halfSize,-halfSize,-halfSize}
//
//    };
//    int edges[12][2] =
//    {
//        {0, 1}, {1, 2}, {2, 3}, {3, 0},
//        {4, 5}, {5, 6}, {6, 7}, {7, 4},
//        {0, 4}, {1, 5}, {2, 6}, {3, 7}
//    };
//    for (const auto& edge : edges)
//    {
//        FaceVertex _vertexPair(vertices[edge[0]], vertices[edge[1]]);
//        result.faceVertexs.push_back(_vertexPair);
//    }
//    return result;
//}


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

    //キューブを作成＆保存
    //MeshObject cube = CreateCuveLines(50.f, { 0.0f,0.0f,50.0f });
    //worldObjects.emplace_back(cube);

    //モデルを読み込むシングルトンクラスを作成
    ObjFile::Create();
    
    //剣のモデルを読み込む
    //MeshObject modelSword = ObjFile::GetInstance()->LoadModel("3DModel/Sting-Sword-lowpoly.obj");
    //worldObjects.emplace_back(modelSword);
    
    //ミクさんを読み込む
    //MeshObject modelmiku = ObjFile::GetInstance()->LoadModel("3DModel/miku.obj");
    //worldObjects.emplace_back(modelmiku);

    //ガチャガチャを読み込む
    //MeshObject modelGacya = ObjFile::GetInstance()->LoadModel("3DModel/Gacya.obj");
    //worldObjects.emplace_back(modelGacya);

    //木を読み込む
    //MeshObject modelTree = ObjFile::GetInstance()->LoadModel("3DModel/Tree.obj");
    //worldObjects.emplace_back(modelTree);


    ObjFile::Destroy();

    Camera* camera = new Camera();

    //TopAngle* topangle = new TopAngle(camera);

    // ウインドウが閉じられるかエスケープキーが押されるまでループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面をクリア
        ClearDrawScreen();

        camera->Update();

        camera->Draw(worldObjects);

        //topangle->Draw(worldLine);

        // 裏画面の内容を表画面に反映
        ScreenFlip();
    }

    ObjFile::Destroy();

    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}