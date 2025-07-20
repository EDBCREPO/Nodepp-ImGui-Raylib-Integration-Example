#!/bin/bash

# Define source directories
IMGUI_DIR="./include/imgui"

# Create a build directory for object files if it doesn't exist
BUILD__DIR="build"
BUILD_DIR="build"
mkdir -p "${BUILD_DIR}"

# List of ImGui source files to include in the library
IMGUI_CPP_SOURCES=(
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/rlImGui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)

INCLUDE_DIRS="-I./include"
LIB_DIRS="-L./lib -L./build"
LIBS="-lraylib" # Raylib is a dependency for rlImGui, so link it into the shared lib

OBJECT_FILES=()

# Step 1: Compile each ImGui source file into PIC object files
echo "Compiling ImGui source files into PIC object files..."
for SOURCE_FILE in "${IMGUI_CPP_SOURCES[@]}"; do

    BASE_NAME=$(basename "${SOURCE_FILE}" .cpp)
    OBJ_FILE="${BUILD_DIR}/${BASE_NAME}.o"
    OBJECT_FILES+=("${OBJ_FILE}")

    if [ -f $OBJ_FILE ]; then
       echo "${OBJ_FILE} - file already exists"
       continue
    fi

    echo "  Compiling ${SOURCE_FILE} -> ${OBJ_FILE}"
    g++ -c -fPIC "${SOURCE_FILE}" ${INCLUDE_DIRS} -o "${OBJ_FILE}"
    if [ $? -ne 0 ]; then
        echo "Compilation of ${SOURCE_FILE} failed!"
        exit 1
    fi
done

# Step 2: Link the PIC object files into a shared library
if [ ! -f "${BUILD_DIR}/libmyimgui.so" ]; then
    echo "Linking object files into shared library libmyimgui.so..."
    g++ -shared "${OBJECT_FILES[@]}" ${LIB_DIRS} ${LIBS} -o "${BUILD_DIR}/libmyimgui.so"

    if [ $? -ne 0 ]; then
        echo "Linking shared library failed!"
        exit 1
    fi

    echo "Shared library libmyimgui.so created successfully in ${BUILD_DIR}/"
fi

# Now, to compile and link your main application that uses this library:
echo "Compiling and linking main application..."
g++ main.cpp ${INCLUDE_DIRS} -L"${BUILD_DIR}" -lmyimgui ${LIB_DIRS} ${LIBS} -o main

if [ $? -ne 0 ]; then
    echo "Main application build failed!"
    exit 1
fi

echo "Main application 'main' built successfully! To run:"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${BUILD_DIR}
./main