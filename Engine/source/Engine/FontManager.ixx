module;

#include <ft2build.h>
#include FT_FREETYPE_H

export module FontManager;

export import std.core;

class FontManager {

	FontManager()
	{
		FT_Library  library;   /* handle to library     */
		FT_Face     face;      /* handle to face object */

		if (auto error = FT_Init_FreeType(&library); error != FT_Err_Ok) {
			throw std::runtime_error(std::format("Received an error trying to initialize freetype. Error Code: %i", error));
		}

		std::string font_path = "/usr/share/fonts/truetype/arial.ttf";

		if (auto error = FT_New_Face(library, font_path.c_str(), 0, &face); error == FT_Err_Unknown_File_Format)
			throw std::runtime_error(std::format("Invalid file format.", error));
		
		else if (error)
			throw std::runtime_error(std::format("Received an error trying to load '%S'. Error Code: %i", font_path, error));

		if (auto error = FT_Set_Pixel_Sizes(face,  0, 16); error != FT_Err_Ok)
			throw std::runtime_error(std::format("Received an error trying to set the pixel size of the glyph to be rendered. Error Code: %i", error));

		auto glyph_index = FT_Get_Char_Index(face, 0x41);

		if (auto error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT); error != FT_Err_Ok)
			throw std::runtime_error(std::format("Received an error trying to load a glyph. Error Code: %i", error));

		/* convert to an anti-aliased bitmap */
		if (auto error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL); error != FT_Err_Ok) 
			throw std::runtime_error(std::format("Received an error trying to render glyph. Error Code: %i", error));

		auto bitmap = face->glyph->bitmap;

		// I think I need to create something to write out pngs?
	}
};