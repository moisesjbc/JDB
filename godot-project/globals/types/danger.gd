class_name Danger

extends AnimatedSprite

var hp = 100


func connect_area_signal(signal_name, method_name):
	var error_code = get_node("area").connect(signal_name, self, method_name)
	if error_code:
		print("ERROR connecting signal :" + str(error_code))


func _ready():
	connect_area_signal("body_entered", "_on_area_body_entered")
	connect_area_signal("body_exited", "_on_area_body_exited")
	play("on")


func _on_area_body_entered(body):
	if body.is_in_group("tools"):
		body.current_danger = self


func _on_area_body_exited(body):
	if body.is_in_group("tools") and body.current_danger == self:
		body.current_danger = null
