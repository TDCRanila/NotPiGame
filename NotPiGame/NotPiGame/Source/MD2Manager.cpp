#include "../Headers/MD2Manager.h"

#include "../Headers/Utilities.h"
#include "../Headers/Texture.h"

////#include <string>

Frame::Frame(std::string nameOfFrame) {
	this->nameFrame = nameOfFrame;
}

Frame::~Frame() {
	
}

FrameState::FrameState(std::string nameFrameState) {
	this->nameFrameState = nameFrameState;
}

FrameState::~FrameState() {	
	
}

MD2Manager::MD2Manager() {
	//Empty
}
MD2Manager::~MD2Manager() {
	//Empty
}

// Predefined Normals for the MD2 Model
float MD2NormalsTable[162][3] = {
	{ -0.525731f,  0.000000f,  0.850651f },
	{ -0.442863f,  0.238856f,  0.864188f },
	{ -0.295242f,  0.000000f,  0.955423f },
	{ -0.309017f,  0.500000f,  0.809017f },
	{ -0.162460f,  0.262866f,  0.951056f },
	{  0.000000f,  0.000000f,  1.000000f },
	{  0.000000f,  0.850651f,  0.525731f },
	{ -0.147621f,  0.716567f,  0.681718f },
	{  0.147621f,  0.716567f,  0.681718f },
	{  0.000000f,  0.525731f,  0.850651f },
	{  0.309017f,  0.500000f,  0.809017f },
	{  0.525731f,  0.000000f,  0.850651f },
	{  0.295242f,  0.000000f,  0.955423f },
	{  0.442863f,  0.238856f,  0.864188f },
	{  0.162460f,  0.262866f,  0.951056f },
	{ -0.681718f,  0.147621f,  0.716567f },
	{ -0.809017f,  0.309017f,  0.500000f },
	{ -0.587785f,  0.425325f,  0.688191f },
	{ -0.850651f,  0.525731f,  0.000000f },
	{ -0.864188f,  0.442863f,  0.238856f },
	{ -0.716567f,  0.681718f,  0.147621f },
	{ -0.688191f,  0.587785f,  0.425325f },
	{ -0.500000f,  0.809017f,  0.309017f },
	{ -0.238856f,  0.864188f,  0.442863f },
	{ -0.425325f,  0.688191f,  0.587785f },
	{ -0.716567f,  0.681718f, -0.147621f },
	{ -0.500000f,  0.809017f, -0.309017f },
	{ -0.525731f,  0.850651f,  0.000000f },
	{  0.000000f,  0.850651f, -0.525731f },
	{ -0.238856f,  0.864188f, -0.442863f },
	{  0.000000f,  0.955423f, -0.295242f },
	{ -0.262866f,  0.951056f, -0.162460f },
	{  0.000000f,  1.000000f,  0.000000f },
	{  0.000000f,  0.955423f,  0.295242f },
	{ -0.262866f,  0.951056f,  0.162460f },
	{  0.238856f,  0.864188f,  0.442863f },
	{  0.262866f,  0.951056f,  0.162460f },
	{  0.500000f,  0.809017f,  0.309017f },
	{  0.238856f,  0.864188f, -0.442863f },
	{  0.262866f,  0.951056f, -0.162460f },
	{  0.500000f,  0.809017f, -0.309017f },
	{  0.850651f,  0.525731f,  0.000000f },
	{  0.716567f,  0.681718f,  0.147621f },
	{  0.716567f,  0.681718f, -0.147621f },
	{  0.525731f,  0.850651f,  0.000000f },
	{  0.425325f,  0.688191f,  0.587785f },
	{  0.864188f,  0.442863f,  0.238856f },
	{  0.688191f,  0.587785f,  0.425325f },
	{  0.809017f,  0.309017f,  0.500000f },
	{  0.681718f,  0.147621f,  0.716567f },
	{  0.587785f,  0.425325f,  0.688191f },
	{  0.955423f,  0.295242f,  0.000000f },
	{  1.000000f,  0.000000f,  0.000000f },
	{  0.951056f,  0.162460f,  0.262866f },
	{  0.850651f, -0.525731f,  0.000000f },
	{  0.955423f, -0.295242f,  0.000000f },
	{  0.864188f, -0.442863f,  0.238856f },
	{  0.951056f, -0.162460f,  0.262866f },
	{  0.809017f, -0.309017f,  0.500000f },
	{  0.681718f, -0.147621f,  0.716567f },
	{  0.850651f,  0.000000f,  0.525731f },
	{  0.864188f,  0.442863f, -0.238856f },
	{  0.809017f,  0.309017f, -0.500000f },
	{  0.951056f,  0.162460f, -0.262866f },
	{  0.525731f,  0.000000f, -0.850651f },
	{  0.681718f,  0.147621f, -0.716567f },
	{  0.681718f, -0.147621f, -0.716567f },
	{  0.850651f,  0.000000f, -0.525731f },
	{  0.809017f, -0.309017f, -0.500000f },
	{  0.864188f, -0.442863f, -0.238856f },
	{  0.951056f, -0.162460f, -0.262866f },
	{  0.147621f,  0.716567f, -0.681718f },
	{  0.309017f,  0.500000f, -0.809017f },
	{  0.425325f,  0.688191f, -0.587785f },
	{  0.442863f,  0.238856f, -0.864188f },
	{  0.587785f,  0.425325f, -0.688191f },
	{  0.688191f,  0.587785f, -0.425325f },
	{ -0.147621f,  0.716567f, -0.681718f },
	{ -0.309017f,  0.500000f, -0.809017f },
	{  0.000000f,  0.525731f, -0.850651f },
	{ -0.525731f,  0.000000f, -0.850651f },
	{ -0.442863f,  0.238856f, -0.864188f },
	{ -0.295242f,  0.000000f, -0.955423f },
	{ -0.162460f,  0.262866f, -0.951056f },
	{  0.000000f,  0.000000f, -1.000000f },
	{  0.295242f,  0.000000f, -0.955423f },
	{  0.162460f,  0.262866f, -0.951056f },
	{ -0.442863f, -0.238856f, -0.864188f },
	{ -0.309017f, -0.500000f, -0.809017f },
	{ -0.162460f, -0.262866f, -0.951056f },
	{  0.000000f, -0.850651f, -0.525731f },
	{ -0.147621f, -0.716567f, -0.681718f },
	{  0.147621f, -0.716567f, -0.681718f },
	{  0.000000f, -0.525731f, -0.850651f },
	{  0.309017f, -0.500000f, -0.809017f },
	{  0.442863f, -0.238856f, -0.864188f },
	{  0.162460f, -0.262866f, -0.951056f },
	{  0.238856f, -0.864188f, -0.442863f },
	{  0.500000f, -0.809017f, -0.309017f },
	{  0.425325f, -0.688191f, -0.587785f },
	{  0.716567f, -0.681718f, -0.147621f },
	{  0.688191f, -0.587785f, -0.425325f },
	{  0.587785f, -0.425325f, -0.688191f },
	{  0.000000f, -0.955423f, -0.295242f },
	{  0.000000f, -1.000000f,  0.000000f },
	{  0.262866f, -0.951056f, -0.162460f },
	{  0.000000f, -0.850651f,  0.525731f },
	{  0.000000f, -0.955423f,  0.295242f },
	{  0.238856f, -0.864188f,  0.442863f },
	{  0.262866f, -0.951056f,  0.162460f },
	{  0.500000f, -0.809017f,  0.309017f },
	{  0.716567f, -0.681718f,  0.147621f },
	{  0.525731f, -0.850651f,  0.000000f },
	{ -0.238856f, -0.864188f, -0.442863f },
	{ -0.500000f, -0.809017f, -0.309017f },
	{ -0.262866f, -0.951056f, -0.162460f },
	{ -0.850651f, -0.525731f,  0.000000f },
	{ -0.716567f, -0.681718f, -0.147621f },
	{ -0.716567f, -0.681718f,  0.147621f },
	{ -0.525731f, -0.850651f,  0.000000f },
	{ -0.500000f, -0.809017f,  0.309017f },
	{ -0.238856f, -0.864188f,  0.442863f },
	{ -0.262866f, -0.951056f,  0.162460f },
	{ -0.864188f, -0.442863f,  0.238856f },
	{ -0.809017f, -0.309017f,  0.500000f },
	{ -0.688191f, -0.587785f,  0.425325f },
	{ -0.681718f, -0.147621f,  0.716567f },
	{ -0.442863f, -0.238856f,  0.864188f },
	{ -0.587785f, -0.425325f,  0.688191f },
	{ -0.309017f, -0.500000f,  0.809017f },
	{ -0.147621f, -0.716567f,  0.681718f },
	{ -0.425325f, -0.688191f,  0.587785f },
	{ -0.162460f, -0.262866f,  0.951056f },
	{  0.442863f, -0.238856f,  0.864188f },
	{  0.162460f, -0.262866f,  0.951056f },
	{  0.309017f, -0.500000f,  0.809017f },
	{  0.147621f, -0.716567f,  0.681718f },
	{  0.000000f, -0.525731f,  0.850651f },
	{  0.425325f, -0.688191f,  0.587785f },
	{  0.587785f, -0.425325f,  0.688191f },
	{  0.688191f, -0.587785f,  0.425325f },
	{ -0.955423f,  0.295242f,  0.000000f },
	{ -0.951056f,  0.162460f,  0.262866f },
	{ -1.000000f,  0.000000f,  0.000000f },
	{ -0.850651f,  0.000000f,  0.525731f },
	{ -0.955423f, -0.295242f,  0.000000f },
	{ -0.951056f, -0.162460f,  0.262866f },
	{ -0.864188f,  0.442863f, -0.238856f },
	{ -0.951056f,  0.162460f, -0.262866f },
	{ -0.809017f,  0.309017f, -0.500000f },
	{ -0.864188f, -0.442863f, -0.238856f },
	{ -0.951056f, -0.162460f, -0.262866f },
	{ -0.809017f, -0.309017f, -0.500000f },
	{ -0.681718f,  0.147621f, -0.716567f },
	{ -0.681718f, -0.147621f, -0.716567f },
	{ -0.850651f,  0.000000f, -0.525731f },
	{ -0.688191f,  0.587785f, -0.425325f },
	{ -0.587785f,  0.425325f, -0.688191f },
	{ -0.425325f,  0.688191f, -0.587785f },
	{ -0.425325f, -0.688191f, -0.587785f },
	{ -0.587785f, -0.425325f, -0.688191f },
	{ -0.688191f, -0.587785f, -0.425325f }
};

bool MD2Manager::loadMD2(const char* md2FilePath, const char* texFilePath, MD2Package &package) {
	printf("###########################################\n");
	printf("#Loading MD2 File: %s#\n#", md2FilePath);
	
#pragma region Opening MD2
	///---Openening the binary MD2 Fil---///
	FILE* filePointer;
	filePointer = fopen(md2FilePath, "rb");
	
	// Make sure we have a valid file pointer
	if (!filePointer) { printf("Error openening MD2File: %s", md2FilePath); return false; }
	
	// Read the header data and store it in our m_Header member variable
	fread(&package.header, 1, sizeof(MD2Header), filePointer);
	// Make sure the version is 8
	if (package.header.version != 8) {
		// Display an error message for bad file format, then stop loading.
		printf("Invalid file format (Version not 8): %s!", md2FilePath);
		return false;
	}

#ifdef ACTIVATE_ERROR_CHECKING
	printf("-------------------------------------------\n");
	printf("Magic Number: %d\n", package.header.magic);
	printf("Version: %d\n", package.header.version);
	printf("SkinWidth: %d\n", package.header.skinWidth);
	printf("SkinHeight: %d\n", package.header.skinHeight);
	printf("Frame Size: %d\n", package.header.frameSize);
		
	printf("Num Skins: %d\n", package.header.numSkins);
	printf("Num Vertices: %d\n", package.header.numVertices);
	printf("Num TexCoords: %d\n", package.header.numTexCoords);
	printf("Num Triangles: %d\n", package.header.numTriangles);
	printf("Num GLCommands: %d\n", package.header.numGlCommands);
	printf("Num Frames: %d\n", package.header.numFrames);
		
	printf("offsetSkins: %d\n", package.header.offsetSkins);
	printf("offsetTexCoords: %d\n", package.header.offsetTexCoords);
	printf("offsetTriangles: %d\n", package.header.offsetTriangles);
	printf("offsetFrames: %d\n", package.header.offsetFrames);
	printf("offsetGLCommands: %d\n", package.header.offsetGlCommands);
	printf("offsetEnd: %d\n", package.header.offsetEnd);
	printf("-------------------------------------------\n");
#endif // ACTIVATE_ERROR_CHECKING
	
#pragma endregion	
	
#pragma region Reading the MD2 Data	
	// Here we allocate all of our memory from the header's information
	package.skins     = (struct MD2Skin*) malloc(sizeof(struct MD2Skin)         * package.header.numSkins);
	package.texCoords = (struct MD2TexCoord*) malloc(sizeof(struct MD2TexCoord) * package.header.numTexCoords);
	package.triangles = (struct MD2Triangle*) malloc(sizeof(struct MD2Triangle) * package.header.numTriangles);
	package.frames    = (struct MD2Frame*) malloc(sizeof(struct MD2Frame)       * package.header.numFrames);
	
	// Next, we start reading in the data by seeking to our skin names offset
	fseek(filePointer, package.header.offsetSkins, SEEK_SET);
	// Depending on the skin count, we read in each skin for this model
	fread(package.skins, sizeof(MD2Skin), package.header.numSkins, filePointer);
	
	// Move the file pointer to the position in the file for texture coordinates
	fseek(filePointer, package.header.offsetTexCoords, SEEK_SET);
	// Read in all the texture coordinates in one fell swoop
	fread(package.texCoords, sizeof(MD2TexCoord), package.header.numTexCoords, filePointer);

	// Move the file pointer to the triangles data
	fseek(filePointer, package.header.offsetTriangles, SEEK_SET);
	// Read in the face data for each triangle (vertex and texCoord indices)
	fread(package.triangles, sizeof(MD2Triangle), package.header.numTriangles, filePointer);
			
	// Move the file pointer to the vertices (frames)
	fseek(filePointer, package.header.offsetFrames, SEEK_SET);
			
	printf("-------------------------------------------\n");
	std::vector<Frame*> framesTemp;
	for (int i = 0; i < package.header.numFrames; i++) {
		// Allocate the memory for the first frame of animation's vertices
		package.frames[i].vertices = (struct MD2Vertex*) malloc(sizeof(struct MD2Triangle) * package.header.numVertices);
		
		// Reading the data of the frame
		fread(package.frames[i].scale, sizeof(float) * 3, 1, filePointer);
		fread(package.frames[i].translate, sizeof(float) * 3, 1, filePointer);
		fread(package.frames[i].strName, sizeof(char), 16, filePointer);
		fread(package.frames[i].vertices, sizeof(struct MD2Vertex), package.header.numVertices, filePointer);
		
		// Put the frame type in a vector of frame types
		std::string str(package.frames[i].strName, package.frames[i].strName + sizeof(package.frames[i].strName) / sizeof(package.frames[i].strName[0]));
		
		// Put all frames into a temporary frame vector
		Frame* a = new Frame(str);
		framesTemp.push_back(a);
		
#ifdef ACTIVATE_ERROR_CHECKING
		//* PRINTS OUT ALL THE FRAMES INSIDE OF THE MD2 FILE
		printf("%s\n", str.c_str());
		//*/
#endif // ACTIVATE_ERROR_CHECKING
	}
	
	// Do all the frames
	std::string frameStateName;
	bool isSame		= false;
	bool isStart	= true;
	int countPerFrameState = 1;
	for (int i = 0; i <= framesTemp.size(); ++i) {
		std::string frameName;
		std::string frameNum;
		if (i == framesTemp.size()) {
			frameName = "Ending";
			frameNum  = "123";
		} else {
			Utilities::seperateIntFromStringAndStore(framesTemp[i]->nameFrame, frameName, frameNum);
		}
		
		// Make the frame state name
		std::string newFrameName;
		newFrameName.append(frameName);
		
		//Check if the frameNum string has 3 digits if so add the first digit to the frameName because it is a seperate framestate
		if (frameNum.length() >= 3) {
			newFrameName += frameNum[0];
		}
		
		// If the name of the current framestate isn't the same as this new frameName then isSame = false
		// We then set the new frameState name to the current FrameState
		if (frameStateName == newFrameName) {
			isSame = true;
			countPerFrameState++;
		} else { 
			isSame = false;
		}
		
		if (isSame == false) {
			if (isStart == false) {
				// Create the Frame State and set the correct values
				FrameState* newFrameState = new FrameState(frameStateName);
				newFrameState->amountOfFrames = countPerFrameState;
				newFrameState->offsetAllFrames = i - countPerFrameState;
				package.frameStates.push_back(newFrameState);
			}
			// Now set the new frameState name
			frameStateName = newFrameName;
			
			countPerFrameState = 1;
		}
		isStart = false;
	}
			
	/* DEBUG: PRINTS OUT ALL THE OFFSETS BETWEEN THE FRAMES
	printf("---\n");
	for (int i = 0; i < package.frameStates.size(); i++) {
		printf("UniqueFrameStates: %s - Offset: %d\n", package.frameStates[i]->nameFrameState.c_str(), package.frameStates[i]->offsetFrameState);
	}
	printf("---\n");
	//*/

#pragma region Texture Creation
		
	// Loading in the texture of the MD2
	if (package.header.numSkins != -1) {
		Texture texture(texFilePath);
		package.textureID = texture.textureID;
	}
		
#pragma endregion
		
printf("-------------------------------------------\n");
#pragma endregion	
	
#pragma region Creating the buffer
	
	int length = package.header.numFrames * package.header.numTriangles * 3 * 8;
	GLfloat* buffer = new GLfloat[length];
	
	for (int k = 0; k < package.header.numFrames; k++) {
		for (int i = 0; i < package.header.numTriangles; ++i) {
			for (int j = 0; j < 3; ++j) {
				MD2Frame* frame = &package.frames[k];
				MD2Vertex vert = frame->vertices[package.triangles[i].vertex[j]];

				GLfloat s = (GLfloat)package.texCoords[package.triangles[i].st[j]].u / package.header.skinWidth;
				GLfloat t = (GLfloat)package.texCoords[package.triangles[i].st[j]].v / package.header.skinHeight;

				int pos = (k * package.header.numTriangles * 3 * 8 + i * 3 * 8) + (j * 8);

				buffer[pos + 0] = MD2NormalsTable[vert.normal][0];
				buffer[pos + 1] = MD2NormalsTable[vert.normal][1];
				buffer[pos + 2] = MD2NormalsTable[vert.normal][2];
				
				buffer[pos + 3] = s;
				buffer[pos + 4] = t;

				buffer[pos + 5] = (frame->scale[0] * vert.vertex[0]) + frame->translate[0];
				buffer[pos + 6] = (frame->scale[2] * vert.vertex[2]) + frame->translate[2];
				buffer[pos + 7] = (frame->scale[1] * vert.vertex[1]) + frame->translate[1];
			}
		}
	}
	
	// Creating the vbo for this package
	if (buffer > 0) {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, (length * sizeof(GLfloat)), buffer, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		package.vb = vbo;
	}
	
#pragma endregion
	
#pragma region Clean Up
	// Clean up after everything
	fclose(filePointer);
	delete[]buffer;
	for (int i = 0; i < package.header.numFrames; i++) {
		delete package.frames[i].vertices;
	}
	if (package.skins)		delete[] package.skins;	// Free the skins data
	if (package.texCoords)	delete package.texCoords;	// Free the texture coord data
	if (package.triangles)	delete package.triangles;	// Free the triangle face data
	if (package.frames)	delete package.frames;		// Free the frames of animation

#pragma endregion
	
	printf("#Succesfully loaded the MD2 File...       #\n");
	printf("###########################################\n");
	return true; // Returns true if we have succesfully loaded in the MD2 File
}