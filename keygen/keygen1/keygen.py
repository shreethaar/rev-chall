import random

def generate_key():
    while True:
        key = 'A'  # First character must be 'A'
        for _ in range(15):
            key += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789')
        checksum = sum(ord(c) for c in key)
        if checksum % 17 == 0:
            return key

print("Generated key:", generate_key())
