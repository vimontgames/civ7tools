
project("base64"
	LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)


add_executable("main" "main.cpp")
target_include_directories("main" 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
