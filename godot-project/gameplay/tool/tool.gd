extends KinematicBody2D

onready var current_tool = null


func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_HIDDEN)
	set_current_tool($hand)


func _process(delta):
	global_position = get_global_mouse_position()


func set_current_tool(new_tool):
	if current_tool:
		current_tool.visible = false
		current_tool.get_node("audio").stop()
	current_tool = new_tool
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
			set_current_tool($hand)
		elif Input.is_action_just_pressed("tool_extinguisher"):
			set_current_tool($extinguisher)
		elif Input.is_action_just_pressed("tool_lighter"):
			set_current_tool($lighter)
		elif Input.is_action_just_pressed("tool_gavel"):
			set_current_tool($gavel)
