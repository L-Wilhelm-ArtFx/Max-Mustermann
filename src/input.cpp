#include "input.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

std::unordered_map<int, bool> keyMap;
std::unordered_map<int, bool> mouseButtonMap;
std::unordered_set<int> justPressedSet;
std::unordered_set<int> justPressedMouseSet;
glm::vec2 mousePos;

void GInput::glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;
	if(action == GLFW_PRESS){
		justPressedSet.insert(key);
	}
	keyMap.insert_or_assign(key, pressed);
	printf("KEY %d %d\n", key, pressed);
}

void GInput::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
	mousePos.x = xpos;
	mousePos.y = ypos;
}

void GInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;
	if(action == GLFW_PRESS){
		justPressedMouseSet.insert(button);
	}
	mouseButtonMap.insert_or_assign(button, pressed);
	printf("MButton %d %d\n", button, pressed);
}

glm::vec2 GInput::getMousePos(){
	return mousePos;
}

bool GInput::isPressed(int key)
{
	auto k = keyMap.find(key);
	return k != keyMap.end() && (*k).second;
}

bool GInput::isJustPressed(int key)
{
	auto k = justPressedSet.find(key);
	return k != justPressedSet.end() && *k;
}

bool GInput::isMousePressed(int button)
{
	auto k = mouseButtonMap.find(button);
	return k != mouseButtonMap.end() && (*k).second;
}

bool GInput::isMouseJustPressed(int button)
{
	auto k = justPressedMouseSet.find(button);
	return k != justPressedMouseSet.end() && *k;
}

void GInput::updatePrePoll(){
	justPressedSet.clear();
	justPressedMouseSet.clear();
}

void GInput::updatePostPoll(){
	if(ImGui::GetIO().WantCaptureKeyboard){
		keyMap.clear();
		justPressedSet.clear();
	}
	if(ImGui::GetIO().WantCaptureMouse){
		mouseButtonMap.clear();
		justPressedMouseSet.clear();
	}
}