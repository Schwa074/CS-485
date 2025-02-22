from pyray import *
from constants import *

# TODO Probably should split map into it's own class that has rooms and we build this array no the fly for organization and sanity
# Possibly Create structure like Enviroment <- Map <- Rooms <- Walls
# So Enviroment will import map, map will import room, and room will build walls like this
walls = [
    Rectangle(H / 2 + 100, 350.0, P_WIDTH * 1.5, P_HEIGHT), # Top Entrance
    Rectangle(H / 2 + 100 + (P_WIDTH * 1.5), 350.0, P_HEIGHT, P_WIDTH), # Right Entrance
    Rectangle(H / 2 + 100 + (P_WIDTH * 1.5), 350.0 + P_WIDTH, P_HEIGHT, P_WIDTH), # Hallway from entrance ( Right )
    Rectangle(H / 2 + 100 + (P_WIDTH * 1.5), 350.0 + (P_WIDTH * 2), P_HEIGHT, P_WIDTH), # Hallway from entrance ( Right )
    Rectangle(H / 2 + 100 + (P_WIDTH * 1.5), 350.0 + (P_WIDTH * 3), P_HEIGHT, P_WIDTH), # Hallway from entrance ( Right )
    Rectangle(H / 2 + 100 + (P_WIDTH * 1.5), 350.0 + (P_WIDTH * 4), P_HEIGHT, P_WIDTH), # Hallway from entrance ( Right )
    Rectangle(H / 2 + 100, 350.0, P_HEIGHT, P_WIDTH), # Left Entrance
    Rectangle(H / 2 + 100, 350.0 + P_WIDTH, P_HEIGHT, P_WIDTH), # Hallway from entrance ( left )
    Rectangle(H / 2 + 100, 350.0 + (P_WIDTH * 2), P_HEIGHT, P_WIDTH), # Hallway from entrance ( left )
    Rectangle(H / 2 + 100, 350.0 + (P_WIDTH * 3), P_HEIGHT, P_WIDTH), # Hallway from entrance ( left )
    Rectangle(H / 2 + 100, 350.0 + (P_WIDTH * 4), P_HEIGHT, P_WIDTH), # Hallway from entrance ( left )

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
    # Needs to be this complex based on large wall heights and widths. Maybe we just make it more tedious and do a bunch of small wallsS
    # If the wall is horizontal (width > height)
    if wall.width > wall.height:
        num_points = 10  # Number of points to check along the width
        step_size = wall.width / (num_points - 1)
        min_distance = float('inf')

        for i in range(num_points):
            x = wall.x + i * step_size
            y = wall.y  # Only vary x-coordinate along the width
            dist = ((x - player_pos.x) ** 2 + (y - player_pos.y) ** 2) ** 0.5
            min_distance = min(min_distance, dist)
        
    # If the wall is vertical (height > width)
    else:
        num_points = 10  # Number of points to check along the height
        step_size = wall.height / (num_points - 1)
        min_distance = float('inf')

        for i in range(num_points):
            x = wall.x  # Only vary y-coordinate along the height
            y = wall.y + i * step_size
            dist = ((x - player_pos.x) ** 2 + (y - player_pos.y) ** 2) ** 0.5
            min_distance = min(min_distance, dist)

    return min_distance

def check_collision_with_walls(player, walls):
        for wall in walls:
            if check_collision_recs(player.rect, wall):
                return True
        return False # Went through all the walls and found no collisions