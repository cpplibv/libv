
model.scale(0.175)
--model.scale_height_to(10)
--model.rotate(0, 90, 90)

model.regex_replace_textures("Tree_med.fixed.game.fbm/bark_0021.jpg", "../../res/model/tree_01_bark.diffuse.jpg");
model.regex_replace_textures("Tree_med.fixed.game.fbm/DB2X2_L01.png", "../../res/model/tree_01_leaves.diffuse.png");
model.regex_replace_textures("Tree_med.fixed.game.fbm/DB2X2_L01_Spec.png", "../../res/model/tree_01_leaves.specular.png");
model.regex_replace_textures("Tree_med.fixed.game.fbm/DB2X2_L01_Nor.png", "../../res/model/tree_01_leaves.normal.png");

model.add_bool("leaves", "twosided", true);

--model.lods{
--		{ model.find_node("high"),   0,  50, },
--		{ model.find_node( "med"),  40, 250, },
--		{ model.find_node( "low"), 200,  -1, },
--}

--model.print_materials()
--model.print_textures()
----model.print_nodes()
-- model.print_model();
