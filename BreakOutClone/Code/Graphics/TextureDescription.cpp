#include "TextureDescription.h"

TextureDescription::TextureDescription(SDL_Texture* texture, long textureId)
	:texture_(texture),
	textureId_(textureId)
{
}