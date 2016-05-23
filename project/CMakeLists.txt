# the exercise name is nothing else than the directory
get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# these files are copied only once
file(GLOB TEXTURES
    Resources/Textures/grid_texture.tga
    Resources/Textures/water.tga
    Resources/Textures/perlin.jpg
    Resources/Textures/perlinNormal.png
    Resources/Textures/minimap.png
    Resources/Textures/waterDUDV.png
    Resources/Textures/waterNormal.png
    Resources/Textures/miramar_bk.tga
    Resources/Textures/miramar_dn.tga
    Resources/Textures/miramar_ft.tga
    Resources/Textures/miramar_lf.tga
    Resources/Textures/miramar_rt.tga
    Resources/Textures/miramar_up.tga)


copy_files_once(${TEXTURES})

# list all the files you want to copy everytime
# you build (ie, you want the shaders there)
file(GLOB SHADERS
  Framework/Object3D/PhysicalObject/Cube/cube_vshader.glsl
  Framework/Object3D/PhysicalObject/Cube/cube_fshader.glsl
  Framework/Object3D/Shaders/plain_vshader.glsl
  Framework/Object3D/Shaders/plain_fshader.glsl
  Framework/Object3D/Shaders/textured_vshader.glsl
  Framework/Object3D/Shaders/textured_fshader.glsl
  Framework/Object3D/Shaders/heightmap_vshader.glsl
  Objects/Water/water_vshader.glsl
  Objects/Water/water_fshader.glsl
  Objects/Terrain/terrain_vshader.glsl
  Objects/Terrain/terrain_fshader.glsl
  Objects/Terrain/height_vshader.glsl
  Objects/Terrain/height_fshader.glsl
  Objects/Sky/sky_vshader.glsl
  Objects/Sky/sky_fshader.glsl)
deploy_shaders_to_build_dir(${SHADERS})

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
