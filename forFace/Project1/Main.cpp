#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include "ObjFile.h"
#include "FaceVertex.h"
#include "MeshObject.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // �E�B���h�E���[�h�ɐݒ�
    ChangeWindowMode(TRUE); 
    //�E�B���h�E�T�C�Y��ς����Ȃ��悤�ɂ���
    SetWindowSizeChangeEnableFlag(FALSE);
    //�E�B���h�E�T�C�Y���Z�b�g
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

    //�v�f�̍폜�������悭���邽��list�ɂ���
    std::list<MeshObject> worldObjects = {};

    //���f����ǂݍ��ރV���O���g���N���X���쐬
    ObjFile::Create();
    
    //���̃��f����ǂݍ���
    MeshObject modelSword = ObjFile::GetInstance()->LoadModel("3DModel/Sting-Sword-lowpoly.obj");
    worldObjects.emplace_back(modelSword);
    
    //�~�N�����ǂݍ���
    //MeshObject modelmiku = ObjFile::GetInstance()->LoadModel("3DModel/miku.obj");
    //worldObjects.emplace_back(modelmiku);

    //�K�`���K�`����ǂݍ���
    //MeshObject modelGacya = ObjFile::GetInstance()->LoadModel("3DModel/Gacya.obj");
    //worldObjects.emplace_back(modelGacya);

    //�؂�ǂݍ���
    //MeshObject modelTree = ObjFile::GetInstance()->LoadModel("3DModel/Tree.obj");
    //worldObjects.emplace_back(modelTree);


    ObjFile::Destroy();

    Camera* camera = new Camera();

    //TopAngle* topangle = new TopAngle(camera);

    // �E�C���h�E�������邩�G�X�P�[�v�L�[���������܂Ń��[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // ��ʂ��N���A
        ClearDrawScreen();

        camera->Update();

        camera->Draw(worldObjects);

        //topangle->Draw(worldLine);

        // ����ʂ̓��e��\��ʂɔ��f
        ScreenFlip();
    }

    ObjFile::Destroy();

    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}