void scroll_wf_dl_wate_002_mesh_layer_1_vtx_0() {
	int i = 0;
	int count = 124;
	int width = 128 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(wf_dl_wate_002_mesh_layer_1_vtx_0);

	deltaX = (int)(0.30000001192092896 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_wf_dl_waterTop_mesh_layer_1_vtx_0() {
	int i = 0;
	int count = 12;
	int width = 128 * 0x20;

	static int currentX = 0;
	int deltaX;
	Vtx *vertices = segmented_to_virtual(wf_dl_waterTop_mesh_layer_1_vtx_0);

	deltaX = (int)(0.05999999865889549 * 0x20) % width;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;
}

void scroll_gfx_mat_wf_dl_water_002_layer1() {
	Gfx *mat = segmented_to_virtual(mat_wf_dl_water_002_layer1);
	static int interval_tex_wf_dl_water_002_layer1 = 3;
	static int cur_interval_tex_wf_dl_water_002_layer1 = 3;


	if (--cur_interval_tex_wf_dl_water_002_layer1 <= 0) {
		shift_s(mat, 12, PACK_TILESIZE(0, 1));
		shift_t_down(mat, 12, PACK_TILESIZE(0, 1));
		cur_interval_tex_wf_dl_water_002_layer1 = interval_tex_wf_dl_water_002_layer1;
	}
	shift_t(mat, 17, PACK_TILESIZE(0, 1));

};

void scroll_gfx_mat_wf_dl_water_001_layer1() {
	Gfx *mat = segmented_to_virtual(mat_wf_dl_water_001_layer1);
	static int interval_tex_wf_dl_water_001_layer1 = 2;
	static int cur_interval_tex_wf_dl_water_001_layer1 = 2;


	if (--cur_interval_tex_wf_dl_water_001_layer1 <= 0) {
		shift_s(mat, 16, PACK_TILESIZE(0, 1));
		shift_t_down(mat, 16, PACK_TILESIZE(0, 1));
		cur_interval_tex_wf_dl_water_001_layer1 = interval_tex_wf_dl_water_001_layer1;
	}
	shift_t(mat, 21, PACK_TILESIZE(0, 1));

};

void scroll_wf() {
	scroll_wf_dl_wate_002_mesh_layer_1_vtx_0();
	scroll_wf_dl_waterTop_mesh_layer_1_vtx_0();
	scroll_gfx_mat_wf_dl_water_002_layer1();
	scroll_gfx_mat_wf_dl_water_001_layer1();
};
