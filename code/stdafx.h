#ifndef STDAFX_H
#define STDAFX_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/debug.h"
#include "engine/log.h"
#include "engine/memorymanager.h"
#include "engine/resourcemanager.h"

#endif // !STDAFX_H
