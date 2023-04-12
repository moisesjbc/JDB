extends Node2D

onready var sandwich_scene = preload("res://gameplay/sandwich/sandwich.tscn")


func _ready():
	spawn_sandwich()


func spawn_sandwich():
	var sandwich = sandwich_scene.instance()
	$sandwiches.call_deferred("add_child", sandwich)
	sandwich.global_position = $spawn_position.global_position


func _on_spawn_area_body_exited(body):
	if body.is_in_group("sandwiches"):
		spawn_sandwich()
