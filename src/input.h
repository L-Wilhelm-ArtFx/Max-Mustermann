#pragma once

#include <GLFW/glfw3.h>
#include "glm_includes.h"

namespace GInput {

	void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	bool isPressed(int key);

	bool isJustPressed(int key);

	bool isMousePressed(int button);

	bool isMouseJustPressed(int button);

	glm::vec2 getMousePos();

	void updatePrePoll();
	void updatePostPoll();
}