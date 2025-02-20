from pyray import *
from enum import Enum
import math

W, H = 800, 800
P_WIDTH, P_HEIGHT = 150.0, 20.0

class BallState(Enum):
    IDLE = 0
    JUMP = 1

class Ball:
    def __init__(self, x, y, radius, color):
        self.pos = Vector2(x, y)
        self.vel = Vector2(0, 0)
        self.radius = radius
        self.ang = 0.0
        self.color = color
        self.state = BallState.IDLE

def draw_platforms(platforms):
    for rect in platforms:
        draw_rectangle(int(rect.x), int(rect.y), int(rect.width), int(rect.height), MAROON)

def draw_ball(ball):
    draw_circle(int(ball.pos.x), int(ball.pos.y), int(ball.radius), ball.color)
    draw_line(int(ball.pos.x), int(ball.pos.y), int(ball.pos.x + math.cos(ball.ang) * ball.radius), int(ball.pos.y + math.sin(ball.ang) * ball.radius), BLACK)

def platform_ball_col_check(platforms, ball):
    if ball.state == BallState.IDLE:
        for rect in platforms:
            if check_collision_circle_rec(ball.pos, ball.radius, rect):
                ball.vel.y *= -0.8
                ball.pos.y -= 0.05
                if ball.pos.y > rect.y and ball.pos.x < rect.x:
                    ball.pos.x -= 0.01
                elif ball.pos.y > rect.y and ball.pos.x > rect.x:
                    ball.pos.x += 0.01
                break

def move_ball(ball):
    ball.vel.x = 0.0
    if is_key_down(KEY_A):
        ball.vel.x = -300.0
        ball.ang = (ball.ang - 0.002) % 6.28318
    elif is_key_down(KEY_D):
        ball.vel.x = 300.0
        ball.ang = (ball.ang + 0.002) % 6.28318
    if is_key_down(KEY_SPACE) and ball.state == BallState.IDLE:
        ball.vel.y = -600.0
    ball.state = BallState.JUMP if ball.vel.y < 0 else BallState.IDLE

def apply_velocity(ball):
    dt = get_frame_time()
    ball.pos.x += ball.vel.x * dt
    ball.pos.y += ball.vel.y * dt

def apply_gravity(ball):
    if ball.vel.y < 500:
        ball.vel.y += 0.5

def move_camera_smooth_follow(camera, ball):
    min_speed, min_effect_dist, fraction_speed = 30.0, 10.0, 0.8
    dt = get_frame_time()
    diff = Vector2(ball.pos.x - camera.target.x, ball.pos.y - camera.target.y)
    length = vector2_length(diff)
    if length > min_effect_dist:
        speed = max(min_speed, fraction_speed * length)
        camera.target.x += diff.x * (speed * dt / length)
        camera.target.y += diff.y * (speed * dt / length)

init_window(W, H, "Ball Game")

platforms = [
    Rectangle(0.0, 780.0, P_WIDTH, P_HEIGHT),
    Rectangle(0.0, 550.0, P_WIDTH, P_HEIGHT),
    Rectangle(P_WIDTH, 680.0, P_WIDTH, P_HEIGHT)
]

ball = Ball(32.0, 700.0, 32.0, RED)
camera = Camera2D(Vector2(W / 2, H / 2), ball.pos, 0.0, 1.0)

while not window_should_close():
    move_ball(ball)
    apply_velocity(ball)
    apply_gravity(ball)
    platform_ball_col_check(platforms, ball)
    move_camera_smooth_follow(camera, ball)
    
    begin_drawing()
    clear_background(RAYWHITE)
    begin_mode_2d(camera)
    draw_platforms(platforms)
    draw_ball(ball)
    end_mode_2d()
    end_drawing()

close_window()
