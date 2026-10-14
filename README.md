“Premature optimization is the root of all evil”- Knuth  

<img width="1151" height="1036" alt="image" src="https://github.com/user-attachments/assets/f2fca6d5-db29-41f4-a85b-f713c8ca3cac" />

https://www.youtube.com/playlist?list=PLC0Nqg1Z3iVs

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
9= [67 maybe] Fix the overlap problem  
    - overlap of balls, maybe substepping is the fix.  
    ways to do it:  
    - I pass a dt object as function argument to Run, which then passes it to Move()  
[DONE] Substepping fixes the issue visually. Somehow the kinetic energy of the entire system is increasing.  

Things break on 1000x1000 with 600 balls. The physics looks weird af as if brownian motion is happening.  


SOMETHING IS REALLY MESSED UP, brownian motion is back.   
Guess is that theres GHOST balls, we are unable to draw them on time.  

10= [along] Learn how profilers work to see the hotspots and fix the hotspots.  
10.5 = Look into SPA and implement them.  
11= Learn about quadtrees and implement them.  
    - before quadtrees I can try to split the whole canvas into four zones, and run the collision check only on those zones.  
10= Write the comments. Refactor loop.  
    - [done] make size proportional to mass.  
    - [bug/skip] change color balls they collide  
        - current implementation would not show the intended coloring cuz the loop is hella fast.  
        - can't add delay or costly Time functions.  
        - slowing down don't help.  
11= Look at how to scale this to MANY BALLS  


TIME COMPLEXITY. (n = number of balls)  
All Vec2 functions are O(1).  
All Ball methods are O(1).  
Simulator:  
    - AllCollisions() is O(n^2).  
    - isBallColliding() is O(n).  
    - Run() is O(rounds*n^2).  
    - GenRand() is O(n).  

Hotspots:  
- the balls.size() seems to be slowing things down a bit.  

- so is the dist() which i believe is due to sqrt() being costly.
