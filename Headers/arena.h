#ifndef ARENA_CLASS_H
#define ARENA_CLASS_H

#define BORDER_SIZE 200.0
#define NUMBER_OF_GRIDS 10

class Arena {
    public:
        Arena();

        void render(float ship_x_position, float ship_y_position, float ship_z_position);

    private:
        float meshSpace;

        void drawFrontPlane(float ship_z_position);
        void drawLeftPlane(float ship_y_position);
        void drawRightPlane(float ship_y_position);
        void drawBackPlane(float ship_z_position);
        void drawBottomPlane(float ship_x_position);
        void drawTopPlane(float ship_x_position);
};
#endif