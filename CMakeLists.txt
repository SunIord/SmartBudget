cmake_minimum_required(VERSION 3.10)
project(SmartBudget)

set(CMAKE_CXX_STANDARD 17)

# Inclui os headers
include_directories(
	lib/imgui
	lib/imgui/backends
	include
)

# Fontes do ImGui
file(GLOB IMGUI_SOURCES
	lib/imgui/*.cpp
	lib/imgui/backends/imgui_impl_glfw.cpp
	lib/imgui/backends/imgui_impl_opengl3.cpp
)

file(GLOB INCLUDES_HPP
	src/transaction.cpp
	src/transactionManager.cpp
	src/budgetAnalyzer.cpp
)

add_executable(SmartBudget
	src/main.cpp
	${IMGUI_SOURCES}
	${INCLUDES_HPP}
)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(glfw3 3.4 REQUIRED)

target_include_directories(SmartBudget PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(SmartBudget PRIVATE
	glfw
	OpenGL::GL
)

# Link extra no Windows
if (WIN32)
	target_link_libraries(SmartBudget PRIVATE
		opengl32
		gdi32
	)
else()
	target_link_libraries(SmartBudget PRIVATE
		dl
		pthread
		X11
	)
endif()
