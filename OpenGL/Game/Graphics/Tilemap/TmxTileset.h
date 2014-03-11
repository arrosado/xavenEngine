//-----------------------------------------------------------------------------
// TmxTileset.h
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
#pragma once

#include <string>
#include <vector>
#include <tinyxml2.h>
#include <Game/Graphics/GraphicsCommon.h>

class SpriteSheet;

using namespace tinyxml2;


namespace Tmx 
{
	class Tile;

	//-------------------------------------------------------------------------
	// A class used for storing information about each of the tilesets.
	// A tileset is a collection of tiles, of whom each may contain properties.
	// The tileset class itself does not have properties.
	//-------------------------------------------------------------------------
	class Tileset 
	{
	public:
		Tileset();
		~Tileset();

		// Parse a tileset element.
		void Parse(const XMLNode *tilesetNode);

		// Returns the global id of the first tile.
		int GetFirstGid() const { return first_gid; }

		// Returns the name of the tileset.
		const std::string &GetName() const { return name; }

		// Get the width of a single tile.
		int GetTileWidth() const { return tile_width; } 

		// Get the height of a single tile.
		int GetTileHeight() const { return tile_height; }

		// Get the margin of the tileset.
		int GetMargin() const { return margin; }

		// Get the spacing of the tileset.
		int GetSpacing() const { return spacing; }

		// Returns a a single tile of the set.
		const Tmx::Tile *GetTile(int index) const;

		// Returns the whole tile collection.
		SpriteSheet* GetTiles() const { return tiles; } 

		const void RenderTile(int id, float x, float y, Vector3Df position);

	private:
		int first_gid;
		
		std::string name;
		
		float tile_width;
		float tile_height;
		float margin;
		float spacing;
		SpriteSheet* tiles;
	};
};