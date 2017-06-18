#include "../Headers/Input.h"

Input::Input() {
	this->Init();
}

Input::~Input() {
}

bool Input::Init() {
    // Empty

    return true;
}

bool Input::getKey(unsigned char WhatKey) {
	return Keys[WhatKey] == TRUE; // Return true or false
}