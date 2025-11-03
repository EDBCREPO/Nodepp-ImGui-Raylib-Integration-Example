# Raylib ImGui Demo
This project demonstrates a robust integration of Nodepp for asynchronous operations, ImGui for immediate-mode GUI rendering, and Raylib for window management and graphics. It showcases how to build interactive, event-driven applications with efficient resource management.

# Features
### Asynchronous Operations with Nodepp:
- Utilizes Nodepp's process::next() to drive an event loop, allowing for non-blocking I/O.
- Demonstrates http::fetch for making asynchronous network requests (e.g., fetching IP information).
- Leverages Nodepp's coroutines for managing long-running tasks and continuous drawing loops without blocking the main thread.

### Dynamic ImGui Window Management:
- Introduces the custom imWindow_t class for encapsulating ImGui window state, drawing logic, and lifecycle.
- Shows how to create, render, and dynamically destroy ImGui windows based on user interaction.
- Handles persistent input for ImGui text fields using static variables within window callbacks.

### Raylib Integration:
- Uses Raylib for window creation, basic drawing, and managing the main application loop.
- Integrates rlImGui (Raylib ImGui backend) to seamlessly render ImGui over Raylib's graphics.

### Garbage Collection: 
- Utilizes Nodepp's ptr_t smart pointer for simplified memory management, abstracting away manual memory deallocation.

# Building
```bash
g++ ./include/imgui/imgui.cpp         \
    ./include/imgui/imgui_demo.cpp    \
    ./include/imgui/imgui_draw.cpp    \
    ./include/imgui/imgui_tables.cpp  \
    ./include/imgui/imgui_widgets.cpp \
    ./include/imgui/rlImGui.cpp       \
    main.cpp -o main                  \
    -L./lib -I./include -lraylib 
    
./main
```

# Showcase
![IMAGE](https://miro.medium.com/v2/resize:fit:720/format:webp/1*WoEEsBCMAE4Y8-myeJUHcA.gif)

# How it works
[![IMAGE](https://miro.medium.com/v2/resize:fit:720/format:webp/1*wPG4kW4A-ToXS0JjdAg1lA.png)](https://medium.com/@EDBCBlog/designing-an-event-driven-imgui-architecture-from-zero-to-hero-no-phd-required-82290c082c6a)

[Click Here](https://medium.com/@EDBCBlog/designing-an-event-driven-imgui-architecture-from-zero-to-hero-no-phd-required-82290c082c6a)
