/*--------------------
| Debugging purposes |
--------------------*/
#include "Debugging/GlmStrings/GlmStrings.h"
#include "Debugging/Reporter/Reporter.h"

/*--------------------
|    Loader class    |
--------------------*/
#include "Loader/Loader.h"

/*--------------------
|     Time class     |
--------------------*/
#include "Time/Time.h"

/*--------------------
|   Input classes    |
--------------------*/
#include "Input/Mouse/Mouse.h"
#include "Input/Keyboard/Keyboard.h"

/*--------------------
| 3D related classes |
--------------------*/
#include "Object3D/Renderer/Material/Material.h"
#include "Object3D/Renderer/Renderer.h"
#include "Object3D/Transform/Transform.h"
#include "Object3D/Object3D.h"
#include "Object3D/PhysicalObject/PhysicalObject.h"
#include "Object3D/EmptyObject/EmptyObject.h"

/*--------------------
|    Core Objects    |
--------------------*/
// Physical objects
#include "Object3D/PhysicalObject/Cube/Cube.h"
#include "Object3D/PhysicalObject/Plane/Plane.h"
#include "Object3D/PhysicalObject/Grid/Grid.h"

// Empty objects
#include "Object3D/EmptyObject/Camera/Camera.h"
#include "Object3D/EmptyObject/Light/Light.h"
#include "Object3D/EmptyObject/Light/PointLight/PointLight.h"
#include "Object3D/EmptyObject/Light/DirectionalLight/DirectionalLight.h"

/*--------------------
|   Custom Objects   |
--------------------*/
#include "../Objects/Objects.h"

/*--------------------
|    World class     |
--------------------*/
#include "World/World.h"
#include "World/WorldInstance/WorldInstance.h"