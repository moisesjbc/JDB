extends KinematicBody2D

onready var current_tool = null
onready var current_tool_type = null
var current_danger = false


func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_HIDDEN)
	set_current_tool($hand, ToolType.HAND)


func is_one_hit_tool(tool_type):
	return tool_type in [ToolType.HAND, ToolType.GAVEL]
	

func _process(delta):
	global_position = get_global_mouse_position()
	if current_danger and not is_one_hit_tool(current_tool_type) and current_tool.animation == "on":
		current_danger.use_tool(current_tool_type, delta)


func set_current_tool(new_tool, new_tool_type):
	if current_tool:
		current_tool.visible = false
		current_tool.get_node("audio").stop()
	current_tool = new_tool
	current_tool_type = new_tool_type
	current_tool.visible = true
	
	current_tool.play("off")


func _input(event):
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT:
		if event.pressed:
			current_tool.play("on")
			current_tool.get_node("audio").play()
		else:
			current_tool.play("off")
			current_tool.get_node("audio").stop()
	else:
		if Input.is_action_just_pressed("tool_hand"):
			set_current_tool($hand, ToolType.HAND)
		elif Input.is_action_just_pressed("tool_extinguisher"):
			set_current_tool($extinguisher, ToolType.EXTINGUISHER)
		elif Input.is_action_just_pressed("tool_lighter"):
			set_current_tool($lighter, ToolType.LIGHTER)
		elif Input.is_action_just_pressed("tool_gavel"):
			set_current_tool($gavel, ToolType.GAVEL)
