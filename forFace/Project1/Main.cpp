#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include "ObjFile.h"
#include "FaceVertex.h"
#include "MeshObject.h"

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

    //モデルを読み込むシングルトンクラスを作成
    ObjFile::Create();
    
    //剣のモデルを読み込む
    MeshObject modelSword = ObjFile::GetInstance()->LoadModel("3DModel/Sting-Sword-lowpoly.obj");
    worldObjects.emplace_back(modelSword);
    
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