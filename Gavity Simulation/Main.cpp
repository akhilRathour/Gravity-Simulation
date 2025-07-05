
#include"Mesh.h"
#include<vector>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"Window.h"


// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void GenerateCircle(
	std::vector<Vertex>& vertices,
	std::vector<GLuint>& indices,
	float radius,
	int segments
) {
	vertices.clear();
	indices.clear();

	// Center vertex
	vertices.push_back({
		glm::vec3(0.0f, 0.0f, 0.0f),     // position
		glm::vec3(0.0f, 1.0f, 0.0f),     // normal
		glm::vec3(1.0f, 1.0f, 1.0f),     // color
		glm::vec2(0.5f, 0.5f)            // tex coord (center of texture)
		});

	for (int i = 0; i <= segments; ++i) {
		float angle = (2.0f * M_PI * i) / segments;
		float x = radius * cos(angle);
		float z = radius * sin(angle);

		float u = (x / (2 * radius)) + 0.5f;
		float v = (z / (2 * radius)) + 0.5f;

		vertices.push_back({
			glm::vec3(x, 0.0f, z),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec2(u, v)
			});
	}

	// Create triangle fan indices
	for (int i = 1; i <= segments; ++i) {
		indices.push_back(0);        // center
		indices.push_back(i);
		indices.push_back(i + 1);
	}
}
void GenerateSphere(
	std::vector<Vertex>& vertices,
	std::vector<GLuint>& indices,
	float radius,
	int sectorCount,   // longitude slices
	int stackCount     // latitude slices
) {
	vertices.clear();
	indices.clear();

	const float PI = 3.14159265359f;

	for (int i = 0; i <= stackCount; ++i) {
		float stackAngle = PI / 2 - i * (PI / stackCount); // from +pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);
		float y = radius * sinf(stackAngle);

		for (int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * (2 * PI / sectorCount); // from 0 to 2pi

			float x = xy * cosf(sectorAngle);
			float z = xy * sinf(sectorAngle);

			float u = (float)j / sectorCount;
			float v = (float)i / stackCount;

			glm::vec3 position = glm::vec3(x, y, z);
			glm::vec3 normal = glm::normalize(position);
			glm::vec2 texCoords = glm::vec2(u, v);

			vertices.push_back({
				position,
				normal,
				glm::vec3(1.0f, 1.0f, 1.0f),  // white color
				texCoords
				});
		}
	}

	for (int i = 0; i < stackCount; ++i) {
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

int main()
{
	Window window1("Gravity Simulation");
	
	int width, height;
	glfwGetFramebufferSize(window1.window, &width, &height);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window1.window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	

	// Texture data
	Texture textures[]
	{
		Texture( "planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture( "planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};


	

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	Mesh light(lightVerts, lightInd, tex);

	



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	//creating a circle
	std::vector<Vertex> circleVertices;
	std::vector<GLuint> circleIndices;

	GenerateCircle(circleVertices, circleIndices, 0.3f, 64);
	Mesh circle(circleVertices, circleIndices, tex);

	//creating sphere
	std::vector<Vertex> sphereVertices;
	std::vector<GLuint> sphereIndices;

	GenerateSphere(sphereVertices, sphereIndices, 0.2f, 36, 18); // smooth sphere
	Mesh sphere(sphereVertices, sphereIndices, tex);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!window1.ShouldClose())
	{

		if (glfwGetKey(window1.GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window1.GetGLFWWindow(), true);

		window1.Clear();
		//// Specify the color of the background
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//// Clean the back buffer and depth buffer
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGuiIO& io = ImGui::GetIO();
		// Handles camera inputs
		camera.Inputs(window1.GetGLFWWindow(),io);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		// Draws different meshes
		//floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);
		glm::vec3 objectPos = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::mat4 objectModel = glm::mat4(1.0f);
		objectModel = glm::translate(objectModel, objectPos);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
		circle.Draw(shaderProgram, camera);

		 objectPos = glm::vec3(0.0f, 1.0f, 0.0f);
		 
		 objectModel = glm::mat4(1.0f);
		objectModel = glm::translate(objectModel, objectPos);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
		sphere.Draw(shaderProgram, camera);

		// ImGUI window creation
		ImGui::Begin("My name is window, ImGUI window");
		// Text that appears in the window
		ImGui::Text("Hello there adventurer!");
		
		// Ends the window
		ImGui::End();

		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		ImGui::Begin("FPS Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapInterval(1); // Enable V-Sync

		window1.SwapBuffers();
		window1.PollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	// used destructor of window class to delete the window

	glfwTerminate();
	return 0;
}