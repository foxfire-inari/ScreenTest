#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include <vector>

std::vector<std::vector<Vector3D>> CreateCuveLines(float size, const Vector3D& center)
{
    std::vector<std::vector<Vector3D>> result;
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
        result.push_back({ vertices[edge[0]],vertices[edge[1]] });
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE); // ウィンドウモードに設定
    SetWindowSizeChangeEnableFlag(FALSE);//ウィンドウサイズを変えられないようにする
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

    //ワールド上の全ての線情報を保持
    std::vector< std::vector<Vector3D>> worldLine = {};

    std::vector< std::vector<Vector3D>>cubeLines = CreateCuveLines(50.f, { 0.0f,0.0f,50.0f });
    worldLine.insert(worldLine.end(), cubeLines.begin(), cubeLines.end());




    Camera* camera = new Camera();
    //TopAngle* topangle = new TopAngle(camera);

    // ウインドウが閉じられるかエスケープキーが押されるまでループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面をクリア
        ClearDrawScreen();

        camera->Update();

        camera->Draw(worldLine);

        //topangle->Draw(worldLine);

        // 裏画面の内容を表画面に反映
        ScreenFlip();
    }

    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}