[![Discord](https://img.shields.io/discord/798876142458109952?logo=Discord "Discord")](https://discord.gg/Xfv3xrxT)

## Paradise Engine

ParadiseEngine is an open source under development game engine based on Vulkan for both Windows and Linux

![Blocky screenshot](doc/main.png)

## CLONING

```bash
git clone --recursive https://github.com/JoseETeixeira/ParadiseEngineVulkan.git
```
Delete everything from `third_party/imgui/backends` EXCEPT FOR:

- imgui_impl_glfw.h/.cpp
- imgui_impl_vulkan.h/.cpp

## Building - WINDOWS

- Install VulkanSDK
- Set the `vulkan_sdk_dir` parameter on the root directory `SConstruct` file to where you installed Vulkan
- Install SCons
```bash
scons -Q
```

- If the process fails, you may need to install Visual Studio 2019

## Building - Linux
```bash
sudo apt install vulkan-tools
sudo apt install libvulkan-dev
sudo apt install vulkan-validationlayers-dev spirv-tools
sudo apt install libglfw3-dev
sudo apt install libglm-dev
```

Shader Compiler
on Ubuntu, download Google's [unofficial binaries](https://github.com/google/shaderc/blob/main/downloads.md) and copy glslc to your /usr/local/bin. Note you may need to sudo depending on your permissions. On Fedora use sudo dnf install glslc. To test, run glslc and it should rightfully complain we didn't pass any shaders to compile:

```bash
glslc: error: no input files
```

```bash
sudo apt-get install -y scons
scons -Q
```


## Tutorials
- https://vulkan-tutorial.com/
- https://www.youtube.com/watch?v=6NWfznwFnMs&t=20s
- https://www.youtube.com/watch?v=JpmK0zu4Mts&t=480s
- https://scons.org/doc/production/PDF/scons-user.pdf
- https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization
- https://www.reddit.com/r/vulkan/comments/fe40bw/vulkan_image_imgui_integration/
- https://github.com/SaschaWillems/Vulkan-glTF-PBR

## COMPILING SHADERS (TEMPORARY, WILL BE AUTO-IMPLEMENTED LATER)

`Windows

Create a compile.bat file with the following contents:

C:/VulkanSDK/x.x.x.x/Bin32/glslc.exe shader.vert -o vert.spv
C:/VulkanSDK/x.x.x.x/Bin32/glslc.exe shader.frag -o frag.spv
pause
Replace the path to glslc.exe with the path to where you installed the Vulkan SDK. Double click the file to run it.

Linux

Create a compile.sh file with the following contents:

/home/user/VulkanSDK/x.x.x.x/x86_64/bin/glslc shader.vert -o vert.spv
/home/user/VulkanSDK/x.x.x.x/x86_64/bin/glslc shader.frag -o frag.spv
Replace the path to glslc with the path to where you installed the Vulkan SDK. Make the script executable with chmod +x compile.sh and run it.`

## TODO:
- Change [staging buffer](https://vulkan-tutorial.com/en/Vertex_buffers/Staging_buffer) to use [VulkanMemoryAllocator] (https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)

## DEBUGGING:

- [RenderDoc](https://renderdoc.org/)