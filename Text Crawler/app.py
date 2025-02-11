import random

class CryptGame:
    def __init__(self):
        # Initialize the game grid and game state
        self.grid = [
            ["Wall", "Wall", "Entrance", "Wall", "Wall", "Wall"],
            ["Wall", "Chamber", "Hallway", "Hallway", "Altar", "Wall"],
            ["Wall", "Hallway", "Hallway", "Locked Door", "Treasure Room", "Wall"],
            ["Wall", "Hallway", "Trap", "Hallway", "Hallway", "Wall"],
            ["Wall", "Storage", "Hallway", "Hallway", "Hidden Passage", "Wall"],
            ["Wall", "Wall", "Wall", "Wall", "Wall", "Wall"],
        ]

        # Descriptions of each room for the player to read
        self.descriptions = {
            "Entrance": "A heavy stone door behind you is sealed shut. Faint torchlight flickers ahead.",
            "Hallway": "Dust and cobwebs cling to the walls. The air is stale.",
            "Chamber": "An ancient chamber with strange markings on the walls. A broken torch lies here.",
            "Altar": "A stone altar covered in dust. A rusted key rests on top.",
            "Locked Door": "A large door with strange carvings. It won't budge without a key.",
            "Treasure Room": "Gold and relics are scattered across the room. An eerie silence fills the air.",
            "Trap": "The floor suddenly shifts! You barely step back in time to avoid a pitfall.",
            "Storage": "Old barrels and crates line the walls. Something moves in the shadows.",
            "Hidden Passage": "A passage with glowing symbols. Something about it feels unnatural.",
            "Wall": "A cold stone wall blocks your way."
        }

        # Movement hints to help the player navigate
        self.movement_hints = {
            "Entrance": "You see the heavy stone door that shut behind you when you started exploring.",
            "Hallway": "The passage stretches forward, all you can see is a long hallway that fades to darkness.",
            "Chamber": "You see a faint light flickering from ahead, it seems to lead into a chamber.",
            "Altar": "A strange glow emanates from further in.",
            "Locked Door": "A massive stone door stands ominously ahead.",
            "Treasure Room": "A sense of importance lingers in the air beyond.",
            "Trap": "A draft of air suggests an open space ahead.",
            "Storage": "The smell of damp wood and rot wafts from somewhere close.",
            "Hidden Passage": "Something feels off about the wall ahead, like it’s not entirely solid."
        }

        # Initial player position and state variables
        self.player_pos = [0, 2]  # Start at the Entrance
        self.inventory = []  # Empty inventory at the start
        self.game_over = False  # Game is not over yet
        self.turn_count = 0 # Counter to keep track of moves to initialize ghost
        self.ghost_kill_limit = 30 # Amount of turns we allow the user to use before we send them back to the entrance. If you change this variable, make sure to change show_ghost_description cases


    # Get the current location of the player based on the grid
    def get_location(self):
        return self.grid[self.player_pos[0]][self.player_pos[1]]

    # Describe the player's current location
    def describe_location(self):
        location = self.get_location()

        print(f"\n{self.descriptions.get(location, 'Nothing but darkness...')}")

        self.show_options()  # Display possible movement options
        self.random_event()  # Trigger a random event
        if location == "Trap":
            print("The floor shifts beneath you! You barely manage to avoid falling into a pit.")

    # Show available directions based on the player's current position
    def show_options(self):
        row, col = self.player_pos

        directions = {
            "north": (row - 1, col),
            "south": (row + 1, col),
            "east": (row, col + 1),
            "west": (row, col - 1),
        }

        print("\nPaths available:")

        for direction, (r, c) in directions.items():
            if 0 <= r < len(self.grid) and 0 <= c < len(self.grid[0]):
                room = self.grid[r][c]
                if room != "Wall":  # Only show directions that aren't blocked by walls
                    print(f"  - {direction.title()}: {self.movement_hints[room]}")

    # Random events that may occur during gameplay
    def random_event(self):
        events = [
            "You hear faint whispers in an unknown language...",
            "A sudden gust of wind blows through the hallway...",
            "You feel something cold brush against your neck...",
            "A distant sound of chains rattling echoes through the crypt...",
            "A strange marking on the wall glows briefly before fading."
        ]

        if random.random() < 0.3:  # 30% chance to trigger a random event for fun
            print(random.choice(events))

    # Move the player in a specific direction
    def move(self, direction):
        row, col = self.player_pos
        temp_pos = list(self.player_pos)  # Make a copy of the current position

        # Adjust the position based on the direction
        if direction == "north" and row > 0:
            temp_pos[0] -= 1
        elif direction == "south" and row < len(self.grid) - 1:
            temp_pos[0] += 1
        elif direction == "east" and col < len(self.grid[0]) - 1:
            temp_pos[1] += 1
        elif direction == "west" and col > 0:
            temp_pos[1] -= 1
        else:
            print("You can't go that way.")
            return

        desired_location = self.grid[temp_pos[0]][temp_pos[1]]

        if desired_location == "Wall":
            print("You bump into a cold, unforgiving stone wall. No way through here.")
        elif desired_location == "Locked Door" and "Key" not in self.inventory:
            print("The door is locked. A keyhole is carved with an eerie symbol.")
        else:
            self.player_pos = temp_pos  # Update the player's position
            self.describe_location()  # Describe the new location
            self.turn_count += 1 # Increment number of turns

    # Inspect the current room for items or important details
    def inspect(self):
        location = self.get_location()

        items = {
            "Chamber": "An old torch lies here, flickering faintly. Might be useful.",
            "Altar": "A rusted key sits on the altar, covered in dust.",
            "Storage": "You find an old note: 'The door hides more than gold… beware what lurks.'",
            "Trap": "You nearly step on a loose tile. Something is hidden below.",
            "Treasure Room": "The glint of gold fills your eyes, but an unsettling feeling lingers."
        }

        print(items.get(location, "Nothing particularly interesting here."))

    # Take an item from the current location
    def take(self, item):
        location = self.get_location()

        if item == "torch" and location == "Chamber":
            self.inventory.append("Torch")  # Add torch to inventory
            print("You pick up the torch. The crypt feels slightly less terrifying.")
        elif item == "key" and location == "Altar":
            self.inventory.append("Key")  # Add key to inventory
            print("You take the rusted key. It hums faintly in your palm.")
        else:
            print("There's nothing like that here.")

    # Use an item (like unlocking doors or lighting a torch)
    def use(self, item):
        if item == "key" and "Key" in self.inventory and self.get_location() == "Locked Door":
            print("You insert the key... The door groans as it unlocks!")
            self.grid[2][4] = "Treasure Room"  # Unlock the treasure room
        elif item == "torch" and "Torch" in self.inventory:
            print("The flickering torch illuminates the crypt. Shadows dance along the walls.")
        else:
            print("That doesn't seem to work.")

    def show_ghost_description(self, turn):
        if turn == self.ghost_kill_limit - 25:
            print("\nYou hear a strange sound off in the distance but think nothing of it.")
        elif turn == self.ghost_kill_limit - 20:
            print("\nThat strange sound you heard earlier sounds like it is getting closer to you, the crypt suddenly gets freezing cold.")
        elif turn == self.ghost_kill_limit - 10:
            print("\nYou heard a loud sound behind you. As you turn around, you see a ghostly figure slowly approaching you. You must find a way out fast!")
        elif turn == self.ghost_kill_limit - 5:
            print("\nThe ghost is right behind you! YOU NEED TO GET TO THE EXIT NOW!")


    # Main game loop
    def play(self):
        print("You wake up in a dark crypt. Your goal: escape and uncover the secrets hidden within.")
        print("Commands: go [north/south/east/west], inspect, take [item], use [item], quit")

        self.describe_location()  # Show the initial location description

        while not self.game_over:
            if self.turn_count == self.ghost_kill_limit:
                print("You see a ghastly figure approach you and stand in fear. Before you know it the ghost grabs onto you and you pass out. \n You wake back up at the entrance not reemembering what happened.")
                self.player_pos = [0, 2]  # Teleport user back to start
                self.inventory = []  # Empty the inventory
                self.turn_count = 0 # Reset turn count
                continue # Go back to while loop on this limit

            command = input("\n> ").strip().lower()
            parts = command.split(" ", 1)
            action = parts[0]

            # Handle player actions (move, inspect, take, use, quit)
            if action == "go" and len(parts) > 1:
                self.move(parts[1])
            elif action == "inspect":
                self.inspect()
            elif action == "take" and len(parts) > 1:
                self.take(parts[1])
            elif action == "use" and len(parts) > 1:
                self.use(parts[1]
                )
            elif action == "quit":
                print("You feel an overwhelming dread... You turn back, abandoning your quest.")
                break
            else:
                print("A whisper in the dark: 'That is not an option...'") # Invalid action

            # Hint the player to pick up the torch if they haven't yet
            if self.get_location() == "Chamber" and "Torch" not in self.inventory:
                print("\nThe faint light flickers ahead in the Chamber. You might want to pick up the torch before moving forward.")
            
            # End the game if the player enters the treasure room with the key
            if self.get_location() == "Treasure Room" and "Key" in self.inventory:
                print("You step into the treasure chamber... but something else is here.\nA dark presence looms...")
                self.game_over = True

            self.show_ghost_description(self.turn_count)

        print("Game Over.")

# Initialize and start the game
if __name__ == "__main__":
    game = CryptGame()
    game.play()
