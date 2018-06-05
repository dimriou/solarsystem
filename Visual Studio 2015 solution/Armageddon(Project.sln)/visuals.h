//-------- Model Definitions --------------------------------

struct point
{
	float x;
	float y;
	float z;
};

struct face
{
	int vtx[3];
	int norm[3];
};

struct model
{
	point obj_points[10000];
	point obj_normals[10000];
	face obj_faces[20000];
	int vertices;
	int normals;
	int faces;
};

//-------- Enumerations --------------------------------
enum planet_type {
	PLANET_1,
	PLANET_2,
	PLANET_3,
	PLANET_4,
	PLANET_5
};
//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();

void Keyboard(unsigned char key, int x, int y);
// Function for handling keyboard events.

void ReadFile(model*);
// Function for reading object file

void DisplayModel(float,model,planet_type,float);
// Function for displaying a model