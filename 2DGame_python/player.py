from pyray import *
from enums import *
from constants import *

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