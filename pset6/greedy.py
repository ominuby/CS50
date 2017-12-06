import cs50
import sys
def main():
    print("O hai! How much change is owed?")
    cash = cs50.get_float()
    while cash < 0.00 :
        print("How much change is owed?")
        cash = cs50.get_float()
    cent = round(cash * 100)
    n = 0
    while cent > 0:
        if cent >= 25:
            cent = cent -25
            n = n + 1

        elif cent >= 10:
            cent = cent - 10
            n = n + 1

        elif cent >=5:
            cent = cent -5
            n = n + 1
        else:
            cent = cent - 1
            n = n + 1
    print("{}".format(n))
    exit(0);

if __name__ == "__main__":
    main()