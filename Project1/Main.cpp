#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include <vector>

std::vector<std::vector<Vector3D>> CreateCuveLines(float size, const Vector3D& center)
{
    std::vector<std::vector<Vector3D>> result;
    //center�𒆐S�ɂ��邽��
    float halfSize = size * 0.5f;

    //�����̂̒��_��8��
    Vector3D vertices[8] =
    {
        //���ꂼ��C���Ȃ���C���[�W
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
    ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
    SetWindowSizeChangeEnableFlag(FALSE);//�E�B���h�E�T�C�Y��ς����Ȃ��悤�ɂ���
    SetGraphMode(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT), 32);
    // �c�w���C�u�����̏�����
    if (DxLib_Init() < 0)
    {
        // �G���[�����������璼���ɏI��
        return -1;
    }
    //�J�[�\����\��
    SetMouseDispFlag(false);
    // �`���𗠉�ʂɕύX
    SetDrawScreen(DX_SCREEN_BACK);

    //���[���h��̑S�Ă̐�����ێ�
    std::vector< std::vector<Vector3D>> worldLine = {};

    std::vector< std::vector<Vector3D>>cubeLines = CreateCuveLines(50.f, { 0.0f,0.0f,50.0f });
    worldLine.insert(worldLine.end(), cubeLines.begin(), cubeLines.end());




    Camera* camera = new Camera();
    //TopAngle* topangle = new TopAngle(camera);

    // �E�C���h�E�������邩�G�X�P�[�v�L�[���������܂Ń��[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // ��ʂ��N���A
        ClearDrawScreen();

        camera->Update();

        camera->Draw(worldLine);

        //topangle->Draw(worldLine);

        // ����ʂ̓��e��\��ʂɔ��f
        ScreenFlip();
    }

    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}