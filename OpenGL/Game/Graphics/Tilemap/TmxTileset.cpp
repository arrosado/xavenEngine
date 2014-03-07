//-----------------------------------------------------------------------------
// TmxTileset.cpp
//
// Copyright (c) 2010-2012, Tamir Atias
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL TAMIR ATIAS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Tamir Atias
//-----------------------------------------------------------------------------
#include <tinyxml2.h>

#include <TmxTileset.h>
#include <TmxTile.h>
#include <Game/Graphics/SpriteSheet/SpriteSheet.h>
#include <Game/Graphics/Image/Image.h>

using namespace tinyxml2;
using std::vector;
using std::string;

namespace Tmx 
{
	Tileset::Tileset() 
		: first_gid(0)
		, name()
		, tile_width(0)
		, tile_height(0)
		, margin(0)
		, spacing(0)
		, tiles(NULL)
	{
	}

	Tileset::~Tileset() 
	{
		if (tiles) {
			delete tiles;
		}
	}

	void Tileset::Parse(const XMLNode *tilesetNode) 
	{
		const XMLElement *tilesetElem = tilesetNode->ToElement();

		// Read all the attributes into local variables.
		first_gid	= atof(tilesetElem->Attribute("firstgid"));
		tile_width	= atof(tilesetElem->Attribute("tilewidth"));
		tile_height = atof(tilesetElem->Attribute("tileheight"));
		margin		= atof(tilesetElem->Attribute("margin"));
		spacing		= atof(tilesetElem->Attribute("spacing"));

		name = tilesetElem->Attribute("name");

		// Parse the image.
		const XMLNode *imageNode = tilesetNode->FirstChildElement("image");
		
		if (imageNode) 
		{
			tiles = new SpriteSheet(imageNode->ToElement()->Attribute("source"), Size2DfMake(tile_width, tile_height), spacing, margin, GL_LINEAR);
		}

	}

	const void Tileset::RenderTile(int id, float x, float y, Vector2Df position) {
		int xs, ys;

		xs = id % tiles->GetColumns();
		ys =  id / tiles->GetColumns();
		
		x *= tile_width;
		y *= tile_height;

		Image* img = NULL;
		img = tiles->GetSprite(xs, ys);
		if(img) img->Render(Vector2DfMake(x + position.x, y + position.y - (tile_height/2)), Size2DfZero, 0.0f, false);
	}
};
