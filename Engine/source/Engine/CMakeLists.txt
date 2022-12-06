target_sources(Engine
	PRIVATE
		Engine.cpp
	PUBLIC
		Debug.ixx
		Engine.ixx
		FontManager.ixx
		MathHelper.ixx
		ObjectPool.ixx
		Status.ixx
)

add_subdirectory("command")
add_subdirectory("input")
add_subdirectory("logging")
add_subdirectory("renderer")
add_subdirectory("time")
add_subdirectory("windows")