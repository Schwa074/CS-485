from pyray import *
from constants import *

walls = [
    Rectangle(H / 2, 780.0, P_WIDTH, P_HEIGHT),
    Rectangle(H / 2, 550.0, P_WIDTH, P_HEIGHT),
    Rectangle(P_WIDTH, 680.0, P_WIDTH, P_HEIGHT)
]

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