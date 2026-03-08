# Estimates a fraction using the continued fraction algorithm
import math

def ContinuedFraction(num):
    """Returns bottom of fraction."""

    #main part of algorithm
    #every denominator is equal to fractional part of num
    #save num as 1 over fractional part of num
    #repeat
    #change range(10) for more accurate continued fracitons
    denominators = []
    for i in range(10):
        denominators.append(int(num))

        #if num is an integer the algorithm is done
        if (num-int(num)) == 0:
            break
        
        else:
            num = 1/(num-int(num))

    #finds the continued fraction's value
    num2 = 0
    for i in range(1, len(denominators)+1):
        if i == len(denominators):
            num2 = 1/num2
            extra = denominators[0]
        else:
            num2 = 1/(denominators[-i]+num2)

    # will return this list
    bottomOfFrac = []

    #converts to int over int and prints
    #change range(1, 27) for possible messages
    for i in range(1, 27):
        gcd = math.gcd(i, int(num2*i))
        bottom = int((num2*i))//gcd
        #print(f'{i}: {(i//gcd)+(extra*bottom)}/{bottom}', end=' ')
        bottomOfFrac.append(bottom)
    #print()

    return bottomOfFrac

