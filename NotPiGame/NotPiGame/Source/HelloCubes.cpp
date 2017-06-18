#include "../glad/include/glad/glad.h"

#include "../Headers/MainGame.h"

#include <chrono>
#include <iostream>

const int SCREENWIDTH	= 1280;
const int SCREENHEIGHT	= 720;
const std::string GAMENAME = "~Not~PiGame";

// Prototypes
void mainLoop();
void updateLoop(float deltaTime);
void drawLoop();

#pragma region DeltaTime Calculations
void StartFrame(std::chrono::high_resolution_clock::time_point& frameStart, 
	std::chrono::high_resolution_clock::time_point& frameEnd, float& deltaTime) {
	deltaTime = float(std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count() * 0.001f);
	frameStart = std::chrono::high_resolution_clock::now();
}

void EndFrame(std::chrono::high_resolution_clock::time_point& frameEnd, float& deltaTime, unsigned int& frames, float& totalTime) {
	totalTime += deltaTime;
	frames++;
		
	if (totalTime >  1.0f) {
		printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totalTime, frames / totalTime);
		totalTime -= 1.0f;
		frames = 0;
	}
		
	frameEnd = std::chrono::high_resolution_clock::now();
}
#pragma endregion

// Stuff in Global Space
MainGame*   mainGame;
Window*     window;

//###---Program Entry---###///
int main(int argc, char *argv[]) {
    // Init GLFW
    if (!glfwInit()) { std::cout << "Failed to intialise GLFW"; return false; }

    // Creating the window
    window = new Window(GAMENAME, SCREENWIDTH, SCREENHEIGHT);
    if (!window->CreateGLFWWindow(true, true)) { delete window; glfwTerminate(); return false; }

    // Init GL(AD) & Extensions
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) { std::cout << "Failed to initialize OpenGL context" << std::endl; return false; }
    window->SetWindowFlags();
    
    // Print out the renderer information
    window->PrintContextInfo();

	// Initialise MainGame
	mainGame = new MainGame(window, SCREENWIDTH, SCREENHEIGHT);
	
	// Now go do the Main Loop of the program
    mainLoop();
}

void mainLoop() {
	float deltaTime		= 0;
	float totalTime		= 0;
	unsigned int frames = 0;
	std::chrono::high_resolution_clock::time_point frameStart;
	std::chrono::high_resolution_clock::time_point frameEnd;
		
	while (!mainGame->getGameExit()) {
		// Begin DeltaTime calculations
		StartFrame(frameStart, frameEnd, deltaTime);
			
        // Update the game
        updateLoop(deltaTime);

        // Draw the game
        drawLoop();

		// Output the frame rate and amount of frames rendered	
		EndFrame(frameEnd, deltaTime, frames, totalTime);
	}
	// Before exiting - delete the MainGame
    delete window;
    window = nullptr;
	delete mainGame;
	mainGame = nullptr;
}

void updateLoop(float deltaTime) {
    // Updating the main game
    mainGame->UpdateMainGame(deltaTime);
}

void drawLoop() {
    // Clear the color buffer and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	// Now goto the MainGame Draw function
	mainGame->Draw();
	
	// After our draw we need to swap buffers to display
	glfwSwapBuffers(window->GLFWWindow);
}