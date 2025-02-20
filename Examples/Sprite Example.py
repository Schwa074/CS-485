from pyray import *
from enum import Enum

W, H = 600, 600

class Direction(Enum):
    LEFT = -1
    RIGHT = 1

class CurrentState(Enum):
    DEAD = 0
    RUNNING = 1
    IDLE = 2
    ROLLING = 3

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
        x = (self.cur % (self.lst + 1)) * 16
        y = self.offset * 16
        return Rectangle(x, y, 16, 16)

class Player:
    def __init__(self, sprite):
        self.rect = Rectangle(W / 2, H / 2, 64, 64)
        self.vel = Vector2(0, 0)
        self.sprite = sprite
        self.dir = Direction.RIGHT
        self.state = CurrentState.IDLE
        self.animations = [
            Animation(0, 7, 0, 0.1, AnimationType.ONESHOT),
            Animation(0, 5, 1, 0.1, AnimationType.REPEATING),
            Animation(0, 3, 5, 0.1, AnimationType.REPEATING),
            Animation(0, 2, 9, 0.1, AnimationType.REPEATING),
        ]

    def move(self):
        self.vel.x = 0
        self.vel.y = 0
        self.state = CurrentState.IDLE

        if is_key_down(KEY_A):
            self.vel.x = -200
            self.dir = Direction.LEFT
            self.state = CurrentState.RUNNING
        elif is_key_down(KEY_D):
            self.vel.x = 200
            self.dir = Direction.RIGHT
            self.state = CurrentState.RUNNING
        
        if is_key_down(KEY_SPACE):
            self.vel.x = self.dir.value * 300
            self.state = CurrentState.ROLLING
    
    def move_rect_by_vel(self):
        self.rect.x += self.vel.x * get_frame_time()
        self.rect.y += self.vel.y * get_frame_time()

    def draw(self):
        source = self.animations[self.state.value].frame()
        source.width *= self.dir.value
        draw_texture_pro(self.sprite, source, self.rect, Vector2(0, 0), 0, WHITE)

init_window(W, H, "Hero Animation Example")
hero = load_texture("Examples/assets/herochar-sprites/herochar_spritesheet.png")
player = Player(hero)

while not window_should_close():
    player.move()
    player.move_rect_by_vel()
    player.animations[player.state.value].update()
    
    begin_drawing()
    clear_background(SKYBLUE)
    player.draw()
    end_drawing()

unload_texture(hero)
close_window()
