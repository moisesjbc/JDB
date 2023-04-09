class_name DangersManager


var _dangers = [
	preload("res://gameplay/dangers/fire/fire.tscn")
]


func get_middle_point(baseline: Line2D):
	return Vector2((baseline.points[0].x + baseline.points[1].x) / 2, (baseline.points[0].y + baseline.points[1].y) / 2)


func generate_dangers(sandwich):
	var sandwich_point = get_middle_point(sandwich.get_node("baseline"))
	
	var new_danger = _dangers[0].instance()
	var danger_point = get_middle_point(new_danger.get_node("baseline"))
	
	var danger_global_position = sandwich.global_position + sandwich_point - danger_point
	
	sandwich.add_danger(new_danger, danger_global_position)
