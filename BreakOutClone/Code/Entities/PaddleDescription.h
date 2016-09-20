#pragma once
#include "EntityDescription.h"

const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 10;
const int MAX_VEL_X = 20;

class PaddleDescription: public EntityDescription {
public:
	void setDefaultValues();
};
