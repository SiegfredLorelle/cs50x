# Print out a double half-pyrmamid of a specific height, similar to pyramids in Mario
# TODO
import cs50

# Keep on asking until a valid height was given
while True:
    height = cs50.get_int("Height: ")
    if (height >= 1) and (height <= 8):
        break

# Loop to each row
for i in range(height):
    # Left pyramid
    # Print white spaces
    for j in range(height - i - 1):
        print(" ", end="")
    # Print hashes
    for k in range(i + 1):
        print("#", end="")

    # Double white spaces between the pyramids
    print("  ", end="")

    # Right pyramid
    # Print hashes
    for l in range(i + 1):
        print("#", end="")

    # Go to next line
    print()