/*
* FractalRenderer.cpp
* Author: Suraj Pal Singh and Varsha Rao
* Date: 2023-06-21
* License: MIT License
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "Shader.h"
#include "FractalGenerator.h"

namespace FractalRenderer {

	FractalGenerator::FractalGenerator() :
		width(FractalGenerator::WIDTH),
		height(FractalGenerator::HEIGHT),
		maxIterations(200),
		escapeRadius(FractalGenerator::DEFAULT_ESCAPE_RADIUS),
		juliaConstantReal(FractalGenerator::DEFAULT_JULIA_REAL),
		juliaConstantImaginary(FractalGenerator::DEFAULT_JULIA_IMAGINARY),
		zoomFactor(1.0f),
		speedFactor(0.4f),
		translationX(0.0f),
		translationY(0.0f),
		renderMode(RenderMode::Julia)
	{
		startTime = std::chrono::high_resolution_clock::now();
		// Set up GLFW error callback
		glfwSetErrorCallback(errorCallback);

		// Initialize GLFW
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		// Set GLFW window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create a GLFW window
		window = glfwCreateWindow(width, height, "Julia Set Renderer", glfwGetPrimaryMonitor(), nullptr);
		if (!window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		// Make the OpenGL context current
		glfwMakeContextCurrent(window);

		// Initialize GLAD
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		// Setup ImGui platform/renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

		// Create the julia shader program
		juliaShaderProgram = createShaderProgram(
			readShaderFile("src/shaders/julia_vertex_shader.glsl").c_str(),
			readShaderFile("src/shaders/julia_frag_shader.glsl").c_str()
		);
		if (juliaShaderProgram == 0) {
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		// Create the mandelbrot shader program
		mandelbrotShaderProgram = createShaderProgram(
			readShaderFile("src/shaders/mandelbrot_vertex_shader.glsl").c_str(),
			readShaderFile("src/shaders/mandelbrot_frag_shader.glsl").c_str()
		);
		if (mandelbrotShaderProgram == 0) {
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		shaderProgram = juliaShaderProgram;

		// Set up vertex data and buffers
		float vertices[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void FractalGenerator::run()
	{
		// Rendering loop
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			render();
		}

		//cleanup();
	}

	// Callback function for GLFW error handling
	void FractalGenerator::errorCallback(int error, const char* description)
	{
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	}

	// Reset the Julia set parameters to their default values
	void FractalGenerator::resetParameters()
	{
		maxIterations = 200;
		escapeRadius = DEFAULT_ESCAPE_RADIUS;
		juliaConstantReal = DEFAULT_JULIA_REAL;
		juliaConstantImaginary = DEFAULT_JULIA_IMAGINARY;
		zoomFactor = 1.0f;
		translationX = 0.0f;
		translationY = 0.0f;
		speedFactor = 0.4f;
		startTime = std::chrono::high_resolution_clock::now();
	}

	void FractalGenerator::render()
	{
		// put time in seconds since start of program in variable elapsedTime
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - startTime).count();

		// calculate the julia constant based on time
		float time = static_cast<float>(elapsedTime) * speedFactor;

		if (speedFactor > 0) {
			juliaConstantReal = DEFAULT_JULIA_REAL + 0.30f * cos(0.31f * time) - 0.15f * sin(1.17f * time);
			juliaConstantImaginary = DEFAULT_JULIA_IMAGINARY + 0.30f * sin(0.37f * time) + 0.15f * cos(2.31f * time);
		}

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui controls
		ImGui::Begin("Fractal Set Controls");

		if (ImGui::Button("Mandelbrot Set")) {
			renderMode = RenderMode::Mandelbrot;
			shaderProgram = mandelbrotShaderProgram;
		}

		ImGui::SameLine();

		if (ImGui::Button("Julia Set Fractal")) {
			renderMode = RenderMode::Julia;
			shaderProgram = juliaShaderProgram;
		}

		ImGui::SliderInt("Max Iterations", &maxIterations, 1, FractalGenerator::MaxIterations);
		ImGui::SliderFloat("Escape Radius", &escapeRadius, 50.0f, 200.0f);

		if (renderMode == RenderMode::Julia) {
			ImGui::SliderFloat("Animation Speed", &speedFactor, 0.0f, 1.0f);
			ImGui::SliderFloat("Julia Constant (Real)", &juliaConstantReal, -2.0f, 2.0f);
			ImGui::SliderFloat("Julia Constant (Imaginary)", &juliaConstantImaginary, -2.0f, 2.0f);
			ImGui::Text("Julia Const (Real): %.3f", juliaConstantReal);
			ImGui::Text("Julia Const (Imag): %.3f", juliaConstantImaginary);
		}
		ImGui::Text("Zoom: %.3f", zoomFactor);
		ImGui::Text("TranslationX: %.5f", translationX);
		ImGui::Text("TranslationY: %.5f", translationY);
		ImGui::Text("Time Elapsed: %.3f", static_cast<float>(elapsedTime));
		if (ImGui::Button("Reset")) {
			resetParameters();
		}
		ImGui::SameLine();
		if (ImGui::Button("Exit")) {
			cleanup();
			return;
		}

		ImGui::End();

		// Handle mouse scroll event for zooming
		double scrollY = ImGui::GetIO().MouseWheel;
		if (scrollY != 0.0 && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			// Get the mouse position relative to the window
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Convert the mouse position to normalized device coordinates (NDC)
			float ndcX = static_cast<float>(mouseX) / width;
			float ndcY = 1.0f - static_cast<float>(mouseY) / height;

			// Calculate the zoom origin in world coordinates
			float zoomOriginX = (ndcX - 0.5f) * 2.0f;
			float zoomOriginY = (ndcY - 0.5f) * 2.0f;

			// Adjust the zoom factor
			float zoomIncrement = static_cast<float>(scrollY) * 0.4f;
			zoomFactor += zoomIncrement;

			// Apply zoom offset based on the mouse position
			translationX += zoomOriginX * zoomIncrement;
			translationY += zoomOriginY * zoomIncrement;

			if (zoomFactor < 0.1f)
				zoomFactor = 0.1f;
		}

		// Handle keyboard controls for zooming
		if (!ImGui::IsAnyItemActive())
		{
			// Get the mouse position relative to the window
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Convert the mouse position to normalized device coordinates (NDC)
			float ndcX = static_cast<float>(mouseX) / width;
			float ndcY = 1.0f - static_cast<float>(mouseY) / height;

			// Calculate the zoom origin in world coordinates
			float zoomOriginX = (ndcX - 0.5f) * 2.0f;
			float zoomOriginY = (ndcY - 0.5f) * 2.0f;

			// Adjust the zoom factor
			float zoomIncrement = static_cast<float>(0.05);
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				zoomFactor += zoomIncrement;

				// Apply zoom offset based on the mouse position
				translationX += zoomOriginX * zoomIncrement;
				translationY += zoomOriginY * zoomIncrement;
			}
			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				zoomFactor -= zoomIncrement;
				if (zoomFactor < 0.1f)
					zoomFactor = 0.1f;

				// Apply zoom offset based on the mouse position
				translationX += zoomOriginX * zoomIncrement;
				translationY += zoomOriginY * zoomIncrement;
			}
		}

		// Handle mouse left click drag for panning
		static bool isMouseDragging = false;
		static double prevMouseX = 0.0;
		static double prevMouseY = 0.0;

		if (!ImGui::IsAnyItemActive())
		{
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				isMouseDragging = false;
			}
			else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
					isMouseDragging = true;
					prevMouseX = ImGui::GetIO().MousePos.x;
					prevMouseY = ImGui::GetIO().MousePos.y;
				}
			}
			else if (isMouseDragging && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				double mouseX = ImGui::GetIO().MousePos.x;
				double mouseY = ImGui::GetIO().MousePos.y;

				translationX -= static_cast<float>(mouseX - prevMouseX) * 0.01f;
				translationY += static_cast<float>(mouseY - prevMouseY) * 0.01f;

				prevMouseX = mouseX;
				prevMouseY = mouseY;
			}
		}

		// Handle keyboard controls for panning using WASD keys
		if (!ImGui::IsAnyItemActive())
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				translationY += 0.01f;
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				translationY -= 0.01f;
			}
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				translationX -= 0.01f;
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				translationX += 0.01f;
			}
		}

		// Update the Julia constant uniform in the shader program
		GLint juliaConstantLocation = glGetUniformLocation(shaderProgram, "juliaConstant");
		glUseProgram(shaderProgram);
		glUniform2f(juliaConstantLocation, juliaConstantReal, juliaConstantImaginary);

		// Update the max iterations uniform in the shader program
		GLint maxIterationsLocation = glGetUniformLocation(shaderProgram, "maxIterations");
		glUniform1i(maxIterationsLocation, maxIterations);

		// Update the escape radius uniform in the shader program
		GLint escapeRadiusLocation = glGetUniformLocation(shaderProgram, "escapeRadius");
		glUniform1f(escapeRadiusLocation, escapeRadius);

		// Update the zoom factor uniform in the shader program
		GLint zoomFactorLocation = glGetUniformLocation(shaderProgram, "zoomFactor");
		glUniform1f(zoomFactorLocation, zoomFactor);

		// Update the translation uniform in the shader program
		GLint translationLocation = glGetUniformLocation(shaderProgram, "translation");
		glUniform2f(translationLocation, translationX, translationY);

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Draw the quad
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		glfwSwapBuffers(window);
	}

	void FractalGenerator::cleanup()
	{
		// Cleanup ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Cleanup OpenGL resources
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);

		// Terminate GLFW
		glfwSetWindowShouldClose(window, true);
		glfwTerminate();

		std::exit(EXIT_SUCCESS);
	}
}

int main()
{
	FractalRenderer::FractalGenerator renderer;
	renderer.run();

	return 0;
}
