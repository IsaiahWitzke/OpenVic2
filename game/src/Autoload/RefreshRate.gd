extends Node

const unlimited_refresh_rate : int = 0
# used as a placeholder, never meant as an actual refresh rate
const vsync_refresh_rate : int = -1
const default_refresh_rate : int = vsync_refresh_rate
const default_vsync_mode := DisplayServer.VSYNC_ENABLED

var refresh_rates := [
	vsync_refresh_rate,
	30,
	60,
	90,
	120,
	144,
	365,
	unlimited_refresh_rate
]

func get_display_name(refresh_rate: int) -> String:
	if refresh_rate == unlimited_refresh_rate:
		return "Unlimited"
	elif refresh_rate == vsync_refresh_rate:
		return "VSync"
	else:
		return str(refresh_rate) + "hz"

# setting refresh rate here does 2 things:
# 1: sets the engine's max fps and/or vsync
# 2: ensures our list of refresh rates includes the given refresh_rate, appending it on if not currently present
# if an invalid refresh rate is passed in, a default value is set
func set_refresh_rate(refresh_rate: int = RefreshRate.default_refresh_rate) -> void:

	# Changing refresh rate on godot 4.0 seems to break the game on linux, leaving it commented out right now
	# see: https://github.com/godotengine/godot/issues/73922 & linked issues within issue

	if refresh_rate < vsync_refresh_rate:
		push_error("Tried to set invalid refresh rate: %d, setting to default refresh rate: %d" % [refresh_rate, unlimited_refresh_rate])
		Engine.set_max_fps(unlimited_refresh_rate)
		# DisplayServer.window_set_vsync_mode(default_vsync_mode)
		return

	if refresh_rate not in refresh_rates:
		push_warning("Setting refresh rate to non-standard value (%d)" % [refresh_rate])
		refresh_rates.append(refresh_rate)

	if refresh_rate == vsync_refresh_rate:
		Engine.set_max_fps(unlimited_refresh_rate)
		# DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_ENABLED)
	else:
		Engine.set_max_fps(refresh_rate)
		# DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_DISABLED)

