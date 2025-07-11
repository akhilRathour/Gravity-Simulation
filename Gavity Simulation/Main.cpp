
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

float simulationSpeed = 1.0f;

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


Body* userBody = nullptr; // The body we’ll add on button press
float userMass = 111.0f;   // Default mass
float objectDepth = 0.3f;
bool spawnBody = false;





int main()
{
	Window window1("Gravity Simulation");
	
	int width, height;
	glfwGetFramebufferSize(window1.GetGLFWWindow(), &width, &height);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window1.GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	

	// Texture data
	Texture Earthtextures[]
	{
		Texture( "2k_earth_daymap.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		//Texture( "2k_earth_clouds.jpg", "diffuse", 1, GL_RGB, GL_UNSIGNED_BYTE),
		//Texture( "2k_earth_spec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};
	Texture SUNtextures[]
	{
		Texture( "2k_sun.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		
	};

	

	

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Texture> Earthtex(Earthtextures, Earthtextures + sizeof(Earthtextures) / sizeof(Texture));
	std::vector <Texture> SUNtex(SUNtextures, SUNtextures + sizeof(SUNtextures) / sizeof(Texture));
	

	//trail shader
	Shader trailShader("trail.vert", "trail.frag");


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Create light mesh
	Mesh light(lightVerts, lightInd, Earthtex);

	



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
	Mesh earthMesh(sphereVertices, sphereIndices, Earthtex);
	Mesh SunMesh(sphereVertices, sphereIndices, SUNtex);

	Renderer renderer;
	Physics physics;

	std::vector<Body*> bodies;

	Body* earth = new Body(earthMesh, Constants::EarthMass,Constants::EarthRadius, Constants::EarthPos, Constants::EarthVel);
	///mesh(mesh), mass(mass), position(position), velocity(velocity), 
	
	Body* Sun = new Body(SunMesh, Constants::SunMass, Constants::SunRadius, Constants::SunPos, Constants::SunVel);

	Body* Mars = new Body(earthMesh, Constants::MarsMass, Constants::MarsRadius, Constants::MarsPos, Constants::MarsVel);

	bodies.push_back(earth);
	bodies.push_back(Sun);
	bodies.push_back(Mars);
	renderer.Submit(Mars);
	renderer.Submit(earth);
	renderer.Submit(Sun);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 220.0f));
	float lastFrame = glfwGetTime();
	// Main while loop
	while (!window1.ShouldClose())
	{
		
		/*if (glfwGetKey(window1.GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window1.GetGLFWWindow(), true);*/


		window1.Clear();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		//exit logic starts
		static bool showExitConfirmation = false;

		// 1. Detect Escape Press
		if (glfwGetKey(window1.GetGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS && !ImGui::IsAnyItemActive())
		{
			if (!showExitConfirmation)
			{
				showExitConfirmation = true;
				ImGui::OpenPopup("Exit Confirmation");
			}
		}

		// 2. Popup Modal
		if (ImGui::BeginPopupModal("Exit Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to exit?");
			ImGui::Separator();

			if (ImGui::Button("Yes", ImVec2(120, 0))) {
				glfwSetWindowShouldClose(window1.GetGLFWWindow(), true); // Close app
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0))) {
				showExitConfirmation = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}


		//exit logic ends


		ImGuiIO& io = ImGui::GetIO();
		// Handles camera inputs
		camera.Inputs(window1.GetGLFWWindow(),io);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);


		//wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		glm::vec3 lightPos = Sun->position;
		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		//light.Draw(lightShader, camera);//dont sraw light just simulate its affect

	
		// Calculate deltaTime
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		if (deltaTime > 0.1f) {
			deltaTime = 0.1f; // Cap deltaTime to prevent large jumps
		}
		physics.ApplyGravitationalForces(bodies);
		physics.UpdateBodies(bodies, deltaTime*simulationSpeed);
		renderer.DrawAll(shaderProgram, camera);
		
			renderer.DrawTrails(trailShader, camera);


		// ImGUI window creation
		ImGui::Begin("Gravity Simulator");
		// Text that appears in the window
		ImGui::Text("Controls");
		ImGui::Text("WASD and mouse to look around");
		ImGui::Text("press F11 to exit full screen");
		ImGui::Text("press Esc to exit program");
		if (ImGui::Button("Reset Simulation")) {
			renderer.ResetSimulation();
		}
		ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.0f, 100.0f, "%.2f");
		static bool showTrails = true;
		static int trailLength = 300;

		ImGui::Checkbox("Show Trails", &showTrails);
		renderer.SetShowTrails(showTrails);

		ImGui::SliderInt("Trail Length", &trailLength, 0, 1000);
		renderer.SetTrailLength(trailLength);
			
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