/*			  ,___,
  /\/\			|
 //\/\\cClain (_|orgensen

 VibeDiff helps you compare and contrast things to find the best among them (whatever "best" means to you).
*/

#define GLFW_DLL
#define GLEW_NO_GLU
#include <glew.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Includes OpenGL headers

#include <fstream>
#include <functional>
#include <limits>
#include <string>

struct Color {
	constexpr Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b) : a(a), r(r), g(g), b(b) {}
	constexpr Color(uint32_t argb) : a((argb >> 24) & 0xFF), r((argb >> 16) & 0xFF), g((argb >> 8) & 0xFF), b(argb & 0xFF) {}

	inline Color operator+(const Color& other) const {
		return Color(addCapped(a, other.a), addCapped(r, other.r), addCapped(g, other.g), addCapped(b, other.b));
	}
	operator ImU32() { return IM_COL32(r,g,b,a); }

	uint8_t a, r, g, b;
private:
	inline uint8_t addCapped(uint8_t a, uint8_t b) const { return a > 255 - b ? 255 : a + b; }
};
namespace Colors {
	constexpr Color Transparent{ 0, 0, 0, 0 };
	constexpr Color White{ 255, 255, 255, 255 };
	constexpr Color Black{ 255, 0,   0,   0 };
	constexpr Color Red{ 255, 255, 0,   0 };
	constexpr Color Green{ 255, 0,   255, 0 };
	constexpr Color Blue{ 255, 0,   0,   255 };
	constexpr Color Magenta{ 255, 255, 0,   255 };
	constexpr Color Yellow{ 255, 255, 255, 0 };
	constexpr Color Cyan{ 255, 0,   255, 255 };
	constexpr Color AggieBlue{ 0xFF0F2439 };
	constexpr Color AliceBlue{ 0xFFF0F8FF };
	constexpr Color AntiqueWhite{ 0xFFFAEBD7 };
	constexpr Color LemonYellowCrayola{ 0xFFFFFF9F };
	constexpr Color LightMint{ 0xFFCCE6D9 };
	constexpr Color MellowApricot{ 0xFFF8B878 };
	constexpr Color NeonCarrot{ 0xFFFFA343 };
	constexpr Color Orange{ 0xFFFFA500 };
	constexpr Color PersianPink{ 0xFFF77FBE };
	constexpr Color Pomegranate{ 0xFF660C21 };
	constexpr Color SandyBrown{ 0xFFF4A460 };
	constexpr Color VampireBlack{ 0xFF080808 };
	constexpr Color VividSkyBlue{ 0xFF00CCFF };
}

namespace AppState {
	enum Page : uint8_t {
		TITLE,
		DECISION_MATRIX,
		TOURNAMENT_BRACKET
	} page{ TITLE };

	struct Category {
		inline double calcFinalScore(double x) {
			finalScore = weight * scoringFunction(x);
			return finalScore;
		}

		Category(std::string _name = "Unnamed", double init_weight = 1.0, std::function<double(double)> scoreFunc = [](double x) { return x; }) : name(_name), weight(init_weight), scoringFunction(scoreFunc) {}

		std::string name;
		double weight{ 1.0 };
		double finalScore{ 0.0 };
		double maxScore{ std::numeric_limits<double>::infinity() };
		double minScore{ -std::numeric_limits<double>::infinity() };
		std::function<double(double)> scoringFunction{ [](double x) { return x; } };
	};

	struct ComparableItem {
		inline void calcFinalScore() {
			finalScore = 0.0;
			for (size_t i = 0; i < categories.size(); i++) {
				finalScore += categories[i].calcFinalScore(categoryScores[i]);
			}
		}

		ComparableItem() : ComparableItem("Unnamed") {}
		ComparableItem(std::string _name) : name(_name) { categoryScores.resize(categories.size()); }

		std::string name;
		std::vector<double> categoryScores; // One score per Category
		double finalScore{ 0.0 };
	};

	inline void pushCategory(const Category& category) {
		categories.push_back(category);
		for (ComparableItem& thing : things) { thing.categoryScores.push_back(0.0); }
	}

	std::vector<Category> categories;
	std::vector<ComparableItem> things;
}

static std::ofstream logFile("log.txt");

static void glfw_error_callback(int error, const char* description) {
	logFile << "GLFW Error " << error << ": " << description << '\n';
}

std::string latestSupportedGLSLVersion() {
	auto version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (!version) return "#version 130";

	std::string versionString = reinterpret_cast<const char*>(version);
	size_t i = versionString.find_first_of('.');
	size_t j = versionString.find_first_not_of("0123456789", i + 1);
	return "#version " + versionString.substr(0, i) + versionString.substr(i, j);
}

inline double linearFunc(double x, double slope = 1.0, double intercept = 0.0) {
	return slope * x + intercept;
}

int main() {
	if (!logFile) return 109;

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
// GL ES 2.0 + GLSL 100 (WebGL 1.0)
	std::string glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
// GL ES 3.0 + GLSL 300 es (WebGL 2.0)
	std::string glsl_version = "#version 300 es";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
// GL 3.2 + GLSL 150
	std::string glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
// GL 3.0 + GLSL 130+
	std::string glsl_version = latestSupportedGLSLVersion();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	int width = 1280, height = 720;
	GLFWwindow* window = glfwCreateWindow(width, height, "VibeDiff", nullptr, nullptr);
	if (!window) return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// glewExperimental = GL_TRUE;
	// if (glewInit() != GLEW_OK) {
	// 	std::cerr << "Failed to initialize GLEW" << '\n';
	// 	glfwDestroyWindow(window);
	// 	glfwTerminate();
	// 	return 1;
	// }
	glViewport(0, 0, width, height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 4.0f; // Windows 11: ControlCornerRadius

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	logFile << "Using GLSL version: " << glsl_version << '\n';
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());

	// Load Fonts
	enum Fonts { DEFAULT };
	const char* FONTS[1] = {
		"assets/fonts/CascadiaCode.ttf"
	};
	if (!io.Fonts->AddFontFromFileTTF(FONTS[DEFAULT], 18.0f)) {
		logFile << "Failed to load font file: " << FONTS[DEFAULT] << '\n';
	}

	#define TEST
	#ifdef TEST /// TEST
	AppState::pushCategory({ "Cost", 2.0, [](double x) { return 10.0 - x/100.0; } });
	AppState::things.push_back({ "Thing 1" });
	#endif /// TEST

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		auto vec = main_viewport->Size;
		ImGui::SetNextWindowSize({ vec.x, vec.y });
		vec = main_viewport->WorkPos;
		ImGui::SetNextWindowPos({ vec.x, vec.y });
		ImGui::Begin("VibeDiff", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		if (AppState::page == AppState::TITLE) {
			/// Title page ///
			float size = ImGui::CalcTextSize("Tournament Bracket").x + style.FramePadding.x * 2.0f;
    		float avail = ImGui::GetContentRegionAvail().x;
			float xOffset = (avail - size) * 0.5f;

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xOffset);
			bool buttonStatus_DM = ImGui::Button("Decision Matrix", { size, 0 });
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xOffset);
			bool buttonStatus_TB = ImGui::Button("Tournament Bracket", { size, 0 });

			AppState::page =
				buttonStatus_DM ? AppState::DECISION_MATRIX
				: (buttonStatus_TB ? AppState::TOURNAMENT_BRACKET
				: AppState::TITLE);
		}
		else { /// not the title page 😛 ///
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::BeginMenu("Export")) {
						if (ImGui::MenuItem("Image...")) {}
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Import")) {
						if (ImGui::MenuItem("From text file...")) {}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit")) {
					if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::Separator();
					if (ImGui::MenuItem("Variables...", "CTRL+SHIFT+V")) {}
					if (ImGui::MenuItem("Functions...", "CTRL+SHIFT+F")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			switch (AppState::page) {
			case AppState::DECISION_MATRIX:
				for (size_t i = 0; i < AppState::things.size(); i++) {
					ImGui::Text(AppState::things[i].name.c_str());
				}
				break;
			case AppState::TOURNAMENT_BRACKET:
				/* code */
				break;
			}
		}

		ImGui::End();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(Colors::Black.r, Colors::Black.g, Colors::Black.b, Colors::Black.a);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}