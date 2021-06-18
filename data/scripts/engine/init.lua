---------------------------------
-- Engine lua-side initialization
---------------------------------

-- Load game object
include("scripts", "engine/entities/game_object.lua");

register_factory(game_object, "Entity");