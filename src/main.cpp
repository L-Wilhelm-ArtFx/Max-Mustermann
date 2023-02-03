#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <fstream>
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm_includes.h"
#include "window.h"
#include "resource_manager.h"

#include "filedialogue.h"
#include "tilemap.h"
#include "input.h"

void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "Glfw Error: %s\n", description);
}

void checkGLErr()
{
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		std::string errName;
		switch (errCode)
		{
		case GL_INVALID_ENUM:
			errName = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errName = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errName = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			errName = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			errName = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			errName = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errName = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:
			errName = "UNKNOWN_ERR";
			break;
		}
		fprintf(stderr, "GL Error(%u): %s", errCode, errName.c_str());
	}
}

int main(int argc, char const *argv[])
{
	std::cout << "Hello World!" << std::endl;

	std::cout << "INIT Glfw:" << std::endl;
	glfwSetErrorCallback(glfw_error_callback);
	int glfwErr = glfwInit();
	if (!glfwErr)
	{
		std::cerr << "Failed" << std::endl;
		return 1;
	}
	std::cout << "Done" << std::endl;

	GWindow window;
	window.create();

	std::cout << "INIT Glew:" << std::endl;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		std::cerr << "Failed" << std::endl;
		fprintf(stderr, "Glew Error: %s\n", glewGetErrorString(glewStatus));
		return 1;
	}
	std::cout << "Done" << std::endl;

	auto shader = ResM::loadShaderProgram("sprite");
	auto shader2 = ResM::loadShaderProgram("line");

	SpriteMesh mesh;
	mesh.load({-0.5f, -0.5f, 0.0f, /**/ 0.0f, 1.0f,
			   0.5f, -0.5f, 0.0f, /**/ 1.0f, 1.0f,
			   0.5f, 0.5f, 0.0f, /**/ 1.0f, 0.0f,
			   -0.5f, 0.5f, 0.0f, /**/ 0.0f, 0.0f},
			  {0, 1, 2,
			   0, 2, 3});

	ShapeMesh mesh2;
	mesh2.load({
		-0.5f,
		-0.5f,
		0.0f,
		0.5f,
		-0.5f,
		0.0f,

		0.5f,
		-0.5f,
		0.0f,
		0.5f,
		0.5f,
		0.0f,

		0.5f,
		0.5f,
		0.0f,
		-0.5f,
		0.5f,
		0.0f,

		-0.5f,
		0.5f,
		0.0f,
		-0.5f,
		-0.5f,
		0.0f,

	});

	auto texture1 = ResM::loadTexture("1.png");
	auto texture2 = ResM::loadTexture("2.png");
	auto texture3 = ResM::loadTexture("3.png");
	auto texture4 = ResM::loadTexture("4.png");
	auto textureE = ResM::loadTexture("emoji.png");

	TilePalette tPalette;
	tPalette.setTileClass(1, std::make_shared<TileClass>(texture1, true));
	tPalette.setTileClass(2, std::make_shared<TileClass>(texture2, true));
	tPalette.setTileClass(3, std::make_shared<TileClass>(texture3, true));
	tPalette.setTileClass(4, std::make_shared<TileClass>(texture4, true));
	tPalette.setTileClass(5, std::make_shared<TileClass>(textureE, true));

	TileMap tMap(8, 8);

	// tMap.setTile(32, 32, {1});
	tMap.setTile(0, 0, {1});
	tMap.setTile(1, 0, {1});
	// tMap.setTile(32, 0, {1});
	// tMap.setTile(0, 32, {1});

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 440 core");

	bool imguidemo = true;
	bool mainMenuBar = true;
	static bool showTilePaletteWindow = false;
	bool cursorUseMouse = true;

	tileId selectedTile = 0;

	int newWidth, newHeight;

	bool shouldClose = false;
	float tilepaletteScale = 1.0;

	unsigned int cursorX = 0, cursorY = 0;
	float orthoSize = 64.0f;
	glm::vec2 camPos = glm::vec2(0.0f, 0.0f);
	glm::mat4 ortho = glm::identity<glm::mat4>();

	while (!window.shoudClose() && !shouldClose)
	{
		bool cursorMovedWithKeys = false;
		GInput::updatePrePoll();
		glfwPollEvents();
		GInput::updatePostPoll();

		int width, height;
		window.getFrameSize(&width, &height);

		cursorX = glm::clamp(cursorX, (unsigned int)0, tMap.getWidth() - 1);
		cursorY = glm::clamp(cursorY, (unsigned int)0, tMap.getHeight() - 1);

		// glm::vec2 camMovement{0.f, 0.f};
		// if (GInput::isPressed(GLFW_KEY_RIGHT))
		// {
		// 	camMovement.x += 1.0f;
		// }
		// if (GInput::isPressed(GLFW_KEY_LEFT))
		// {
		// 	camMovement.x -= 1.0f;
		// }
		// if (GInput::isPressed(GLFW_KEY_UP))
		// {
		// 	camMovement.y += 1.0f;
		// }
		// if (GInput::isPressed(GLFW_KEY_DOWN))
		// {
		// 	camMovement.y += 1.0f;
		// }

		if (GInput::isJustPressed(GLFW_KEY_0))
			selectedTile = 0;
		if (GInput::isJustPressed(GLFW_KEY_1))
			selectedTile = 1;
		if (GInput::isJustPressed(GLFW_KEY_2))
			selectedTile = 2;
		if (GInput::isJustPressed(GLFW_KEY_3))
			selectedTile = 3;
		if (GInput::isJustPressed(GLFW_KEY_4))
			selectedTile = 4;
		if (GInput::isJustPressed(GLFW_KEY_5))
			selectedTile = 5;
		if (GInput::isJustPressed(GLFW_KEY_6))
			selectedTile = 6;
		if (GInput::isJustPressed(GLFW_KEY_7))
			selectedTile = 7;
		if (GInput::isJustPressed(GLFW_KEY_8))
			selectedTile = 8;
		if (GInput::isJustPressed(GLFW_KEY_9))
			selectedTile = 9;

		if (GInput::isJustPressed(GLFW_KEY_RIGHT))
		{
			if (GInput::isPressed(GLFW_KEY_LEFT_CONTROL) || cursorUseMouse)
			{
				camPos.x++;
			}
			else
			{
				cursorMovedWithKeys = true;
				if (cursorX < tMap.getWidth() - 1)
				{
					cursorX++;
				}
			}
		}
		if (GInput::isJustPressed(GLFW_KEY_LEFT))
		{
			if (GInput::isPressed(GLFW_KEY_LEFT_CONTROL) || cursorUseMouse)
			{
				camPos.x--;
			}
			else
			{
				cursorMovedWithKeys = true;
				if (cursorX > 0)
				{
					cursorX--;
				}
			}
		}
		if (GInput::isJustPressed(GLFW_KEY_UP))
		{
			if (GInput::isPressed(GLFW_KEY_LEFT_CONTROL) || cursorUseMouse)
			{
				camPos.y++;
			}
			else
			{
				cursorMovedWithKeys = true;
				if (cursorY < tMap.getHeight() - 1)
				{
					cursorY++;
				}
			}
		}
		if (GInput::isJustPressed(GLFW_KEY_DOWN))
		{
			if (GInput::isPressed(GLFW_KEY_LEFT_CONTROL) || cursorUseMouse)
			{
				camPos.y--;
			}
			else
			{
				cursorMovedWithKeys = true;
				if (cursorY > 0)
				{
					cursorY--;
				}
			}
		}

		if (cursorUseMouse)
		{
			glm::vec2 mPos = GInput::getMousePos();
			mPos /= glm::vec2(width, height);

			float orthoScale = 1 / orthoSize;

			float xw = width * .5f * orthoScale;
			float yh = height * .5f * orthoScale;

			cursorX = glm::lerp(-xw, xw, mPos.x) + camPos.x;
			cursorY = glm::lerp(yh, -yh, mPos.y) + camPos.y;
		}

		if (GInput::isPressed(GLFW_KEY_SPACE) || GInput::isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			tMap.setTile(cursorX, cursorY, {selectedTile});
		}

		glViewport(0, 0, width, height);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		GLuint shadID;

		// shader->use();
		// shadID = shader->getProgramId()

		float orthoScale = 1 / orthoSize;
		ortho = glm::ortho(
			-width * .5f * orthoScale,
			width * .5f * orthoScale,
			-height * .5f * orthoScale,
			height * .5f * orthoScale,
			0.0f,
			100.0f);

		glm::vec2 cursorVec2{cursorX + .5f, cursorY + .5f};
		if (cursorMovedWithKeys)
		{
			camPos = glm::clamp(camPos, cursorVec2 - glm::vec2(width, height) * 0.33f * orthoScale, cursorVec2 + glm::vec2(width, height) * 0.33f * orthoScale);
		}

		glm::mat4 camTransform = glm::identity<glm::mat4>();
		camTransform = glm::translate(glm::vec3(-camPos.x, -camPos.y, 0.0f));
		// glUniformMatrix4fv(glGetUniformLocation(shadID, "projection"), 1, false, glm::value_ptr(ortho));
		// texture->use();
		// mesh.draw();

		for (unsigned int y = 0; y < tMap.getHeight(); ++y)
		{
			for (unsigned int x = 0; x < tMap.getWidth(); ++x)
			{
				auto tile = tMap.getTile(x, y);

				if (tile.type == 0)
					continue;

				auto tileClassWeakPtr = tPalette.getTileClass(tile.type);

				if (tileClassWeakPtr.expired())
					continue;

				auto tileClass = tileClassWeakPtr.lock();
				auto texture = tileClass->texture;

				shader->use();
				GLuint shadID = shader->getProgramId();
				glm::mat4 transform = glm::identity<glm::mat4>();
				transform = glm::translate(transform, glm::vec3((float)x + .5f, (float)y + .5f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shadID, "projection"), 1, false, glm::value_ptr(ortho));
				glUniformMatrix4fv(glGetUniformLocation(shadID, "transform"), 1, false, glm::value_ptr(transform));
				glUniformMatrix4fv(glGetUniformLocation(shadID, "viewTransform"), 1, false, glm::value_ptr(camTransform));
				texture->use();
				mesh.draw();
			}
		}

		// Draw TileMap Bounding Box
		{
			shader2->use();
			shadID = shader2->getProgramId();
			glm::mat4 transform = glm::identity<glm::mat4>();
			float w = tMap.getWidth();
			float h = tMap.getHeight();
			transform = glm::translate(transform, glm::vec3(w * .5f, h * .5f, 0.0f));
			transform = glm::scale(transform, glm::vec3(w, h, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shadID, "projection"), 1, false, glm::value_ptr(ortho));
			glUniformMatrix4fv(glGetUniformLocation(shadID, "transform"), 1, false, glm::value_ptr(transform));
			glUniformMatrix4fv(glGetUniformLocation(shadID, "viewTransform"), 1, false, glm::value_ptr(camTransform));
			glUniform4f(glGetUniformLocation(shadID, "uCol1"), 1.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(glGetUniformLocation(shadID, "uCol2"), 0.5f, 0.0f, 0.0f, 1.0f);
			glLineWidth(2.0f);
			mesh2.draw();
		}

		// Draw Cursor
		{
			shader2->use();
			shadID = shader2->getProgramId();
			glm::mat4 transform = glm::identity<glm::mat4>();
			float w = tMap.getWidth();
			float h = tMap.getHeight();
			transform = glm::translate(transform, glm::vec3(cursorX + .5f, cursorY + .5f, 0.0f));
			// transform = glm::scale(transform, glm::vec3(w, h, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shadID, "projection"), 1, false, glm::value_ptr(ortho));
			glUniformMatrix4fv(glGetUniformLocation(shadID, "transform"), 1, false, glm::value_ptr(transform));
			glUniformMatrix4fv(glGetUniformLocation(shadID, "viewTransform"), 1, false, glm::value_ptr(camTransform));
			glUniform4f(glGetUniformLocation(shadID, "uCol1"), 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(glGetUniformLocation(shadID, "uCol2"), 0.0f, 0.0f, 0.0f, 1.0f);
			glLineWidth(2.0f);
			mesh2.draw();
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();

		// ImGui::ShowDemoWindow(&imguidemo);

		bool openResize = false;
		bool openNew = false;
		bool openSave = false;
		bool openOpen = false;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New..."))
				{
					printf("File > New...\n");
					openNew = true;
				}
				if (ImGui::MenuItem("Open"))
				{
					printf("File > Open\n");
					openOpen = true;
				}
				if (ImGui::MenuItem("Save As..."))
				{
					printf("File > Save As...\n");
					openSave = true;
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					printf("File > Exit\n");
					shouldClose = true;
				}

				// ImGui::Image((void*)(intptr_t)(texture->getTexId()), ImVec2(ImGui::GetContentRegionMax().x, texture->getHeight()));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Resize"))
				{
					printf("File > Resize\n");
					newWidth = tMap.getWidth();
					newHeight = tMap.getHeight();
					openResize = true;
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();
			ImGui::SetNextItemWidth(200.0f);
			ImGui::SliderFloat("Zoom", &orthoSize, 0.1f, 100.0f);
			if (ImGui::Button("ResetZoom"))
			{
				orthoSize = 64.f;
			}

			ImGui::Separator();
			ImGui::Checkbox("Mouse Cursor", &cursorUseMouse);

			ImGui::EndMainMenuBar();
		}

		if (openResize)
		{
			ImGui::OpenPopup("Resize");
		}

		if (openNew)
		{
			ImGui::OpenPopup("New");
		}

		if (openOpen)
		{
			ImGui::OpenPopup("Open");
		}

		if (openSave)
		{
			ImGui::OpenPopup("Save");
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Resize", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Width: %u Height: %u", tMap.getWidth(), tMap.getHeight());
			ImGui::InputInt("New Width", &newWidth, 1, 10);
			ImGui::InputInt("New Height", &newHeight, 1, 10);
			newWidth = glm::clamp(newWidth, 1, 64);
			newHeight = glm::clamp(newHeight, 1, 64);
			if (newWidth < tMap.getWidth() || newHeight < tMap.getHeight())
			{
				ImGui::Text("Note: New dimensions are smaller than current dimensions.");
			}
			if (ImGui::Button("Resize!"))
			{
				tMap.resize(newWidth, newHeight);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("New", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputInt("New Width", &newWidth, 1, 10);
			ImGui::InputInt("New Height", &newHeight, 1, 10);
			newWidth = glm::clamp(newWidth, 1, 64);
			newHeight = glm::clamp(newHeight, 1, 64);
			if (ImGui::Button("Create!"))
			{
				tMap = TileMap(newWidth, newHeight);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Open", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string path = "./levels";
			std::string compString = ".yaml";
			if (ImGui::BeginListBox("Choose File", ImVec2(350, 300)))
			{
				for (const auto &entry : std::filesystem::directory_iterator(path))
				{
					std::string fName = entry.path().filename().generic_u8string();
					if (fName.length() >= compString.length())
					{
						if (0 == fName.compare(fName.length() - compString.length(), compString.length(), compString))
						{
							bool selected = false;
							ImGui::Selectable(fName.c_str(), &selected);
							if (selected)
							{
								YAML::Node node = YAML::LoadFile(entry.path().u8string().c_str());
								tMap.fromYAML(node["tilemap"]);
								ImGui::CloseCurrentPopup();
								break;
							}
						}
					}
				}
				ImGui::EndListBox();
			}

			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static char buff[64] = "my_level";
			ImGui::InputText("Filename", buff, IM_ARRAYSIZE(buff));
			if (ImGui::Button("Save!"))
			{
				YAML::Node node;
				node["version"] = "0.1.0";
				node["tilemap"] = tMap.toYAML();

				std::ofstream of;
				of.open("./levels/" + std::string(buff) + ".yaml");
				YAML::Emitter emit{of};
				emit << node;
				of.flush();
				of.close();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Begin("Tile Palette", nullptr, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::SliderFloat("Scale", &tilepaletteScale, 0.1f, 4.0f);
				ImGui::EndMenuBar();
			}

			float size = 64.0f * tilepaletteScale;
			ImGui::Text("0");
			ImGui::SameLine();
			if (ImGui::Button("Empty"))
			{
				selectedTile = 0;
			}
			if (selectedTile == 0)
			{
				ImGui::SameLine();
				ImGui::Text("(Selected)");
			}
			for (auto it = tPalette.tileMap.begin(); it != tPalette.tileMap.end(); ++it)
			{
				ImVec2 imSize{size, size};
				std::string nId = "TPal##" + std::to_string(it->first);
				auto texId = it->second->texture->getTexId();
				ImGui::Text("%u", it->first);
				ImGui::SameLine();
				if (ImGui::ImageButton(nId.c_str(), (void *)(intptr_t)texId, imSize))
				{
					selectedTile = it->first;
				}
				if (selectedTile == it->first)
				{
					ImGui::SameLine();
					ImGui::Text("(Selected)");
				}
				bool sleek = false;
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.swapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	window.destroy();

	glfwTerminate();
}