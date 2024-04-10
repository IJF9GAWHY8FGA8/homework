import sys

class MazeGame:
    """Class to run a maze game where the player navigates from start to finish."""
    
    def __init__(self, filename, dimensions):
        self.filename = filename
        self.width, self.height = dimensions
        self.maze, self.player_position = self.load_maze()

    def load_maze(self):
        """Load the maze from a file and find the start position."""
        try:
            with open(self.filename, 'r') as f:
                maze = [list(line.strip()) for line in f.readlines()]
            player_position = next(((i, j) for i, row in enumerate(maze) 
                                    for j, val in enumerate(row) if val == 'S'), None)
            if player_position is None:
                raise ValueError("Starting point 'S' not found in the maze.")
            return maze, player_position
        except FileNotFoundError:
            print("Error: Maze file not found.")
            sys.exit(1)
        except ValueError as e:
            print(e)
            sys.exit(1)

    def print_maze(self):
        """Print the current state of the maze with the player's position."""
        for i, row in enumerate(self.maze):
            print(' '.join('X' if (i, j) == self.player_position else val for j, val in enumerate(row)))

    def move_player(self, direction):
        """Move the player to a new position based on the input direction."""
        movement = {'w': (-1, 0), 'a': (0, -1), 's': (1, 0), 'd': (0, 1)}
        dx, dy = movement.get(direction.lower(), (0, 0))
        new_row, new_col = self.player_position[0] + dx, self.player_position[1] + dy
        
        if self.is_valid_move(new_row, new_col):
            self.player_position = (new_row, new_col)
            if self.maze[new_row][new_col] == 'E':
                print("Congratulations! You've reached the exit.")
                sys.exit(0)

    def is_valid_move(self, row, col):
        """Check if the move is within the maze boundaries and not through walls."""
        if 0 <= row < self.height and 0 <= col < self.width:
            if self.maze[row][col] != '#':
                return True
            else:
                print("You cannot move through walls.")
                return False
        print("You cannot move off the edge of the map.")
        return False

    def start_game(self):
        """Start the maze game loop."""
        print("Welcome to the Maze Game!")
        self.game_loop()

    def game_loop(self):
        """Game loop to process user input and move the player."""
        while True:
            user_input = input("Enter W/A/S/D to move (or M to view map, Q to quit): ").strip().lower()
            if user_input == 'q':
                print("Thanks for playing. Goodbye!")
                break
            elif user_input == 'm':
                self.print_maze()
            elif user_input in 'wasd':
                self.move_player(user_input)
            else:
                print("Invalid input. Please use W, A, S, D to move or M to view the map.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: ./maze filename width height")
        sys.exit(1)

    filename = sys.argv[1]
    dimensions = tuple(map(int, sys.argv[2:4]))
    if not (5 <= dimensions[0] <= 100 and 5 <= dimensions[1] <= 100):
        print("Error: Width and height must be between 5 and 100.")
        sys.exit(1)

    game = MazeGame(filename, dimensions)
    game.start_game()
