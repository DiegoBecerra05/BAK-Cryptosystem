from encryptionDecryption import encryptDecryptor
from Continuedfraction import ContinuedFraction

# Call the function; encrypts message, returns b-value
bValue, messageLength, ciphertext = encryptDecryptor()
possible_bVals = []
for num in range(messageLength):
    number = ciphertext[num]
    new_bVals = ContinuedFraction(number)
    for item in new_bVals:
        possible_bVals.append(item)

total_tries = 26 * messageLength
correct_ans = 0

for item in possible_bVals:
    if item == bValue:
        correct_ans += 1


print(f"Total numbers: {total_tries}")
print(f"Correct numbers: {correct_ans}")