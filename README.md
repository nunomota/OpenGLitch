# OpenGLitch

A 3D world created using OpenGL.

## Features

- Procedural terrain generation
- Infinite terrain
- Water
- Water rippling, reflection, refraction and fresnel effect
- Underwater view
- Simple physics
- Custom .obj loader

## Screenshots

<p>
	<img src="/project/Resources/Screenshots/Abovewater_Screenshot.png" width="48%" alt="View above water">
	<img src="/project/Resources/Screenshots/Underwater_Screenshot.png" width="48%" alt="View under water">
</p>

## How to run

 1. From within the project's root, create a new `build` directory (to keep everything clean)
 	- `mkdir build`
 2. Go into the newly created directory and run `cmake` on the root
 	- `cd build/`
 	- `cmake ..`
 3. When that is finished running you can call `make` - from the current `build` directory
 	- `make`
 4. After that you're done! Open the newly generated `project` directory and run the `project` executable inside
 	- `cd project/`
 	- `./project`
