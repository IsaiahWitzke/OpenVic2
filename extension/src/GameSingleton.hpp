#pragma once

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>

#include "openvic2/GameManager.hpp"

namespace OpenVic2 {
	struct TerrainVariant : HasIdentifier, HasColour {
	private:
		const godot::Ref<godot::Image> image;
	public:
		TerrainVariant(std::string const& new_identfier, colour_t new_colour,
			godot::Ref<godot::Image> const& new_image);
		TerrainVariant(TerrainVariant&&) = default;

		godot::Ref<godot::Image> get_image() const;
	};
	class GameSingleton : public godot::Object {
		GDCLASS(GameSingleton, godot::Object)

		static GameSingleton* singleton;

		GameManager game_manager;

		godot::Vector2i image_subdivisions;
		godot::Ref<godot::Texture2DArray> province_shape_texture;
		godot::Ref<godot::Image> province_colour_image;
		godot::Ref<godot::ImageTexture> province_colour_texture;
		Mapmode::index_t mapmode_index = 0;
		IdentifierRegistry<TerrainVariant> terrain_variants;
		Map::terrain_variant_map_t terrain_variant_map;
		godot::Ref<godot::Texture2DArray> terrain_texture;

		godot::Error _parse_province_identifier_entry(godot::String const& identifier, godot::Variant const& entry);
		godot::Error _parse_region_entry(godot::String const& identifier, godot::Variant const& entry);
		godot::Error _parse_terrain_entry(godot::String const& identifier, godot::Variant const& entry);
		void _tick();
	protected:
		static void _bind_methods();

	public:
		static GameSingleton* get_singleton();

		GameSingleton();
		~GameSingleton();

		godot::Error load_province_identifier_file(godot::String const& file_path);
		godot::Error load_water_province_file(godot::String const& file_path);
		godot::Error load_region_file(godot::String const& file_path);
		godot::Error load_terrain_file(godot::String const& file_path);
		godot::Error load_map_images(godot::String const& province_image_path, godot::String const& terrain_image_path);

		/* Post-load/restart game setup - reset the game to post-load state
		 * and (re)generate starting data, e.g. buildings.
		 */
		godot::Error setup();

		int32_t get_province_index_from_uv_coords(godot::Vector2 const& coords) const;
		godot::Dictionary get_province_info_from_index(int32_t index) const;
		int32_t get_width() const;
		int32_t get_height() const;
		float get_aspect_ratio() const;

		/* The cosmetic terrain textures stored in a Texture2DArray.
		 */
		godot::Ref<godot::Texture> get_terrain_texture() const;

		/* Number of (vertical, horizontal) subdivisions the province shape image
		 * was split into when making the province_shape_texture to ensure no
		 * piece had a dimension greater than 16383.
		 */
		godot::Vector2i get_province_shape_image_subdivisions() const;

		/* The map, encoded in RGB8 with RG representing province index and B representing terrain texture.
		 * To support a wider range of GPUs, the image is divided so that no piece has a dimension
		 * greater than 16383 and the pieces are stored in a Texture2DArray.
		 */
		godot::Ref<godot::Texture> get_province_shape_texture() const;

		/* The colour each province should be tinted, arranged in
		 * index order into a 256x256 RGB8 texture.
		 */
		godot::Ref<godot::Texture> get_province_colour_texture() const;

		/* Generate the province_colour_texture from the current mapmode.
		 */
		godot::Error update_colour_image();

		int32_t get_mapmode_count() const;
		godot::String get_mapmode_identifier(int32_t index) const;
		godot::Error set_mapmode(godot::String const& identifier);
		int32_t get_selected_province_index() const;
		void set_selected_province(int32_t index);

		godot::Error expand_building(int32_t province_index, godot::String const& building_type_identifier);

		void set_paused(bool paused);
		void toggle_paused();
		bool is_paused() const;
		void increase_speed();
		void decrease_speed();
		bool can_increase_speed() const;
		bool can_decrease_speed() const;
		godot::String get_longform_date() const;
		void try_tick();
	};
}
