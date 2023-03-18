void scroll_wf_dl_bottom_platform_001_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 12;
	int width = 32 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int timeX;
	float amplitudeX = 0.1899999976158142;
	float frequencyX = 0.12999999523162842;
	float offsetX = 0.0;
	Vtx *vertices = segmented_to_virtual(wf_dl_bottom_platform_001_mesh_layer_5_vtx_0);

	deltaX = (int)(amplitudeX * frequencyX * coss((frequencyX * timeX + offsetX) * (1024 * 16 - 1) / 6.28318530718) * 0x20);

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
	}
	currentX += deltaX;	timeX += 1;
}

void scroll_sts_mat_wf_dl_water_layer5() {
	Gfx *mat = segmented_to_virtual(mat_wf_dl_water_layer5);
	shift_s(mat, 14, PACK_TILESIZE(0, 1));
	shift_t(mat, 14, PACK_TILESIZE(0, 1));
	shift_s_down(mat, 19, PACK_TILESIZE(0, 2));
	shift_t_down(mat, 19, PACK_TILESIZE(0, 1));
};

void scroll_wf() {
	scroll_wf_dl_bottom_platform_001_mesh_layer_5_vtx_0();
	scroll_sts_mat_wf_dl_water_layer5();
}
