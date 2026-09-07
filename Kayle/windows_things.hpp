#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shobjidl.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

std::string openFileDialog(HWND parentWindow = nullptr);