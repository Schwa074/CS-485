from pyray import *
from player import *
from enum import Enum
from constants import *
from environment import *

def move_camera_smooth_follow(camera, player):
    min_speed, min_effect_dist, fraction_speed = 30.0, 10.0, 0.8
    dt = get_frame_time()
    
    # Use player.rect.x and player.rect.y for camera target updates
    diff = Vector2(player.rect.x - camera.target.x, player.rect.y - camera.target.y)
    length = vector2_length(diff)
    
    if length > min_effect_dist:
        speed = max(min_speed, fraction_speed * length)
        camera.target.x += diff.x * (speed * dt / length)
        camera.target.y += diff.y * (speed * dt / length)

# Initialize window and player
init_window(W, H, "Crypt Escape")
hero = load_texture("assets/character-test/charactersheet.png")
player = Player(hero)

# Camera initialized to follow player
camera = Camera2D(Vector2(W / 2, H / 2), Vector2(player.rect.x, player.rect.y), 0.0, 1.0)

while not window_should_close():
    if is_key_pressed(KEY_L):
        light_enabled = not light_enabled  # Toggle the light effect, TODO Remove this functionality during submission

    # Update player movement and camera position
    player.move()
    player.move_rect_by_vel()
    player.animations[player.state.value].update()
    
    # Update camera target position to follow the player
    camera.target = Vector2(player.rect.x, player.rect.y)
    
    # Smoothly move the camera
    move_camera_smooth_follow(camera, player)
    
    begin_drawing()
    clear_background(BLACK)
    begin_mode_2d(camera)
    
    if light_enabled:
        # Draw light circle
        draw_circle_gradient(int(player.rect.x + player.rect.width / 2), int(player.rect.y + player.rect.height / 2), LIGHT_RADIUS, Color(255, 255, 0, 100), Color(255, 255, 0, 0))
    
    player.draw()

    if light_enabled:
        draw_walls(walls, Vector2(player.rect.x, player.rect.y))
    else:
        # If light is disabled, draw the walls normally (no fade effect)
        for rect in walls:
            draw_rectangle(int(rect.x), int(rect.y), int(rect.width), int(rect.height), DARKBROWN)  # Standard wall color
    
    end_mode_2d()
    end_drawing()

# Unload resources and close window
unload_texture(hero)
close_window()