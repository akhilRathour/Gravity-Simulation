
#include"Mesh.h"
#include<vector>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"Window.h"
#include"physics.h"
#include "Body.h"
#include"renderer.h"
#include"physicsConstants.h"


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
	Texture SUNtextures[]
	{
		Texture( "2k_sun.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		
	};

	

	

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector <Texture> SUNtex(SUNtextures, SUNtextures + sizeof(SUNtextures) / sizeof(Texture));
	


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


	
	//creating sphere
	std::vector<Vertex> sphereVertices;
	std::vector<GLuint> sphereIndices;

	GenerateSphere(sphereVertices, sphereIndices, 2.0f, 36, 18); // smooth sphere
	Mesh earthMesh(sphereVertices, sphereIndices, tex);
	Mesh SunMesh(sphereVertices, sphereIndices, SUNtex);

	Renderer renderer;
	Physics physics;

	std::vector<Body*> bodies;
	//glm::vec3 earthPosition = glm::vec3(0.0f, 0.0f, -2.0f);
	//glm::vec3 earthInitialVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	Body* earth = new Body(earthMesh, Constants::EarthMass, Constants::EarthPos, Constants::EarthVel);
	///mesh(mesh), mass(mass), position(position), velocity(velocity), 
	/*glm::vec3 SunPosition = glm::vec3(9.0f, 3.0f, 0.0f);
	glm::vec3 SunInitialVelocity = glm::vec3(0.0f, 0.0f, 0.0f);*/
	Body* Sun = new Body(SunMesh, Constants::SunMass, Constants::SunPos, Constants::SunVel);

	

	bodies.push_back(earth);
	bodies.push_back(Sun);

	renderer.Submit(earth);
	renderer.Submit(Sun);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 22.0f));
	float lastFrame = glfwGetTime();
	// Main while loop
	while (!window1.ShouldClose())
	{

		if (glfwGetKey(window1.GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window1.GetGLFWWindow(), true);

		window1.Clear();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGuiIO& io = ImGui::GetIO();
		// Handles camera inputs
		camera.Inputs(window1.GetGLFWWindow(),io);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		
		light.Draw(lightShader, camera);
		// Calculate deltaTime
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		 
		/*physics.ApplyGravitationalForces(bodies);
		physics.UpdateBodies(bodies, deltaTime);*/
		renderer.DrawAll(shaderProgram, camera);
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