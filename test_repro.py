import subprocess
import time

def run_test():
    print("Running QEMU...")
    cmd = ["./lenovo_300e.sh", "qemu", "live"]
    
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    
    start_time = time.time()
    found_prompt = False
    while time.time() - start_time < 30:
        line = proc.stdout.readline()
        if not line: break
        print(f"QEMU: {line.strip()}")
        if "root@sageos:/#" in line:
            found_prompt = True
            break
            
    if not found_prompt:
        print("Timeout waiting for prompt")
        proc.kill()
        return False

    print("Sending command...")
    proc.stdin.write("sagepkg init\n")
    proc.stdin.flush()
    
    success = False
    start_time = time.time()
    while time.time() - start_time < 10:
        line = proc.stdout.readline()
        if not line: break
        print(f"QEMU: {line.strip()}")
        if "SagePkg" in line or "info" in line or "Forcing" in line:
            success = True
            break
        if "error:" in line or "failed to load module" in line:
            print("FOUND ERROR")
            break
            
    proc.kill()
    return success

if __name__ == "__main__":
    if run_test():
        print("SUCCESS")
    else:
        print("FAILURE")
