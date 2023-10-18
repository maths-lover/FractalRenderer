#pragma once

#include <iostream>
#include <chrono>
#include "GLFW/glfw3.h"

namespace FractalRenderer {
	class FractalGenerator {
	public:
		static const int WIDTH = 1920;
		static const int HEIGHT = 1080;
		static const int MaxIterations = 1000;
		static constexpr float DEFAULT_JULIA_REAL = -0.8f; // -0.776
		static constexpr float DEFAULT_JULIA_IMAGINARY = 0.156f; // 0.143
		static constexpr float DEFAULT_ESCAPE_RADIUS = 100.0f;
		FractalGenerator();
		void run();
		//~FractalGenerator();

	private:
		const int width;
		const int height;
		GLFWwindow* window;
		GLuint mandelbrotShaderProgram;
		GLuint juliaShaderProgram;
		GLuint shaderProgram;
		GLuint VAO, VBO;
		int maxIterations;
		float escapeRadius;
		float juliaConstantReal;
		float juliaConstantImaginary;
		float zoomFactor;
		float speedFactor;
		float translationX;
		float translationY;
		float zoomCenterX = 0.0f;
		float zoomCenterY = 0.0f;
		std::chrono::high_resolution_clock::time_point startTime;
		enum class RenderMode { Julia, Mandelbrot };
		RenderMode renderMode;

		// Callback function for GLFW error handling
		static void errorCallback(int error, const char* description);

		// Reset the Julia set parameters to their default values
		void resetParameters();

		void render();

		void cleanup();
	};
}
