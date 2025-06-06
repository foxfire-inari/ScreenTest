#include "Common.h"
#include "DxLib.h"

/// <summary>
/// ���ʏ�̒��_
/// </summary>
struct Vec2 {
    float x, y;
};

/// <summary>
/// 
/// </summary>
enum Edge {
    LEFT, RIGHT, BOTTOM, TOP
};

struct Triangle {
    Vec2 a, b, c;
};

std::vector<Triangle> triangulate(const std::vector<Vec2>& polygon) {
    std::vector<Triangle> triangles;

    if (polygon.size() < 3) return triangles;

    for (size_t i = 1; i + 1 < polygon.size(); ++i) {
        triangles.push_back({ polygon[0], polygon[i], polygon[i + 1] });
    }

    return triangles;
}


const float CLIP_LEFT = 100;
const float CLIP_RIGHT = 300;
const float CLIP_BOTTOM = 100;
const float CLIP_TOP = 300;

bool inside(const Vec2& p, Edge edge) {
    switch (edge) {
    case LEFT:   return p.x >= CLIP_LEFT;
    case RIGHT:  return p.x <= CLIP_RIGHT;
    case BOTTOM: return p.y >= CLIP_BOTTOM;
    case TOP:    return p.y <= CLIP_TOP;
    }
    return false;
}

Vec2 intersect(const Vec2& p1, const Vec2& p2, Edge edge) {
    float x, y;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    switch (edge) {
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

    return { x, y };
}

std::vector<Vec2> clipEdge(const std::vector<Vec2>& input, Edge edge) {
    std::vector<Vec2> output;

    for (size_t i = 0; i < input.size(); ++i) {
        Vec2 current = input[i];
        Vec2 prev = input[(i + input.size() - 1) % input.size()];

        bool currIn = inside(current, edge);
        bool prevIn = inside(prev, edge);

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

std::vector<Vec2> sutherlandHodgmanClip(const std::vector<Vec2>& polygon) {
    std::vector<Vec2> output = polygon;
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

    std::vector<Vec2> polygon = {
    { 50, 150 },
    { 200, 50 },
    { 350, 150 },
    { 350, 300 },
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

        std::vector<Vec2> movedPolygon;
        for (const auto& p : polygon) {
            movedPolygon.push_back({ p.x + offset.x, p.y + offset.y });
        }

        DrawFormatString(0, 0, GetColor(255, 255, 255), "offset.x:%f\noffset.y:%f", offset.x, offset.y);

        auto clipped = sutherlandHodgmanClip(movedPolygon);
        auto triangles = triangulate(clipped);

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