import time
import sys
import ctypes
import random
from datetime import datetime

class TimingProtectedChallenge:
    def __init__(self, flag):
        self.flag = ''.join(chr(ord(c) ^ 0x33) for c in flag)  # Basic XOR encoding
        self.debug_detected = False
        self.timing_samples = []
        
    def precise_timing(self):
        """Get high-precision timestamp"""
        if hasattr(time, 'clock_gettime'):
            return time.clock_gettime(time.CLOCK_MONOTONIC)
        return time.time()
    
    def timing_check(self):
        """Perform timing analysis to detect debuggers"""
        samples = []
        for _ in range(10):
            start = self.precise_timing()
            # Simple operation that should take consistent time
            sum([i * i for i in range(1000)])
            end = self.precise_timing()
            samples.append(end - start)
            
        # Calculate statistical measures
        avg = sum(samples) / len(samples)
        variance = sum((x - avg) ** 2 for x in samples) / len(samples)
        
        # Debuggers typically cause higher variance in execution time
        VARIANCE_THRESHOLD = 1e-8
        if variance > VARIANCE_THRESHOLD:
            self.debug_detected = True
            
        self.timing_samples = samples
        return variance
        
    def reveal_flag(self):
        """Reveals flag with anti-debug protection"""
        if self.debug_detected:
            # If debugger detected, show fake flag
            fake_flag = "CTF{n1c3_try_d3bugg3r}"
            self._slow_print(fake_flag)
            return
            
        # Additional runtime timing checks
        base_time = self.precise_timing()
        
        for i in range(len(self.flag)):
            # Continuous timing verification
            current_time = self.precise_timing()
            time_diff = current_time - base_time
            
            # Check if time progression is natural
            if time_diff < 0 or time_diff > 30:  # Allow max 30 seconds
                print("\nTime manipulation detected!")
                return
                
            # Decode and print character
            char = chr(ord(self.flag[i]) ^ 0x33)
            sys.stdout.write(char)
            sys.stdout.flush()
            
            # Random sleep between 0.1 and 0.3 seconds
            sleep_time = random.uniform(0.1, 0.3)
            time.sleep(sleep_time)
            
    def _slow_print(self, text):
        """Helper for printing with delays"""
        for char in text:
            sys.stdout.write(char)
            sys.stdout.flush()
            time.sleep(0.2)

if __name__ == "__main__":
    # Real flag
    real_flag = "CTF{t1m1ng_4n4ly515_15_k3y}"
    
    challenge = TimingProtectedChallenge(real_flag)
    
    # Initial timing analysis
    variance = challenge.timing_check()
    
    # Start flag reveal
    print("Revealing flag...")
    challenge.reveal_flag()
    print("\n")
