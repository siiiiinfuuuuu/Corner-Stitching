#include "plane.h"
#include<iostream>
#include<bits/stdc++.h>

using namespace std;

int main(){
    // int n = 40;
    // int y = 40;
    // if(n==30)
    //     if(y==50)
    //         cout << "n=30 y=50";
    //     else
    //         cout << "n=30 y!=50";
    time_t start, end;
    Plane* plane = new Plane;
    plane->ReadFiles("top2.trp");
    plane->Initialize();
    start = time(NULL);
    plane->Creation();
    end = time(NULL);
    cout <<"Creation time = "<< difftime(end, start) << endl;
    cout << "creation finish\n";
    cout << "Creation new tile = " << plane->number_of_tiles_ << endl;
    //plane->OutputFile("chip.gdt");
    cout << "Total tiles area = " << plane->TilesArea() << endl;
    cout << "Plane area       = " << plane->TotalArea() << endl;

    cout << "Ideal area = " << plane->ideal_area << endl;
    cout << "Solid area = " << plane->solid_area << endl;
    start = time(NULL);
    plane->Deletion();
    end = time(NULL);
    cout <<"Deletion time = "<< difftime(end, start) << endl;
    cout << "Deletion new tile = " << plane->number_of_tiles_ << endl;
    plane->OutputFile("top2_test.gdt");
    
    // plane->MakeTestCase();
    // vector<Tile*> tilsEnum = plane->DirectedAreaEnumeration(0,0,700,650);
    // plane->PrintTiles(tilsEnum);
    // plane->OutputFile("test.gdt");

    //plane->MakeTestCase("case_1.txt");
    /*
    //test point finding
    cout << "test point finding\n";
    Tile* t = plane->PointFinding(0,649);
    t->PrintTile();

    //test neighbor finding
    cout << "test neighbor finding\n";
    vector<Tile *> neighbors = plane->NeighborFinding(t);
    for (auto it = neighbors.begin(); it != neighbors.end(); it++)
        (*it)->PrintTile();
    
    //test area searches
    cout << "test area searches\n";
    Point left_bottom(210, 400), right_top(400, 500);
    cout << plane->AreaSearches(left_bottom, right_top) << endl;

    //test directed area enumeration
    cout << "test directed area enumeration\n";
    tilsEnum =  plane->DirectedAreaEnumeration(0,0,700,649);
    for (auto it = tilsEnum.begin(); it != tilsEnum.end(); it++)
        (*it)->PrintTile();

    //test tile creation
    cout << "\ntest tile creation\n";
    Point left_bottom_3(50, 60), right_top_3(100, 70);
    plane->TileCreation(left_bottom_3, right_top_3);


    tilsEnum = plane->DirectedAreaEnumeration(0,0,700,649);
    for (auto it = tilsEnum.begin(); it != tilsEnum.end(); it++)
        (*it)->PrintTile();
    
    cout << "\ntest tile creation\n";
    Point left_bottom_4(300, 400), right_top_4(400, 640);
    plane->TileCreation(left_bottom_3, right_top_3);


    tilsEnum = plane->DirectedAreaEnumeration(0,0,700,649);
    for (auto it = tilsEnum.begin(); it != tilsEnum.end(); it++)
        (*it)->PrintTile();
    */
}
