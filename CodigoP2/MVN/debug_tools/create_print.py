import sys

for line in sys.stdin:
    for char in line:
        print("LV /00%02X" % ord(char))
        print("PD /100")
