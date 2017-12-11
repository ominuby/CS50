import cs50
import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: ./caesar k")
        exit(1)
    else:
        k = int (sys.argv[1])
        print("plaintext:",end = "")
        pt = cs50.get_string()
        ct = []
        for i in pt:
            if i.isalpha():
                if i.isupper():
                    ct.append(chr((ord(i) - 65 + k) % 26 + 65))
                else:
                    ct.append(chr((ord(i) - 97 + k) % 26 + 97))
            else:
                ct.append(i)
        print("ciphertext:",end = "")
        print("".join(ct))
        exit(0)

if __name__ == "__main__":
    main()