#include "../Headers/MyFiles.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Headers/stb_image_write.h"

#include "../glad/include/glad/glad.h"

#include <iostream>
#include <fstream>
#include <string>

MyFiles::MyFiles() {
	// Empty
}

MyFiles::~MyFiles() {
	// Empty
}

char* MyFiles::Load(char const *filename, int* width, int* height, int* comp) { 
	unsigned char *data = stbi_load(filename, width, height, comp, 4); // ask it to load 4 componants since its rgba
	return (char*) data;
}

const char* MyFiles::LoadText(char const *filename) {
	std::string line;
	std::ifstream myfile(filename);
	
	if (myfile.is_open()) {
		getline(myfile, line, (char) myfile.eof()); // *this.eof isn't ideal but it works
		myfile.close();
	} else {
		printf("Unable to open: %s\n", filename);
	}
	
	return line.c_str();
}

const char* MyFiles::NewLoadText(char const *filename) {
	std::ifstream file(filename);
	std::string str;
	std::string file_contents;
	if (file.is_open()) {
		while (std::getline(file, str))	{
			file_contents += str;
			file_contents.push_back('\n');
		}  
	} else {
		printf("Unable to open: %s\n", filename);
	}
	return file_contents.c_str();
}

void MyFiles::Screenshot(GLint endX, GLint endY, GLint startX, GLint startY) {
	GLubyte *pixels = new GLubyte[3 * endX * endY];

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadPixels(startX, startY, endX, endY, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	std::string imageName = "PiGame - ScreenShot - #"; // Standard file name
	imageName.append(std::to_string(screenShotCount)); // This is to counter act overwriting the screenshot
	imageName.append(".png"); // Make sure the extension is a png.
	printf("Taking ScreenShot - FileName: %s", imageName.c_str());
	
	screenShotCount++; // Add 1 to count so that the filename is never the same in the same runtime
	
	stbi_write_png(imageName.c_str(), endX, endY, 3, pixels, endX * 3);

	delete[] pixels;
}