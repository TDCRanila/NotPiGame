#pragma once

// Wrapper for graphic loading, "do not use for binary or text files."
class MyFiles
{
public:
	MyFiles();
	~MyFiles();
	
	int height;
	int width;
	// Loader specifically for graphics
	char* Load(char const *filename, int*, int*, int* comp);
	// Loads in the text from a text file, do use for text files
	const char* LoadText(char const *filename);
	const char* NewLoadText(char const *filename);
	// Makes a screenshot of the current frame and creates a png file - stores it in the temporay DEBUG Folder
	void Screenshot(int endX, int endY, int startX, int startY);
	int screenShotCount = 0;
	
};

