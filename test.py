import subprocess

# File paths
valid_files = ["valid/reg_5x5.txt", "valid/reg_10x6.txt", "valid/reg_15x8.txt"]
invalid_files = ["invalid/ireg_height_5x5.txt", "invalid/ireg_width_5x5.txt"]

# Test cases
test_cases = [
    {"input": "W\nA\nS\nD\nQ\n", "description": "Move in all directions and quit."},
    {"input": "W\nW\n", "description": "Try to move up twice."},
    {"input": "A\nA\n", "description": "Try to move left twice."},
    {"input": "S\nS\n", "description": "Try to move down twice."},
    {"input": "D\nD\n", "description": "Try to move right twice."},
    {"input": "M\nQ\n", "description": "View map and quit."},
    {"input": "Q\n", "description": "Quit without moving."},
    {"input": "X\n", "description": "Invalid input."},
    {"input": "W\n" * 100, "description": "Move up multiple times."},
    {"input": "A\n" * 100, "description": "Move left multiple times."},
]

# Run test
def run_test(file, width, height):
    print(f"Testing {file}:")
    for i, case in enumerate(test_cases, 1):
        print(f"Test {i}: {case['description']}")
        try:
            result = subprocess.run(["python", "maze_game.py", file, width, height], input=case["input"], text=True, capture_output=True, timeout=5)
            print("Output:")
            print(result.stdout)
        except subprocess.TimeoutExpired:
            print("Test timed out.")
        print("=" * 50)

# Run test cases
for file in valid_files:
    width = input(f"Enter the width for {file}: ")
    height = input(f"Enter the height for {file}: ")
    run_test(file, width, height)

for file in invalid_files:
    width = input(f"Enter the width for {file}: ")
    height = input(f"Enter the height for {file}: ")
    run_test(file, width, height)
