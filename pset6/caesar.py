import cs50
import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: ./caesar k")
        exit(1)
    else:
        k = sys.argv[1]
        print("plaintext:",end = "")
        pt = cs50.get_string()
        ct = pt
        for i in range(len(pt)):
            if pt

    exit(0)

if __name__ == "__main__":
    main()