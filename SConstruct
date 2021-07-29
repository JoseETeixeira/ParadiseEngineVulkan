import os
import sys;


project_name = "Paradise Engine"
output_folder = "bin/"

vulkan_sdk_dir = "C:/VulkanSDK/1.2.182.0"

if sys.platform == 'win32':
    cpp17 = Environment(
        CCFLAGS=['-std=c++17', '-Wall', '-O2', '-g'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi')
    cpp17[VULKAN_DIR] = vulkan_sdk_dir


else:
    # dependencies: libglfw3-dev (>= 3.2.1)

    cpp17 = Environment(
        CCFLAGS=['-std=c++17', '-Wall', '-O2', '-g'],
        LDFLAGS = '-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi')
    


#------------------------------------------------------------------------------


Export("cpp17")
objs = []
for subdir in ['engine']:
    o = SConscript('%s/SCsub' % subdir)
    objs.append(o)


#------------------------------------------------------------------------------
program = cpp17.Program(target=(output_folder + project_name), source=objs,LIBPATH=['.'])
Default(program)