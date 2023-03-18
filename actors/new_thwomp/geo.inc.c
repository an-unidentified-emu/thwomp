#include "src/game/envfx_snow.h"

const GeoLayout new_thwomp_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 0, 186, 0, 0, -90, 0, new_thwomp_back_mesh_layer_1),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 0, 186, 0, 0, -90, 0, new_thwomp_face_mesh_layer_1),
		GEO_TRANSLATE_ROTATE(LAYER_OPAQUE, 0, 186, 0, 0, -90, 0),
		GEO_OPEN_NODE(),
			GEO_SHADOW(50, 191, 55),
			GEO_OPEN_NODE(),
				GEO_DISPLAY_LIST(LAYER_OPAQUE, new_thwomp_view_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, new_thwomp_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
