#pragma once

#include "../Headers/Utilities.h"

struct MouseData {
	int	PositionX;
	int	PositionY;
	unsigned char	LeftButton;
	unsigned char	MidButton;
	unsigned char	RightButton;
};

class Input
{
public:
	Input();
	~Input();
		
	bool Init();
	int AreYouMyKeyboard();
	static void* ProcessMouseThread(void* arg);
	static void* ProcessKeyboardThread(void* arg);

	// To check if a key has been pressed
	bool getKey(unsigned char WhatKey);
	
	char Keys[512];
	MouseData	TheMouse;
	
	int iterations;
	bool KeyPressed;
};
