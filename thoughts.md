15/08/2026
Our feats are:
1= [DONE] Getting a particle to show up on the screen. 
2= [DONE] Getting it to move in a line.
2.5= [DONE] Getting it to come back after reaches window's end
3= [DONE] Getting it to bounce in the window.
4= [DONE] Adding another ball.
5= [DONE]Collision detection.
6= [DONE]Use physics to calculate movement after collision.
7= [DONE]Do it for balls of diff masses.
8= [DONE]Fix the physics for tangential collisions.
 ---FUCKED UP PHYSICS ---
 What do???
 -> I don't know why its fucked up, I feel like the functions are fine.
 TODO = [DONE]refactor functions in Balls (move to Vec2)
 TODO = [DONE] use those vec2 functions
 TODO = [DONE] write the mass of each ball on the ball.
9= Getting a whole lot of errors cuz random balls can spawn on each other.

some fucking how, collided is returning true for everything


        float dist = Vec2::mag(n1);
        float overlap = (radius + b.radius) - dist;  // how deep they're sunk
        if (overlap > 0) {
          float total_mass = mass + b.mass;
          s -= (n1/dist) * overlap * (b.mass / total_mass);   // push lighter ball more
          b.s += (n2/dist) * overlap * (mass / total_mass);   // (equal mass → half each)
        }       
