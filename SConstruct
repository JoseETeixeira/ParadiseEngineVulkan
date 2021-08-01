import os
import sys;


project_name = "Paradise Engine"
output_folder = "bin/"

vulkan_sdk_dir = "C:/VulkanSDK/1.2.182.0"

if sys.platform == 'win32':
    cpp17 = Environment(
        CCFLAGS=['/std:c++17', '-Wall', '-O2', '-g'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi')

    cpp17.Append(LIBS = [
        "vulkan-1",
        "glfw3dll"
    ])

    cpp17.Append(CPPDEFINES = [
        "GLFW_DLL"
    ])

    cpp17.Append(LIBPATH = [
		"third_party/glfw/lib-vc2019",
		os.path.join(vulkan_sdk_dir, "Lib")
	])

    cpp17.Append(CPPPATH = [
		os.path.join(vulkan_sdk_dir, "Include")
	])


else:
    # dependencies: libglfw3-dev (>= 3.2.1)

    cpp17 = Environment(
        CCFLAGS=['-std=c++17', '-Wall', '-O2', '-g'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi')
    
    cpp17.Append(LIBS = [
        "vulkan",
        "glfw"
    ])


cpp17.Append(CPPPATH=['third_party/glfw/include','third_party/glm','third_party/stb','third_party/imgui','third_party/imgui/backends','.'])
cpp17.SharedLibrary('vulkan_device',Glob('engine/vulkan_device/*.cpp'))
cpp17.SharedLibrary('vulkan_tools',Glob('engine/vulkan_tools/*.cpp'))
cpp17.SharedLibrary('vulkan_buffer',Glob('engine/vulkan_buffer/*.cpp'))
cpp17.SharedLibrary('imgui',Glob('third_party/imgui/*.cpp'))
cpp17.SharedLibrary('imgui_impl_vulkan',Glob('third_party/imgui/backends/imgui_impl_vulkan.cpp','third_party/imgui/backends/imgui_impl_glfw.cpp'))

#------------------------------------------------------------------------------


sources = []

def add_sources(sources, dir):
	for f in os.listdir(dir):
		if f.endswith('.cpp') or f.endswith('.c'):
			sources.append(dir + '/' + f)

add_sources(sources, 'engine/vulkan_device')
add_sources(sources, 'engine/vulkan_tools')
add_sources(sources, 'engine/vulkan_buffer')
add_sources(sources, 'engine/vulkan_debug')

add_sources(sources, 'third_party/imgui')
add_sources(sources, 'engine')


sources.append(['third_party/imgui/backends/imgui_impl_vulkan.cpp','third_party/imgui/backends/imgui_impl_glfw.cpp'])
#------------------------------------------------------------------------------
if sys.platform == 'win32':
    program = cpp17.Program(target=(output_folder + project_name), source=sources)
else:
    program = cpp17.Program(target=(output_folder + project_name), source=sources,LIBPATH=['.'])
Default(program)