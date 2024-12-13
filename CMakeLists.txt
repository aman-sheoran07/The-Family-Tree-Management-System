cmake_minimum_required(VERSION 3.10)
project(FamilyTreeSystem VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Add all source files recursively
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Add header files directory
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS}
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SQLite3 library (simpler version)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    sqlite3
)

# Adding source files for UI
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/ui/*.cpp"
)

# Adding UI include directory
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/ui
    ${SQLITE3_INCLUDE_DIRS}
)