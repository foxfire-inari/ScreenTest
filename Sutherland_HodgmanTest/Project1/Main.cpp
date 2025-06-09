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
/// ���ʏ�̒��_
/// </summary>
struct Vec2 {
    float x, y;
};

/// <summary>
/// �͂ݏo�Ă������
/// </summary>
enum Edge {
    LEFT, RIGHT, BOTTOM, TOP
};

/// <summary>
/// �O�p�`�̒��_
/// </summary>
struct Triangle {
    Vec2 a, b, c;
};

/// <summary>
/// �|���S������O�p�`���쐬
/// </summary>
/// <param name="polygon">�|���S��</param>
/// <returns>�O�p�`�̔z��</returns>
std::vector<Triangle> Triangulate(const std::vector<Vec2>& polygon) 
{
    //�O�p�`�𕡐��ۑ�
    std::vector<Triangle> triangles;

    //�O�p�`�����
    if (polygon.size() < 3) return triangles;

    //�|���S����1�ڂ̒��_����O�p�`���쐬
    for (size_t i = 1; i + 1 < polygon.size(); ++i) 
    {
        triangles.push_back({ polygon[0], polygon[i], polygon[i + 1] });
    }

    return triangles;
}

/// <summary>
/// �����ɂ��邩
/// </summary>
/// <param name="p">����</param>
/// <param name="edge">�m�F�������</param>
/// <returns>�͂ݏo�Ă��邩�ǂ���</returns>
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
/// �Ӂip1��p2�j�ƃN���b�v���Ƃ̌�_���v�Z
/// </summary>
/// <param name="p1">�����̊J�n�ʒu</param>
/// <param name="p2">�����̏I���ʒu</param>
/// <param name="edge">�ΏۂƂ���N���b�v��</param>
/// <returns>�N���b�v���ꂽ�_��Ԃ�</returns>
Vec2 intersect(const Vec2& p1, const Vec2& p2, Edge edge) 
{
    //�����̌�_
    float x, y;
    //���`�ۊǗp�̒l�i�X���j
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    //���������ӂ��ƂɌ�_�����߂�
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
    //x=CLIP_LEFT�̏ꍇ
    //CLIP_LEFT=p1.x+t*dx
    //t=(CLIP_LEFT-p1.x)/dx


    return { x, y };
}

/// <summary>
/// �|���S�����N���b�v���A�V�����|���S�������
/// </summary>
/// <param name="input">�|���S��</param>
/// <param name="edge">�ΏۂƂ���N���b�v��</param>
/// <returns>�N���b�v��̃|���S��</returns>
std::vector<Vec2> clipEdge(const std::vector<Vec2>& input, Edge edge) 
{
    //���ʂ��܂Ƃ߂��z��
    std::vector<Vec2> output;

    //�|���S���̕ӂ�1���v�Z
    for (size_t i = 0; i < input.size(); ++i) 
    {
        //�|���S���̕Ӂi���_�Ƃ���1�O�̒��_�j��1�{������
        Vec2 current = input[i];
        Vec2 prev = input[(i + input.size() - 1) % input.size()];

        //�ӂ̎n�_�I�_���N���b�v���̓������O�����𔻒�
        bool currIn = inside(current, edge);
        bool prevIn = inside(prev, edge);

        //�͂ݏo�Ă��Ȃ����A�n�_���I�_���͂ݏo�Ă���ꍇ�͕ۑ�����
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
/// �|���S�����N���b�v�͈͂Ɏ��߂鏈��
/// </summary>
/// <param name="polygon">�|���S��</param>
/// <returns>�N���b�v��̃|���S��</returns>
std::vector<Vec2> sutherlandHodgmanClip(const std::vector<Vec2>& polygon) 
{
    std::vector<Vec2> output = polygon;

    //�e�N���b�v���Ő���������
    for (Edge e : {LEFT, RIGHT, BOTTOM, TOP}) {
        output = clipEdge(output, e);
    }
    return output;
}




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

    Vec2 offset = { 0, 0 };
    Vec2 prevMouse = { 0, 0 };
    bool dragging = false;

    // �`���𗠉�ʂɕύX
    SetDrawScreen(DX_SCREEN_BACK);

    //�N���b�v�Ώ�
    std::vector<Vec2> polygon = {
    { 50, 150 },
    { 200, 50 },
    { 350, 150 },
    { 200, 350 },
    { 50, 300 }
    };

    // �E�C���h�E�������邩�G�X�P�[�v�L�[���������܂Ń��[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        int mx, my;
        GetMousePoint(&mx, &my);
        Vec2 mouse = { (float)mx, (float)my };

        // ���{�^������
        if (GetMouseInput() & MOUSE_INPUT_LEFT) {
            if (!dragging) {
                dragging = true;
                prevMouse = mouse;
            }
            else {
                // ��������������
                offset.x += mouse.x - prevMouse.x;
                offset.y += mouse.y - prevMouse.y;
                prevMouse = mouse;
            }
        }
        else {
            dragging = false;
        }

        // ��ʂ��N���A
        ClearDrawScreen();

        //��������̃|���S��
        std::vector<Vec2> movedPolygon;

        //��������̃|���S����ۑ�
        for (const auto& p : polygon) {
            movedPolygon.push_back({ p.x + offset.x, p.y + offset.y });
        }

        //�|���S�����N���b�v����
        auto clipped = sutherlandHodgmanClip(movedPolygon);
        //�N���b�v��̃|���S�����O�p�|���S���ɕύX
        auto triangles = Triangulate(clipped);


        DrawBox(CLIP_LEFT, CLIP_TOP, CLIP_RIGHT, CLIP_BOTTOM, GetColor(0, 0, 255), FALSE);

        // ���|���S���i�ԁj
        for (int i = 0; i < movedPolygon.size(); ++i) {
            auto& a = movedPolygon[i];
            auto& b = movedPolygon[(i + 1) % movedPolygon.size()];
            DrawLine(a.x, a.y, b.x, b.y, GetColor(255, 0, 0));
        }

        // �N���b�v��|���S���i�O�p�`�E�΁j
        for (const auto& tri : triangles) {
            DrawTriangle(
                static_cast<int>(tri.a.x), static_cast<int>(tri.a.y),
                static_cast<int>(tri.b.x), static_cast<int>(tri.b.y),
                static_cast<int>(tri.c.x), static_cast<int>(tri.c.y),
                GetColor(0, 255, 255), TRUE  // �h��Ԃ�ON
            );
        }


        // �O�p�`�i�j
        for (const auto& tri : triangles) {
            DrawLine(tri.a.x, tri.a.y, tri.b.x, tri.b.y, GetColor(255, 0, 0));
            DrawLine(tri.b.x, tri.b.y, tri.c.x, tri.c.y, GetColor(255, 0, 0));
            DrawLine(tri.c.x, tri.c.y, tri.a.x, tri.a.y, GetColor(255, 0, 0));
        }


        // ����ʂ̓��e��\��ʂɔ��f
        ScreenFlip();
    }


    // �c�w���C�u�����̌�n��
    DxLib_End();

    // �\�t�g�̏I��
    return 0;
}