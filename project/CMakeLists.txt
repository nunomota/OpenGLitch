# the exercise name is nothing else than the directory
get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# these files are copied only once
file(GLOB TEXTURES
    Resources/Textures/grid_texture.tga
    Resources/Textures/water.tga)
copy_files_once(${TEXTURES})

# list all the files you want to copy everytime
# you build (ie, you want the shaders there)
file(GLOB SHADERS
  Framework/Object3D/PhysicalObject/Cube/cube_vshader.glsl
  Framework/Object3D/PhysicalObject/Cube/cube_fshader.glsl
  Framework/Object3D/PhysicalObject/Plane/plane_vshader.glsl
  Framework/Object3D/PhysicalObject/Plane/plane_fshader.glsl
  Framework/Object3D/PhysicalObject/Grid/grid_vshader.glsl
  Framework/Object3D/PhysicalObject/Grid/grid_fshader.glsl
  Objects/Water/water_vshader.glsl
  Objects/Water/water_fshader.glsl)
deploy_shaders_to_build_dir(${SHADERS})

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
