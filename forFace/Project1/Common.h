#pragma once
#include<vector>
#include<list>
//Xは横
//Yは縦
//Zは奥行


namespace
{
    //円周率
    static const float PI = 3.141592f;

    //ウィンドウ＆スクリーンサイズ
    static const float WINDOW_WIDTH = 800.f;        //横幅
    static const float WINDOW_HEIGHT = 600.f;       //縦幅

    //回転
    static const float ONE_DEGREE = PI / 180.0f;    //1度

    //視点からスクリーンまでの距離
    static const float SCREEN = 500.f;

    //フロートを0fとして扱う範囲
    static const float NEAR_ZERO = NEAR_ZERO;

}
