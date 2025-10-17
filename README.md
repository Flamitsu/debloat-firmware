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

It also enables the savedconfig USE flag for linux-firmware automatically, so Portage will respect your minimal configuration.

---

## 2. Why is this project useful?

This project is useful for users who want a minimalist Gentoo system with only the firmware that their hardware actually uses.

Benefits:

- Faster downloads when updating linux-firmware
- Less disk space used (especially on small SSDs or embedded systems)
- Less CPU usage when decompressing/installing large firmware archives
- Cleaner and more transparent system state

---

## 3. How can you use this script?

- This script needs root privileges because it modifies Portage configuration.
- gcc // clang main.c -o main .
- doas // sudo ./main

## 4. Current state
Already rewritted the script in C.
Waiting it to be merged into a useflag with emerge in the linux-firmware package.
- The kernel ring buffer can be read already, i dont need to rely on log files.
