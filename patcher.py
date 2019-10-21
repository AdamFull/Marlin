import os, mmap
path_folder = "Marlin"

replace = "lcd/ultralcd.h"
to = "display.h"

include_path = {
    path_folder: ["Configuration_nextion.h"], 
    path_folder+"/src/": ["display.h", "HardwareSerial0.cpp"],
    path_folder+"/src/nextion/": ["command_queue.cpp", "command_queue.h", "nextionlcd.cpp", "nextionlcd.h", "nextionlcdelements.cpp", "nextionlcdelements.h", "printercontrol.cpp", "printercontrol.h"],
    path_folder+"/src/nextion/NextionLib/": []}

ignore = ["display.h"]

for path, subdirs, files in os.walk(path_folder):
    for name in files:
        if not name in ignore:
            with open(os.path.join(path, name), "r+") as file:
                s = mmap.mmap(file.fileno(), 0, access=mmap.ACCESS_READ)
                if s.find(replace.encode()) != -1:
                    print(os.path.join(path, name))
                else:
                    file.close()