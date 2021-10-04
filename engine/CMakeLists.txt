file(GLOB BASE_SRC 
"*.cpp" 
"*.hpp" 
"*.h" 
"../third_party/imgui/*.cpp" 
"../third_party/vulkan_memory_allocator/vk_mem_alloc.cpp" 
"../third_party/imGuizmo/*.cpp" 
"ecs/*.hpp"
"ecs/components/*.hpp" 
"ecs/components/*.h"
"ecs/components/*.cpp"  
"ecs/math/*.hpp" 
"ecs/systems/*.hpp" 
"ecs/systems/*.cpp" 
"gui/*.hpp"
"vulkan_buffer/*.h" 
"vulkan_buffer/*.cpp" 
"vulkan_debug/*.h" 
"vulkan_debug/*.cpp" 
"vulkan_device/*.h"
"vulkan_device/*.cpp"
"vulkan_example_base/*.h" 
"vulkan_example_base/*.cpp" 
"vulkan_gltf_model/*.hpp" 
"vulkan_swap_chain/*.h" 
"vulkan_swap_chain/*.cpp" 
"vulkan_texture/*.h" 
"vulkan_texture/*.cpp" 
"vulkan_tools/*.h" 
"vulkan_tools/*.cpp" 
"vulkan_ui_overlay/*.h" 
"vulkan_ui_overlay/*.cpp"
"myvk/*.cpp"
"myvk/*.hpp"
"myvk/*.h"
"voxelizer/*.cpp"
"voxelizer/*.hpp"
"voxelizer/*.h"

)
file(GLOB BASE_HEADERS "*.hpp" "*.h")

set(KTX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/ktx)
set(KTX_SOURCES
    ${KTX_DIR}/lib/texture.c
    ${KTX_DIR}/lib/hashlist.c
    ${KTX_DIR}/lib/checkheader.c
    ${KTX_DIR}/lib/swap.c
    ${KTX_DIR}/lib/memstream.c
    ${KTX_DIR}/lib/filestream.c)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-std=c++2a")

IF(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
ELSEIF(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
ENDIF(WIN32)

add_library(base STATIC ${BASE_SRC} ${KTX_SOURCES})
if(WIN32)
    target_link_libraries(base ${Vulkan_LIBRARY} ${WINLIBS})
 else(WIN32)
    target_link_libraries(base ${Vulkan_LIBRARY} ${XCB_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)



