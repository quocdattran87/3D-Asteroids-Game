#define GL_SILENCE_DEPRECATION

#include "Headers/asteroid.h"

vec3f vec3f_normalise_2(vec3f v)
{
    vec3f r = {0.0,0.0,0.0}; //Default
    const float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    r.x = v.x / length;
    r.y = v.y / length;
    r.z = v.z / length;
    return r;
}

vec3f vec3f_negate_2(vec3f v)
{
    vec3f r = {0.0,0.0,0.0}; //Default

    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;

    return r;
}

Asteroid::Asteroid(GLuint texture, int health, vec3f shipPosition) {
    side = rand() % 6 + 1;
    inArena = false;
    rotationX = rand() % 2;
    rotationY = rand() % 2;
    rotationZ = rand() % 2;
    Asteroid::health = health;
    radius = 5.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(15 - 5.0)));
    maxSpeed = 5.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(15.0 - 5.0)));
    // rotationSpeed_x = 0.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(8.0)));
    // rotationSpeed_y = 0.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(8.0)));
    // rotationSpeed_z = 0.0 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(8.0)));
    if (side == 1) { // Front
        position.x = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.y = -200.0 - 2*radius;
        position.z = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
    } else if (side == 2) { // Back
        position.x = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.y = 200.0 + 2*radius;
        position.z = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
    } else if (side == 3) { // Left
        position.x = -200.0 - 2*radius;
        position.y = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.z = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
    } else if (side == 4) { // Right
        position.x = 200.0 + 2*radius;
        position.y = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.z = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
    } else if (side == 5) { // Up
        position.x = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.y = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.z = -200.0 - 2*radius;
    } else if (side == 6) { // Down
        position.x = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.y = -200.0 - 2*radius + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(200 - (-200.0 - 2*radius))));
        position.z = 200.0 + 2*radius;
    }

    for (int i = 0; i < (STACKS+1)*(SLICES+1); i++) {
        randomEdges.push_back(0.9 + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(1.1 - 0.9))));
    }

    // Initialise arrays
    int counter = 0;
    for (int stack = 0; stack <= STACKS; stack++) {
        float theta = stack * M_PI / (float)STACKS; 
        for (int slice = 0; slice <= SLICES; slice++) { 
            float phi = slice * 2 * M_PI / (float)SLICES;
            // Vertex coordinates
            vec3f v;
            v.x = randomEdges[counter] * radius * sinf(theta) * cosf(phi); 
            v.z = randomEdges[counter] * radius * sinf(theta) * sinf(phi); 
            v.y = randomEdges[counter] * radius * cosf(theta); 
            vertices[stack][slice] = v;

            // Texture coordinates
            vec2f tc;
            tc.x = 1 - (float)slice / (float)SLICES; 
            tc.y = 1 - (float)stack / (float)STACKS; 
            texes[stack][slice] = tc;

            counter++;
        } 
    }


    Asteroid::texture = texture;

    aimAtPlayer(shipPosition);

    rotation.x = 0.0;
    rotation.y = 0.0;
    rotation.z = 0.0;
}

bool Asteroid::crash(vec3f otherPosition) {
    float x = position.x-otherPosition.x;
    float y = position.y-otherPosition.y;
    float z = position.z-otherPosition.z;

    return (x*x)+(y*y)+(z*z) <= radius*radius;
}

void Asteroid::reduceHealth() {
    health -= 1;
}

int Asteroid::getHealth() {
    return health;
}

void Asteroid::setHealth(int n) {
    health = n;
}

vec3f Asteroid::getPosition() {
    return position;
}

void Asteroid::rotate(float deltaTime) {
    //std::cout << "x:" << rotationSpeed_x << ", y:" << rotationSpeed_y << ", z:" << rotationSpeed_z << std::endl;
    if (rotationX == 0) {
        rotation.x += 5.0 * deltaTime;
        if (rotation.x >= 360.0) {
            rotation.x -= 360.0;
        }
    } else {
        rotation.x -= 5.0 * deltaTime;
        if (rotation.x <= 0.0) {
            rotation.x += 360.0;
        }
    }
    if (rotationY == 0) {
        rotation.y += 5.0 * deltaTime;
        if (rotation.y >= 360.0) {
            rotation.y -= 360.0;
        }
    } else {
        rotation.y -= 5.0 * deltaTime;
        if (rotation.y <= 0.0) {
            rotation.y += 360.0;
        }
    }
    if (rotationZ == 0) {
        rotation.z += 5.0 * deltaTime;
        if (rotation.z >= 360.0) {
            rotation.z -= 360.0;
        }
    } else {
        rotation.z -= 5.0 * deltaTime;
        if (rotation.z <= 0.0) {
            rotation.z += 360.0;
        }
    }
}

void Asteroid::move(float deltaTime, vec3f shipPosition) {
    position.x += direction.x * maxSpeed * deltaTime;
    position.y += direction.y * maxSpeed * deltaTime;
    position.z += direction.z * maxSpeed * deltaTime;

    if (!inArena && position.y < 200.0 - radius && position.x > -200.0 + radius && position.x < 200.0 - radius
    && position.y > -200 + radius && position.z > -200.0 + radius && position.z < 200.0 - radius) {
        inArena = true;
    }

    if (inArena && ((position.y > 200.0 - radius)||(position.y < -200.0 + radius) || (position.x > 200.0 - radius)||(position.x < -200.0 + radius) || (position.z > 200.0 - radius)||(position.z < -200.0 + radius))) {
        aimAtPlayer(shipPosition);
    }
}

void Asteroid::aimAtPlayer(vec3f shipPosition) {
    direction.x = position.x - shipPosition.x;
    direction.y = position.y - shipPosition.y;
    direction.z = position.z - shipPosition.z;
    direction = vec3f_normalise_2(direction);
    direction = vec3f_negate_2(direction);
}

void Asteroid::render() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    // setup materials

    glPushMatrix();
        if (health == 3) {
            GLfloat mat_colour[] = { 1.0, 1.0, 1.0, 1.0 };  
            glColor3fv(mat_colour);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour); 
        } else if (health == 2) {
            GLfloat mat_colour[] = { 1.0, 0.64, 0.0, 1.0 };
            glColor3fv(mat_colour);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour); 
        } else if (health == 1) {
            GLfloat mat_colour[] = { 1.0, 0.0, 0.0, 1.0 };
            glColor3fv(mat_colour);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour); 
        } else {
            GLfloat mat_colour[] = { 1.0, 1.0, 1.0, 1.0 };  
            glColor3fv(mat_colour);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour); 
        }
        GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat mat_shininess[] = {60.0};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_DEPTH_TEST);
        glTranslatef(position.x, position.y, position.z);
        glRotatef(rotation.x,1,0,0);
        glRotatef(rotation.y,0,1,0);
        glRotatef(rotation.z,0,0,1);
        glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D,texture);
        glBegin(GL_QUADS);
        for (int stack = 0; stack < STACKS; stack++) {
            for (int slice = 0; slice < SLICES; slice++) { 
                glTexCoord2fv((GLfloat*)&texes[stack][slice]); 
                glVertex3fv((GLfloat*)&vertices[stack][slice]); 
                glTexCoord2fv((GLfloat*)&texes[stack+1][slice]); 
                glVertex3fv((GLfloat*)&vertices[stack+1][slice]); 
                glTexCoord2fv((GLfloat*)&texes[stack+1][slice+1]); 
                glVertex3fv((GLfloat*)&vertices[stack+1][slice+1]); 
                glTexCoord2fv((GLfloat*)&texes[stack][slice+1]); 
                glVertex3fv((GLfloat*)&vertices[stack][slice+1]);
            } 
        }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0); 
        glDisable(GL_LIGHTING);
    glPopMatrix();
}