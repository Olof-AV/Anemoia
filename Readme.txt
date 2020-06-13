GitHub link: https://github.com/Olof-AV/Anemoia
Build link: https://github.com/Olof-AV/Anemoia/releases/download/v1.0/Anemoia_Builds.zip

-Main engine:
	-Game loop:
		-Input
		-Fixed update
		-Update
		-Late update
		-Render
		-AI thread runs alongside main thread
	-Can be extended for a particular game, Bubble Bobble has:
		-Gamemode (singleplayer, coop, versus)
		-Music
		-Holds score/lives
	-Catch2 + VLD + SDL + XInput

-Managers:
	-AI Manager, runs on a separate thread, can attach AI functions to it to have them run there
	-InputManager, functions with Commands (through bound functions), recognises keyboard+controllers, button hold/down/up, haptics...
	-Renderer, takes care of setting up the SDL_Renderer, has render functions which can use ClipRects, flips, rotations, colouring...
	-ResourceManager, applies the FlyWeight pattern -> shared resource is only loaded once (texture, font, sound) then shared
	-SceneManager, only one scene is active at a time, some callbacks when scenes are swapped, applies the gameloop
	-Locator for services such as the engine, the window, the renderer...

-GameObject:
	-Knows about position, parent scene, attached components and assigned tags
	-Can be enabled/disabled
	-Can be marked for delete, which will cleanly remove it on the next update loop
	-Any attached components will be managed by the object, and can be retrieved through functions
	-Objects are also subjects -> they can have observers attached, to which notify events are relayed
	
-Scene:
	-A scene runs the game loop (fixed/normal/late update, render...)
	-Objects are attached to scenes, it then manages these objects
	-Can retrieve all objects with a given tag/all colliders through a getter
	-Some helpers like Initialise, OnSceneActivated, OnSceneDeactivated...
	
-BaseComponent:
	-Serves as the core of other components
	-Holds a Transform (pos offset from parent, pivot, scale, rotation, flip mode) + a parent GameObject
	-OnCollide callback if needed

-Engine components:
	-Animated sprites (can load several animations, swap between them, play linearly/pong mode, callback on finish, transform...)
	-Collider (defines a hitbox, draws collider in debug, can be set to not affect RigidBodies/let them pass from below)
	-Rigid Body (defines a moving body, interacts with colliders, use ignore tags to stop collision with specific objects)
	-Text (draws some text, using the transform it can be rotated/scaled/flipped/... as you want)
	-Texture (a stripped down version of animated sprites, some shared functionality: transparency, colouring)
	
-Game Components:
	-Boulder Behaviour, defines how the Maita's boulder functions and interacts with other objects
	-Bubble Behaviour, defines how the player's bubbles function and interact with other objects
	-HUD Component, shows some text and sprites related to score, life points...
	-Item Behaviour, defines how collectibles function, how they give score to players...
	-Maita Behaviour, defines how the Maita enemy functions, along with their AI and some state functionality/transitions
	-Player Behaviour, defines how the player characters function, their states and state transitions
	-Zen Behaviour, defines how the ZenChan enemy functions, holds the AI, some states & transitions