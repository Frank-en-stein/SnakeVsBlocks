#include <bits/stdc++.h>
using namespace std;

class Snake {
public:
    int position;
    double color_r,color_g,color_b;
    vector<double> last_position;
    long long int length;
    double radius;
    double del_coeff;
    double tail_lag;
    double smoothness;

    int score = 0;

    int max_food;
    bitset<10> food_upper1;
    bitset<5> food_lower;

    Snake()
    {
        position = 0;
        color_r = 1;
        color_g = 1;
        color_b = 0;
        length = 5;
        radius = 8;
        del_coeff = 1;
        tail_lag = .1;
        smoothness = 0;
        for(int i=0; i<length; i++) last_position.push_back(0);
        food_upper1.reset();
        food_lower.reset();
        max_food = rand()%3+1;
        for(int i=0; i<max_food; i++) food_upper1.set(rand()%10);
        for(int i=0; i<max_food; i++) food_lower.set(rand()%5);
    }

    void resetFood()
    {
        food_upper1.reset();
        food_lower.reset();
    }

};

class Block {
    int max_block_weight;
public:
    float color_r[5], color_g[5], color_b[5];
    int weight[5];
    int gap;
    int block_frames,block_frames_set;
    double position;
    double speed;
    bitset<5> view;
    bool view_to_set;
    Block(double h)
    {
        gap = 2;
        max_block_weight = 9;
        position = h*.75;
        block_frames = block_frames_set = 3;
        speed = 1;
        view.set();
        view_to_set = true;
        for(int i=0; i<5; i++)
        {
            color_r[i] = (float)(rand()%10000)/10000;
            color_g[i] = (float)(rand()%10000)/10000;
            color_b[i] = (float)(rand()%10000)/10000;

            weight[i] = rand()%max_block_weight+1;
        }
    }
};

class Bar {
public:
    double length,width;
    int position;

    Bar()
    {

    }
    bool appear()
    {
        return rand()<.5;
    }
};
