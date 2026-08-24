#include "raylib.h"
#include <iostream>
#include <cassert>
#include <vector>

#define WINDOW_HEIGHT 800.0
#define WINDOW_WIDTH 800.0


// Vec2 makes life easy. Since its a 2D simluation.
// Overloaded all common operators.
struct Vec2 : public Vector2 {
    Vec2(){x = 0.0f, y = 0.0f;}
    Vec2(float _x, float _y){x = _x, y = _y;};
    Vec2(float _x){x = _x, y = _x;};
    Vec2(const Vector2& v){x = v.x, y = v.y;};

    // v += v
    Vec2& operator+=(const Vec2& b) { x += b.x; y += b.y; return *this; }
    Vec2& operator-=(const Vec2& b) { x -= b.x; y -= b.y; return *this; }
    Vec2& operator*=(const Vec2& b) { x *= b.x; y *= b.y; return *this; }
    Vec2& operator/=(const Vec2& b) { x /= b.x; y /= b.y; return *this; }

    // v x= scalarF
    Vec2& operator+=(float val) { x += val; y += val; return *this; }
    Vec2& operator-=(float val) { x -= val; y -= val; return *this; }
    Vec2& operator*=(float val) { x *= val; y *= val; return *this; }
    Vec2& operator/=(float val) { x /= val; y /= val; return *this; }

    // v x v
    Vec2 operator+(const Vec2& b) const { return Vec2(x + b.x, y + b.y); }
    Vec2 operator-(const Vec2& b) const { return Vec2(x - b.x, y - b.y); }
    Vec2 operator*(const Vec2& b) const { return Vec2(x * b.x, y * b.y); }
    Vec2 operator/(const Vec2& b) const { return Vec2(x / b.x, y / b.y); }

    // v x scalarF
    Vec2 operator+(float val) const { return Vec2(x + val, y + val); }
    Vec2 operator-(float val) const { return Vec2(x - val, y - val); }
    Vec2 operator*(float val) const { return Vec2(x * val, y * val); }
    Vec2 operator/(float val) const { return Vec2(x / val, y / val); }
};

// Ball constructor initialises with pos, radius, vel, acc and color.
// Move() updates the position & velocity of ball.
// Draw() draws the ball using DrawCircleV.
// MakeRandom() fills initialises public instances with random values
// of an acceptable range.
// CheckCollision() checks if this ball is colliding with any other ball object.
class Ball {
public:
    Vec2 s;
    Vec2 u;
    Vec2 a;
    float radius;
    Color color;
    int m_id;

    Ball(Vec2 pos = Vec2(WINDOW_HEIGHT/2, WINDOW_WIDTH/2), 
         float r = 20.0f, 
         Vec2 vel = Vec2(0.0f, 0.0f), 
         Vec2 acc = Vec2(0.0f, 0.0f),
         Color c = GREEN,
         int id = 0) 
        : s(pos), radius(r), u(vel), a(acc), color(c), m_id(id) {}

    void Draw() const {
        DrawCircleV(s, radius, color);
    }

    void Move() {
        float dt = GetFrameTime();
        u += a * dt;
        if(s.x <= radius || s.x >= WINDOW_WIDTH- radius){ u.x*=-1;  a.x*=-1;}
        if(s.y <= radius || s.y >= WINDOW_WIDTH- radius){ u.y*=-1;  a.y*= -1;}
        s += u * dt;
    }

    void MakeRandom(){
        s = Vec2(GetRandomValue(0, 800), GetRandomValue(0, 800));
        u = Vec2(GetRandomValue(10, 50));
        a = Vec2(GetRandomValue(10, 50));
        radius = GetRandomValue(3, 10);
    }

    bool CheckCollision(Ball& b){
        Vec2 dist_squared_vec2 = (s-b.s)*(s-b.s);
        assert(dist_squared_vec2.x > 0 && dist_squared_vec2.y > 0 && "Absolute distance negative.\n");
        float dist_squared = dist_squared_vec2.x + dist_squared_vec2.y;
        if(dist_squared <= radius*radius)return true;
        return false;
    }

    void SwapVel(Ball& b){
        Vec2 temp = u;
        if(b.u.x != 0)u.x = b.u.x;
        if(b.u.y != 0)u.y = b.u.y;
        if(u.x != 0)b.u.x = temp.x;
        if(u.x != 0)b.u.x = temp.y;
    }
};


// Simulator is made to abstract the entire process, its THE WORLD here.
// Constructor initialises balls container that has all the balls, and
// gives them random initial properties.
// Run() draws and moves every ball in the world.
class Simulator{
private:
    std::vector<Ball> balls;
    std::vector<Color> colors = { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, MAGENTA, LIME };
    int m_n;
public:
    Simulator(int n) : m_n(n) {};
    void AllCollisions(){
        //vector<bool> vis(m_n);
        for(int i = 0; i < m_n; i++){
            for(int j = i+1; j < m_n; j++){
                bool collided = balls[i].CheckCollision(balls[j]);
                if(collided){
                    balls[i].SwapVel(balls[j]);
                }
            }
        }
    }

    void Run(){
        for(auto& b: balls){
            b.Move();
            b.Draw();
        }
    }
    void GenRand(){
        for(int i = 0; i < m_n; i++){
            Ball x;
            x.MakeRandom();
            x.color = colors[i % colors.size()];
            x.m_id = i;
            balls.push_back(x);
        }
    }

    void MakeCollisionTest(){
        Ball mak(Vec2(400.0, 400.0), 20.0f, Vec2(100.0f, 100.0f), Vec2(0.0f, 0.0f), RED);
        Ball kus(Vec2(300.0, 300.0), 20.0f, Vec2(60.0f, 60.0f), Vec2(0.0f, 0.0f), BLUE);
        balls.push_back(mak);
        balls.push_back(kus);
    }
};

int main(){
    const std::string makeki = "[MAKEKI] ";
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Balls");
    if(IsWindowReady())std::cout << makeki << "Window is ready\n";
    SetTargetFPS(100);

    Simulator world(2);
    world.MakeCollisionTest();
    while (!WindowShouldClose()) {
        BeginDrawing();
        world.AllCollisions();
        world.Run();
        ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
}
