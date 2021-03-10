#ifndef _TILE_H_
#define _TILE_H_

class Tile;
struct Point;
struct Rct;

enum TypesOfTile{kSpace, kSolid};

struct Point{
    int x, y;
    Point() : x(0), y(0){}
    Point(int i, int j) : x(i), y(j){}
    void operator=(const Point& p){
        x = p.x;
        y = p.y;
    }
    bool operator==(const Point& p) const{
        return p.x == x && p.y == y;
    }
    bool operator!=(const Point& p) const{
        return p.x != x || p.y != y;
    }
    bool operator<(const Point& p)const{
        return (x < p.x) || (x == p.x && y < p.y);
    }
    Point operator-(const Point &p) const{
        Point temp;
        temp.x = x - p.x;
        temp.y = y - p.y;
        return temp;
    }

};

class Tile{
private:

public:
    Tile *rt_, *tr_, *bl_, *lb_;
    TypesOfTile type_;
    Point left_bottom_, right_top_;
    int num;

    Tile(){}
    Tile(TypesOfTile type, Point left_bottom, Point right_top);
    Tile(TypesOfTile type, int x1, int y1, int x2, int y2);
    void SetPtr(Tile *rt, Tile* tr, Tile* bl, Tile* lb);
    bool IsInTile(Point p);
    void PrintTile();
};

struct Rct{
    Point left_bottom, right_top;
    Rct(Point l, Point r) : left_bottom(l), right_top(r){}
};

#endif

