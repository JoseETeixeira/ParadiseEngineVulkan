import os
import sys;


project_name = "Paradise Engine"
output_folder = "bin/"

vulkan_sdk_dir = "C:/VulkanSDK/1.2.182.0"

if sys.platform == 'win32':
    cpp17 = Environment(
        CPPDEFINES=['WIN32',
        '_WIN32',
        '_WINDOWS',
        'VK_USE_PLATFORM_WIN32_KHR',
        'NOMINMAX',
        '_USE_MATH_DEFINES',
        '_CRT_SECURE_NO_WARNINGS'],
        CCFLAGS=['/std:c++17', '-Wall', '-O2', '-g'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi')

    cpp17.Append(LIBS = [
        "vulkan-1",
        "glfw3dll",
        "kernel32",
        "user32",
        "gdi32",
        "winspool",
        "shell32",
        "ole32",
        "oleaut32",
        "uuid",
        "comdlg32",
        "advapi32",
        "lz4",
        "xxhash"
    ])

    cpp17.Append(CPPDEFINES = [
        "GLFW_DLL"
    ])

    cpp17.Append(LIBPATH = [
        "third_party/lz4",
		"third_party/glfw/lib-vc2019",
		os.path.join(vulkan_sdk_dir, "Lib")
	])

    cpp17.Append(CPPPATH = [
		os.path.join(vulkan_sdk_dir, "Include")
	])


else:
    # dependencies: libglfw3-dev (>= 3.2.1)

    cpp17 = Environment(
        CPPDEFINES=[
        'LINUX',
        'USE_D2D_WSI',
        '__linux__',
        'NOMINMAX',
        '_USE_MATH_DEFINES',
        '_CRT_SECURE_NO_WARNINGS'],
        CCFLAGS=['-std=c++17', '-Wall', '-O2', '-g','-DVK_USE_PLATFORM_XCB_KHR'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi' )
    
    cpp17.Append(LIBS = [
        "vulkan",
        "glfw",
        "xcb",
        "lz4"
    ])


cpp17.Append(CPPPATH=['third_party/glfw/include',
'third_party/glm',
'third_party/tinygltf',
'third_party/glm',
'third_party/ktx/include',
'third_party/ktx/lib',
'third_party/ktx/other_include',
'third_party/stb',
'third_party/vulkan',
'third_party/imgui'
'third_party/imgui/backends',
'third_party/json',
'engine/imgui_impl_vulkan'
'.'])

cpp17.SharedLibrary('imgui',Glob('third_party/imgui/*.cpp'))
cpp17.SharedLibrary('imgui_impl_vulkan',Glob('third_party/imgui/backends/imgui_impl_vulkan.cpp','third_party/imgui/backends/imgui_impl_glfw.cpp'))

#------------------------------------------------------------------------------


sources = []

def add_sources(sources, dir):
	for f in os.listdir(dir):
		if f.endswith('.cpp') or f.endswith('.c'):
			sources.append(dir + '/' + f)

add_sources(sources, 'engine/imgui_impl_vulkan')
add_sources(sources, 'engine/vulkan_device')
add_sources(sources, 'engine/vulkan_tools')
add_sources(sources, 'engine/vulkan_buffer')
add_sources(sources, 'engine/vulkan_debug')
add_sources(sources, 'engine/vulkan_gltf_model')
add_sources(sources, 'engine/vulkan_swap_chain')
add_sources(sources, 'engine/vulkan_ui_overlay')
add_sources(sources, 'engine/vulkan_texture')
add_sources(sources, 'engine/vulkan_example_base')
add_sources(sources, 'third_party/imgui')
add_sources(sources, 'third_party/ktx/lib')
add_sources(sources, 'third_party/ktx/include')
add_sources(sources, 'engine/asset_loader')
add_sources(sources, 'engine')

#------------------------------------------------------------------------------
if sys.platform == 'win32':
    program = cpp17.Program(target=(output_folder + project_name), source=sources)
else:
    program = cpp17.Program(target=(output_folder + project_name), source=sources,LIBPATH=['.'])
Default(program)