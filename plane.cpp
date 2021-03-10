#include "plane.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <algorithm>

using namespace std;

char* NextNum(char* curr){
    while (*curr >= '0' && *curr <= '9')
        curr++;
    while (*curr < '0' || *curr > '9')
        curr++;
    return curr;
}

bool MinX(Rct r1, Rct r2){
    return r1.left_bottom.x < r2.left_bottom.x;
} 

bool MinY(Rct r1, Rct r2){
    return r1.left_bottom.y < r2.left_bottom.y;
}
    
bool MaxX(Rct r1, Rct r2){
    return r1.right_top.x < r2.right_top.x;
}

bool MaxY(Rct r1, Rct r2){
    return r1.right_top.y < r2.right_top.y;
}

Plane::Plane(){
    left_bottom_tile_ = nullptr;
    hint_tile_ = nullptr;
    number_of_tiles_ = 0;
    rt_of_new_left_space_tile = nullptr;
    min_x_ = 0;
    min_y_ = 0;
    max_x_ = 0;
    max_y_ = 0;
    ideal_area = 0;
    solid_area = 0;
}

void Plane::MakeTestCase(string file_name){
    fstream file;
    char buffer[100];
    file.open(file_name, ios::in);
    if(file){
        while(!file.eof()){
            file.getline(buffer, sizeof(buffer));
            Point left_bottom, right_top;
            TypesOfTile type;
            //cout << buffer << endl;
            char* curr = buffer+6;
            type = (buffer[1] == 'p') ? kSpace : kSolid;
            left_bottom.x = atoi(curr);
            curr = NextNum(curr);
            left_bottom.y = atoi(curr);
            curr = NextNum(curr);
            right_top.x = atoi(curr);
            curr = NextNum(curr);
            right_top.y = atoi(curr);
            tiles.push_back(NewTile(type, left_bottom, right_top));
        }
    }
    auto it = tiles.begin();
    tiles[0]->SetPtr(*(it+4), nullptr, nullptr, nullptr);
    tiles[1]->SetPtr(*(it+7), *(it+2), nullptr, *(it+0));
    tiles[2]->SetPtr(*(it+9), *(it+8), *(it+1), *(it+0));
    tiles[3]->SetPtr(*(it+8), *(it+4), *(it+2), *(it+0));
    tiles[4]->SetPtr(*(it+8), nullptr, *(it+3), *(it+0));
    tiles[5]->SetPtr(*(it+12), *(it+6), nullptr, *(it+1));
    tiles[6]->SetPtr(*(it+12), *(it+20), *(it+5), *(it+1));
    tiles[7]->SetPtr(*(it+9), *(it+2), *(it+6), *(it+1));
    tiles[8]->SetPtr(*(it+9), nullptr, *(it+2), *(it+3));
    tiles[9]->SetPtr(*(it+10), nullptr, *(it+6), *(it+7));
    tiles[10]->SetPtr(*(it+15), nullptr, *(it+11), *(it+9));
    tiles[11]->SetPtr(*(it+13), *(it+14), *(it+20), *(it+9));
    tiles[12]->SetPtr(*(it+13), *(it+11), nullptr, *(it+5));
    tiles[13]->SetPtr(*(it+18), *(it+14), nullptr, *(it+12));
    tiles[14]->SetPtr(*(it+18), *(it+15), *(it+11), *(it+10));
    tiles[15]->SetPtr(*(it+18), nullptr, *(it+14), *(it+10));
    tiles[16]->SetPtr(*(it+19), *(it+17), nullptr, *(it+13));
    tiles[17]->SetPtr(*(it+19), *(it+18), *(it+16), *(it+13));
    tiles[18]->SetPtr(*(it+19), nullptr, *(it+17), *(it+13));
    tiles[19]->SetPtr(nullptr, nullptr, nullptr, *(it+16));
    tiles[20]->SetPtr(*(it+12), *(it+11), *(it+6), *(it+9));
    hint_tile_ = *it;
}

void Plane::MakeTestCase(){
    min_x_ = 0;
    min_y_ = 0;
    max_x_ = 700;
    max_y_ = 650;
    Initialize();
    TileCreation(350, 50, 450, 200);
    TileCreation(450, 50, 650, 100);
    TileCreation(100, 150, 200, 350);
    TileCreation(250, 300, 400, 400);
    TileCreation(400, 375, 550, 500);
    TileCreation(150, 500, 300, 600);

    TileDeletion(350, 50, 450, 200);
    TileDeletion(450, 50, 650, 100);
    TileDeletion(100, 150, 200, 350);
    TileDeletion(250, 300, 400, 400);
    TileDeletion(400, 375, 550, 500);
    TileDeletion(150, 500, 300, 600);
}

Tile* Plane::HorizontalMove(Tile* curr, Point target){
    while (curr!=nullptr && target.x < curr->left_bottom_.x)
        curr = curr->bl_;
    while (curr!=nullptr && target.x >= curr->right_top_.x)
        curr = curr->tr_;
    return curr;
}

Tile* Plane::VerticalMove(Tile* curr, Point target){
    while (curr!=nullptr && target.y < curr->left_bottom_.y)
        curr = curr->lb_;
    while (curr!=nullptr && target.y >= curr->right_top_.y)
        curr = curr->rt_;
    return curr;
}

Tile* Plane::PointFinding(Point target){
    Tile* curr_tile = hint_tile_;
    while(curr_tile!= nullptr && !curr_tile->IsInTile(target)){
        curr_tile = VerticalMove(curr_tile, target);
        curr_tile = HorizontalMove(curr_tile, target);
    }
    if (curr_tile != nullptr)
        hint_tile_ = curr_tile;
    return curr_tile;
}

Tile* Plane::PointFinding(int x, int y){
    Point p(x, y);
    return PointFinding(p);
}

vector<Tile*> Plane::NeighborFinding(Tile* t){
    vector<Tile*> neighbors;
    Tile* curr_tile = t->tr_;
    while(curr_tile!=NULL && curr_tile->right_top_.y > t->left_bottom_.y){
        neighbors.push_back(curr_tile);
        curr_tile = curr_tile->lb_;
    }
    return neighbors;
}

bool Plane::AreaSearches(Point left_bottom, Point right_top){
    Point start_point(left_bottom.x, right_top.y);
    Tile* curr_tile = PointFinding(start_point);
    if (curr_tile->left_bottom_.y == right_top.y){
        Point nextPoint(left_bottom.x, curr_tile->left_bottom_.y - 1);
        curr_tile = PointFinding(nextPoint);
    }
    while (curr_tile!=nullptr && curr_tile->right_top_.y > left_bottom.y){
        if(curr_tile->type_ == kSolid || curr_tile->right_top_.x < right_top.x)
            return false;
        Point nextPoint(left_bottom.x, curr_tile->left_bottom_.y - 1);
        curr_tile = PointFinding(nextPoint);
    }
    return true;
}

vector<Tile*> Plane::DirectedAreaEnumeration(Point left_bottom, Point right_top){
    vector<Tile*> tiles_enum;
    queue<Tile*> tiles_queue;
    Point start_point(left_bottom.x, right_top.y);
    Tile* curr_tile = PointFinding(start_point);
    while (curr_tile!=nullptr && curr_tile->right_top_.y > left_bottom.y){
        tiles_queue.push(curr_tile);
        Point nextPoint(left_bottom.x, curr_tile->left_bottom_.y - 1);
        curr_tile = PointFinding(nextPoint);
    }
    while(!tiles_queue.empty()){
        curr_tile = tiles_queue.front();
        tiles_queue.pop();
        if(curr_tile->left_bottom_.y < right_top.y)
            R_Procedure(curr_tile, &tiles_enum, left_bottom, right_top);
    }
    return tiles_enum;
}

vector<Tile*> Plane::DirectedAreaEnumeration(int x1, int y1, int x2, int y2){
    Point left_bottom(x1, y1);
    Point right_top(x2, y2);
    return DirectedAreaEnumeration(left_bottom, right_top);
}

vector<Tile *> Plane::EnumerateAllTiles(){
    return DirectedAreaEnumeration(min_x_-1, min_y_-1, max_x_+1, max_y_+1);
}

void Plane::R_Procedure(Tile *t, vector<Tile*>* tile_enum, Point left_bottom, Point right_top)
{
    //R1
    tile_enum->push_back(t);
    //R2
    if(t->right_top_.x > right_top.x)
        return;
    //R3
    vector<Tile*> neighbors = NeighborFinding(t);
    for (auto it = neighbors.begin(); it != neighbors.end(); it++){
        if ((*it)->left_bottom_.y >= right_top.y || (*it)->right_top_.y <= left_bottom.y)
            continue;
        //R4
        if ((*it)->bl_ == t)
            R_Procedure((*it), tile_enum, left_bottom, right_top);
        //R5
        else if (t->left_bottom_.y <= left_bottom.y && (*it)->left_bottom_.y <= left_bottom.y){
            R_Procedure((*it), tile_enum, left_bottom, right_top);
        }
    }
}

bool Plane::TopEdgeOfAreaOnTopSpaceTileEdge(Tile* top_space_tile, Point left_bottom, Point right_top){
    return right_top.y == top_space_tile->right_top_.y || right_top.y == top_space_tile->left_bottom_.y;
}

bool Plane::BottomEdgeOfAreaOnBottomSpaceTileEdge(Tile* bottom_space_tile, Point left_bottom, Point right_top){
    return left_bottom.y == bottom_space_tile->right_top_.y || left_bottom.y == bottom_space_tile->left_bottom_.y;
}

Tile* Plane::SplitTopSpaceTile(Tile* top_space_tile, Point left_bottom, Point right_top){
    Tile* new_tile = NewTile(kSpace, top_space_tile->left_bottom_.x, right_top.y, top_space_tile->right_top_.x, top_space_tile->right_top_.y);
    new_tile->SetPtr(top_space_tile->rt_, top_space_tile->tr_, top_space_tile->bl_, top_space_tile);
    while (new_tile->bl_!=nullptr && new_tile->bl_->right_top_.y <= new_tile->left_bottom_.y)
        new_tile->bl_ = new_tile->bl_->rt_;

    //fix the tiles whose lb = top_space
    Tile *candidate_tile = top_space_tile->rt_;
    while(candidate_tile!=nullptr && candidate_tile->lb_ == top_space_tile){
        candidate_tile->lb_ = new_tile;
        candidate_tile = candidate_tile->bl_;
    }
    //fix the tiles whose tr = top_space
    candidate_tile = top_space_tile->bl_;
    while (candidate_tile != nullptr && candidate_tile->tr_ == top_space_tile){
        if(candidate_tile->right_top_.y > new_tile->left_bottom_.y)
            candidate_tile->tr_ = new_tile;
        candidate_tile = candidate_tile->rt_;
    }
    //fix the tiles whose bl = top_space
    candidate_tile = top_space_tile->tr_;
    while(candidate_tile!=nullptr && candidate_tile->bl_ == top_space_tile){
        if(candidate_tile->left_bottom_.y >= new_tile->left_bottom_.y)
            candidate_tile->bl_ = new_tile;
        candidate_tile = candidate_tile->lb_;//hi hi 
    }
    //fix top_space
    top_space_tile->rt_ = new_tile;
    top_space_tile->right_top_.y = right_top.y;
    while (top_space_tile->tr_ != nullptr && top_space_tile->tr_->left_bottom_.y >= top_space_tile->right_top_.y)
        top_space_tile->tr_ = top_space_tile->tr_->lb_;
    return new_tile;
}

Tile* Plane::SplitBottomSpaceTile (Tile* bottom_space_tile, Point left_bottom, Point right_top){
    Tile* new_tile = NewTile(kSpace, bottom_space_tile->left_bottom_.x, bottom_space_tile->left_bottom_.y, bottom_space_tile->right_top_.x, left_bottom.y);
    new_tile->SetPtr(bottom_space_tile, bottom_space_tile->tr_, bottom_space_tile->bl_, bottom_space_tile->lb_);
    while(new_tile->tr_!= nullptr && new_tile->tr_->left_bottom_.y >= new_tile->right_top_.y)
        new_tile->tr_ = new_tile->tr_->lb_;

    //fix the tiles whose rt = bottom_space
    Tile *candidate_tile = bottom_space_tile->lb_;
    while(candidate_tile!=nullptr && candidate_tile->rt_ == bottom_space_tile){
        candidate_tile->rt_ = new_tile;
        candidate_tile = candidate_tile->tr_;
    }
    //fix the tiles whose tr = bottom_space
    candidate_tile = bottom_space_tile->bl_;
    while (candidate_tile != nullptr && candidate_tile->tr_ == bottom_space_tile){
        if(candidate_tile->right_top_.y <= new_tile->right_top_.y)
            candidate_tile->tr_ = new_tile;
        candidate_tile = candidate_tile->rt_;
    }
    //fix the tiles whose bl = bottom_space
    candidate_tile = bottom_space_tile->tr_;
    while(candidate_tile!=nullptr && candidate_tile->bl_ == bottom_space_tile){
        if(candidate_tile->left_bottom_.y < new_tile->right_top_.y)
            candidate_tile->bl_ = new_tile;
        candidate_tile = candidate_tile->lb_;
    }
    //fix bottom_space
    bottom_space_tile->lb_ = new_tile;
    bottom_space_tile->left_bottom_.y = left_bottom.y;
    while (bottom_space_tile->bl_ != nullptr && bottom_space_tile->bl_->right_top_.y <= bottom_space_tile->left_bottom_.y)
        bottom_space_tile->bl_ = bottom_space_tile->bl_->rt_;
    return new_tile;
}

void Plane::SplitMiddleSpaceTile (Tile *middle_space_tile, Tile *new_solid_tile, Tile* top_space_tile, Tile* bottom_space_tile){
    bool no_left_space = middle_space_tile->left_bottom_.x == new_solid_tile->left_bottom_.x;
    bool no_right_space = middle_space_tile->right_top_.x == new_solid_tile->right_top_.x;
    Tile* candidate_tile;
    if(no_left_space && no_right_space){
        //fix the tiles whose tr = middle_space
        candidate_tile = middle_space_tile->bl_;
        while(candidate_tile!=nullptr && candidate_tile->tr_ == middle_space_tile){
            candidate_tile->tr_ = new_solid_tile;
            candidate_tile = candidate_tile->rt_;
        }
        //fix the tiles whose bl = middle_space
        candidate_tile = middle_space_tile->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == middle_space_tile){
            candidate_tile->bl_ = new_solid_tile;
            candidate_tile = candidate_tile->lb_;
        }
        //fix the tiles whose lb = middle_space
        candidate_tile = middle_space_tile->rt_;
        while(candidate_tile!=nullptr && candidate_tile->lb_ == middle_space_tile){
            //if(middle_space_tile == top_space_tile && candidate_tile->left_bottom_.x < new_solid_tile->right_top_.x && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x)
                //new_solid_tile->rt_ = candidate_tile;
            candidate_tile->lb_ = new_solid_tile;
            candidate_tile = candidate_tile->bl_;
        }
        //fix the tiles whose rt = middle_space
        candidate_tile = middle_space_tile->lb_;
        while(candidate_tile!=nullptr && candidate_tile->rt_ == middle_space_tile){
            if(middle_space_tile == bottom_space_tile)
                candidate_tile->rt_ = new_solid_tile;
            else
                candidate_tile->rt_ = nullptr;
            candidate_tile = candidate_tile->tr_;
        }
        if(middle_space_tile == top_space_tile){
            new_solid_tile->tr_ = middle_space_tile->tr_;
            new_solid_tile->rt_ = middle_space_tile->rt_;
        }
        if(middle_space_tile == bottom_space_tile){
            new_solid_tile->bl_ = middle_space_tile->bl_;
            new_solid_tile->lb_ = middle_space_tile->lb_;
        }
            
        rt_of_new_left_space_tile = middle_space_tile->bl_;
        DeleteTile(middle_space_tile);
    }
    else if (no_left_space){
        //fix the tiles whose tr = middle_space
        candidate_tile = middle_space_tile->bl_;
        while(candidate_tile!=nullptr && candidate_tile->tr_ == middle_space_tile){
            candidate_tile->tr_ = new_solid_tile;
            candidate_tile = candidate_tile->rt_;
        }
        //fix the tiles whose lb = middle_space
        candidate_tile = middle_space_tile->rt_;
        while(candidate_tile!=nullptr && candidate_tile->lb_ == middle_space_tile){
            if(candidate_tile->left_bottom_.x <new_solid_tile->right_top_.x)
                candidate_tile->lb_ = new_solid_tile;
            candidate_tile = candidate_tile->bl_;
        }
        //fix the tiles whose rt = middle_space
        candidate_tile = middle_space_tile->lb_;
        while(candidate_tile!=nullptr && candidate_tile->rt_ == middle_space_tile){
            if (candidate_tile->right_top_.x <= new_solid_tile->right_top_.x)
                if(middle_space_tile == bottom_space_tile)
                    candidate_tile->rt_ = new_solid_tile;
                else
                    candidate_tile->rt_ = nullptr;
            else if(candidate_tile->right_top_.x <= middle_space_tile->right_top_.x)//xxxxxxxxxxxxxx
                candidate_tile->rt_ = middle_space_tile;//xxxxxxxxxxxxxxxxxxxxxxxxxx
            candidate_tile = candidate_tile->tr_;
        }
        
        // if(middle_space_tile == top_space_tile){//???????????????????????????????????????????????????????????
        //     new_solid_tile->tr_ = middle_space_tile;
        //     candidate_tile = middle_space_tile->rt_;
        //     do{
        //         if(candidate_tile != nullptr && candidate_tile->left_bottom_.x < new_solid_tile->right_top_.x && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x)
        //             new_solid_tile->rt_ = candidate_tile;
        //         candidate_tile = candidate_tile->bl_;
        //     } while (candidate_tile != nullptr && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x);//??????????????????????????????
        // }

        if(middle_space_tile == top_space_tile){//???????????????????????????????????????????????????????????
            new_solid_tile->tr_ = middle_space_tile;
            candidate_tile = middle_space_tile->rt_;
            while(candidate_tile != nullptr && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x){
                if(candidate_tile->left_bottom_.x < new_solid_tile->right_top_.x)
                    new_solid_tile->rt_ = candidate_tile;
                candidate_tile = candidate_tile->bl_;
            }//??????????????????????????????
        }
            
        if(middle_space_tile == bottom_space_tile){
            new_solid_tile->bl_ = middle_space_tile->bl_;
            new_solid_tile->lb_ = middle_space_tile->lb_;
        }
        //turn middle_tile into right_space
        rt_of_new_left_space_tile = middle_space_tile->bl_;
        middle_space_tile->left_bottom_.x = new_solid_tile->right_top_.x;
        middle_space_tile->bl_ = new_solid_tile;
        while(middle_space_tile->lb_!=nullptr && middle_space_tile->lb_->right_top_.x<=middle_space_tile->left_bottom_.x)
            middle_space_tile->lb_ = middle_space_tile->lb_->tr_;
    }
    else if(no_right_space){
        //fix the tiles whose bl = middle_space
        candidate_tile = middle_space_tile->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == middle_space_tile){
            candidate_tile->bl_ = new_solid_tile;
            candidate_tile = candidate_tile->lb_;
        }
        //fix the tiles whose rt = middle_space
        candidate_tile = middle_space_tile->lb_;
        while(candidate_tile!=nullptr && candidate_tile->rt_ == middle_space_tile){
            if (candidate_tile->right_top_.x > new_solid_tile->left_bottom_.x){
                if(middle_space_tile == bottom_space_tile)
                    candidate_tile->rt_ = new_solid_tile;
                else
                    candidate_tile->rt_ = nullptr;
            }
            candidate_tile = candidate_tile->tr_;
        }
        //fix the tiles whose lb = middle_space
        candidate_tile = middle_space_tile->rt_;
        while(candidate_tile!=nullptr && candidate_tile->lb_ == middle_space_tile){
            if(candidate_tile->left_bottom_.x >= new_solid_tile->left_bottom_.x)
                candidate_tile->lb_ = new_solid_tile;
            candidate_tile = candidate_tile->bl_;
        }

        if(middle_space_tile == top_space_tile){
            new_solid_tile->tr_ = middle_space_tile->tr_;
            new_solid_tile->rt_ = middle_space_tile->rt_;
        }
        // if(middle_space_tile == bottom_space_tile){
        //     new_solid_tile->bl_ = middle_space_tile;
        //     candidate_tile = middle_space_tile->lb_;
        //     do{
        //         if( candidate_tile->right_top_.x > new_solid_tile->left_bottom_.x && candidate_tile->left_bottom_.x <= new_solid_tile->left_bottom_.x)
        //             new_solid_tile->lb_ = candidate_tile;
        //         candidate_tile = candidate_tile->tr_;
        //     } while (candidate_tile != nullptr && candidate_tile->left_bottom_.x <= new_solid_tile->left_bottom_.x);
        // }

        if(middle_space_tile == bottom_space_tile){
            new_solid_tile->bl_ = middle_space_tile;
            candidate_tile = middle_space_tile->lb_;
            while (candidate_tile != nullptr && candidate_tile->left_bottom_.x <= new_solid_tile->left_bottom_.x){
                if( candidate_tile->right_top_.x > new_solid_tile->left_bottom_.x)
                    new_solid_tile->lb_ = candidate_tile;
                candidate_tile = candidate_tile->tr_;
            }
        }
            
        //turn middle_tile into left_space
        middle_space_tile->right_top_.x = new_solid_tile->left_bottom_.x;
        middle_space_tile->tr_ = new_solid_tile;
        middle_space_tile->rt_ = rt_of_new_left_space_tile;
        rt_of_new_left_space_tile = middle_space_tile;
    }
    else{//have both left_space and right_space
        Tile* new_right_space_tile = NewTile(kSpace, new_solid_tile->right_top_.x, middle_space_tile->left_bottom_.y, middle_space_tile->right_top_.x, middle_space_tile->right_top_.y);
        new_right_space_tile->SetPtr(middle_space_tile->rt_, middle_space_tile->tr_, new_solid_tile, middle_space_tile->lb_);
        while (new_right_space_tile->lb_!=nullptr && new_right_space_tile->lb_->right_top_.x <= new_right_space_tile->left_bottom_.x)
            new_right_space_tile->lb_ = new_right_space_tile->lb_->tr_;

        //fix the tiles whose bl = middle_space
        candidate_tile = middle_space_tile->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == middle_space_tile){
            candidate_tile->bl_ = new_right_space_tile;
            candidate_tile = candidate_tile->lb_;
        }
        //fix the tiles whose rt = middle_space
        candidate_tile = middle_space_tile->lb_;
        while(candidate_tile!=nullptr && candidate_tile->rt_ == middle_space_tile){
            if(candidate_tile->right_top_.x > new_solid_tile->right_top_.x)
                candidate_tile->rt_ = new_right_space_tile;
            else if(candidate_tile->right_top_.x > new_solid_tile->left_bottom_.x)
                if(middle_space_tile == bottom_space_tile)
                    candidate_tile->rt_ = new_solid_tile;
                else
                    candidate_tile->rt_ = nullptr;
            candidate_tile = candidate_tile->tr_;
        }
        //fix the tiles whose lb = middle_space
        candidate_tile = middle_space_tile->rt_;
        while(candidate_tile!=nullptr && candidate_tile->lb_ == middle_space_tile){
            if(candidate_tile->left_bottom_.x >= new_solid_tile->right_top_.x)
                candidate_tile->lb_ = new_right_space_tile;
            else if (candidate_tile->left_bottom_.x >= new_solid_tile->left_bottom_.x)
                if(middle_space_tile == top_space_tile)
                    candidate_tile->lb_ = new_solid_tile;
                else//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                    candidate_tile->lb_ = nullptr;
            candidate_tile = candidate_tile->bl_;
        }

        // if(middle_space_tile == top_space_tile){
        //     new_solid_tile->tr_ = new_right_space_tile;
        //     candidate_tile = middle_space_tile->rt_;
        //     do{
        //         if(candidate_tile->left_bottom_.x < new_solid_tile->right_top_.x && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x)
        //             new_solid_tile->rt_ = candidate_tile;
        //         candidate_tile = candidate_tile->bl_;
        //     } while (candidate_tile != nullptr && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x);
        // }
            
        // if(middle_space_tile == bottom_space_tile){
        //     new_solid_tile->bl_ = middle_space_tile;
        //     candidate_tile = middle_space_tile->lb_;
        //     do{
        //         if(candidate_tile->right_top_.x > new_solid_tile->left_bottom_.x && candidate_tile->left_bottom_.x <= new_solid_tile->left_bottom_.x)
        //             new_solid_tile->lb_ = candidate_tile;
        //         candidate_tile = candidate_tile->tr_;
        //     } while (candidate_tile != nullptr && candidate_tile->left_bottom_.x <= new_solid_tile->left_bottom_.x);
        // }

        if(middle_space_tile == top_space_tile){
            new_solid_tile->tr_ = new_right_space_tile;
            // candidate_tile = middle_space_tile->rt_;
            candidate_tile = new_right_space_tile->rt_;
            while (candidate_tile != nullptr && candidate_tile->right_top_.x >= new_solid_tile->right_top_.x){
                if(candidate_tile->left_bottom_.x < new_solid_tile->right_top_.x)
                    new_solid_tile->rt_ = candidate_tile;
                candidate_tile = candidate_tile->bl_;
            }
        }
            
        if(middle_space_tile == bottom_space_tile){
            new_solid_tile->bl_ = middle_space_tile;
            candidate_tile = middle_space_tile->lb_;
            while (candidate_tile != nullptr && candidate_tile->left_bottom_.x <= new_solid_tile->left_bottom_.x){
                if(candidate_tile->right_top_.x > new_solid_tile->left_bottom_.x)
                    new_solid_tile->lb_ = candidate_tile;
                candidate_tile = candidate_tile->tr_;
            }
        }
            

        middle_space_tile->right_top_.x = new_solid_tile->left_bottom_.x;
        middle_space_tile->tr_ = new_solid_tile;
        middle_space_tile->rt_ = rt_of_new_left_space_tile;
        rt_of_new_left_space_tile = middle_space_tile;
    }
}

bool Plane::TileCreation(Point left_bottom, Point right_top){
    if(!AreaSearches(left_bottom, right_top))
        return false;
    Point delta = right_top - left_bottom;
    ideal_area += 1LL * delta.x * delta.y;
    if(left_bottom.x==46885&&left_bottom.y == 233096)
        cout << "hi";
    //Step1
    Point top_point(left_bottom.x, right_top.y);
    Tile* top_space_tile = PointFinding(top_point);
    Tile* new_top_space_tile = nullptr;
    if(right_top.y == top_space_tile->left_bottom_.y){
        top_point.y--;
        top_space_tile = PointFinding(top_point);
    }
    //Step2
    if (!TopEdgeOfAreaOnTopSpaceTileEdge(top_space_tile, left_bottom, right_top)){
        new_top_space_tile = SplitTopSpaceTile(top_space_tile, left_bottom, right_top);
        rt_of_new_left_space_tile = new_top_space_tile;
    }
    else
        rt_of_new_left_space_tile = PointFinding(left_bottom.x - 1, right_top.y);//??????????????????

    //Step3
    Point bottom_point = left_bottom;
    Tile *bottom_space_tile = PointFinding(bottom_point);
    Tile* new_bottom_space_tile = nullptr;
    if(!BottomEdgeOfAreaOnBottomSpaceTileEdge(bottom_space_tile, left_bottom, right_top))
        new_bottom_space_tile = SplitBottomSpaceTile(bottom_space_tile, left_bottom, right_top);
    //Step4: split space tile
    Point next_point = top_point;
    Tile* next_of_last_space_tile = PointFinding(left_bottom.x, left_bottom.y-1);
    Tile* middle_space_tile = top_space_tile;
    Tile* new_solid_tile = NewTile(kSolid, left_bottom, right_top);
    vector<Tile *> middle_space_tile_queue;
    while(middle_space_tile!=next_of_last_space_tile){
        middle_space_tile_queue.push_back(middle_space_tile);
        next_point.y = middle_space_tile->left_bottom_.y - 1;
        middle_space_tile = PointFinding(next_point);
    }
    for (auto it = middle_space_tile_queue.begin(); it != middle_space_tile_queue.end(); it++)
        SplitMiddleSpaceTile((*it), new_solid_tile, top_space_tile, bottom_space_tile);

    //Step4: merge space tile
    vector<Tile *> right_tiles = NeighborFinding(new_solid_tile);
    vector<Tile *> left_tiles = LeftTiles(new_solid_tile);
    if (right_tiles.size() > 0)
        MergeRightTiles(right_tiles);
    if (left_tiles.size() > 0)
        MergeLeftTiles(left_tiles);

    return true;
}

bool Plane::TileCreation(int x1, int y1, int x2, int y2){
    Point left_bottom(x1, y1), right_top(x2, y2);
    return TileCreation(left_bottom, right_top);
}

void Plane::MergeRightTiles (vector<Tile*> right_tiles){
    Tile* first_tile = *(right_tiles.begin());
    Tile* first_tile_rt = first_tile->rt_;
    Tile* last_tile = right_tiles.back();
    Tile* last_tile_lb = last_tile->lb_;
    if(first_tile_rt!=nullptr && first_tile->left_bottom_.x == first_tile_rt->left_bottom_.x && first_tile->right_top_.x == first_tile_rt->right_top_.x)
        right_tiles.insert(right_tiles.begin(), first_tile_rt);
    if(last_tile_lb!=nullptr && last_tile->left_bottom_.x == last_tile_lb->left_bottom_.x && last_tile->right_top_.x == last_tile_lb->right_top_.x)
        right_tiles.push_back(last_tile_lb);

    Tile* candidate_tile;
    for (int i = 0; i < right_tiles.size()-1 ; i++){
        Tile* up = *(right_tiles.begin() + i);
        Tile* down = *(right_tiles.begin() + i + 1);
        bool need_to_merge = up->left_bottom_.x == down->left_bottom_.x && up->right_top_.x == down->right_top_.x && up->type_ == kSpace && down->type_ == kSpace;
        if(need_to_merge){
            candidate_tile = up->rt_;
            while(candidate_tile!=nullptr && candidate_tile->lb_ == up){
                candidate_tile->lb_ = down;
                candidate_tile = candidate_tile->bl_;
            }
            candidate_tile = up->tr_;
            while(candidate_tile!=nullptr && candidate_tile->bl_ == up){
                candidate_tile->bl_ = down;
                candidate_tile = candidate_tile->lb_;
            }
            candidate_tile = up->bl_;
            while(candidate_tile!=nullptr && candidate_tile->tr_ == up){
                candidate_tile->tr_ = down;
                candidate_tile = candidate_tile->rt_;
            }
            down->right_top_ = up->right_top_;
            down->rt_ = up->rt_;
            down->tr_ = up->tr_;
            DeleteTile(up);
        }
    }
}

void Plane::MergeLeftTiles (vector<Tile*> left_tiles){
    Tile* first_tile = *(left_tiles.begin());
    Tile* first_tile_lb = first_tile->lb_;
    Tile* last_tile = left_tiles.back();
    Tile* last_tile_rt = last_tile->rt_;
    if(first_tile_lb!=nullptr && first_tile->left_bottom_.x == first_tile_lb->left_bottom_.x && first_tile->right_top_.x == first_tile_lb->right_top_.x)
        left_tiles.insert(left_tiles.begin(), first_tile_lb);
    if(last_tile_rt!=nullptr && last_tile->left_bottom_.x == last_tile_rt->left_bottom_.x && last_tile->right_top_.x == last_tile_rt->right_top_.x)
        left_tiles.push_back(last_tile_rt);

    Tile* candidate_tile;
    for (int i = 0; i < left_tiles.size()-1 ; i++){
        Tile* down = *(left_tiles.begin() + i);
        Tile* up = *(left_tiles.begin() + i + 1);
        bool need_to_merge = up->left_bottom_.x == down->left_bottom_.x && up->right_top_.x == down->right_top_.x && up->type_ == kSpace && down->type_ == kSpace;
        if(need_to_merge){
            candidate_tile = down->lb_;
            while(candidate_tile!=nullptr && candidate_tile->rt_ == down){
                candidate_tile->rt_ = up;
                candidate_tile = candidate_tile->tr_;
            }
            candidate_tile = down->tr_;
            while(candidate_tile!=nullptr && candidate_tile->bl_ == down){
                candidate_tile->bl_ = up;
                candidate_tile = candidate_tile->lb_;
            }
            candidate_tile = down->bl_;
            while(candidate_tile!=nullptr && candidate_tile->tr_ == down){
                candidate_tile->tr_ = up;
                candidate_tile = candidate_tile->rt_;
            }
            up->left_bottom_ = down->left_bottom_;
            up->lb_ = down->lb_;
            up->bl_ = down->bl_;
            DeleteTile(down);
        }
    }
}

vector<Tile *> Plane::LeftTiles(Tile* t){
    vector<Tile *> left_tiles;
    Tile *candidate_tile = t->bl_;
    // while(candidate_tile!=nullptr && candidate_tile->tr_ == t){
    while(candidate_tile!=nullptr && candidate_tile->left_bottom_.y < t->right_top_.y){
        left_tiles.push_back(candidate_tile);
        candidate_tile = candidate_tile->rt_;
    }
    return left_tiles;
}

Tile* Plane::NewTile (TypesOfTile type, Point left_bottom, Point right_top){
    Tile* new_tile = new Tile(type, left_bottom, right_top);
    new_tile->num = number_of_tiles_++;
    // if(new_tile->num ==506344)
    //     cout << "hi";
    return new_tile;
}

Tile* Plane::NewTile(TypesOfTile type, int x1, int y1, int x2, int y2){
    Tile* new_tile = new Tile(type, x1, y1, x2, y2);
    new_tile->num = number_of_tiles_++;
    return new_tile;
}

void Plane::DeleteTile (Tile* t){
    if(t == hint_tile_)
        if(hint_tile_->rt_!=nullptr)
            hint_tile_ = hint_tile_->rt_;
        else if(hint_tile_->tr_!=nullptr)
            hint_tile_ = hint_tile_->tr_;
        else if(hint_tile_->bl_!= nullptr)
            hint_tile_ = hint_tile_->bl_;
        else if(hint_tile_->lb_!= nullptr)
            hint_tile_ = hint_tile_->lb_;
        else{
            cout << "delete tile fail" << endl;
            t->PrintTile();
        }
            
    delete t;
}

void Plane::Initialize(){
    number_of_tiles_ = 0;
    hint_tile_ =  NewTile(kSpace, min_x_ - 2, min_y_ - 2, max_x_ + 2, max_y_ + 2);
}

void Plane::PrintTiles(vector<Tile*> T){
    for (auto it = T.begin(); it != T.end(); it++)
        (*it)->PrintTile();
}

void Plane::PrintTiles(Tile* t){
    t->PrintTile();
}

void Plane::ReadFiles(string fileName){
    fstream file;
    file.open(fileName, ios::in);
    string line;
    int i = 0;
    while (getline(file, line)){
        if((line[0] >'9' || line[0] <'0')&&line[0] !=' ')
            continue;
        stringstream ss(line);
        //cout << line << endl;
        Point p[4];
        for (int i = 0; i < 4; i++){
            ss >> p[i].x;
            ss >> p[i].y;
        }
        Rct r(*(min_element(p, p + 4)), *(max_element(p, p + 4)));
        if (r.left_bottom.x < r.right_top.x && r.left_bottom.y < r.right_top.y)
            RctList.push_back(r);
        if(i++%100000 == 0)
            cout <<"ReadFile "<< i-1 << endl;
    }

    min_x_ = (*min_element(RctList.begin(), RctList.end(), MinX)).left_bottom.x;
    min_y_ = (*min_element(RctList.begin(), RctList.end(), MinY)).left_bottom.y;
    max_x_ = (*max_element(RctList.begin(), RctList.end(), MaxX)).right_top.x;
    max_y_ = (*max_element(RctList.begin(), RctList.end(), MaxY)).right_top.y;
}

void Plane::OutputFile(string fileName){
    fstream file;
    file.open(fileName, ios::out);
    file<<"gds2{600\nm=2018-09-14 14:26:15 a=2018-09-14 14:26:15\nlib 'asap7sc7p5t_24_SL' 1 1e-6\ncell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";
    vector<Tile *> tilsEnum = EnumerateAllTiles();
    for (auto it = tilsEnum.begin(); it != tilsEnum.end(); it++)
        file << "b{" << (*it)->type_ << " xy(" << (*it)->left_bottom_.x << " " << (*it)->left_bottom_.y << " " << (*it)->left_bottom_.x << " " << (*it)->right_top_.y << " " << (*it)->right_top_.x << " " << (*it)->right_top_.y << " " << (*it)->right_top_.x << " " << (*it)->left_bottom_.y <<")}"<< endl;


    /*int t[] = {142191,142193,4870,142471,142474,142507,142508,142353,142356,142529,142530,142379,142380,142357,142358,142435,142436,142305,142308,142195,142198,142385,142386,142190};
    for (auto it = tilsEnum.begin(); it != tilsEnum.end(); it++)
        if(find(t, t+24,(*it)->num)!=t+24){
            file << "b{" << "3" << " xy(" << (*it)->left_bottom_.x << " " << (*it)->left_bottom_.y << " " << (*it)->left_bottom_.x << " " << (*it)->right_top_.y << " " << (*it)->right_top_.x << " " << (*it)->right_top_.y << " " << (*it)->right_top_.x << " " << (*it)->left_bottom_.y <<")}"<< endl;
            cout << (*it)->num;
        }*/
    file << "}\n}\n";
}

void Plane::Creation(){
    //cout << "size = " << RctList.size() << endl;
    auto first = RctList.begin();
    for (int i = 0 ; i < RctList.size(); i++){
        TileCreation((*(first+i)).left_bottom, (*(first+i)).right_top);

            
        // if(i%100000 == 0){
        //     cout << i << endl;
        //     if(TilePtrCheck(EnumerateAllTiles()))
        //         break;
        //     if(AreaTest())
        //         break;
        // }
        
        // if(i>1787300){
        //     cout << i << endl;
        //     if(TilePtrCheck(EnumerateAllTiles()))
        //         break;
        // }
            
            
        /*if(i==57006){
            cout << "Creation " << i;
            EnumerateAllTiles();
            cout << " Success" << endl;
        }*/
    }
}

bool Plane::TilePtrCheck(Tile* t){
    bool rt = false;
    if(t->rt_!=nullptr){
        if(t->right_top_.x > t->rt_->right_top_.x || t->right_top_.x <= t->rt_->left_bottom_.x){
            PrintTiles(t);
            PrintTiles(t->rt_);
            cout << "Tile " << t->num << " rt = Tile " << t->rt_->num << " error\n\n";
            rt = true;
        }
    }
    else{
        if(t->right_top_.y!=max_y_+2){
            PrintTiles(t);
            cout << "Tile " << t->num << " rt = nullptr error\n\n";
            rt = true;
        }
    }
    
    if(t->tr_!=nullptr){
        if(t->right_top_.y > t->tr_->right_top_.y || t->right_top_.y <= t->tr_->left_bottom_.y){
            PrintTiles(t);
            PrintTiles(t->tr_);
            cout << "Tile " << t->num << " tr = Tile " << t->tr_->num << " error\n\n";
            rt = true;
        }
    }
    else{
        if(t->right_top_.x!=max_x_+2){
            PrintTiles(t);
            cout << "Tile " << t->num << " tr = nullptr error\n\n";
            rt = true;
        }
    }

    if(t->bl_!=nullptr){
        if(t->left_bottom_.y >= t->bl_->right_top_.y || t->left_bottom_.y < t->bl_->left_bottom_.y){
            PrintTiles(t);
            PrintTiles(t->bl_);
            cout << "Tile " << t->num << " bl = Tile " << t->bl_->num << " error\n\n";
            rt = true;
        }
    }
    else{
        if(t->left_bottom_.x!=min_x_-2){
            PrintTiles(t);
            cout << "Tile " << t->num << " bl = nullptr error\n\n";
            rt = true;
        }
    }

    if(t->lb_!=nullptr){
        if(t->left_bottom_.x < t->lb_->left_bottom_.x || t->left_bottom_.x >= t->lb_->right_top_.x){
            PrintTiles(t);
            PrintTiles(t->lb_);
            cout << "Tile " << t->num << " lb = Tile " << t->lb_->num << " error\n\n";
            rt = true;
        }
    }
    else{
        if(t->left_bottom_.y!=min_y_-2){
            PrintTiles(t);
            cout << "Tile " << t->num << " lb = nullptr error\n\n";
            rt = true;
        }
    }
    return rt;
}

bool Plane::TilePtrCheck(vector<Tile* > t){
    bool rt = false;
    for (auto it = t.begin(); it != t.end(); it++)
        if(TilePtrCheck(*it))
            rt = true;
    return rt;
}

long long Plane::TilesArea(){
    vector<Tile *> tilesEnum = EnumerateAllTiles();
    int i = tilesEnum.size();
    long long rt = 0;
    int solidNum = 0;
    for (auto it = tilesEnum.begin(); it != tilesEnum.end(); it++){
        Tile* t = (*it);
        Point delta = t->right_top_ - t->left_bottom_;
        rt += 1LL * delta.x * delta.y;
        if(t->type_==kSolid){
            solid_area += 1LL * delta.x * delta.y;
            solidNum++;
        }
    }
    cout << "total solid num = " << solidNum << endl;
    return rt;
}

long long Plane::TotalArea(){
    int deltaX = max_x_ - min_x_ + 4;
    int deltaY = max_y_ - min_y_ + 4;
    cout << "idea = " << 1LL * (deltaX-4) * (deltaY-4) << endl;
    return 1LL * deltaX * deltaY;
}

bool Plane::AreaTest(){
    if(TilesArea()!=TotalArea()){
        cout << "area error";
        return true;
    }
    return false;
}

bool Plane::TileMergeCheck(Tile* t){
    bool rt = false;
    if(t->type_ == kSolid)
        return false;
    if(t->rt_!=nullptr){
        if(t->left_bottom_.x == t->rt_->left_bottom_.x && t->right_top_.x == t->rt_->right_top_.x && t->rt_->type_ == kSpace){
            PrintTiles(t);
            PrintTiles(t->rt_);
            cout << "Tile " << t->num << " can merge Tile " << t->rt_->num << " error(rt)\n\n";
            rt = true;
        }
    }
    if(t->tr_!=nullptr){
        if(t->left_bottom_.y == t->tr_->left_bottom_.x && t->right_top_.y == t->tr_->right_top_.y && t->tr_->type_ == kSpace){
            PrintTiles(t);
            PrintTiles(t->tr_);
            cout << "Tile " << t->num << " can merge Tile " << t->rt_->num << " error(tr)\n\n";
            rt = true;
        }
    }
    if(t->bl_!=nullptr){
        if(t->left_bottom_.y == t->bl_->left_bottom_.x && t->right_top_.y == t->bl_->right_top_.y && t->bl_->type_ == kSpace){
            PrintTiles(t);
            PrintTiles(t->bl_);
            cout << "Tile " << t->num << " can merge Tile " << t->rt_->num << " error(bl)\n\n";
            rt = true;
        }
    }
    if(t->lb_!=nullptr){
        if(t->left_bottom_.x == t->lb_->left_bottom_.x && t->right_top_.x == t->lb_->right_top_.x && t->lb_->type_ == kSpace){
            PrintTiles(t);
            PrintTiles(t->lb_);
            cout << "Tile " << t->num << " can merge Tile " << t->rt_->num << " error(lb)\n\n";
            rt = true;
        }
    }
    return rt;
}

bool Plane::TileMergeCheck(vector<Tile*> t){
    bool rt = false;
    for (auto it = t.begin(); it != t.end(); it++)
        if(TileMergeCheck(*it))
            rt = true;
    return rt;
}

bool Plane::TileDeletion(int x1, int y1, int x2, int y2){
    Point left_bottom(x1, y1), right_top(x2, y2);
    return TileDeletion(left_bottom, right_top);
}

bool Plane::TileDeletion(Point left_bottom, Point right_top){
    Tile* dead_tile = PointFinding(left_bottom);
    if(dead_tile->type_ == kSpace)
        return false;
    if (dead_tile->left_bottom_ != left_bottom || dead_tile->right_top_ != right_top)
        return false;
    dead_tile->type_ = kSpace;
    vector<Tile *> left_neighbors = LeftTiles(dead_tile);
    SpaceSplitRightNeighbors(dead_tile);
    for (auto it = left_neighbors.begin(); it != left_neighbors.end(); it++){
        if((*it)->type_ == kSpace){
            SpaceSplitRightNeighbors(*it);
        }
        else if ((*it)->type_ == kSolid)
            SolidSplitRightNeighbors(*it);
    }
        
    
}

void Plane::SpaceSplitRightNeighbors(Tile* main_tile){
    vector<Tile*> right_neighbors = NeighborFinding(main_tile);
    Tile *first_neighbor;
    Tile *last_neighbor;
    if (right_neighbors.size() != 0){
        first_neighbor = *(right_neighbors.begin());
        last_neighbor = right_neighbors.back();
    }
    for (auto it = right_neighbors.begin(); it != right_neighbors.end(); it++){
        Tile* right_neighbor = *it;
        //right neighbor need to cut by upper bound
        if(right_neighbor->right_top_.y > main_tile->right_top_.y && right_neighbor->type_ == kSpace){
            SplitRightNeighbor(right_neighbor, main_tile, main_tile->right_top_.y, true);
            it--;
        }
        //main tile need to split and merge by right neighbor
        else if(right_neighbor->left_bottom_.y >= main_tile->left_bottom_.y){
            if(right_neighbor->type_ == kSpace){
                SplitMainTileAndHorizontalMerge(main_tile, right_neighbor);
                //check vertical merge
                if (right_neighbor == first_neighbor && right_neighbor->rt_!=nullptr){
                    Tile* up = right_neighbor->rt_;
                    if(up->left_bottom_.x == right_neighbor->left_bottom_.x && up->right_top_.x == right_neighbor->right_top_.x)
                        VerticalMerge(right_neighbor, up);
                }
                if (right_neighbor == last_neighbor && right_neighbor->lb_ != nullptr){
                    Tile *down = right_neighbor->lb_;
                    if (down->left_bottom_.x == right_neighbor->left_bottom_.x & down->right_top_.x == right_neighbor->right_top_.x)
                        VerticalMerge(right_neighbor, down);
                }
            }
            else if(right_neighbor->type_ == kSolid){
                //next right neigbor is also solid
                if (it + 1 != right_neighbors.end() && (*(it + 1))->type_ == kSolid)
                    continue;
                if(right_neighbor == last_neighbor)
                    continue;
                SplitMainTile(main_tile, right_neighbor->left_bottom_.y);
            }
        }
        //right neighbor need to cut by lowwer bound
        else if(right_neighbor->left_bottom_.y < main_tile->left_bottom_.y && right_neighbor->type_ == kSpace){
            SplitRightNeighbor(right_neighbor, main_tile, main_tile->left_bottom_.y, false);
            it--;
        }

    }
}

void Plane::SolidSplitRightNeighbors(Tile* main_tile){
    vector<Tile*> right_neighbors = NeighborFinding(main_tile);
    Tile *first_neighbor = *(right_neighbors.begin());
    Tile *last_neighbor = right_neighbors.back();
    for (auto it = right_neighbors.begin(); it != right_neighbors.end(); it++){
        Tile* right_neighbor = *it;
        //first neighbor need to cut by upper bound
        if(right_neighbor == first_neighbor && right_neighbor->right_top_.y > main_tile->right_top_.y && right_neighbor->type_ == kSpace)
            SplitRightNeighbor(right_neighbor, main_tile, main_tile->right_top_.y, true);
        if (right_neighbor == last_neighbor && right_neighbor->left_bottom_.y < main_tile->left_bottom_.y && right_neighbor->type_ == kSpace)
            SplitRightNeighbor(right_neighbor, main_tile, main_tile->left_bottom_.y, false);
    }
}

void Plane::SplitRightNeighbor(Tile* right_neighbor, Tile* main_tile, int y, bool new_on_top){
    if(new_on_top){
        Tile *new_tile = NewTile(kSpace, right_neighbor->left_bottom_.x, y, right_neighbor->right_top_.x, right_neighbor->right_top_.y);
        new_tile->SetPtr(right_neighbor->rt_, right_neighbor->tr_, main_tile->rt_, right_neighbor);
        //fix the tiles whose lb = right neighbor
        Tile* candidate_tile = right_neighbor->rt_;
        while(candidate_tile!=nullptr && candidate_tile->lb_ == right_neighbor){
            candidate_tile->lb_ = new_tile;
            candidate_tile = candidate_tile->bl_;
        }
        //fix the tiles whose tr = right neighbor
        candidate_tile = right_neighbor->bl_;
        while(candidate_tile!= nullptr && candidate_tile->tr_ == right_neighbor){
            if(candidate_tile->right_top_.y > new_tile->left_bottom_.y)
                candidate_tile->tr_ = new_tile;
            candidate_tile = candidate_tile->rt_;
        }
        //fix the tiles whose bl = right neighbor
        candidate_tile = right_neighbor->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == right_neighbor){
            if(candidate_tile->left_bottom_.y >= new_tile->left_bottom_.y)
                candidate_tile->bl_ = new_tile;
            candidate_tile = candidate_tile->lb_;
        }
        //fix right neighbor
        right_neighbor->rt_ = new_tile;
        right_neighbor->right_top_.y = y;
        while(right_neighbor->tr_!=nullptr && right_neighbor->tr_->left_bottom_.y >= right_neighbor->right_top_.y)
            right_neighbor->tr_ = right_neighbor->tr_->lb_;
    }
    else{
        Tile* new_tile = NewTile(kSpace, right_neighbor->left_bottom_.x, right_neighbor->left_bottom_.y, right_neighbor->right_top_.x, y);
        new_tile->SetPtr(right_neighbor, right_neighbor->tr_, right_neighbor->bl_, right_neighbor->lb_);
        while(new_tile->tr_!=nullptr && new_tile->tr_->left_bottom_.y >= new_tile->right_top_.y)
            new_tile->tr_ = new_tile->tr_->lb_;
        //fix the tiles whose rt = right neighbor
        Tile *candidate_tile = right_neighbor->lb_;
        while (candidate_tile != nullptr && candidate_tile->rt_ == right_neighbor){
            candidate_tile->rt_ = new_tile;
            candidate_tile = candidate_tile->tr_;
        }
        //fix the tiles whose tr = right neighbor
        candidate_tile = right_neighbor->bl_;
        while (candidate_tile != nullptr && candidate_tile->tr_ == right_neighbor){
            if(candidate_tile->right_top_.y <= new_tile->right_top_.y)
                candidate_tile->tr_ = new_tile;
            candidate_tile = candidate_tile->rt_;
        }
        //fix the tiles whose bl = right neighbor
        candidate_tile = right_neighbor->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == right_neighbor){
            if(candidate_tile->left_bottom_.y < new_tile->right_top_.y)
                candidate_tile->bl_ = new_tile;
            candidate_tile = candidate_tile->lb_;
        }
        //fix right neighbor
        right_neighbor->lb_ = new_tile;
        right_neighbor->left_bottom_.y = y;
        right_neighbor->bl_ = main_tile;
    }
}

void Plane::SplitMainTile(Tile* main_tile, int y){
    Tile *new_tile = NewTile(kSpace, main_tile->left_bottom_.x, y, main_tile->right_top_.x, main_tile->right_top_.y);
    new_tile->SetPtr(main_tile->rt_, main_tile->tr_, main_tile->bl_, main_tile);
    while(new_tile->bl_!= nullptr && new_tile->bl_->right_top_.y <= y)
        new_tile->bl_ = new_tile->bl_->rt_;
    //fix the tiles whose lb = main tile
    Tile* candidate_tile = main_tile->rt_;
    while(candidate_tile!=nullptr && candidate_tile->lb_ == main_tile){
        candidate_tile->lb_ = new_tile;
        candidate_tile = candidate_tile->bl_;
    }
    //fix the tiles whose tr = main tile
    candidate_tile = main_tile->bl_;
    while(candidate_tile!= nullptr && candidate_tile->tr_ == main_tile){
        if(candidate_tile->right_top_.y > new_tile->left_bottom_.y)
            candidate_tile->tr_ = new_tile;
        candidate_tile = candidate_tile->rt_;
    }
    //fix the tiles whose bl = main tile
    candidate_tile = main_tile->tr_;
    while(candidate_tile!=nullptr && candidate_tile->bl_ == main_tile){
        if(candidate_tile->left_bottom_.y >= new_tile->left_bottom_.y)
            candidate_tile->bl_ = new_tile;
        candidate_tile = candidate_tile->lb_;
    }
    //fix main tile
    main_tile->rt_ = new_tile;
    main_tile->right_top_.y = y;
    while(main_tile->tr_!=nullptr && main_tile->tr_->left_bottom_.y >= y)
        main_tile->tr_ = main_tile->tr_->lb_;
}

void Plane::VerticalMerge(Tile* right_neighbor, Tile* merged_tile){
    if(merged_tile == right_neighbor->rt_){
        //fix the tiles whose lb = merged tile
        Tile* candidate_tile = merged_tile->rt_;
        while(candidate_tile!=nullptr && candidate_tile->lb_ == merged_tile){
            candidate_tile->lb_ = right_neighbor;
            candidate_tile = candidate_tile->bl_;
        }
        //fix the tiles whose tr = merged tile
        candidate_tile = merged_tile->bl_;
        while(candidate_tile!= nullptr && candidate_tile->tr_ == merged_tile){
            candidate_tile->tr_ = right_neighbor;
            candidate_tile = candidate_tile->rt_;
        }
        //fix the tiles whose bl = merged tile
        candidate_tile = merged_tile->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == merged_tile){
            candidate_tile->bl_ = right_neighbor;
            candidate_tile = candidate_tile->lb_;
        }
        //fix right neighbor
        right_neighbor->right_top_.y = merged_tile->right_top_.y;
        right_neighbor->rt_ = merged_tile->rt_;
        right_neighbor->tr_ = merged_tile->tr_;
        DeleteTile(merged_tile);
    }
    else if(merged_tile == right_neighbor->lb_){
        //fix the tiles whose rt = merged tile
        Tile* candidate_tile = merged_tile->lb_;
        while(candidate_tile!=nullptr && candidate_tile->rt_ == merged_tile){
            candidate_tile->rt_ = right_neighbor;
            candidate_tile = candidate_tile->tr_;
        }
        //fix the tiles whose bl = merged tile
        candidate_tile = merged_tile->tr_;
        while(candidate_tile!=nullptr && candidate_tile->bl_ == merged_tile){
            candidate_tile->bl_ = right_neighbor;
            candidate_tile = candidate_tile->lb_;
        }
        //fix the tiles whose tr = merged tile
        candidate_tile = merged_tile->bl_;
        while(candidate_tile!=nullptr && candidate_tile->tr_ == merged_tile){
            candidate_tile->tr_ = right_neighbor;
            candidate_tile = candidate_tile->rt_;
        }
        //fix right neighbor
        right_neighbor->left_bottom_ = merged_tile->left_bottom_;
        right_neighbor->lb_ = merged_tile->lb_;
        right_neighbor->bl_ = merged_tile->bl_;
        DeleteTile(merged_tile);
    }
}

void Plane::SplitMainTileAndHorizontalMerge(Tile* main_tile, Tile* right_neighbor){
    //fix the tiles whose lb = main tile
    Tile* candidate_tile = main_tile->rt_;
    while(candidate_tile!=nullptr && candidate_tile->lb_ == main_tile){
        candidate_tile->lb_ = right_neighbor;
        candidate_tile = candidate_tile->bl_;
    }
    //fix the tiles whose tr = main tile
    candidate_tile = main_tile->bl_;
    while(candidate_tile!= nullptr && candidate_tile->tr_ == main_tile){
        if(candidate_tile->right_top_.y > right_neighbor->left_bottom_.y)
            candidate_tile->tr_ = right_neighbor;
        candidate_tile = candidate_tile->rt_;
    }
    //fix main tile
    main_tile->rt_ = right_neighbor;
    main_tile->right_top_.y = right_neighbor->left_bottom_.y;
    // while(main_tile->tr_!=nullptr && main_tile->tr_->right_top_.y > right_neighbor->left_bottom_.y) 09/19 14:20
    while(main_tile->tr_!=nullptr && main_tile->tr_->left_bottom_.y >= right_neighbor->left_bottom_.y)
        main_tile->tr_ = main_tile->tr_->lb_;
    
    //extend and fix right neighbor
    right_neighbor->left_bottom_.x = main_tile->left_bottom_.x;
    right_neighbor->lb_ = main_tile;
    if(main_tile->left_bottom_.y == right_neighbor->left_bottom_.y)
        right_neighbor->lb_ = main_tile->lb_;
    right_neighbor->bl_ = main_tile->bl_;
    while(right_neighbor->bl_!=nullptr && right_neighbor->bl_->right_top_.y <= right_neighbor->left_bottom_.y)
        right_neighbor->bl_ = right_neighbor->bl_->rt_;
    
    //delete main tile
    if(main_tile->left_bottom_.y == right_neighbor->left_bottom_.y){
        //fix the tiles whose rt = main tile
        candidate_tile = main_tile->lb_;
        while(candidate_tile!= nullptr && candidate_tile->rt_ == main_tile){
            candidate_tile->rt_ = right_neighbor;
            candidate_tile = candidate_tile->tr_;
        }
        DeleteTile(main_tile);
    }
        
}

void Plane::Deletion(){
    auto first = RctList.begin();
    for (int i = 0 ; i < RctList.size(); i++){
        // if(i==521666){
        //     cout << "hi";
        //     cout << "hi";
        // }
            
        TileDeletion((*(first+i)).left_bottom, (*(first+i)).right_top);
        // if(i%100000==0 &&i>500000){
        //     cout << i << endl;
        //     vector<Tile *> tile_enum = EnumerateAllTiles();
        //     if(TilePtrCheck(tile_enum)){
        //         cout << i << endl;
        //         break;
        //     }
        // }
                
        //     if(TileMergeCheck(tile_enum)) {
        //         cout << i << endl;
        //         break;
        //     }
        //     if(AreaTest()){
        //         cout << i << endl;
        //         break;
        //     }
        // }
        
    }
        
}