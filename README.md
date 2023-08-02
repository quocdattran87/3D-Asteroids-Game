## Third Year University Assignment
By Quoc Tran (s3827826)

### How to build:
    Install C/C++ Compiler and OpenGL Libraries
    Run the command 'bash build.sh'

### Controls:
    W - Move forward
    A - Strafe left
    D - Strafe Right
    S - Move back
    Q - Left barrel roll
    E - Right barrel roll
    F - Change guns
    Z - Look left
    X - Look back
    C - Look right
    1 - Zoom camera in
    2 - Zoom camera out
    
    Left Click - Shoot
    Right Click - Aim/Look up and down/Rotate left and right (Read note in CAMERA & SHIP MOVEMENT)



## Features Implemented:
### ARENA
        EASY
        MEDIUM
        HARD - Added skybox but did not implement the bounce back when ship collides into it. The ship still crashes and respawns in the middle.

### SPACESHIP & GUN MODEL
        EASY
        MEDIUM
        HARD - Change to secondary gun to see the weapon barrels appear on wings. Shooting will cause the barrels to recoil back.

### ASTEROID MODEL
        EASY
        MEDIUM - Used the code from week 9 lecture
        HARD - Created a vector with random lengths between 0.8-1.2 of the radius for each STACK*SLICE

### ASTEROID MOVEMENT
        EASY - Each level will add an extra asteroid. They spawn from a random side within a random area from outside of the grid.
        MEDIUM - Each time asteroid bounces, it will aim for the player. Couldn't work out how to bounce naturally.

### LIGHTING
        EASY
        MEDIUM - Only asteroids and ships secondary gun have specular. 
                 It made the ship too dark and I couldn't figure out how to brighten the light in time. 
                 When the asteroid is too dark, you cannot see the colour change when it is damage. 
                 It changes from regular, yellow, red, when it is lit up.

### BULLETS & SHOOTING
        EASY
        MEDIUM
        HARD - Used the bullet animation that was given as an asset. 
               Found a billboarding code from online that I referenced in the code.

### EXPLOSIONS
        MEDIUM
        HARD - Used an explosion animation that was billboarded. The animation has transparency effects on it. 
               I used a particle engine for the ship's engine effects.

### CAMERA & SHIP MOVEMENT 
        EASY
        MEDIUM
        HARD - Controls are listed as above. I couldn't fully figure out the rotational camera movements. 
               As you turn left and right, rotate up and down gradually turns into left and right barrel rolls 
               while the left and right barrel rolls gradually turn into rotate up and rotate down. 
               When the ship moves forward it will pull ahead of the camera, when the ship moves back it will back into the camera. 
               When not moving the camera will move back into the regular position. 
               When you turn left or right 180 degrees, up and down becomes inverted..


## Features Not Implemented:
### ASTEROID MOVEMENT
        HARD
### LIGHTING
        HARD
