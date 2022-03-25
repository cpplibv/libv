
model.scale(0.175)
--model.scale(2)
model.rotate(0, 90, 90)

--model.property("sprite_offset", vec3f{})

model.regex_replace_textures("bark_0021.jpg", "res/model/Tree_med.bark_0021.jpg");
model.regex_replace_textures("Tree.fbm/", "res/model/Tree_med.");

--model.lods{
--		{ model.find_node("high"),   0,  50, },
--		{ model.find_node( "med"),  40, 250, },
--		{ model.find_node( "low"), 200,  -1, },
--}

--model.print_materials()
--model.print_textures()
