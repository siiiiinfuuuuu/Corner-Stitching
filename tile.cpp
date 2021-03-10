#include "tile.h"
#include <iostream>

using namespace std;

Tile::Tile(TypesOfTile type, Point left_bottom, Point right_top){
    left_bottom_ = left_bottom;
    right_top_ = right_top;
    type_ = type;
    rt_ = nullptr;
    tr_ = nullptr;
    bl_ = nullptr;
    lb_ = nullptr;
}

Tile::Tile(TypesOfTile type, int x1, int y1, int x2, int y2){
    left_bottom_.x = x1;
    left_bottom_.y = y1;
    right_top_.x = x2;
    right_top_.y = y2;
    type_ = type;
    rt_ = nullptr;
    tr_ = nullptr;
    bl_ = nullptr;
    lb_ = nullptr;
}

void Tile::SetPtr(Tile *rt, Tile* tr, Tile* bl, Tile* lb){
    rt_ = rt;
    tr_ = tr;
    bl_ = bl;
    lb_ = lb;
}

bool Tile::IsInTile(Point p){
    return p.x >= left_bottom_.x && p.x < right_top_.x && p.y >= left_bottom_.y && p.y < right_top_.y;
}

void Tile::PrintTile(){
    if(this == nullptr)
        cout << "Tile null" << endl;
    else
        cout <<"Tile "<< num << ": " <<  left_bottom_.x << " " << left_bottom_.y << "  " << right_top_.x << " " << right_top_.y << endl;
}