#include "Common.h"
#include "DxLib.h"
#include "Camera.h"
#include "ObjFile.h"
#include "FaceVertex.h"
#include "FaceObject.h"

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
    std::list<FaceObject> worldObjects = {};

    //���f����ǂݍ��ރV���O���g���N���X���쐬
    ObjFile::Create();
    
    //���̃��f����ǂݍ���--------------�O�p�`�Ŗ����I�u�W�F�N�g�̕`��
    FaceObject modelSword = ObjFile::GetInstance()->LoadModel("3DModel/Sting-Sword-lowpoly.obj");
    worldObjects.emplace_back(modelSword);
    
    //�~�N�����ǂݍ���--------------�c��Ȓ��_�����I�u�W�F�N�g�̕`��i��ɓǂݍ��ݑ��x�̃e�X�g�p�j
    //FaceObject modelmiku = ObjFile::GetInstance()->LoadModel("3DModel/miku.obj");
    //worldObjects.emplace_back(modelmiku);

    //�K�`���K�`����ǂݍ���--------------�l�b�g��ɂȂ����f���̕`��i������̏󋵂��Ȃ����̊m�F�j
    //FaceObject modelGacya = ObjFile::GetInstance()->LoadModel("3DModel/Gacya.obj");
    //worldObjects.emplace_back(modelGacya);

    //�؂�ǂݍ���--------------���ɒ��_�����W���Ă���󋵉��ł̃e�X�g�p
    //FaceObject modelTree = ObjFile::GetInstance()->LoadModel("3DModel/Tree.obj");
    //worldObjects.emplace_back(modelTree);

    //�ǂݍ��ݏI�������폜
    ObjFile::Destroy();

    //�J�����I�u�W�F�N�g�𐶐�
    Camera* camera = new Camera();

    // �E�C���h�E�������邩�G�X�P�[�v�L�[���������܂Ń��[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // ��ʂ��N���A
        ClearDrawScreen();

        //�J�����̍X�V����
        camera->Update();
        //�`�揈��
        camera->Draw(worldObjects);

        // ����ʂ̓��e��\��ʂɔ��f
        ScreenFlip();
    }

    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}