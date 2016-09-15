#include "PaddleDescription.h"
#include "..\Graphics\GraphicsProperties.h"
//-------------------------------------------------------------------------------------------
void
PaddleDescription::setDefaultValues()
{
	setInitialConditions((SCREEN_WIDTH - PADDLE_WIDTH) / 2, (SCREEN_HEIGHT - PADDLE_HEIGHT), PADDLE_WIDTH, PADDLE_HEIGHT, 0, 0);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------