/* Your code here! */
#include <cstdlib>
#include <ctime> 
#include <random>
#include <algorithm>
#include <queue>

#include "maze.h"

using namespace cs225;

SquareMaze::SquareMaze() {
    //code here
}

void SquareMaze::makeMaze(int width, int height) {
    // code here
    width_ = width;
    height_ = height;
    h_walls_ = std::vector<bool>((width-1)*height, true);
    v_walls_ = std::vector<bool>((height-1)*width, true);
    int num_walls = (height-1)*width + (width-1)*height;
    connected_ = DisjointSets(width*height);

    std::vector<int> wall_index;
    for (int i = 0; i < num_walls; i++) { wall_index.push_back(i); }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(wall_index.begin(), wall_index.end(), g);
    for (int n : wall_index) {
        if (connected_.size(0) == width*height) { break; }
        
        if (n < (width-1)*height) {
            HorizontalWall(width, height, n, 1);
        } else {
            int new_rand = n - (width-1)*height;
            VerticalWall(width, height, new_rand, 1);
        }
    }
}

void SquareMaze::HorizontalWall(int width, int height, int randNum, int count) {
    if (count > ((width-1)*height)) { return; }
    int h = randNum/(width-1);
    int w_num = randNum % (width-1);
    int a = h*width + w_num;
    int b = h*width + w_num + 1;
    if (randNum >= ((width-1)*height)) { std::cout << randNum << std::endl; }
    if (!h_walls_[randNum] || connected_.Connected(a, b)) { 
        return;
        count++;
        HorizontalWall(width, height, (randNum+1) % ((width-1)*height), count); 
        //return;
    }
    h_walls_[randNum] = false;
    connected_.setUnion(a, b);
}

void SquareMaze::VerticalWall(int width, int height, int randNum, int count) {
    if (count > ((height-1)*width)) { return; }
    int h = randNum/width;
    int w_num = randNum % width;
    int a = randNum;
    int b = randNum + width;
    if (randNum >= ((height-1)*width)) { std::cout << randNum << std::endl; }
    if (!v_walls_[randNum] || connected_.Connected(a, b)) { 
        return;
        count++;
        randNum = (randNum + 1) % ((height-1)*width);
        VerticalWall(width, height, randNum, count);
    }
    v_walls_[randNum] = false;
    connected_.setUnion(a, b);
}

bool SquareMaze::canTravel(int x, int y, Direction dir) const {
    int num = y*width_ + x;
    //int num_dir;
    if (dir == Direction::RIGHT) { 
        if (x == width_-1) { return false; }
        return !h_walls_[y*(width_-1) + x]; 
    }
    if (dir == Direction::DOWN) { 
        if (y == width_-1) { return false; }
        return !v_walls_[y*width_ + x]; 
    }
    if (dir == Direction::LEFT) {
        if (x == 0) { return false; } 
        return !h_walls_[y*(width_-1) + x-1]; 
    }
    if (dir == Direction::UP) { 
        if (y == 0) { return false; }
        return !v_walls_[(y-1)*width_ + x]; 
    }
    return false;
    
}

void SquareMaze::setWall(int x, int y, Direction dir, bool exists) {
    //code here
    if (dir == Direction::DOWN) {
        v_walls_[y*width_ + x] = exists;
    } else if (dir == Direction::RIGHT) {
        h_walls_[y*(width_-1) + x] = exists;
    }
}

std::vector<Direction> SquareMaze::solveMaze(int startX) {
    //placeholder code
    std::vector<Direction> solution;
    std::vector<int> distance(width_*height_, -1);
    std::vector<int> previous(width_*height_, -1);
    std::queue<int> q;

    distance[startX] = 0;
    q.push(startX);
    std::vector<std::pair<Direction, int>> dirs = {{RIGHT, 1}, {DOWN, width_}, {LEFT, -1}, {UP, -width_}};   
    while (!q.empty()) {
        int curr = q.front();
        int x = curr % width_;
        int y = curr / width_;
        q.pop();
        for (auto [dir, change] : dirs) {
            if (canTravel(x, y, dir)) {
                int next = curr + change;
                if (distance[next] == -1) {
                    distance[next] = distance[curr] + 1;
                    previous[next] = curr;
                    q.push(next);
                }
            }
        }
    }
    int best_cell = 0;
    int best_dist = -1;
    for (int index = (height_-1)*width_; index < height_*width_; index++) {
        if (distance[index] > best_dist) {
            best_cell = index;
            best_dist = distance[index];
        }
    }

    while (best_cell != startX) {
        int prev = previous[best_cell];
        int diff = best_cell - prev;
        if (diff == 1) {
            solution.insert(solution.begin(), Direction::RIGHT);
        }
        if (diff == -1) {
            solution.insert(solution.begin(), Direction::LEFT);
        }
        if (diff == width_) {
            solution.insert(solution.begin(), Direction::DOWN);
        }
        if (diff == -width_) {
            solution.insert(solution.begin(), Direction::UP);
        }
        best_cell = previous[best_cell];
    }

    return solution;

}

PNG * SquareMaze::drawMaze(int start) const {
    //placeholder code
    PNG* image = new PNG(width_*10+1,height_*10+1);
    int img_width = width_*10+1;
    int img_height = height_*10+1;
    HSLAPixel black(0,0,0,1);
    HSLAPixel white(0,0,1,1);
    //int start = start;

    for (int x = 0; x < img_width; x++) { image->getPixel(x, 0) = black; }
    for (int y = 0; y < img_height; y++) { image->getPixel(0, y) = black; }
    for (int x = (start*10)+1; x < (start+1)*10; x++) { image->getPixel(x, 0) = white; }

    for (int cell = 0; cell < width_*height_; cell++) {
        int x = cell % width_;
        int y = cell / width_;
        if (!canTravel(x, y, Direction::RIGHT)) {
            for (int y_i = (y*10); y_i <= (y+1)*10; y_i++) { image->getPixel((x+1)*10, y_i) = black; }
        }
        if (!canTravel(x, y, Direction::DOWN)) {
            for (int x_i = (x*10); x_i <= (x+1)*10; x_i++) { image->getPixel(x_i, (y+1)*10) = black; }
        }
    }
    return image;
}

PNG * SquareMaze::drawMazeWithSolution(int start) {
    //placehoder code
    PNG* image = drawMaze(start);
    std::vector<Direction> path = solveMaze(start);
    HSLAPixel red(0,1,0.5,1);
    HSLAPixel white(0,0,1,1);
    int x = start;
    int y = 0;
    for (Direction d : path) {
        int next_x = x;
        int next_y = y;
        if (d == Direction::RIGHT) { 
            next_x += 1; 
            for (int i = (x*10) + 5; i <= next_x*10 + 5; i++) { image->getPixel(i, y*10 + 5) = red; }
        } else if (d == Direction::LEFT) { 
            next_x -= 1; 
            for (int i = next_x*10 + 5; i <= x*10 + 5; i++) { image->getPixel(i, y*10 + 5) = red; }
        } else if (d == Direction::DOWN) { 
            next_y += 1; 
            for (int i = (y*10) + 5; i <= next_y*10 + 5; i++) { image->getPixel(x*10 + 5, i) = red; }
        } else { 
            next_y -= 1; 
            for (int i = next_y*10 + 5; i <= y*10 + 5; i++) { image->getPixel(x*10 + 5, i) = red; }
        }
        x = next_x;
        y = next_y;
    }

    for (int i = (x*10)+1; i < (x+1)*10; i++) { image->getPixel(i, (y+1)*10) = white; }

    return image;
}

void SquareMaze::printMaze() {
    std::cout << " ------ " << std::endl;
    //code here
    int square = 1;
    //std::cout << h_walls_.size() << std::endl;
    //std::cout << " " << square << " ";
    //square++;
    /*
    for (unsigned int j = 0; j < h_walls_.size() - 1; j++) {
        for (unsigned int i = 0; i < h_walls_[j].size(); i++) {
            if (h_walls_[j][i]) {
                std::cout << " " << square << " |";
                
            } else {
                std::cout << " " << square << "  ";
            }
            square++;
        }
        std::cout << square << std::endl;
        square++;
        for (unsigned int i = 0; i < v_walls_[j].size(); i++) {
            if (v_walls_[j][i]) {
                std::cout << " -  ";
            } else {
                std::cout << "    ";
            }
        }
        std::cout << "" << std::endl;

    }  

    for (unsigned int i = 0; i < h_walls_[h_walls_.size() - 1].size(); i++) {
            if (h_walls_[h_walls_.size() - 1][i]) {
                std::cout << " " << square << " |";
                
            } else {
                std::cout << " " << square << "  ";
            }
            square++;
        }
        std::cout << square << std::endl;
    std::cout << " ------ " << std::endl;
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            std::cout << x << " " << y << " Right: " << canTravel(x, y, Direction::RIGHT) << std::endl;
            std::cout << x << " " << y << " Down: " << canTravel(x, y, Direction::DOWN) << std::endl;
        }
    }*/
}