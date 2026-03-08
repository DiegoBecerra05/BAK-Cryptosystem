import random
# 1. Alice & Bob get a private b (integer)
# 2. Alice puts messsage m in a vector
# 3. Alice divides each element in m by b and adds a small error e1...en to it.
# 4. Alice shares it with the whole wide world.
# 5. Bob receives c (ciphertext vector)
# 6. Bob multiplies each element in c by b
# 7. The error is small enough for Bob to round it out.

def encryptDecryptor():
    """Returns the b entered, ciphertext, and size."""
    # input b. b must be greater than 26 so it doesn't cause m to be irrecoverable.
    b = int(input("Input a private integer b: "))

    # a wonderful message in integers (a=1, b=2, ... z=26)
    m = [9, 1, 13, 1, 19, 5, 18, 22, 1, 14, 20, 15, 6, 20, 8,
        5, 19, 5, 3, 18, 5, 20, 6, 9, 18, 5, 23, 9, 5, 12, 4,
        5, 18, 15, 6, 20, 8, 5, 6, 12, 1, 13, 5, 15, 6, 1, 14,
        15, 18, 25, 15, 21, 3, 1, 14, 14, 15, 20, 16, 1, 19, 19,
        20, 8, 5, 4, 1, 18, 11, 6, 9, 18, 5, 23, 9, 12, 12, 14,
        15, 20, 1, 22, 1, 9, 12, 25, 15, 21, 6, 12, 1, 13, 5, 15,
        6, 21, 4, 21, 14, 7, 15, 2, 1, 3, 11, 20, 15, 20, 8, 5,
        19, 8, 1, 4, 15, 23, 25, 15, 21, 3, 1, 14, 14, 15, 20,
        16, 1, 19, 19]

    size = len(m)

    # divide each element in m by b
    new_m = [(element / b) for element in m]

    # generate error
    # error bound is e < |1/(2b)|
    error_bound = 1 / (2 * b)
    e = [random.uniform(-error_bound, error_bound) for num in range(size)]
    #e = [0 for num in range(len(m))] # no error

    #random.uniform has inclusive bounds, so try again for boundaries
    for item in e:
        while item == error_bound or item == -error_bound:
            item = random.uniform(-error_bound, error_bound)

    #print("Error:\n")
    #print(e)

    # create ciphertext
    c = []
    for num in range(size):
        c.append(new_m[num] + e[num])

#    print("\n\nEvil Eve knows the ciphertext:")
#    for item in c:
#        print(item, end=" ")
#    print()

    # Bob multiplies ciphertext by private int b
    bobs_array = [num * b for num in c]

    # print original
#    print("Original:")
#    for item in m:
#        print(item, end=" ")
#    print()

    recovered =[round(item) for item in bobs_array]
#    print("\nBob recovers:")
#    for item in recovered:
#        print(item, end=" ")
#    print()

    # difference between Bob's recovered & original mesesage
    #difference = []
    #for num in range(len(m)):
    #    difference.append(m[num] - recovered[num])

    # print out difference
    #print("Difference between original and recovered:")
    #for item in difference:
    #    print(item, end=" ")
    #print()

    return b, size, c