
#include <gl/freeglut.h>
#include <conio.h>
#include <cstdio>
#include <glm\vec3.hpp>


// integrator timestep.
#define TIMESTEP 30

// ground level ( y-plane ).
const float GROUND_LEVEL = -2.0f;

// ball radius.
const float BALL_RADIUS = 2.0f;

// ball mass.
const float BALL_MASS = 1.0f;

// inverse ball mass.
const float INV_BALL_MASS = 1.0f / BALL_MASS;

// ball reseting on ground height.
#define BALL_GROUND GROUND_LEVEL + BALL_RADIUS

// total balls.
#define NUM_BALLS 4

// performance counter frequency.
LARGE_INTEGER gQpcFreq;

struct sBall
{
	// current position.
	glm::vec3 Position;

	// current velocity.
	glm::vec3 Velocity;

	// accumulated forces.
	glm::vec3 Forces;

	// true if moving.
	int Moving;

	// draw colour.
	glm::vec3 Colour;
};

sBall gBalls[ NUM_BALLS ];

// resets the ball physics state.
void ResetBall( sBall& ball )
{
	ball.Moving = false;
	ball.Position.y = GROUND_LEVEL + BALL_RADIUS;
	ball.Velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
	ball.Forces = glm::vec3( 0.0f, 0.0f, 0.0f );
}

// Euler integrator.
void StepPhysicsEuler( sBall& ball, double frameTime )
{
	// convert accumulated forces to an acceleration vector.
	glm::vec3 accel;
	accel = ball.Forces * INV_BALL_MASS;

	// if above ground apply gravity.
	if ( ball.Position.y > BALL_GROUND )
	{
		accel += glm::vec3( 0.0f, -9.8f, 0.0f );
	}

	// update the position.
	glm::vec3 stepVel = ball.Velocity;
	stepVel *= frameTime;
	ball.Position += stepVel;		// NewPos = OldPos + ( vel * timestep );

	// update the velocity.
	//ball.Velocity += accel * frameTime;				// NewVel = OldVel + ( acc * timestep );
	glm::vec3 stepAcc = accel;
	stepAcc *= frameTime;
	ball.Velocity += stepAcc;				// NewVel = OldVel + ( acc * timestep );
}

// RK4 integrator.
void StepPhysicsRK4( sBall& ball, double frameTime )
{
	// convert accumulated forces to an acceleration vector.
	glm::vec3 accel;

	accel = ball.Forces * INV_BALL_MASS;

	// if above ground apply gravity.
	if ( ball.Position.y > BALL_GROUND )
	{
		accel += glm::vec3( 0.0f, -9.8f, 0.0f );
	}

	// this code based on the Essential Mathematics for Games example code.

	glm::vec3 xk1 = ball.Velocity;	xk1 *= frameTime;
	glm::vec3 vk1 = accel;			vk1 *= frameTime;

	glm::vec3 midVelocity = ball.Velocity; 
	midVelocity += (0.5f * vk1);
	glm::vec3 xk2 = midVelocity;	xk2 *= frameTime;
	glm::vec3 vk2 = accel;			vk2 *= frameTime;

	midVelocity = ball.Velocity;	
	midVelocity += (0.5f * vk2);
	glm::vec3 xk3 = midVelocity;	xk3 *= frameTime;
	glm::vec3 vk3 = accel;			vk3 *= frameTime;

	midVelocity = ball.Velocity;	
	midVelocity += vk3;
	glm::vec3 xk4 = midVelocity;	xk4 *= frameTime;
	glm::vec3 vk4 = accel;			vk4 *= frameTime;

// From: https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Explicit_Runge.E2.80.93Kutta_methods
// Explicit Runge–Kutta methods aka "Butcher tableau"
// 
//	The RK4 method falls in this framework.Its tableau is:
// 
//        0 |
//      1/2	| 1/2
//      1/2 |  0    1/2
//        1	|  0     0     1
//      -------------------------------
//          | 1/6   1/3   1/3   1/6
//


	// update position and velocity.
//	ball.Position += (         xk1 
//					  + 2.0f * xk2 
//					  + 2.0f * xk3 
//					  +        xk4 ) / 6.0f;
	// or...
	ball.Position +=   ( xk1 * (1.0f/6.0f) )
		             + ( xk2 * (1.0f/3.0f) )	// Or 2.0f/6.0f
		             + ( xk3 * (1.0f/3.0f) )	// Or 2.0f/6.0f
		             + ( xk4 * (1.0f/6.0f) );

	ball.Velocity += (         vk1 
					  + 2.0f * vk2 
					  + 2.0f * vk3 
					  +        vk4 ) / 6.0f;

	return;
}

// RK4 integrator.
void StepPhysicsRK4_3_8_Rule(sBall& ball, double frameTime)
{
	// convert accumulated forces to an acceleration vector.
	glm::vec3 accel;

	accel = ball.Forces * INV_BALL_MASS;

	// if above ground apply gravity.
	if (ball.Position.y > BALL_GROUND)
	{
		accel += glm::vec3(0.0f, -9.8f, 0.0f);
	}

	// this code based on the Essential Mathematics for Games example code.

	glm::vec3 xk1 = ball.Velocity;	xk1 *= frameTime;
	glm::vec3 vk1 = accel;			vk1 *= frameTime;

	glm::vec3 midVelocity = ball.Velocity;
	midVelocity += (0.5f * vk1);
	glm::vec3 xk2 = midVelocity;	xk2 *= frameTime;
	glm::vec3 vk2 = accel;			vk2 *= frameTime;

	midVelocity = ball.Velocity;
	midVelocity += (0.5f * vk2);
	glm::vec3 xk3 = midVelocity;	xk3 *= frameTime;
	glm::vec3 vk3 = accel;			vk3 *= frameTime;

	midVelocity = ball.Velocity;
	midVelocity += vk3;
	glm::vec3 xk4 = midVelocity;	xk4 *= frameTime;
	glm::vec3 vk4 = accel;			vk4 *= frameTime;

// From: https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Explicit_Runge.E2.80.93Kutta_methods
// Explicit Runge–Kutta methods aka "Butcher tableau"
// 
//	A slight variation of "the" Runge–Kutta method is also due to Kutta in 1901 and is called the 3/8-rule.
// 
//        0 |
//      1/3	|  1/3
//      2/3 | -1/3    1
//        1	|   1    -1     1
//      -------------------------------
//          |  1/8   3/8   3/8   1/8
//


	// update position and velocity.
//	ball.Position += (         xk1 
//					  + 3.0f * xk2 
//					  + 3.0f * xk3 
//					  +        xk4 ) / 8.0f;
	// or...
	ball.Position +=   (xk1 * (1.0f / 8.0f))
		             + (xk2 * (3.0f / 8.0f))	
		             + (xk3 * (3.0f / 8.0f))	
		             + (xk4 * (1.0f / 8.0f));

	ball.Velocity += (         vk1
					  + 3.0f * vk2
					  + 3.0f * vk3
					  +        vk4 ) / 8.0f;

	return;
}

// TODO: Fix this (it's still sort of messed up)
void StepPhysicsRK6( sBall& ball, double frameTime )
{
	// convert accumulated forces to an acceleration vector.
	glm::vec3 accel;
	accel = ball.Forces * INV_BALL_MASS;

	// if above ground apply gravity.
	if ( ball.Position.y > BALL_GROUND )
	{
		accel += glm::vec3( 0.0f, -9.8f, 0.0f );
	}

	// this code based on the Essential Mathematics for Games example code.

	glm::vec3 xk1 = ball.Velocity;	xk1 *= frameTime;
	glm::vec3 vk1 = accel;			vk1 *= frameTime;

	glm::vec3 midVelocity = ball.Velocity;		
	midVelocity += ( 0.5f * vk1 );
	glm::vec3 xk2 = midVelocity;	xk2 *= frameTime;
	glm::vec3 vk2 = accel;			vk2 *= frameTime;

	midVelocity = ball.Velocity;		
	midVelocity += ( 0.25f * vk2 );
	glm::vec3 xk3 = midVelocity;	xk3 *= midVelocity;
	glm::vec3 vk3 = accel;			vk3 *= frameTime;

	midVelocity = ball.Velocity;	
	midVelocity += ( 0.25f * vk3 );
	glm::vec3 xk4 = midVelocity;	xk4 *= frameTime;
	glm::vec3 vk4 = accel;			vk4 *= frameTime;

	midVelocity = ball.Velocity;	
	midVelocity += ( 0.5f * vk4 );
	glm::vec3 xk5 = midVelocity;	xk5 *= frameTime;
	glm::vec3 vk5 = accel;			vk5 *= frameTime;

	midVelocity = ball.Velocity;	
	midVelocity += vk5;
	glm::vec3 xk6 = midVelocity;	xk6 *= frameTime;
	glm::vec3 vk6 = accel;			vk6 *= frameTime;

// From: http://ijmcs.future-in-tech.net/17.3/R-AhmedHussein.pdf
// Explicit Runge–Kutta methods aka "Butcher tableau"
// 
//	Table 2: Set of coefficients of Butcher’s tableau for (IRK6M):
// 
//      1/3 | 0.3333
//      2/3	|    0      0.6667
//      1/3 | 0.1101    0.1120    0.1112
//      5/6 | 0.3281   -0.1879    0.5264   0.1667
//      1/6 | 0.5165    0.7894   -0.2493   0.3333   -1.2232
//        1	| 0.2858   -0.1429    0.2755   0.3072    0.1418   0.1326
//      ------------------------------------------------------------------
//          | 213/400   -1/15     41/120      0      83/300   -7/60     0
//
// Or this more straight forward solution from: https://www.researchgate.net/figure/The-Butcher-Tableau-for-the-RK6-Method_tbl3_318284280
// 
// 
//        0 |    0
//      1/3	|   1/3     0
//      2/3 |    0     2/3     0
//      1/3 |  1/12    1/3   -1/12
//      1/2 | -1/16    9/8   -2/16   -3/8      0
//      1/2	|    0     9/8   -3/8    -3/4     1/2      0
//        1	|  9/44   -9/11  63/44   18/11     0    -16/11     0
//      ------------------------------------------------------------------
//          | 11/120    0    27/40   27/40   -4/15   -4/15   11/120
//                           81/120


	// update position and velocity.
	ball.Position += (   (11.0f/120.0f) * xk1
		               +           0.0f * xk2 
					   +  (81.0f*120.0f) * xk3 
					   +  (81.0f*120.0f) * xk4
					   +  (-40.0f/120.0f) * xk5
					   +         xk6 ) / 20.0f;

	ball.Velocity += (           vk1
		               + (3.0f * vk2)
					   + (6.0f * vk3)
					   + (6.0f * vk4)
					   + (3.0f * vk5)
					   +         vk6 ) / 20.0f;

	return;
}

void DrawBall( sBall& ball )
{
	// draw ball.
	glPushMatrix();

		glTranslatef( ball.Position.x, ball.Position.y, ball.Position.z );

		// draw velocity vector.
		glBegin( GL_LINES );
			glColor3f( 1.0f, 1.0f, 1.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			GLfloat velTemp[3];
			velTemp[0] = ball.Velocity.x;	velTemp[1] = ball.Velocity.y; velTemp[2] = ball.Velocity.z;
			glVertex3fv( velTemp );
		glEnd();

		// draw the ball.
		glColor3f( ball.Colour.x, ball.Colour.y, ball.Colour.z );
		glutSolidSphere( BALL_RADIUS, 16, 16 );

	glPopMatrix();
}

void DrawFrame()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	float camY = 25.0f;
	float camZ = 85.0f;
	gluLookAt(
		0.0f, camY, camZ,
		0.0f, camY, camZ - 1.0f,
		0.0f, 1.0f,  0.0f );

	// draw ground plane.
	int GROUND_SCALE = 20;
	glBegin( GL_QUADS );
		glColor3f( 0.4f, 0.7f, 0.8f );
		glVertex3i( -GROUND_SCALE, (GLint)GROUND_LEVEL, -GROUND_SCALE );
		glVertex3i( -GROUND_SCALE, (GLint)GROUND_LEVEL, +GROUND_SCALE );
		glVertex3i( +GROUND_SCALE, (GLint)GROUND_LEVEL, +GROUND_SCALE );
		glVertex3i( +GROUND_SCALE, (GLint)GROUND_LEVEL, -GROUND_SCALE );
	glEnd();

	// draw the balls.
	for ( int i = 0; i < NUM_BALLS; i++ )
	{
		DrawBall( gBalls[i] );
	}

	glutSwapBuffers();
}

static bool bDoOnce = false;

void GlutOnDraw()
{
	//if (!bDoOnce)
	//{
	//	for ( int i = 0; i < NUM_BALLS; i++ )
	//	{
	//		if ( !gBalls[i].Moving )
	//		{
	//			gBalls[i].Moving = true;

	//			// apply an impulse.
	//			gBalls[i].Forces += glm::vec3( 0.0f, 1500.0f, 0.0f );
	//		}
	//	}
	//	bDoOnce = true;
	//}

	static double frameTime = 0.01;

	// sample frame begin time.
	LARGE_INTEGER frameBeg;
	QueryPerformanceCounter( &frameBeg );

	//frameTime = 0.02;

	// ball 0 uses euler.
	StepPhysicsEuler( gBalls[0], frameTime );

	// ball 1 uses RK4.
	StepPhysicsRK4( gBalls[1], frameTime );

	// ball 2 uses RK4 with 3/8th rule.
	StepPhysicsRK4_3_8_Rule( gBalls[2], frameTime );

	// ball 3 uses RK6.
//	StepPhysicsRK6( gBalls[3], frameTime );

	for ( int i = 0; i < NUM_BALLS; i++ )
	{
		// clear forces.
		gBalls[i].Forces = glm::vec3( 0.0f, 0.0f, 0.0f );

		// HACK: dont let the ball fall below the ground.
		if ( gBalls[i].Position.y < BALL_GROUND )
		{
			gBalls[i].Moving = false;
			ResetBall( gBalls[i] );
		}
	}

	DrawFrame();

	Sleep( TIMESTEP );

	// sample frame end time.
	LARGE_INTEGER frameEnd;
	QueryPerformanceCounter( &frameEnd );

	// update frame time.
	frameTime = static_cast<double>( frameEnd.QuadPart - frameBeg.QuadPart ) / gQpcFreq.QuadPart;

	// re-draw.
	glutPostRedisplay();
}

void GlutOnSize( int width, int height )
{
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 90.0f, static_cast<float>(width) / height, 1.0f, 1000.0f );
}

void GlutOnKey( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'w':
		for ( int i = 0; i < NUM_BALLS; i++ )
		{
			if ( !gBalls[i].Moving )
			{
				gBalls[i].Moving = true;

				// apply an impulse.
				gBalls[i].Forces += glm::vec3( 0.0f, 1500.0f, 0.0f );
			}
		}
		break;
	case 'r':
		{
			printf( "Simulation state reset.\n" );
			for ( int i = 0; i < NUM_BALLS; i++ )
			{
				ResetBall( gBalls[i] );
			}
			break;
		}
	default:
		break;
	}
}

int main( int argc, char** argv )
{
	QueryPerformanceFrequency( &gQpcFreq );

	// init glut.
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize( 1024, 800 );
	glutCreateWindow( "Press W to throw balls. Left is Euler, right is RK4" );

	// initialize ball positions.
	gBalls[0].Position.x = -12.0f;
	gBalls[0].Position.y = BALL_GROUND;
	gBalls[0].Position.z =  0.0f;
	gBalls[0].Colour.x = 1.0f;
	gBalls[0].Colour.y = 0.0f;
	gBalls[0].Colour.z = 0.0f;

	gBalls[1].Position.x = -4.0f;
	gBalls[1].Position.y = BALL_GROUND;
	gBalls[1].Position.z =  0.0f;
	gBalls[1].Colour.x = 0.0f;
	gBalls[1].Colour.y = 1.0f;
	gBalls[1].Colour.z = 0.0f;

	gBalls[2].Position.x = +4.0f;
	gBalls[2].Position.y = BALL_GROUND;
	gBalls[2].Position.z =  0.0f;
	gBalls[2].Colour.x = 0.0f;
	gBalls[2].Colour.y = 0.0f;
	gBalls[2].Colour.z = 1.0f;

	gBalls[3].Position.x = +12.0f;
	gBalls[3].Position.y = BALL_GROUND;
	gBalls[3].Position.z =  0.0f;
	gBalls[3].Colour.x = 0.0f;
	gBalls[3].Colour.y = 1.0f;
	gBalls[3].Colour.z = 1.0f;

	ResetBall( gBalls[0] );
	ResetBall( gBalls[1] );
	ResetBall( gBalls[2] );
	ResetBall( gBalls[3] );

	// setup depth testing.
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	// register callbacks.
	glutDisplayFunc( GlutOnDraw );
	glutReshapeFunc( GlutOnSize );
	glutKeyboardFunc( GlutOnKey );



	// go!
	glutMainLoop();
	return 0;
}
