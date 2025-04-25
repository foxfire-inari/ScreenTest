#include "Common.h"
#include "DxLib.h"
#include "Camera.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE); // ウィンドウモードに設定
    SetWindowSizeChangeEnableFlag(FALSE);//ウィンドウサイズを変えられないようにする
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);


    // ＤＸライブラリの初期化
    if (DxLib_Init() < 0)
    {
        // エラーが発生したら直ちに終了
        return -1;
    }

    SetMouseDispFlag(false);

    // 描画先を裏画面に変更
    SetDrawScreen(DX_SCREEN_BACK);

    std::vector< std::vector<Vector3D>> worldLine = {};

    //一辺200の立方体の辺の情報
    {
        std::vector<Vector3D> nearLine_under;
        nearLine_under.emplace_back(SetVec3D(-100, -100, 150));
        nearLine_under.emplace_back(SetVec3D(100, -100, 150));

        std::vector<Vector3D> nearLine_left;
        nearLine_left.emplace_back(SetVec3D(-100, -100, 150));
        nearLine_left.emplace_back(SetVec3D(-100, 100, 150));

        std::vector<Vector3D> nearLine_top;
        nearLine_top.emplace_back(SetVec3D(-100, 100, 150));
        nearLine_top.emplace_back(SetVec3D(100, 100, 150));

        std::vector<Vector3D> nearLine_right;
        nearLine_right.emplace_back(SetVec3D(100, -100, 150));
        nearLine_right.emplace_back(SetVec3D(100, 100, 150));

        std::vector<Vector3D> midLine_under;
        midLine_under.emplace_back(SetVec3D(-100, -100, 150));
        midLine_under.emplace_back(SetVec3D(-100, -100, 350));

        std::vector<Vector3D> midLine_left;
        midLine_left.emplace_back(SetVec3D(-100, 100, 150));
        midLine_left.emplace_back(SetVec3D(-100, 100, 350));

        std::vector<Vector3D> midLine_top;
        midLine_top.emplace_back(SetVec3D(100, 100, 150));
        midLine_top.emplace_back(SetVec3D(100, 100, 350));

        std::vector<Vector3D> midLine_right;
        midLine_right.emplace_back(SetVec3D(100, -100, 150));
        midLine_right.emplace_back(SetVec3D(100, -100, 350));

        std::vector<Vector3D> farLine_under;
        farLine_under.emplace_back(SetVec3D(-100, -100, 350));
        farLine_under.emplace_back(SetVec3D(100, -100, 350));

        std::vector<Vector3D> farLine_left;
        farLine_left.emplace_back(SetVec3D(-100, -100, 350));
        farLine_left.emplace_back(SetVec3D(-100, 100, 350));

        std::vector<Vector3D> farLine_top;
        farLine_top.emplace_back(SetVec3D(-100, 100, 350));
        farLine_top.emplace_back(SetVec3D(100, 100, 350));

        std::vector<Vector3D> farLine_right;
        farLine_right.emplace_back(SetVec3D(100, -100, 350));
        farLine_right.emplace_back(SetVec3D(100, 100, 350));

        worldLine.emplace_back(nearLine_under);
        worldLine.emplace_back(nearLine_left);
        worldLine.emplace_back(nearLine_top);
        worldLine.emplace_back(nearLine_right);

        worldLine.emplace_back(midLine_under);
        worldLine.emplace_back(midLine_left);
        worldLine.emplace_back(midLine_top);
        worldLine.emplace_back(midLine_right);

        worldLine.emplace_back(farLine_under);
        worldLine.emplace_back(farLine_left);
        worldLine.emplace_back(farLine_top);
        worldLine.emplace_back(farLine_right);

    }

    Camera* camera = new Camera();

    // ウインドウが閉じられるかエスケープキーが押されるまでループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面をクリア
        ClearDrawScreen();

        camera->Update();
        camera->Draw(worldLine);


        // 裏画面の内容を表画面に反映
        ScreenFlip();
    }

    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}