from pyray import *
from enum import Enum

W, H = 1200, 800
P_WIDTH, P_HEIGHT = 150.0, 20.0
LIGHT_RADIUS = 200  # Example radius for light
light_enabled = True # Set to True when testing / playing, otherwise False for development

class Direction(Enum):
    LEFT = -1
    RIGHT = 1

class CurrentState(Enum):
    RUNNING_DOWN = 0
    RUNNING_LEFT = 1
    RUNNING_RIGHT = 2
    RUNNING_UP = 3
    IDLE = 4

class AnimationType(Enum):
    REPEATING = 0
    ONESHOT = 1

class Animation:
    def __init__(self, fst, lst, offset, spd, anim_type):
        self.fst = fst
        self.lst = lst
        self.cur = fst
        self.offset = offset
        self.spd = spd
        self.rem = spd
        self.type = anim_type

    def update(self):
        dt = get_frame_time()
        self.rem -= dt
        if self.rem < 0:
            self.rem = self.spd
            self.cur += 1
            if self.cur > self.lst:
                if self.type == AnimationType.REPEATING:
                    self.cur = self.fst
                elif self.type == AnimationType.ONESHOT:
                    self.cur = self.lst

    def frame(self):
        frame_width = 48
        frame_height = 68
        x = (self.cur % 3) * frame_width  # 3 frames horizontally
        y = self.offset * frame_height    # 4 rows vertically (offset is row number)
        return Rectangle(x, y, frame_width, frame_height)

class Player:
    def __init__(self, sprite):
        self.rect = Rectangle(W / 2, H / 2, 64, 64)
        self.pos = Vector2(0, 0)
        self.vel = Vector2(0, 0)
        self.sprite = sprite
        self.dir = Direction.RIGHT
        self.state = CurrentState.RUNNING_DOWN
        self.animations = [
            Animation(0, 2, 0, 0.1, AnimationType.REPEATING), # Walk DOWN
            Animation(0, 2, 1, 0.1, AnimationType.REPEATING), # Walk LEFT
            Animation(0, 2, 2, 0.1, AnimationType.REPEATING), # Walk RIGHT
            Animation(0, 2, 3, 0.1, AnimationType.REPEATING), # Walk UP
            Animation(1, 1, 0, 0.1, AnimationType.ONESHOT), # IDLE
        ]

    def move(self):
        self.vel.x = 0
        self.vel.y = 0
        self.state = CurrentState.IDLE

        if is_key_down(KEY_A):
            self.vel.x = -200
            self.dir = Direction.LEFT
            self.state = CurrentState.RUNNING_LEFT
        elif is_key_down(KEY_D):
            self.vel.x = 200
            self.dir = Direction.RIGHT
            self.state = CurrentState.RUNNING_RIGHT
        elif is_key_down(KEY_W):
            self.vel.y = -200
            self.state = CurrentState.RUNNING_UP
        elif is_key_down(KEY_S):
            self.vel.y = 200
            self.state = CurrentState.RUNNING_DOWN
        
        if is_key_down(KEY_A) and is_key_down(KEY_W):
            self.vel.x = -200
            self.vel.y = -200
            self.dir = Direction.LEFT
            self.state = CurrentState.RUNNING_LEFT
        elif is_key_down(KEY_A) and is_key_down(KEY_S):
            self.vel.x = -200
            self.vel.y = 200
            self.dir = Direction.LEFT
            self.state = CurrentState.RUNNING_LEFT
        elif is_key_down(KEY_D) and is_key_down(KEY_W):
            self.vel.x = 200
            self.vel.y = -200
            self.dir = Direction.RIGHT
            self.state = CurrentState.RUNNING_RIGHT
        elif is_key_down(KEY_D) and is_key_down(KEY_S):
            self.vel.x = 200
            self.vel.y = 200
            self.dir = Direction.RIGHT
            self.state = CurrentState.RUNNING_RIGHT
    
    def move_rect_by_vel(self):
        dt = get_frame_time()
        self.rect.x += self.vel.x * dt
        self.rect.y += self.vel.y * dt

    def draw(self):
        source = self.animations[self.state.value].frame()
        draw_texture_pro(self.sprite, source, self.rect, Vector2(0, 0), 0, WHITE)

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

walls = [
    Rectangle(H / 2, 780.0, P_WIDTH, P_HEIGHT),
    Rectangle(H / 2, 550.0, P_WIDTH, P_HEIGHT),
    Rectangle(P_WIDTH, 680.0, P_WIDTH, P_HEIGHT)
]

# Function to check if wall is in the light range
def distance_to_light(wall, player_pos):
    # Calculate distance from each corner of the wall to the light source
    corners = [
        (wall.x, wall.y),  # top-left corner
        (wall.x + wall.width, wall.y),  # top-right corner
        (wall.x, wall.y + wall.height),  # bottom-left corner
        (wall.x + wall.width, wall.y + wall.height)  # bottom-right corner
    ]
    
    # Find the shortest distance from any corner to the light using Euclidean Distance
    min_distance = min([((corner_x - player_pos.x) ** 2 + (corner_y - player_pos.y) ** 2) ** 0.5 for corner_x, corner_y in corners])
    return min_distance

# Function to draw walls only if they are within the light range
def draw_walls(walls, player_pos):
    for rect in walls:
        if light_enabled:
            # Only calculate and draw walls within the light range
            dist = distance_to_light(rect, player_pos)
            if dist <= LIGHT_RADIUS:
                # Calculate the alpha based on distance (closer = more visible)
                alpha = int(255 * (1 - (dist / LIGHT_RADIUS)))
                alpha = max(0, alpha)  # Ensure alpha doesn't go below 0
                color = Color(139, 69, 19, alpha)  # Dark brown with fading alpha

                # Draw the wall with the calculated color (faded effect)
                draw_rectangle(int(rect.x), int(rect.y), int(rect.width), int(rect.height), color)
        else:
            # If light is disabled, just draw the walls normally without the fade effect for development purposes
            draw_rectangle(int(rect.x), int(rect.y), int(rect.width), int(rect.height), Color(139, 69, 19))


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