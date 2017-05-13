#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class cube;

std::vector<cube> cubes;

void rotX(float* m, int nrvectors, float r) {
	float rot[] = {
		1.f, 0.f, 0.f,
		0.f, cos(r), -sin(r),
		0.f, sin(r), cos(r)
	};

	float tmp[3];

	for (int i = 0; i < nrvectors - 2; i += 3) {
		tmp[0] = rot[0] * m[i] + rot[1] * m[i + 1] + rot[2] * m[i + 2];
		tmp[1] = rot[3] * m[i] + rot[4] * m[i + 1] + rot[5] * m[i + 2];
		tmp[2] = rot[6] * m[i] + rot[7] * m[i + 1] + rot[8] * m[i + 2];

		m[i] = tmp[0];
		m[i + 1] = tmp[1];
		m[i + 2] = tmp[2];
	}
}

void rotY(float* m, int nrvectors, float r) {


	float rot[] = {
		cos(r), 0.f, sin(r),
		0.f,	1.f, 0.f,
		-sin(r), 0, cos(r)
	};

	float tmp[3];

	for (int i = 0; i < nrvectors - 2; i += 3) {
		tmp[0] = rot[0] * m[i] + rot[1] * m[i + 1] + rot[2] * m[i + 2];
		tmp[1] = rot[3] * m[i] + rot[4] * m[i + 1] + rot[5] * m[i + 2];
		tmp[2] = rot[6] * m[i] + rot[7] * m[i + 1] + rot[8] * m[i + 2];

		m[i] = tmp[0];
		m[i + 1] = tmp[1];
		m[i + 2] = tmp[2];
	}
}

void rotZ(float* m, int nrvectors, float r) {
	float rot[] = {
		cos(r), -sin(r), 0.f,
		sin(r), cos(r), 0.f,
		0.f, 0.f, 1.f
	};

	float tmp[3];

	for (int i = 0; i < nrvectors - 2; i += 3) {
		tmp[0] = rot[0] * m[i] + rot[1] * m[i + 1] + rot[2] * m[i + 2];
		tmp[1] = rot[3] * m[i] + rot[4] * m[i + 1] + rot[5] * m[i + 2];
		tmp[2] = rot[6] * m[i] + rot[7] * m[i + 1] + rot[8] * m[i + 2];

		m[i] = tmp[0];
		m[i + 1] = tmp[1];
		m[i + 2] = tmp[2];
	}
}

void translate(float* m, int nrvectors, float* p) {

	/*Initial translation of cube*/
	for (int i = 0; i < nrvectors*3; i += 3) {
		m[i]	 += p[0];
		m[i + 1] += p[1];
		m[i + 2] += p[2];
	}
}

class phys {
public:
	float m_p[3];
	float m_v[3];
	float m_a[3];

	float m_roll;
	float m_pitch;
	float m_yaw;

	phys() {};
};

class cube : phys{
private:
	float m_points[108];
	float m_colours[108];

	GLuint m_pvbo;
	GLuint m_cvbo;

	static int nrOfCubes;
	int m_id;
public:
	cube(std::vector<float> pos, std::vector<float> rot) {
		m_id = nrOfCubes++;

		m_pvbo = m_id;
		m_cvbo = m_id;

		/*Predefined points of cube*/
		float points[108] = {
			-1.0f,-1.0f,-1.0f, // triangle 1 : begin
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f, // triangle 1 : end
			1.0f, 1.0f,-1.0f, // triangle 2 : begin
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f, // triangle 2 : end
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
		};

		/*Scaler*/
		for (auto &a : points) {
			a = a / 10;
		}

		/*Predefined colors of cube*/
		float colours[108] = {
			/*1.first*/
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,

			/*2.first*/
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,

			/*3.first*/
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,

			/*2.last*/
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,

			/*1.last*/
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,

			/*3.last*/
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,

			/*4.first*/
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,

			/*5.first*/
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,

			/*5.last*/
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,

			/*6.first*/
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,

			/*6.last*/
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,

			/*4.last*/
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f
		};

		m_p[0] = pos.at(0);
		m_p[1] = pos.at(1);
		m_p[2] = pos.at(2);

		m_v[0] = 0.f;
		m_v[1] = 0.f;
		m_v[2] = 0.f;

		m_a[0] = 0.f;
		m_a[1] = 0.f;
		m_a[2] = 0.f;

		m_roll = rot.at(0);
		m_pitch = rot.at(1);
		m_yaw = rot.at(2);

		memcpy(m_points, points, sizeof(points));
		memcpy(m_colours, colours, sizeof(colours));

		/*Initial translation of cube*/
		translate(m_points, sizeof(m_points) / (sizeof(float) * 3), m_p);

		glGenBuffers(1, &m_pvbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points, GL_STATIC_DRAW);

		glGenBuffers(1, &m_cvbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colours), m_colours, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	int getNumberOfPoints() {
		return sizeof(m_points) / sizeof(float);
	}

	void update(float ts) {
		float inverted_p[] = { -m_p[0], -m_p[1], -m_p[2] };

		translate(m_points, sizeof(m_points) / (sizeof(float) * 3), inverted_p);
		rotX(m_points, sizeof(m_points) / sizeof(float), m_roll * ts);
		rotY(m_points, sizeof(m_points) / sizeof(float), m_yaw * ts);
		rotZ(m_points, sizeof(m_points) / sizeof(float), m_pitch * ts);

		m_a[1] = -1.0;

		m_p[0] += m_v[0] * ts;
		m_p[1] += m_v[1] * ts;
		m_p[2] += m_v[2] * ts;

		m_v[0] += m_a[0] * ts;
		m_v[1] += m_a[1] * ts;
		m_v[2] += m_a[2] * ts;

		for (int pnt = 0; pnt < sizeof(m_points) / sizeof(float); pnt+=3) {
			if (m_points[pnt + 1] + m_p[1] <= -1.0 && m_v[1] < 0.0) {
				m_v[1] = -1.0*m_v[1]*0.90f;
				break;
			}
		}

		translate(m_points, sizeof(m_points) / (sizeof(float) * 3), m_p);

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colours), m_colours, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_pvbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, m_cvbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
};

int cube::nrOfCubes = 0;
float mpos_x = 0.0;
float mpos_y = 0.0;
int W = 1080;
int H = 1080;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mpos_x = xpos;
	mpos_y = ypos;

	//gluLookAt(xpos, ypos, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	std::cout << "X: " << xpos << " Y: " << ypos << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float rand_x = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_y = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_z = ((float)rand() / (RAND_MAX)) - 0.5f;

	float rand_rot_x = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_rot_y = ((float)rand() / (RAND_MAX)) - 0.5f;
	float rand_rot_z = ((float)rand() / (RAND_MAX)) - 0.5f;

	switch (key) {
	case GLFW_KEY_1: {
		cube tmp_cube({ rand_x, rand_y , rand_z }, { rand_rot_x * 3.1415f / 2.0f, rand_rot_y * 3.1415f / 2.0f, rand_rot_z * 3.1415f / 2.0f });

		cubes.push_back(tmp_cube);
	}
		break;
	case GLFW_KEY_2: {
		cube tmp_cube({ (mpos_x / (float)W)*2.0f - 1.0f, 1.0f - (mpos_y / (float)H)*2.0f, 0.0f }, { rand_rot_x * 3.1415f / 2.0f, rand_rot_y * 3.1415f / 2.0f, rand_rot_z * 3.1415f / 2.0f });

		cubes.push_back(tmp_cube);
	}
		break;
	case GLFW_KEY_LEFT:

		break;
	case GLFW_KEY_SPACE:
		cubes.clear();
		break;
	default:
		break;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {
	srand(time(NULL));

	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// uncomment these lines if on Apple OS X
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	GLFWwindow* window = glfwCreateWindow(W, H, "Cube test", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	//glOrtho(0, 1920, 0, 1080, -1, 1);

	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, W, 0, H, -1, 1);
		//glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 1);	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	//Sets the key callback
	glfwSetKeyCallback(window, key_callback);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
	//

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	float r = 0.5f;
	int nrcubes = 10;

	for (int i = 0; i < nrcubes; i++) {
		float rand_rot_x = ((float)rand() / (RAND_MAX)) - 0.5f;
		float rand_rot_y = ((float)rand() / (RAND_MAX)) - 0.5f;
		float rand_rot_z = ((float)rand() / (RAND_MAX)) - 0.5f;

		cube tmp_cube({ r*(float)cos(i*(2 * 3.1415 / nrcubes)), r*(float)sin(i*(2 * 3.1415 / nrcubes)), 0.0f }, { rand_rot_x * 3.1415f / 2.0f, rand_rot_y * 3.1415f / 2.0f, rand_rot_z * 3.1415f / 2.0f });//{ 3.1415f / 2.0f, 3.1415f / 2.0f, 3.1415f / 2.0f });
		
		cubes.push_back(tmp_cube);
	}

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_colour;"
		"out vec3 colour;"
		"void main() {"
		"	colour = vertex_colour;"
		"	gl_Position = vec4(vertex_position, 1.0);"
		"};";

	const char* fragment_shader =
		"#version 400\n"
		"in vec3 colour;"
		"out vec4 frag_colour;"
		"void main() {"
		"	frag_colour = vec4(colour, 1.0);"
		"};";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);

	// insert location binding code here
	glBindAttribLocation(shader_programme, 0, "vertex_position");
	glBindAttribLocation(shader_programme, 1, "vertex_colour");

	glLinkProgram(shader_programme);

	//

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	int loops = 0;

	int timeStepMs = 10;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset the matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//printf("Loop: %d\n", loops);

		glUseProgram(shader_programme);
		glBindVertexArray(vao);

		/*rotY(points, sizeof(points) / sizeof(float), 0.02);
		rotZ(points, sizeof(points) / sizeof(float), 0.02);
		rotX(points, sizeof(points) / sizeof(float), 0.02);

		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);*/

		for (cube &a : cubes) {
			a.update((float)timeStepMs/1000.0);


			glDrawArrays(GL_TRIANGLES, 0, a.getNumberOfPoints()/3);
		}




		/*c.update();

		glDrawArrays(GL_TRIANGLES, 0, c.getNumberOfPoints());*/

		glfwPollEvents();
		glfwSwapBuffers(window);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, 1);
		}

		loops++;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeStepMs));
	}

						  /* OTHER STUFF GOES HERE NEXT */

						  // close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
