#include "Headers/spaceShip.h"
# include <OpenGL/glu.h>
# include <GLUT/glut.h>

SpaceShip::SpaceShip(GLuint texture) {
    lives = 3;
    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;
    forward.x = 0.0;
    forward.y = -1.0;
    forward.z = 0.0;
    up.x = 0.0;
    up.y = 0.0;
    up.z = -1.0;
    right.x = 1.0;
    right.y = 0.0;
    right.z = 0.0;
    cameraPosition.y = 0.8;
    cameraPosition.z = -0.3;
    radius = 5.0;
    previousTime = 0.0;

    horizontalTiltAnimation = 0.0;

    isAlive = true;
    doubleGun = false;
    isRecoiling = false;
    isReturning = false;
    recoil = 0.0;

    maxSpeed = 15;
    turnLeftAndRightSpeed = 18.0;
    turnUpAndDownSpeed = 18.0;
    SpaceShip::texture = texture;
    parseObjectFile();

    leftEngineParticleEffect = new ParticleEffect();
    rightEngineParticleEffect = new ParticleEffect();
    leftEngineParticleEffect->createRandomEffect();
    rightEngineParticleEffect->createRandomEffect();
}

void SpaceShip::reset() {
    isAlive = false;
    // previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    // if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 3.34) {
        // Timer before reset
        position.x = 0.0;
        position.y = 0.0;
        position.z = 0.0;
        rotation.x = 0.0;
        rotation.y = 0.0;
        rotation.z = 0.0;
        isAlive = true;
	// }
}


/**
 * Rotate a point provided a transform
 * This effectively rotates a point via a 3x3 rotation matrix
 * Code derived and modified from THREE.js Vector3 and Matrix3 classes
 * See https://github.com/mrdoob/three.js
 *
 * NOTE: There are a few easier ways to do this including Chaining simple
 * 3x3 matrix multiplications for x, y and z axis from transform.rotation OR
 * using Quaternions (not covered in course)
 */
vec3f transform_rotate_point(vec3f p, vec3f rotation)
{
    vec3f r = {0.0,0.0,0.0}; //Default

    // make a rotation matrix from euler angles
    // see https://github.com/mrdoob/three.js/blob/master/src/math/Matrix4.js#L159

    const float x = rotation.x * DEG_2_RAD;
    const float y = rotation.y * DEG_2_RAD;
    const float z = rotation.z * DEG_2_RAD;

    const float a = cos(x);
    const float b = sin(x);
    const float c = cos(y);
    const float d = sin(y);
    const float e = cos(z);
    const float f = sin(z);

    // using XYZ order as if X * Y * Z * vec
    const float ae = a * e;
    const float af = a * f;
    const float be = b * e;
    const float bf = b * f;

    // setting the 3x3 matrix
    const float e0 = c * e;
    const float e4 = -c * f;
    const float e8 = d;

    const float e1 = af + be * d;
    const float e5 = ae - bf * d;
    const float e9 = -b * c;

    const float e2 = bf - ae * d;
    const float e6 = be + af * d;
    const float e10 = a * c;

    // multiply matrix with vector
    // see https://github.com/mrdoob/three.js/blob/dev/src/math/Vector3.js#L262
    r.x = e0 * p.x + e4 * p.y + e8 * p.z;
    r.y = e1 * p.x + e5 * p.y + e9 * p.z;
    r.z = e2 * p.x + e6 * p.y + e10 * p.z;

    return r;
}

/**
 * Converts the provided vector to a unit vector. Unit vectors have a length of 1
 */
vec3f vec3f_normalise(vec3f v)
{
    vec3f r = {0.0,0.0,0.0}; //Default

    // NOTE: This could cause a math error of length is 0
    // and could happen if a 0,0,0 is normalised
    // for students to solve! (if needed)
    const float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    r.x = v.x / length;
    r.y = v.y / length;
    r.z = v.z / length;

    return r;
}

vec3f vec3f_negate(vec3f v)
{
    vec3f r = {0.0,0.0,0.0}; //Default

    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;

    return r;
}

/**
 * Cross Product of 2 Vectors
 * See https://github.com/mrdoob/three.js/blob/dev/src/math/Vector3.js#L514
 */
vec3f vec3f_cross(vec3f a, vec3f b)
{
    vec3f r = {0.0,0.0,0.0}; //Default

    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;

    return r;
}


/**
 * Calculate the normalised forward vector (-z) from provided transform
 */
vec3f SpaceShip::getForward()
{
    vec3f tForward = transform_rotate_point(forward, rotation);

    return vec3f_normalise(tForward);
}

vec3f SpaceShip::getBack()
{
    return vec3f_negate(getForward());
}

vec3f SpaceShip::getCameraPosition()
{
    vec3f tCameraPosition = transform_rotate_point(cameraPosition, rotation);

    return vec3f_normalise(tCameraPosition);
}

/**
 * Calculate the normalised right vector (+x) from provided transform
 */
vec3f SpaceShip::getRight()
{
    vec3f tRight = transform_rotate_point(right, rotation);

    return vec3f_normalise(tRight);
}

vec3f SpaceShip::getLeft()
{
    return vec3f_negate(getRight());
}

/**
 * Calculate the normalised up vector (+y) from provided transform
 */
vec3f SpaceShip::getUp()
{
    // NOTE: this can also be calculated as up = forward x right where x = cross product
    vec3f tUp = transform_rotate_point(up, rotation);

    return vec3f_normalise(tUp);
}

vec3f SpaceShip::getDown()
{
    return vec3f_negate(getUp());
}

vec3f SpaceShip::getLeftEngine(vec3f back, vec3f left, vec3f down)
{
    leftEnginePosition.x = position.x + back.x + left.x*0.56 + down.x*0.05;
    leftEnginePosition.y = position.y + back.y + left.y*0.56 + down.y*0.05;
    leftEnginePosition.z = position.z + back.z + left.z*0.56 + down.z*0.05;

    return leftEnginePosition;
}

vec3f SpaceShip::getRightEngine(vec3f back, vec3f right, vec3f down)
{
    rightEnginePosition.x = position.x + back.x + right.x*0.53 + down.x*0.05;
    rightEnginePosition.y = position.y + back.y + right.y*0.53 + down.y*0.05;
    rightEnginePosition.z = position.z + back.z + right.z*0.53 + down.z*0.05;

    return rightEnginePosition;
}

std::vector<std::string> splitToTokens(std::string string, char delimiter) {
    //Reference: https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
    std::vector <std::string> tokens;
    std::stringstream ss(string);
    std::string intermediate;
        
    while(getline(ss, intermediate, delimiter))
        tokens.push_back(intermediate);
        
    return tokens;
}

void SpaceShip::parseObjectFile() {
	std::ifstream in;
	in.open(OBJECT_PATH);
	std::string nextLine;

	std::vector<std::string> tokens;
    std::vector<std::string> tokens2;
    int vertexCount = 0;
    int textureCount = 0;
    int normalCount = 0;
    int faceCount = 0;
	while (getline(in, nextLine)) {
        if (nextLine[0] == 'v' && nextLine[1] == ' ') {
            // Vertices
            tokens = splitToTokens(nextLine, ' ');
            for (int i = 0; i < tokens.size(); i++) {
                vertices[vertexCount].x = std::stof(tokens[1]);
                vertices[vertexCount].y = std::stof(tokens[2]);
                vertices[vertexCount].z = std::stof(tokens[3]);
            }
            vertexCount++;
        }
        if (nextLine[0] == 'v' && nextLine[1] == 't') {
            // Texture coords
            tokens = splitToTokens(nextLine, ' ');
            for (int i = 0; i < tokens.size(); i++) {
                textureCoords[textureCount].x = std::stof(tokens[1]);
                textureCoords[textureCount].y = std::stof(tokens[2]);
            }
            textureCount++;
        }
        if (nextLine[0] == 'v' && nextLine[1] == 'n') {
            // Normals
            tokens = splitToTokens(nextLine, ' ');
            for (int i = 0; i < tokens.size(); i++) {
                normals[normalCount].x = std::stof(tokens[1]);
                normals[normalCount].y = std::stof(tokens[2]);
                normals[normalCount].z = std::stof(tokens[3]);
            }
            normalCount++;
        }
        if (nextLine[0] == 'f' && nextLine[1] == ' ') {
            // Faces
            tokens = splitToTokens(nextLine, ' ');
            int facesIndex = 0;
            for (int i = 0; i < tokens.size(); i++) {
                tokens2 = splitToTokens(tokens[i], '/');
                if (tokens2[0] == "f") {
                    facesIndex = 0;
                }
                else if (tokens2[0] != "f") {
                    if (facesIndex == 0) {
                        faces[faceCount].x.x = stoi(tokens2[0]);
                        faces[faceCount].x.y = stoi(tokens2[1]);
                        faces[faceCount].x.z = stoi(tokens2[2]);
                    }
                    else if (facesIndex == 1) {
                        faces[faceCount].y.x = stoi(tokens2[0]);
                        faces[faceCount].y.y = stoi(tokens2[1]);
                        faces[faceCount].y.z = stoi(tokens2[2]);
                    }
                    else if (facesIndex == 2) {
                        faces[faceCount].z.x = stoi(tokens2[0]);
                        faces[faceCount].z.y = stoi(tokens2[1]);
                        faces[faceCount].z.z = stoi(tokens2[2]);
                    }
                    facesIndex++;
                }       
            }
            faceCount++;
        }
	}
	in.close();
}



void SpaceShip::moveForward(float deltaTime) {
    if (isAlive) {
        position.x += getForward().x * maxSpeed * deltaTime;
        position.y += getForward().y * maxSpeed * deltaTime;
        position.z += getForward().z * maxSpeed * deltaTime;
    }
}



void SpaceShip::moveBack(float deltaTime) {
    if (isAlive) {
        position.x -= getForward().x * maxSpeed * deltaTime;
        position.y -= getForward().y * maxSpeed * deltaTime;
        position.z -= getForward().z * maxSpeed * deltaTime;
    }
}

void SpaceShip::strafeLeft(float deltaTime) {
    if (isAlive) {
        position.x += getLeft().x * maxSpeed * deltaTime;
        position.y += getLeft().y * maxSpeed * deltaTime;
        position.z += getLeft().z * maxSpeed * deltaTime;
    }
}

void SpaceShip::strafeRight(float deltaTime) {
    if (isAlive) {
        position.x += getRight().x * maxSpeed * deltaTime;
        position.y += getRight().y * maxSpeed * deltaTime;
        position.z += getRight().z * maxSpeed * deltaTime;
    }
}

void SpaceShip::rotateUp(float deltaTime) {
    if (isAlive) {
        rotation.x += turnUpAndDownSpeed * deltaTime;
        if (rotation.x >= 360.0) {
            rotation.x -= 360.0;
        }
    }
}

void SpaceShip::rotateDown(float deltaTime) {
    if (isAlive) {
        rotation.x -= turnUpAndDownSpeed * deltaTime;
        if (rotation.x <= 0) {
            rotation.x += 360.0;
        }
    }
}

void SpaceShip::rotateLeft(float deltaTime) {
    if (isAlive) {
        rotation.z -= turnLeftAndRightSpeed * deltaTime;
        if (rotation.z <= 0) {
            rotation.z += 360.0;
        }
    }
}

void SpaceShip::rotateRight(float deltaTime) {
    if (isAlive) {
        rotation.z += turnLeftAndRightSpeed * deltaTime;
        if (rotation.z >= 360.0) {
            rotation.z -= 360.0;
        }
    }
}

void SpaceShip::rotateClockWise(float deltaTime) {
    if (isAlive) {
        rotation.y += 40 * deltaTime;
        if (rotation.y > 360) {
            rotation.y -= 360.0;
        }
    }
}

void SpaceShip::rotateCounterClockWise(float deltaTime) {
    if (isAlive) {
        rotation.y -= 40 * deltaTime;
        if (rotation.y < 0.0) {
            rotation.y += 360.0;
        }
    }
}

vec3f SpaceShip::getPosition() {
    return position;
}

vec3f SpaceShip::getRotation() {
    return rotation;
}

bool SpaceShip::maxDistanceTravelled() {
    if (position.x > MAX_DISTANCE || position.y > MAX_DISTANCE || position.z > MAX_DISTANCE || position.x < -MAX_DISTANCE || position.y < -MAX_DISTANCE || position.z < -MAX_DISTANCE) {
        return true;
    }
    return false;
}

vec3f SpaceShip::addEngineFlames(float deltaTime, int n) {
    vec3f strafeRotation = {0.0,0.0,0.0};
    strafeRotation.x = rotation.x;
    strafeRotation.y = rotation.y+horizontalTiltAnimation;
    strafeRotation.z = rotation.z;

    //left
    vec3f tRight = vec3f_normalise(transform_rotate_point(right, strafeRotation));
    vec3f tLeft = vec3f_negate(tRight);
    //back
    vec3f tForward = vec3f_normalise(transform_rotate_point(forward, strafeRotation));
    vec3f tBack = vec3f_negate(tForward);
    //down
    vec3f tUp = vec3f_normalise(transform_rotate_point(up, rotation));
    vec3f tDown = vec3f_negate(tUp);
    if (n == 1) {
        return getRightEngine(tBack, tRight, tDown);
    } else if (n == 2) {
        return getLeftEngine(tBack, tLeft, tDown);
    }
    return getRightEngine(tBack, tRight, tDown);
}

void SpaceShip::switchDoubleGun() {
    doubleGun = !doubleGun;
}
bool SpaceShip::isDoubleGun() {
    return doubleGun;
}

int SpaceShip::getLives() {
    return lives;
}

void SpaceShip::setLives(int lives) {
    SpaceShip::lives = lives;
}

void SpaceShip::render(bool moveForwardIsPressed, bool leftStrafeIsPressed, bool rightStrafeIsPressed, bool shootIsPressed, float deltaTime) {
    leftEngineParticleEffect->updateEffect(deltaTime);
    rightEngineParticleEffect->updateEffect(deltaTime);

    // Y rotation (Barrel Roll)
    if (leftStrafeIsPressed && rightStrafeIsPressed) {
        if (horizontalTiltAnimation > 0) {
            horizontalTiltAnimation -= 50.0*deltaTime;
            if (horizontalTiltAnimation < 0) {
                horizontalTiltAnimation = 0;
            }
        }
        if (horizontalTiltAnimation < 0) {
            horizontalTiltAnimation += 50.0*deltaTime;
            if (horizontalTiltAnimation > 0) {
                horizontalTiltAnimation = 0;
            }
        }
    } else if (leftStrafeIsPressed) {
        horizontalTiltAnimation += 50.0*deltaTime;
        if (horizontalTiltAnimation > 20.0) {
            horizontalTiltAnimation = 20.0;
        }
    } else if (rightStrafeIsPressed) {
        horizontalTiltAnimation -= 50.0*deltaTime;
        if (horizontalTiltAnimation < -20.0) {
            horizontalTiltAnimation = -20.0;
        }
    } else { 
        if (horizontalTiltAnimation > 0) {
            horizontalTiltAnimation -= 50.0*deltaTime;
            if (horizontalTiltAnimation < 0) {
                horizontalTiltAnimation = 0;
            }
        }
        if (horizontalTiltAnimation < 0) {
            horizontalTiltAnimation += 50.0*deltaTime;
            if (horizontalTiltAnimation > 0) {
                horizontalTiltAnimation = 0;
            }
        }
    }

    glPushMatrix();
        vec3f strafeRotation = {0.0,0.0,0.0};
        strafeRotation.x = rotation.x;
        strafeRotation.y = rotation.y+horizontalTiltAnimation;
        strafeRotation.z = rotation.z;
        if (moveForwardIsPressed) {
            leftEngineParticleEffect->render(addEngineFlames(deltaTime, 1), strafeRotation, moveForwardIsPressed, leftStrafeIsPressed, rightStrafeIsPressed, deltaTime);
            rightEngineParticleEffect->render(addEngineFlames(deltaTime, 2), strafeRotation, moveForwardIsPressed, leftStrafeIsPressed, rightStrafeIsPressed, deltaTime);
        } else if (leftStrafeIsPressed && !rightStrafeIsPressed) {
            leftEngineParticleEffect->render(addEngineFlames(deltaTime, 1), strafeRotation, moveForwardIsPressed, leftStrafeIsPressed, rightStrafeIsPressed, deltaTime);
        } else if (rightStrafeIsPressed && ! leftStrafeIsPressed) {
            rightEngineParticleEffect->render(addEngineFlames(deltaTime, 2), strafeRotation, moveForwardIsPressed, leftStrafeIsPressed, rightStrafeIsPressed, deltaTime);
        }
        leftEngineParticleEffect->render2(addEngineFlames(deltaTime, 1), strafeRotation, deltaTime);
        rightEngineParticleEffect->render2(addEngineFlames(deltaTime, 2), strafeRotation, deltaTime);
        
    glPopMatrix();
    glLineWidth(1);
            // glEnable(GL_LIGHTING);
            // glEnable(GL_LIGHT0);
            // glEnable(GL_LIGHT1);
            // glEnable(GL_DEPTH_TEST);
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);

        // X rotation (Up and Down)
        //glRotatef(rotation.x,getRight().x,getRight().y,getRight().z);
        glRotatef(rotation.x,1,0,0);
        //glRotatef(rotation.y+horizontalTiltAnimation,getBack().x,getBack().y,getBack().z);
        glRotatef(rotation.y+horizontalTiltAnimation,0,1,0);
        // Z rotation (Left and Right)
        //glRotatef(rotation.z,getDown().x,getDown().y,getDown().z);
        glRotatef(rotation.z,0,0,1);

        glDisable(GL_BLEND);
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < sizeof(faces)/sizeof(*faces); i++) {
            glNormal3f(normals[faces[i].x.z-1].x, normals[faces[i].x.z-1].y, normals[faces[i].x.z-1].z);
            glTexCoord2f(textureCoords[faces[i].x.y-1].x, textureCoords[faces[i].x.y-1].y);
            glVertex3f(vertices[faces[i].x.x-1].x, vertices[faces[i].x.x-1].y, vertices[faces[i].x.x-1].z);

            glNormal3f(normals[faces[i].y.z-1].x, normals[faces[i].y.z-1].y, normals[faces[i].y.z-1].z);
            glTexCoord2f(textureCoords[faces[i].y.y-1].x, textureCoords[faces[i].y.y-1].y);
            glVertex3f(vertices[faces[i].y.x-1].x, vertices[faces[i].y.x-1].y, vertices[faces[i].y.x-1].z);

            glNormal3f(normals[faces[i].z.z-1].x, normals[faces[i].z.z-1].y, normals[faces[i].z.z-1].z);
            glTexCoord2f(textureCoords[faces[i].z.y-1].x, textureCoords[faces[i].z.y-1].y);
            glVertex3f(vertices[faces[i].z.x-1].x, vertices[faces[i].z.x-1].y, vertices[faces[i].z.x-1].z);
        }
        glEnd();
        if (doubleGun) {
            GLfloat mat_colour[] = { 0.8, 0.8, 0.8, 1.0 };      
            GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat mat_shininess[] = {60.0};

            // glClearColor(0.0, 0.0, 0.0, 0.0);
            glShadeModel(GL_SMOOTH);
            // setup materials
            glColor3fv(mat_colour); 
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_colour); 
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            glEnable(GL_DEPTH_TEST);


            if (shootIsPressed && !isRecoiling && !isReturning && ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.49)) {
                isRecoiling = true;
                previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
            }
            if (isRecoiling) {
                recoil += deltaTime;
                if (recoil >= 0.3) {
                    isRecoiling = false;
                    isReturning = true;
                }
            } else if (isReturning) {
                recoil -= deltaTime;
                if (recoil <= 0.0) {
                    recoil = 0.0;
                    isReturning = false;
                }
            }

            // position and draw
            glTranslatef(1.5, 0.3+recoil, 0.1); //
            glPushMatrix();
                glScalef(0.4, 1.7, 0.4); //w,forward, h
                glutSolidCube(1.0);
            glPopMatrix();
            glTranslatef(-3.0, 0.0, 0.0); //
            glPushMatrix();
                glScalef(0.4, 1.7, 0.4); //w,forward, h
                glutSolidCube(1.0);
            glPopMatrix();
            glDisable(GL_LIGHTING);
        }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        // render axis using world-coordinates
        // x axis - red
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(getRight().x * 5.0, getRight().y * 5.0, getRight().z * 5.0);
        glEnd();

        // y axis - blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(getUp().x * 5.0, getUp().y * 5.0, getUp().z * 5.0);
        glEnd();

        // z axis - green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(getForward().x * 5.0, getForward().y * 5.0, getForward().z * 5.0);
        glEnd();
        glColor3f(1,1,1);
    glPopMatrix();
    // glDisable(GL_LIGHTING);
}