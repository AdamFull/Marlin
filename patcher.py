import os, mmap
path_folder = "Marlin"

replace = "lcd/ultralcd.h"
to = "display.h"

ignore = ["display.h"]

for path, subdirs, files in os.walk(path_folder):
    for name in files:
        if not name in ignore:
            with open(os.path.join(path, name), "r") as file:
                s = mmap.mmap(file.fileno(), 0, access=mmap.ACCESS_READ)
                if s.find(replace.encode()) != -1:
                    print(os.path.join(path, name))
                else:
                    file.close()