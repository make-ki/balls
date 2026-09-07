#include "raylib.h"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>

#define WINDOW_HEIGHT 800.0
#define WINDOW_WIDTH 800.0
#define RANDOM_BALLS 2
#define MAKEKI "[MAKEKI] "
#define dbg(x) std::cout << MAKEKI << x << std::endl

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

    // v == v
    bool operator==(const Vec2& b) const {return (x == b.x && y == b.y);}


    static float dist(const Vec2& a, const Vec2& b){
        float X = (a.x-b.x) * (a.x-b.x);
        float Y = (a.y-b.y)* (a.y-b.y);
        return sqrt(X + Y);
    }

    static Vec2 normal(const Vec2& a, const Vec2& b){
        return Vec2(b.x-a.x, b.y-a.y);
    }

    static float mag(const Vec2& sb) {
        float X = sb.x * sb.x;
        float Y = sb.y * sb.y;
        return sqrt(X + Y);
    }

    static float udot(const Vec2& b, const Vec2& a){
        Vec2 prod = a*b;
        return prod.x + prod.y;
    }

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
    float mass;
    Color color;
    int m_id;

    Ball(Vec2 pos = Vec2(WINDOW_HEIGHT/2, WINDOW_WIDTH/2), 
         float r = 20.0f, 
         float m = 20.0f,
         Vec2 vel = Vec2(0.0f, 0.0f), 
         Vec2 acc = Vec2(0.0f, 0.0f),
         Color c = GREEN,
         int id = 0) 
        : s(pos), radius(r), mass(m), u(vel), a(acc), color(c), m_id(id) {}

    void Draw() const {
        DrawCircleV(s, radius, color);
        DrawText(TextFormat("%.1f", mass), s.x-5, s.y-5, 12, BLACK);
    }

    void Move() {
        float dt = GetFrameTime();
        u += a * dt;
        if(s.x <= radius || s.x >= WINDOW_WIDTH- radius){ u.x*=-1;  a.x*=-1;}
        if(s.y <= radius || s.y >= WINDOW_HEIGHT- radius){ u.y*=-1;  a.y*= -1;}
        s += u * dt;
    }

    void FillRandom(){
        s = Vec2(GetRandomValue(30, 770), GetRandomValue(30, 770));
        u = Vec2(GetRandomValue(10, 50));
        a = Vec2(GetRandomValue(1, 10));
        radius = GetRandomValue(15, 20);
    }

    bool CheckCollision(const Ball& b) const{
        float dist = Vec2::dist(this->s, b.s);
        assert(dist >= 0.0f && "fabsolute distance negative.\n");
        if(dist <= (radius+b.radius)-0.5)return true;
        return false;
    }


    void ChangeVel(Ball& b){
        float v1, v2, u1, u2;
        Vec2 i_u = u;
        Vec2 i_bu = b.u;
        Vec2 n = Vec2::normal(this->s, b.s);
        n /= Vec2::mag(n);
        u1 = Vec2::udot(this->u, n);
        u2 = Vec2::udot(b.u, n);
        u -= n*u1;
        b.u -= n*u2;

        //Ball::metric(b);
        float del_mass_AB = mass - b.mass;
        float sum_mass_AB = mass + b.mass;
        v1 = u1*(del_mass_AB/sum_mass_AB) + u2*(2*b.mass/sum_mass_AB);
        v2 = u1*(2*mass/sum_mass_AB) + u2*(-1*del_mass_AB/sum_mass_AB);
        u += n*v1;
        b.u += n*v2;

        float KE1 = 0.5 * mass * Vec2::mag(i_u) * Vec2::mag(i_u) + 0.5 * b.mass * Vec2::mag(i_bu) * Vec2::mag(i_bu);
        float KE2 = 0.5 * mass * Vec2::mag(u) * Vec2::mag(u) + 0.5 * b.mass * Vec2::mag(b.u) * Vec2::mag(b.u);
        assert(fabs(KE1 - KE2) <= 1.0 && "KE not conserved!");
    }

    static void metric(const Ball& b) {
        std::cout << "POS: " << b.s.x << ' ' << b.s.y << '\n';
        std::cout << "VEL: " << b.u.x << ' ' << b.u.y << '\n';
    }

    float kinEnergy(){
        return 0.5 * mass * Vec2::mag(u) * Vec2::mag(u);
    }
};


// Simulator is made to fabstract the entire process, its THE WORLD here.
// Constructor initialises balls container that has all the balls, and
// gives them random initial properties.
// Run() draws and moves every ball in the world.
class Simulator{
private:
    std::vector<Ball> balls;
    std::vector<Color> colors = { RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE, MAGENTA, LIME };
    int m_n;
    float total_ke = 0;
    float begin_ke = 0;
public:
    Simulator(int n) : m_n(n) {};

    void AllCollisions(){
        total_ke = 0;
        for(int i = 0; i < m_n; i++){
            for(int j = i+1; j < m_n; j++){
                bool collided = balls[i].CheckCollision(balls[j]);
                if(collided){
                    //Ball::metric(balls[i]);
                    //Ball::metric(balls[j]);
                    balls[i].ChangeVel(balls[j]);
                }
            }
            total_ke += balls[i].kinEnergy();
        }
        DrawText(TextFormat("Cur KE: %.1f", total_ke), 20,40, 20, GREEN);
        DrawText(TextFormat("Begin KE: %.1f", begin_ke), 20,60, 20, GREEN);
    }

    bool isBallColliding(const Ball& b) const{
        bool collided = false;
        for(int i = 0; i < balls.size(); i++){
            collided = b.CheckCollision(balls[i]);
            if(collided)break;
        }
        return collided;
    }

    void Run(){
        for(auto& b: balls){
            b.Move();
            b.Draw();
        }
    }
    void GenRand(){
        std::cout << MAKEKI << "Making " << RANDOM_BALLS << " random balls.\n";
        for(int i = 0; i < m_n; i++){
            Ball x;
            x.FillRandom();
            while(isBallColliding(x))x.FillRandom();
            x.color = colors[i % colors.size()];
            x.m_id = i;
            begin_ke += x.kinEnergy();
            balls.push_back(x);
        }
    }

    void MakeCollisionTest(){
        Ball mak(Vec2(400.0, 50.0), 20.0f, 30.0f, Vec2(0.0f, 100.0f), Vec2(0.0f, 0.0f), RED, 1);
        Ball kus(Vec2(370.0, 700.0), 20.0f, 30.0f, Vec2(0.0f, 60.0f), Vec2(0.0f, 0.0f), BLUE, 2);
        begin_ke += mak.kinEnergy();
        begin_ke += kus.kinEnergy();
        balls.push_back(mak);
        balls.push_back(kus);
    }
};

int main(){
    const std::string makeki = "[MAKEKI] ";
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Balls");
    if(IsWindowReady())std::cout << MAKEKI << "Window is ready\n";
    SetTargetFPS(100);

    Simulator world(RANDOM_BALLS);
    world.MakeCollisionTest();
    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawText(TextFormat("FPS: %i\n", GetFPS()), 20, 20, 20, GREEN);
        world.AllCollisions();
        world.Run();
        ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
}
