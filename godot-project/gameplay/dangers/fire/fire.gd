extends Danger

export var tool_damage_per_second = 150

func use_tool(used_tool_type, delta):
	if used_tool_type == ToolType.EXTINGUISHER:
		if hp < tool_damage_per_second * delta:
			hp = 0
			play("off")
		else:
			hp = hp - tool_damage_per_second * delta
