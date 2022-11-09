# The Chosen Frog
The Chosen Frog is a 3D platformer developed during the _Computer Graphics_ course. The main goal was to develop a small game engine using C++ and OpenGL. No helper graphics libraries and frameworks were allowed. A small game using the custom game engine including a set of visual effects had to be implemented.

## Background Story
In the world of frogs there are a lot of these casual bedtime stories and fairy tales about frogs, which traveled to distant places about frogs, which could eat more than any other frog, about frogs, which saved an entire pond, etc. But, there is one story every frog knows.
The legend says that once every 100 years (10 frog lives for comparison) **ONE**, only one frog gets chosen to leave its beloved terrain and start a journey of no return to *the heavenly pond of the gods*.
*The heavenly pond of the gods*: a place where you will never suffer from hunger or illness, a place untouched from filthy humans with fresh sparkling water, simply put, a paradise.

## Gameplay
Since the primary focus of the course was developing a game engine and implementing visual effects, the gameplay is very minimalistic.

The goal of the The Chosen Frog is to reach the heavenly pond of the gods by jumping from cloud to cloud.
The faster the player reaches the heavenly pond, the higher the score.

## Technical Highlights
- Custom game engine using C++ and OpenGL, all rendering and visual effects are coded without additional libraries
- Shader and asset reloading during runtime
- Procedural vertex animation in GLSL
- Custom Entity-Component system and scene graph 

## Controls
### Gameplay
- **W A S D**: Movement of the frog - W to move the frog forward, A to move the frog to the left, S to move backwards, D to move to the right. The frog can also be moved in the air up to a certain degree.

- **Space**: Jump - When pressing the space key the player charges up a jump, which is performed upon releasing the space key. The longer the space key has been hold, the farther and higher the jump will be.
Upon holding the space key, the jump, that will be performed, is visualized with a parable indicating how strong the jump will be and where the frog will land, assuming the player does not influence the jump by pressing the movement keys.

- **Mouse**: Camera Rotation - The camera will rotate through solely moving the mouse up and down or left and right.

### Debug
- **F11**: Restart game
- **F10**: Toggle Backface Culling
- **F9**: Toggle Wireframe Rendering
- **F8**: Enable/Disable Cursor
- **Ctrl + 1**: Toggle debug logging window
- **Ctrl + 2**: Toggle debug component viewer window
- **Ctrl + 3**: Toggle debug asset viewer window



## Libraries
- [GLFW](https://www.glfw.org/) - Windowing Toolkit
- [GLM](https://github.com/g-truc/glm) - Math
- [Dear ImGui](https://github.com/ocornut/imgui) - Debug GUI
- [SoLoud](https://github.com/jarikomppa/soloud) - Audio (unused)
- [RuntimeCompiledCpp](https://github.com/RuntimeCompiledCPlusPlus/RuntimeCompiledCPlusPlus) - Hot code reloading (unused)
- [Assimp 5.0.0 RC1](https://github.com/assimp/assimp) - Model Loader
- [stb_image](https://github.com/nothings/stb) - Disk Image loading
- [stb_truetype](https://github.com/nothings/stb) - True Type Fonts
- [cute_filewatch](https://github.com/RandyGaul/cute_headers) - File change watcher used for runtime asset reloading.