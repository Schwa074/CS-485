from pyray import *

init_window(1280, 720, "Hello, World")

while not window_should_close():

    begin_drawing()

    clear_background(WHITE)

    fps = get_fps()
    draw_text(f"FPS: {fps}", 0, 0, 12, RED)

    end_drawing()

close_window()