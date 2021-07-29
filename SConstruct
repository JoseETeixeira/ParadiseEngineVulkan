import os
import sys;


project_name = "Paradise Engine"
output_folder = "bin/"

vulkan_sdk_dir = "C:/VulkanSDK/1.2.182.0"

if sys.platform == 'win32':
    cpp17 = Environment(
        CCFLAGS=['-std=c++17', '-Wall', '-O2', '-g'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi')
    
    cpp17.Append(LIBS = [
        "vulkan-1",
        "glfw3dll",
        "imgui",
        "imgui_impl_vulkan"
    ])

    cpp17.Append(CPPPATH=['third_party/glfw/include','third_party/glm','third_party/imgui','third_party/imgui/backends','.'])

    cpp17.Library('imgui',Glob('third_party/imgui/*.cpp'))
    cpp17.Library('imgui_impl_vulkan',Glob('third_party/imgui/backends/*.cpp'))
    

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
        "glfw",
        "imgui",
        "imgui_impl_vulkan"
    ])

    cpp17.Append(CPPPATH=['third_party/glfw/include','third_party/glm','third_party/imgui','third_party/imgui/backends','.'])

    cpp17.Library('imgui',Glob('third_party/imgui/*.cpp'))
    cpp17.Library('imgui_impl_vulkan',Glob('third_party/imgui/backends/*.cpp'))
    

    cpp17.Append(CPPDEFINES = [
        "GLFW_DLL"
    ])


#------------------------------------------------------------------------------
sources = []

def add_sources(sources, dir):
	for f in os.listdir(dir):
		if f.endswith('.cpp') or f.endswith('.c'):
			sources.append(dir + '/' + f)

add_sources(sources, 'engine')
#add_sources(sources, 'core/math')
#add_sources(sources, 'game')

#------------------------------------------------------------------------------
program = cpp17.Program(target=(output_folder + project_name), source=sources,LIBPATH=['.'])
Default(program)