#include "tile.h"
#include <vector>
#include <string>


class Plane{
private:

    int min_x_, min_y_, max_x_, max_y_;
    Tile* rt_of_new_left_space_tile;
    Tile* HorizontalMove(Tile* curr, Point target);
    Tile* VerticalMove(Tile* curr, Point target);
    void R_Procedure(Tile* t,  std::vector<Tile*>* tileEnum, Point left_bottom, Point right_top);
    bool TopEdgeOfAreaOnTopSpaceTileEdge(Tile* top_space_tile, Point left_bottom, Point right_top);
    bool BottomEdgeOfAreaOnBottomSpaceTileEdge(Tile* bottom_space_tile, Point left_bottom, Point right_top);
    Tile* SplitTopSpaceTile(Tile* top_space_tile, Point left_bottom, Point right_top);
    Tile* SplitBottomSpaceTile(Tile* bottom_space_tile, Point left_bottom, Point right_top);
    void SplitMiddleSpaceTile(Tile *middle_space_tile, Tile *new_solid_tile, Tile* top_space_tile, Tile* bottom_space_tile);
    Tile* NewTile(TypesOfTile type, Point left_bottom, Point right_top);
    Tile* NewTile(TypesOfTile type, int x1, int y1, int x2, int y2);
    void DeleteTile(Tile* t);
    std::vector<Tile *> LeftTiles(Tile* t);
    void MergeRightTiles(std::vector<Tile*> right_tiles);
    void MergeLeftTiles(std::vector<Tile*> left_tiles);
    void SpaceSplitRightNeighbors(Tile* main_tile);
    void SolidSplitRightNeighbors(Tile* main_tile);
    void SplitRightNeighbor(Tile* right_neighbor, Tile* main_tile, int y, bool new_on_top);
    void SplitMainTileAndHorizontalMerge(Tile* main_tile, Tile* right_neighbor);
    void SplitMainTile(Tile* main_tile, int y);
    void VerticalMerge(Tile* right_neighbor, Tile* merged_tile);

public:
    std::vector<Tile*> tiles;
    std::vector<Rct> RctList;
    long long ideal_area;
    long long solid_area;
    int number_of_tiles_;
    Plane();
    Tile* hint_tile_;
    Tile* left_bottom_tile_;
    Tile* PointFinding(Point target);
    Tile* PointFinding(int x, int y);
    std::vector<Tile*> NeighborFinding(Tile* t);
    bool AreaSearches(Point left_bottom, Point right_top);
    std::vector<Tile*> DirectedAreaEnumeration(Point left_bottom, Point right_top);
    std::vector<Tile*> DirectedAreaEnumeration(int x1, int y1, int x2, int y2);
    bool TileCreation(Point left_bottom, Point right_top);
    bool TileCreation(int x1, int y1, int x2, int y2);
    bool TileDeletion(Point left_bottom, Point right_top);
    bool TileDeletion(int x1, int y1, int x2, int y2);
    void TileDeletion(Tile* deleted_tile);
    void Initialize();
    void OutputFile(std::string fileName);
    void MakeTestCase(std::string file_name);
    void MakeTestCase();
    void PrintTiles(std::vector<Tile*> T);
    void PrintTiles(Tile* t);
    void ReadFiles(std::string file_name);
    void Creation();
    bool TilePtrCheck(Tile* t);
    bool TilePtrCheck(std::vector<Tile*> t);
    std::vector<Tile *> EnumerateAllTiles();
    long long TilesArea();
    long long TotalArea();
    bool AreaTest();
    void Deletion();
    bool TileMergeCheck(Tile* t);
    bool TileMergeCheck(std::vector<Tile*> t);
};