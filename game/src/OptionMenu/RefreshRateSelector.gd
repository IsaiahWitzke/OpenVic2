extends SettingOptionButton

func _setup_button():
	clear()
	for refresh_rate in RefreshRate.refresh_rates:
		add_item(RefreshRate.get_display_name(refresh_rate))
		set_item_metadata(item_count - 1, refresh_rate)
	
	# set to default value initially
	RefreshRate.set_refresh_rate()
	default_selected = RefreshRate.refresh_rates.find(
		RefreshRate.default_refresh_rate
	)


	
func _get_value_for_file(index: int):
	if not _valid_index(index):
		return null
	return get_item_metadata(index)

func _set_value_from_file(load_value) -> void:
	if typeof(load_value) != TYPE_INT or load_value < RefreshRate.vsync_refresh_rate:
		push_error("Setting value '%s' invalid for setting [%s] %s" % [str(load_value), section_name, setting_name])
		return
	
	RefreshRate.set_refresh_rate(load_value)
	# add a new option if this is a new refresh rate being loaded
	# i.e. the user manually edited the settings .cfg file so they could play the game at a nonstandard fps
	if load_value not in RefreshRate.refresh_rates:
		add_item(RefreshRate.get_display_name(load_value))
		set_item_metadata(item_count - 1, load_value)
		selected = item_count - 1
	else:
		selected = RefreshRate.refresh_rates.find(load_value)


func _on_item_selected(index : int):
	DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_ADAPTIVE)
	if _valid_index(index):
		RefreshRate.set_refresh_rate(get_item_metadata(index))
	else:
		push_error("Invalid RefreshRateSelector index: %d" % index)
