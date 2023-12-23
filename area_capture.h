#pragma once
#include "cell.h"

class AreaCapture : public Cell {
public:
	AreaCapture(int x, int y) : Cell(x,y,LoadTexture("data/texture/area_capture.png")) { }

	void Update(void) override{}
	MAP_READ GetCellType(void) const override { return MAP_READ_MULTI_AREA_CAPTURE; }
};