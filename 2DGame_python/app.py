from pyray import *
from player import *
from enum import Enum
from constants import *
from environment import *
import pygame
from pytmx.util_pygame import load_pygame
import pytmx

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

class Tile(pygame.sprite.Sprite):
    def __init__(self,pos,surf,groups):
        super().__init__(groups)
        self.image = surf
        self.rect = self.image.get_rect(topleft = pos)

pygame.init()
screen = pygame.display.set_mode((W,H))
tmx_data = pytmx.util_pygame.load_pygame('Tile_files\\test_map.tmx')
sprite_group = pygame.sprite.Group()

for layer in tmx_data.visible_layers:
    if hasattr(layer, 'data'):
        for x,y,surf in layer.tiles():
            pos = (x * 16, y * 16)
            Tile(pos = pos, surf = surf, groups = sprite_group)

for obj in tmx_data.objects:
    pos = obj.x, obj.y
    if obj.image:
        Tile(pos = pos, surf = obj.image, groups = sprite_group)

while not window_should_close():
    if is_key_pressed(KEY_L):
        light_enabled = not light_enabled  # Toggle the light effect, TODO Remove this functionality during submission

    previous_x = player.rect.x
    previous_y = player.rect.y

    # Update player movement and camera position
    player.move()
    player.move_rect_by_vel()

    if check_collision_with_walls(player, walls):
        # If there's a collision, revert to previous position
        player.rect.x = previous_x
        player.rect.y = previous_y
    
    player.animations[player.state.value].update()
    
    # Update camera target position to follow the player
    camera.target = Vector2(player.rect.x, player.rect.y)
    
    # Smoothly move the camera
    move_camera_smooth_follow(camera, player)
    
    begin_drawing()
    # clear_background(BLACK)
    begin_mode_2d(camera)
    
    player.draw()

    if light_enabled:
        # Draw light circle
        # draw_circle_gradient(int(player.rect.x + player.rect.width / 2),
        #     int(player.rect.y + player.rect.height / 2),
        #     LIGHT_RADIUS, Color(255, 255, 0, 100),
        #     Color(255, 255, 0, 0))

        draw_walls(walls, Vector2(player.rect.x, player.rect.y))
    else:
        # If light is disabled, draw the walls normally (no fade effect)
        for rect in walls:
            draw_rectangle(int(rect.x), int(rect.y), int(rect.width), int(rect.height), DARKBROWN)  # Standard wall color
    
    end_mode_2d()
    end_drawing()
    sprite_group.draw(screen)
    pygame.display.update()

# Unload resources and close window
unload_texture(hero)
close_window()