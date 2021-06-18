r_shaders = {}

-- Basic deferred shader for geometry
r_shaders["def_geom"] = {

	vertex_path = "def_geom.vs",
	fragment_path = "def_geom.fs",	
	
	vertex_declaration = {
		position = { "float", 3 },
		normal = { "float", 3 },
		texcoord0 = { "float", 2 }
	}
	
}

-- Basic forward shader for geometry
r_shaders["forw_geom"] = {
	vertex_path = "forw_geom.vs",
	fragment_path = "forw_geom.fs",

	vertex_declaration = {
		position = { "float", 3 },
		normal = { "float", 3 },
		texcoord0 = { "float", 2 }
	}
}

-- shader for draw fbo on ndc quad
r_shaders["quad"] = {
	vertex_path = "quad.vsh",
	fragment_path = "quad.fsh",

	vertex_declaration = {
		position = { "float", 2 },
		texcoord0 = { "float", 2 }
	}
}

-- deferred shader (light pass)
r_shaders["def_light"] = {

	vertex_path = "quad.vsh",
	fragment_path = "def_light.psh",

	vertex_declaration = {
		position = { "float", 2 },
		texcoord0 = { "float", 2 }
	}
}

-- only for old shaders or material name's
r_reassign_table = {
	
	-- material names
	diffuse = "def_geom",
	diffuse_det = "def_geom",
	diffuse_dt = "def_geom",
	skybox = "forw_geom",
}

