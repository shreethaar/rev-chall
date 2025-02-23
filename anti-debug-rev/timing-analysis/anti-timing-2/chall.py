# Challenge Creator Implementation
import time
import sys

class DelayedFlagChallenge:
    def __init__(self, flag):
        self.flag = flag
        self.reveal_delay = 0.5  # 500ms delay between each character
        
    def slow_reveal(self):
        """Reveals the flag character by character with delays"""
        sys.stdout.write("Flag: ")
        sys.stdout.flush()
        
        for char in self.flag:
            time.sleep(self.reveal_delay)  # Intentional delay
            sys.stdout.write(char)
            sys.stdout.flush()
            
    def encode_flag(self):
        """Stores flag in memory but obfuscated"""
        self.encoded = ''.join(chr(ord(c) ^ 0x33) for c in self.flag)
        
    def check_char(self, position, guess):
        """Validates a single character guess at a position"""
        if position >= len(self.flag):
            return False
        return self.flag[position] == guess

# Example usage to create challenge
if __name__ == "__main__":
    challenge = DelayedFlagChallenge("CTF{t1m3_1s_k3y}")
    challenge.encode_flag()
    challenge.slow_reveal()  # Will take ~7.5 seconds to reveal full flag
