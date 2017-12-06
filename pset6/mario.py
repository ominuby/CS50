import cs50
import sys
def main():
    print("Height:")
    h = cs50.get_int()
    while(h < 0 or h > 23):
        print("Height:")
        h = cs50.get_int()
    print("{}".format(h))

    if h != 0:
        for i in range(h):
            for space in range(h-1-i):
                print("", end="")
            for block in range(i+2):
                print("#", end="")
            print("");
    exit(1);

if __name__ == "__main__":
    main()