#include "texture.h"

struct texture_t {
	std::string path;
	Texture *texture = nullptr;
};
COLLECTION::List<texture_t> g_texture;



int LoadTexture(std::string path) {
	texture_t tex;

	for (int i = 0; i < g_texture.Count(); i++) {
		tex = g_texture.GetIndex(i);
		if (tex.path == path) return i;
	}

	int idx = g_texture.Count();
	tex.path = path;
	tex.texture = new Texture;
	// “Ç‚Ýž‚Þ
	HRESULT hr = tex.texture->Load(path.c_str());
	if (hr != S_OK) std::cout << path << std::endl;
	// “o˜^‚·‚é
	g_texture.Add(tex);

	return idx;
}

Texture *GetTexture(int index) {
	return g_texture[index].texture;
}

void AllReleaseTexture(void) {
	while (g_texture.Count()) {
		texture_t tex = g_texture.GetIndex(0);
		delete tex.texture;
		g_texture.Remove(0);
	}
}

