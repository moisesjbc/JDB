extends KinematicBody2D

export var speed = 400


func _process(delta):
	var velocity: Vector2 = Vector2(-1, 0)
	translate(speed * velocity * delta)


func _on_visibility_notifier_screen_exited():
	queue_free()
