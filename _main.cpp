#include <math.h>
#include <iostream>
#include <vector>
#include "Headers/arena.h"
#include "Headers/spaceShip.h"
#include "Headers/skyBox.h"
#include "Headers/bullet.h"
#include "Headers/explosion.h"
#include "Headers/asteroid.h"

#define GL_SILENCE_DEPRECATION
#define GAME_TITLE "Asteroid Arena 3D"
#define KEY_ESC 27

#if _WIN32
# include <windows.h>
#endif
#if __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

Arena* arena;
SpaceShip* spaceShip;
SkyBox* skyBox;
std::vector<Bullet*> bullets;
std::vector<Explosion*> explosions;
std::vector<Asteroid*> asteroids;

GLuint asteroidTexture;
animation bulletAnimation;
animation bulletExplosionAnimation;
animation asteroidExplosionAnimation;

float dt;
float previousTime;

float cam_x = 0.0;
float cam_y = 0.0;
float cam_z = 0.0;
float horizontalTiltAnimation = 0.0;
float verticalTiltAnimation = 0.0;
float forwardAnimation = 0.0;
float backAnimation = 0.0;

bool W_IsPressed = false;
bool A_IsPressed = false;
bool S_IsPressed = false;
bool D_IsPressed = false;
bool Q_IsPressed = false;
bool E_IsPressed = false;

bool LEFT_MOUSE_IsPressed = false;
bool RIGHT_MOUSE_IsPressed = false;

bool ZOOM_IN_IsPressed = false;
bool ZOOM_OUT_IsPressed = false;
bool lookLeft = false;
bool lookRight = false;
bool lookBack = false;
float cameraSlide = 1.0;

float mouse_ini_x;
float mouse_ini_y;

int level = 0;
float gameTimer;
int minutes, seconds = 0;
bool setUp = true;
bool newGame = false;
bool oneTime = true;
bool isPlaying = false;
bool gameOver = false;
bool popped = false;

g_screen window = {
    .width = 0,
    .height = 0,
    .last_time = 0.0,
    .fov = 65.0,
    .m_near = 1.0,
    .m_far = 1000.0,
    .fullscreen = true,
    .wireframe = true};

void end_app()
{
	delete arena;
	delete spaceShip;
	delete skyBox;
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	for (int i = 0; i < explosions.size(); i++) {
		delete explosions[i];
	}
	for (int i = 0; i < asteroids.size(); i++) {
		delete asteroids[i];
	}
	std::cout << "Goodbye Asteroid 3D" << std::endl;
	exit(EXIT_SUCCESS);
}

void render_string(float x, float y, void *font, const char *string)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Set the projection - use this to render text
	gluOrtho2D(0, window.width, 0, window.height);
	glMatrixMode(GL_MODELVIEW);

	glRasterPos2f(x, y);
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}

	glMatrixMode(GL_PROJECTION);

	// Restore the original projection matrix for rendering everything else
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

void createAsteroids() {
	for (int i = 0; i < level; i++) {
		asteroids.push_back(new Asteroid(asteroidTexture, 3, spaceShip->getPosition()));
	}
}

uint32_t load_texture(const char* filename) 
{
	int width, height, components;
	unsigned char *data = stbi_load(filename, &width, &height, &components, STBI_rgb_alpha);
	if (!data)
    {
        fprintf(stderr, "failed to load image %s\nreason - %s\n", filename, stbi_failure_reason());
        exit(0);
    }
	stbi_set_flip_vertically_on_load(true);
	unsigned int id;
	glPushAttrib(GL_TEXTURE_BIT);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glPopAttrib();
	return id;
}

animation init_anim(GLuint texture, animation anim, int n)
{
	// load texture for our animation
	anim.texture = texture;

	// set animation properties
	anim.speed = 0.05;	   // switch frames every 1/2 seconds
	anim.timeAccum = 0.0; // reset time accumulation
	anim.cframe = 0;	   // start at frame zero

	int xNumber = 0;
	int yNumber = 0;

	if (n == 1) {
		// all animation frames have the same size
		// there are 5 graphics on x axis and 5 more on y axis
		// NOTE: This will be different for every texture/graphic
		xNumber = 6;
		yNumber = 6;

		anim.maxFrame = 24;
	} else if (n == 2) {
		xNumber = 5;
		yNumber = 5;

		anim.maxFrame = 25;		
	}

	float size_x = 1.0 / xNumber; // {.x = 1.0 / xNumber, .y = 1.0 / yNumber};
    float size_y = 1.0 / yNumber;

	int frame = 0;

	// setup animation frames
	for (int y = yNumber; y > 0; y--)
	{
		for (int x = 0; x < xNumber; x++)
		{
			float xPos = (float)x / xNumber;
			float yPos = ((float)y / yNumber) - (1.0 / yNumber);
            anim.frames[frame].position.x = xPos;
            anim.frames[frame].position.y = yPos;
            anim.frames[frame].size.x = size_x;
            anim.frames[frame].size.y = size_y;
			frame++;
		}
	}
   return anim;
}


void set_viewport()
{
	glViewport(0, 0, window.width*2, window.height*2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(window.fov, (float)window.width / window.height, window.m_near, window.m_far);
	glMatrixMode(GL_MODELVIEW);
}

void on_key_press(unsigned char key, int x, int y)
{
	if (key == KEY_ESC) {
		end_app();
	}
	if (key == '1' || key == '!') {
		ZOOM_IN_IsPressed = true;
	}
	if (key == '2' || key == '@') {
		ZOOM_OUT_IsPressed = true;
	}
	if (key == 'W' || key == 'w') {
		W_IsPressed = true;
	}
	if (key == 'A' || key == 'a') {
		A_IsPressed = true;
	}
	if (key == 'S' || key == 's') {
		S_IsPressed = true;
	}
	if (key == 'D' || key == 'd') {
		D_IsPressed = true;
	}
	if (key == 'Q' || key == 'q') {
		Q_IsPressed = true;
	}
	if (key == 'E' || key == 'e') {
		E_IsPressed = true;
	}
	if (key == 'Z' || key == 'z') {
		lookLeft = true;
	}
	if (key == 'C' || key == 'c') {
		lookRight = true;
	}
	if (key == 'X' || key == 'x') {
		lookBack = true;
	}
	if (key == 'f' || key == 'F') {
		spaceShip->switchDoubleGun();
	}
	if ((key == 'y' || key == 'Y') && gameOver) {
		level = 1;
		spaceShip->reset();
		spaceShip->setLives(3);
		setUp = true;
		gameOver = false;
	}
}

void on_key_release(unsigned char key, int x, int y)
{
	if (key == '1' || key == '!') {
		ZOOM_IN_IsPressed = false;
	}
	if (key == '2' || key == '@') {
		ZOOM_OUT_IsPressed = false;
	}
	if (key == 'W' || key == 'w') {
		W_IsPressed = false;
	}
	if (key == 'A' || key == 'a') {
		A_IsPressed = false;
	}
	if (key == 'S' || key == 's') {
		S_IsPressed = false;
	}
	if (key == 'D' || key == 'd') {
		D_IsPressed = false;
	}
	if (key == 'Q' || key == 'q') {
		Q_IsPressed = false;
	}
	if (key == 'E' || key == 'e') {
		E_IsPressed = false;
	}
	if (key == 'Z' || key == 'z') {
		lookLeft = false;
	}
	if (key == 'C' || key == 'c') {
		lookRight = false;
	}
	if (key == 'X' || key == 'x') {
		lookBack = false;
	}
}

void on_mouse_button(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isPlaying) {
		LEFT_MOUSE_IsPressed = true;
		if (spaceShip->isDoubleGun()) {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.5) {
				bullets.push_back(new Bullet(spaceShip->getPosition(), spaceShip->getForward(), spaceShip->getLeft(), spaceShip->getRight(), bulletAnimation, 2));
				bullets.push_back(new Bullet(spaceShip->getPosition(), spaceShip->getForward(), spaceShip->getLeft(), spaceShip->getRight(), bulletAnimation, 3));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		} else {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.34) {
				bullets.push_back(new Bullet(spaceShip->getPosition(), spaceShip->getForward(), spaceShip->getLeft(), spaceShip->getRight(), bulletAnimation, 1));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		LEFT_MOUSE_IsPressed = false;
		if (newGame) {
			createAsteroids();
			newGame = false;
			oneTime = false;
			isPlaying = true;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		mouse_ini_x = (float)x;
        mouse_ini_y = (float)y;
		RIGHT_MOUSE_IsPressed = true;
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		RIGHT_MOUSE_IsPressed = false;
	}
}

void on_camera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Normal Camera
	if (!lookLeft && !lookRight && !lookBack) {
		// Forward and Back
		if ((W_IsPressed && S_IsPressed)) {
			if (forwardAnimation > 0) {
				forwardAnimation -= 2.0 * dt;
				if (forwardAnimation < 0) {
					forwardAnimation = 0;
				}
			}
			if (forwardAnimation < 0) {
				forwardAnimation += 2.0 * dt;
				if (forwardAnimation > 0) {
					forwardAnimation = 0;
				}
			}
		} else if (W_IsPressed) {
			forwardAnimation += 7.0 * dt;
			if (forwardAnimation > 3.5) {
				forwardAnimation = 3.5;
			}
		} else if (S_IsPressed) {
			forwardAnimation -= 1.0 * dt;
			if (forwardAnimation < -1.2) {
				forwardAnimation = -1.2;
			}
		} else {
			if (forwardAnimation > 0) {
				forwardAnimation -= 2.0 * dt;
				if (forwardAnimation < 0) {
					forwardAnimation = 0;
				}
			}
			if (forwardAnimation < 0) {
				forwardAnimation += 2.0 * dt;
				if (forwardAnimation > 0) {
					forwardAnimation = 0;
				}
			}
		} 
		cam_x = spaceShip->getPosition().x+(cameraSlide*spaceShip->getCameraPosition().x*12)+(spaceShip->getUp().x*verticalTiltAnimation)+(spaceShip->getCameraPosition().x*forwardAnimation)+(spaceShip->getBack().x*backAnimation);
		cam_y = spaceShip->getPosition().y+(cameraSlide*spaceShip->getCameraPosition().y*12)+(spaceShip->getUp().y*verticalTiltAnimation)+(spaceShip->getCameraPosition().y*forwardAnimation)+(spaceShip->getBack().y*backAnimation);
		cam_z = spaceShip->getPosition().z+(cameraSlide*spaceShip->getCameraPosition().z*12)+(spaceShip->getUp().z*verticalTiltAnimation)+(spaceShip->getCameraPosition().z*forwardAnimation)+(spaceShip->getBack().z*backAnimation);
	} else if (lookLeft) {
		cam_x = spaceShip->getPosition().x+(spaceShip->getRight().x*11)+(spaceShip->getUp().x*0);
		cam_y = spaceShip->getPosition().y+(spaceShip->getRight().y*11)+(spaceShip->getUp().y*0);
		cam_z = spaceShip->getPosition().z+(spaceShip->getRight().z*11)+(spaceShip->getUp().z*0);
	} else if (lookRight) {
		cam_x = spaceShip->getPosition().x+(spaceShip->getLeft().x*11)+(spaceShip->getUp().x*0);
		cam_y = spaceShip->getPosition().y+(spaceShip->getLeft().y*11)+(spaceShip->getUp().y*0);
		cam_z = spaceShip->getPosition().z+(spaceShip->getLeft().z*11)+(spaceShip->getUp().z*0);
	} else if (lookBack) {
		cam_x = spaceShip->getPosition().x+(spaceShip->getForward().x*11)+(spaceShip->getUp().x*0);
		cam_y = spaceShip->getPosition().y+(spaceShip->getForward().y*11)+(spaceShip->getUp().y*0);
		cam_z = spaceShip->getPosition().z+(spaceShip->getForward().z*11)+(spaceShip->getUp().z*0);
	}

	gluLookAt(cam_x, cam_y, cam_z, // Camera position
	spaceShip->getPosition().x+(3*spaceShip->getUp().x), spaceShip->getPosition().y+(3*spaceShip->getUp().y), spaceShip->getPosition().z+(3*spaceShip->getUp().z),  // Look at
	spaceShip->getUp().x, spaceShip->getUp().y, spaceShip->getUp().z); // Rotate around
}

bool collision(float distance, vec3f v1, vec3f v2) {
	float x = v1.x - v2.x;
	float y = v1.y - v2.y;
	return distance * distance >= (x * x) + (y * y);
}

void check() {
	for (int j = 0; j < asteroids.size(); j++) {
		if (asteroids[j]->crash(spaceShip->getPosition())) {
			// Asteroids collide with spaceShip
			explosions.push_back(new Explosion(spaceShip->getPosition(),asteroidExplosionAnimation, 10.0));
			spaceShip->setLives(spaceShip->getLives()-1);
			isPlaying = false;
			if (spaceShip->getLives() > 0) {
				setUp = true;
			}
			return;
		}
		for (int i = 0; i < bullets.size(); i++) {
			if (asteroids[j]->crash(bullets[i]->getPosition())) {
				// Asteroid collides with bullet
				explosions.push_back(new Explosion(bullets[i]->getPosition(),bulletExplosionAnimation, 5.0));
				bullets.erase(bullets.begin() + i);
				asteroids[j]->reduceHealth();
				if (asteroids[j]->getHealth() == 0) {
					explosions.push_back(new Explosion(asteroids[j]->getPosition(),asteroidExplosionAnimation, 34.0));
					asteroids.erase(asteroids.begin() + j);
				}
			}
		}	
	}
}


void on_render()
{
	if (gameOver) {
		glColor3f(1.0, 1.0, 1.0);
		std::string temp3 = std::to_string(level).c_str();
		char const *level_as_char = temp3.c_str();
		render_string(window.width/2, window.height/2, GLUT_BITMAP_TIMES_ROMAN_24, "You Reached Level ");
		render_string(window.width/2+100, window.height/2, GLUT_BITMAP_TIMES_ROMAN_24, level_as_char);
		render_string(window.width/2, window.height/2 - 20, GLUT_BITMAP_TIMES_ROMAN_24, "Play Again?");
		render_string(window.width/2, window.height/2 - 40, GLUT_BITMAP_TIMES_ROMAN_24, "Y - New Game");
		render_string(window.width/2, window.height/2 - 60, GLUT_BITMAP_TIMES_ROMAN_24, "Esc - Quit");
	}
	if (setUp) {
		if (asteroids.size() > 0) {
			asteroids.erase(asteroids.begin());
		}
		if (bullets.size() > 0) {
			bullets.erase(bullets.begin());
		}
		if (asteroids.size() == 0 && bullets.size() == 0) {
			spaceShip->reset();
			setUp = false;
			newGame = true;
			gameTimer = glutGet(GLUT_ELAPSED_TIME) / 1000;
			minutes = 0;
			seconds = 0;
		}
	}
	if (newGame) {
		if (oneTime) {
			glColor3f(1.0, 1.0, 1.0);
			render_string(window.width/2-50, window.height/2 - 20, GLUT_BITMAP_TIMES_ROMAN_24, "ASTEROID ARENA 3D!");
			render_string(window.width/2-50, window.height/2 - 40, GLUT_BITMAP_TIMES_ROMAN_24, "left click to start..");
		} else {
			createAsteroids();
			newGame = false;
			isPlaying = true;
		}
	}
	if (isPlaying) {
		check();
		glPushMatrix();
			skyBox->render(spaceShip->getPosition().x+(spaceShip->getCameraPosition().x*10), spaceShip->getPosition().y+(spaceShip->getCameraPosition().y*10), spaceShip->getPosition().z+((spaceShip->getCameraPosition().z)*10));
		glPopMatrix();
		glPushMatrix();
			arena->render(spaceShip->getPosition().x, spaceShip->getPosition().y, spaceShip->getPosition().z);
		glPopMatrix();
		glPushMatrix();
			for (int i = 0; i < asteroids.size(); i++) {
				asteroids[i]->render();
				asteroids[i]->rotate(dt);
				asteroids[i]->move(dt, spaceShip->getPosition());
			}
		glPopMatrix();
		glPushMatrix();
			if (spaceShip->maxDistanceTravelled()) {
				explosions.push_back(new Explosion(spaceShip->getPosition(),asteroidExplosionAnimation, 10.0));
				spaceShip->reset();
			} else {
				spaceShip->render(W_IsPressed, A_IsPressed, D_IsPressed, LEFT_MOUSE_IsPressed, dt);
			}
		glPopMatrix();
		glPushMatrix();
			for (int i = 0; i < bullets.size(); i++) {
				bullets[i]->render(dt);
				bullets[i]->move(dt);
				if (bullets[i]->maxDistanceTravelled()) {
					bullets.erase(bullets.begin() + i);
				}
			}
		glPopMatrix();
		glPushMatrix();
			for (int i = 0; i < explosions.size(); i++) {
				explosions[i]->render(dt);
				if (explosions[i]->animationIsFinished()) {
					explosions.erase(explosions.begin() + i);
				}
			}
		glPopMatrix();
	}
}

void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (isPlaying) {
		on_camera();
	} else {
		// Remove camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	set_viewport();
	on_render();

	int err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("error: %s\n", gluErrorString(err));

	glutSwapBuffers();
}

void update_game_state()
{
	if (spaceShip->getLives() <= 0) {
		isPlaying = false;
		gameOver = true;
	}
	if (asteroids.size() <= 0 && isPlaying) {
		isPlaying = false;
		setUp = true;
		level += 1;
	}
}

void on_idle()
{
	float cur_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	dt = cur_time - window.last_time;
	update_game_state();
	window.last_time = cur_time;
	glutPostRedisplay();

	if (ZOOM_IN_IsPressed && !ZOOM_OUT_IsPressed) {
		cameraSlide -= 0.34*dt;
		if (cameraSlide <= 0.72) {
			cameraSlide = 0.72;
		}
	}
	if (ZOOM_OUT_IsPressed && !ZOOM_IN_IsPressed) {
		cameraSlide += 0.34*dt;
		if (cameraSlide >= 1.28) {
			cameraSlide = 1.28;
		}
	}
	if (Q_IsPressed) {
		spaceShip->rotateCounterClockWise(dt);
	}
	if (A_IsPressed) {
		spaceShip->strafeLeft(dt);
	}
	if (E_IsPressed) {
		spaceShip->rotateClockWise(dt);
	}
	if (D_IsPressed) {
		spaceShip->strafeRight(dt);
	}
	if (W_IsPressed) {
		spaceShip->moveForward(dt);
	}
	if (S_IsPressed) {
		spaceShip->moveBack(dt);
	}
	if (LEFT_MOUSE_IsPressed) {
		if (spaceShip->isDoubleGun()) {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.5) {
				bullets.push_back(new Bullet(spaceShip->getPosition(), spaceShip->getForward(), spaceShip->getLeft(), spaceShip->getRight(), bulletAnimation, 2));
				bullets.push_back(new Bullet(spaceShip->getPosition(), spaceShip->getForward(), spaceShip->getLeft(), spaceShip->getRight(), bulletAnimation, 3));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		} else {
			if ((glutGet(GLUT_ELAPSED_TIME) / 1000.0) - previousTime  > 0.34) {
				bullets.push_back(new Bullet(spaceShip->getPosition(), spaceShip->getForward(), spaceShip->getLeft(), spaceShip->getRight(), bulletAnimation, 1));
				previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
		}
	}
}

void on_reshape(int w, int h) {
	window.width = w;
	window.height = h;
}

void on_mouse_drag(int x, int y) 
{
	if (RIGHT_MOUSE_IsPressed) {
		// https://stackoverflow.com/questions/13294842/detecting-horizontal-mouse-motion-with-glut-opengl
		if (x - (int)mouse_ini_x > 0) {
			mouse_ini_x = (float) x;
			spaceShip->rotateRight(dt);
		} else if (x - (int)mouse_ini_x < 0) {
			mouse_ini_x = (float) x;
			spaceShip->rotateLeft(dt);
		}
		if (y - (int)mouse_ini_y > 0) {
			mouse_ini_y = (float) y;
			spaceShip->rotateDown(dt);
		} else if (y - (int)mouse_ini_y < 0) {
			mouse_ini_y = (float) y;
			spaceShip->rotateUp(dt);
		}
	}
}

void light_init()
{
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	GLfloat light_direction[] = {0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_position);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, light_direction);
}

void init_app(int *argcp, char **argv, g_screen *mainwinp)
{
	glutInit(argcp, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// glutSetCursor(GLUT_CURSOR_NONE);

	// Display related setup
	glutInitWindowPosition(mainwinp->x_pos, mainwinp->y_pos);
	glutInitWindowSize(mainwinp->width, mainwinp->height);
	glutCreateWindow(GAME_TITLE);
	if (mainwinp->fullscreen == true) {
		glutFullScreen();
	}

	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(on_key_press);
	glutKeyboardUpFunc(on_key_release);
	glutMouseFunc(on_mouse_button);
	glutMotionFunc(on_mouse_drag);

	glutIdleFunc(on_idle);

	window.last_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

int main(int argc, char **argv)
{
	std::cout << "Hello Asteroid 3D" << std::endl;
	init_app(&argc, argv, &window);
	light_init();
	srand((int) time(0));
	arena = new Arena();
	spaceShip = new SpaceShip(load_texture("Textures/ship.png"));
	// skybox from https://opengameart.org/content/space-skyboxes-0
	skyBox = new SkyBox(load_texture("Textures/skybox/front.png"),load_texture("Textures/skybox/back.png"),
						load_texture("Textures/skybox/left.png"),load_texture("Textures/skybox/right.png"),
						load_texture("Textures/skybox/up.png"),load_texture("Textures/skybox/down.png"));
	asteroidTexture = load_texture("Textures/asteroid.jpg");
	asteroids.push_back(new Asteroid(asteroidTexture, 3, spaceShip->getPosition()));
	// fireball_ani.png from https://www.pngwing.com/
	bulletAnimation = init_anim(load_texture("Textures/animation/fireball_ani.png"), bulletAnimation, 1);
	// purchased from https://www.artstation.com/marketplace/p/pk3G/2d-explosions-animations-pack
	bulletExplosionAnimation = init_anim(load_texture("Textures/animation/explosion-11.PNG"), bulletExplosionAnimation, 2);
	asteroidExplosionAnimation = init_anim(load_texture("Textures/animation/explosion-20.PNG"), bulletExplosionAnimation, 2);
 	glutMainLoop();
	return (EXIT_SUCCESS);
}