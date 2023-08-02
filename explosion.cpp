#define GL_SILENCE_DEPRECATION
#include "Headers/explosion.h"

Explosion::Explosion(vec3f bulletDestroyPosition, animation anim, float scale) {
    position = bulletDestroyPosition;
    Explosion::scale.x = scale;
    Explosion::scale.y = scale;
    Explosion::scale.z = scale;
    Explosion::anim = anim;

    animationFinished = false;
}

bool Explosion::animationIsFinished() {
    return animationFinished;
}

void Explosion::billboardCheatSphericalBegin() {
	// http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
	    for( j=0; j<3; j++ ) {
		if ( i==j )
		    modelview[i*4+j] = 1.0;
		else
		    modelview[i*4+j] = 0.0;
	    }

	// set the modelview with no rotations
	glLoadMatrixf(modelview);
}

void Explosion::billboardEnd() {
    // http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat
	// restore the previously 
	// stored modelview matrix
	glPopMatrix();
}

void Explosion::render(float deltaTime) {
    glColor3f(1,1,1);
	glPushMatrix();
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.1f);

    	glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, anim.texture);
        glTranslatef(position.x, position.y, position.z);

        // get our current animation frame
        animation_frame frame = anim.frames[anim.cframe];

        // calculate UV coordinates for each vertex
        const float uv_start_x = frame.position.x;
        const float uv_start_y = frame.position.y;
        const float uv_width = frame.size.x;
        const float uv_height = frame.size.y;
        billboardCheatSphericalBegin();
            glScalef(scale.x, scale.y, scale.z);
            glBegin(GL_POLYGON);
            glTexCoord2f(uv_start_x, uv_start_y);
            glVertex2f(-1, -1);
            glTexCoord2f(uv_start_x + uv_width, uv_start_y);
            glVertex2f(1, -1);
            glTexCoord2f(uv_start_x + uv_width, uv_start_y + uv_height);
            glVertex2f(1, 1);
            glTexCoord2f(uv_start_x, uv_start_y + uv_height);
            glVertex2f(-1, 1);
            glEnd();
        billboardEnd();
	glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);

	// integrate animation - at 33ms per frame
	// NOTE to Students: Good idea to use delta-time here for more accurate results
	anim.timeAccum += deltaTime;

	// if time has exceeded speed, we increment the animation frame
	if (anim.timeAccum > anim.speed)
	{
		anim.timeAccum -= anim.speed;
		anim.cframe++;

		// wrap around if frames end
		// NOTE to Students: Find a better way to structure this
		if (anim.cframe >= anim.maxFrame)
		{
			animationFinished = true;
		}
	}
}