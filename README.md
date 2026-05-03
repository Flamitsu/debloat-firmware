# debloat-firmware
This is a script made for gentoo to debloat linux firmware package to the minimal required.
## Index

1. What does this project do?
2. Why is this project useful?
3. How can you use this script?
4. Current state

---

## 1. What does this project do?

This project provides an automated script to debloat your linux-firmware package on Gentoo.

By default, linux-firmware can consume up to ~1.6GiB, including hundreds of firmware files you don't actually need.
This script extracts only the firmware files that your system actively loads during boot, based on dmesg logs.
Proof of how it works: https://www.youtube.com/watch?v=N_J7fijXpo8
---

## 2. Why is this project useful?

This project is useful for users who want a minimalist Gentoo system with only the firmware that their hardware actually uses.

Benefits:

- Faster downloads when updating linux-firmware
- Less disk space used (especially on small SSDs or embedded systems)
- Less CPU usage when decompressing/installing large firmware archives

---

## 3. How can you use this script?

- This script needs root privileges because it modifies Portage configuration.
```bash
    clang -O3 main.c -o debloat-firmware-x86_64
    sudo ./debloat-firmware-x86_64; doas ./debloat-firmware-x86_64
```

## 4. Current state
Now the script works as intended, trying to make a new flag in the "linux-firmware" package in Gentoo!
