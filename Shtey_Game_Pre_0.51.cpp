#include <iostream>
#include "TXLib.h"
using namespace std;

class my_map{
    HDC terrain;
    int x, y;
    int tr[50][30];

public:
    my_map();
    void draw_terrain();
    void create_lvl();
    void delete_map();
    int gettr(int j, int i);
};

class sprite{

    int x, y, sizex, sizey;
    double scx, scy;
    HDC main_model, effect_model;
    int r, g, b, prtx, prty;
    int step, freq, n, m;
    bool flag;

public:
    sprite();
    void move_up(my_map lvl);
    void move_down(my_map lvl);
    void move_right(my_map lvl);
    void move_left(my_map lvl);
    void check_for_shift();
    void chng_anim();
    void chng_anim_rl();
    void draw();
    void effect(int x1, int y1, int ptx, int pty);
    void set_start_value(int st, int fr, bool fg);
    //void sled_begin(int num, int sl, sprite sled[], sprite wz[]);
    void draw_sled();
    void delete_sprites();
    void gravity(my_map lvl);
};

int main(){
    srand(time(NULL));
    txCreateWindow(1280, 720);
    txBegin();
    txSetFillColor(TX_WHITE);
    txClear();
    int num = 1, sl = 15, it = 0;
    sprite wz;
    my_map lvl;

    while(!GetAsyncKeyState(VK_ESCAPE)){
        lvl.create_lvl();
        txClear();
        it++;
        wz.set_start_value(6, 5, false);       // Эти аргументы передаются из мейна только для того, чтобы их удобно было менять.
        if(GetAsyncKeyState(VK_UP)){
            wz.check_for_shift();
            wz.move_up(lvl);
        }
        //if(GetAsyncKeyState(VK_DOWN)){
        //    wz.check_for_shift();
        //    wz.move_down(lvl);
        //}
        if(GetAsyncKeyState(VK_RIGHT)){
            wz.check_for_shift();
            wz.move_right(lvl);
        }
        if(GetAsyncKeyState(VK_LEFT)){
            wz.check_for_shift();
            wz.move_left(lvl);
        }
        wz.gravity(lvl);
        lvl.draw_terrain();
        wz.draw();

        txSleep(2);

    }
    wz.delete_sprites();
    lvl.delete_map();
    txEnd();
}

void sprite::move_up(my_map lvl){
    if(lvl.gettr((x + 10)/ 70, (y - step) / 70) >= 10 && lvl.gettr((x + 48)/ 70, (y - step) / 70) >= 10){
        y -= step;
        prty = 730;
        prtx = 160 * (n % 4);
        chng_anim();
    }
}

/*void sprite::move_down(my_map lvl){
    if(lvl.gettr((x + 20)/ 70, (y + step + 100) / 70) >= 10){
        y += step;
        prty = 10;
        prtx = 160 * (n % 4);
        chng_anim();
    }
} */

void sprite::move_right(my_map lvl){
    if(lvl.gettr((x + step + 40)/ 70, (y + 100) / 70) >= 10 && lvl.gettr((x + step + 48)/ 70, y / 70) >= 10){
        x += step;
        prty = 490;
        prtx = 160 * (n % 4);
        chng_anim_rl();
    }
}

void sprite::move_left(my_map lvl){
    if(lvl.gettr((x - step + 10)/ 70, (y + 100) / 70) >= 10 && lvl.gettr((x - step + 10)/ 70, y / 70) >= 10){
        x -= step;
        prty = 250;
        prtx = 160 * (n % 4);
        chng_anim_rl();
    }
}

void sprite::check_for_shift(){
    if(GetAsyncKeyState(VK_LSHIFT) && !flag) {step += 6; freq -= 3;}
}

void sprite::chng_anim_rl(){
    if(/*!GetAsyncKeyState(VK_DOWN) && */!GetAsyncKeyState(VK_UP) && m % freq == 0 ) n++;
    flag = true;
}

void sprite::chng_anim(){
    if(m % freq == 0 && !flag)n++;
    flag = true;
}

void sprite::draw(){
    Win32::TransparentBlt(txDC(), x, y, sizex * scx, sizey * scy, main_model, prtx, prty, sizex, sizey, RGB(r, g, b));
}

void sprite::set_start_value(int st, int fr, bool fg){
    m++;
    step = st;
    freq = fr;
    flag = fg;
}

void sprite::effect(int x1, int y1, int ptx, int pty){
    Win32::TransparentBlt(txDC(), x1, y1, sizex * scx, sizey * scy, main_model, ptx, pty, sizex, sizey, RGB(r, g, b));
}

sprite::sprite(){
    x = 0; y = 0; sizex = 160; sizey = 225;
    scx = 0.5; scy = 0.5;
    main_model = txLoadImage("Dark_Wizard_alpha.bmp");
    effect_model = txLoadImage("White_Wizard.bmp");
    r = 209; g = 59; b = 76; prtx = 0; prty = 0;
    step = 5; freq =  6; n = 0; m = -1;
    flag = false;
}

void sprite::delete_sprites(){
    txDeleteDC(main_model);
    txDeleteDC(effect_model);
}

void sprite::gravity(my_map lvl){
    if(lvl.gettr((x + step + 20)/ 70, (y + 103) / 70) >= 10) y+=3;
}





void my_map::draw_terrain(){
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            Win32::TransparentBlt(txDC(), i * 70, j * 70, 70 * 1.15, 70 * 1.15, terrain, 75 * tr[j][i], 0, 70, 70, RGB(255, 255, 255));
        }
    }
}

void my_map::create_lvl(){
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if(j < 4) tr[j][i] = 10;
            else if(j == 4) tr[j][i] = 1;
            else tr[j][i] = 2;

        }
    }
    tr[5][3] = 10; tr[5][4] = 10; tr[6][4] = 10; tr[8][4] = 10;
    tr[6][3] = 10; tr[7][4] = 10; tr[7][5] = 10; tr[8][5] = 10;
    tr[5][6] = 2; tr[5][7] = 2;tr[4][7] = 1;
}

my_map::my_map(){
    terrain = txLoadImage("Map_Textures_fix.bmp");
    x = 50;
    y = 30;
}

void my_map::delete_map(){
    txDeleteDC(terrain);
}

int my_map::gettr(int j, int i){
    return tr[i][j];
}



