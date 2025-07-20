#!/bin/bash

if [ ! -d " ./system/world" ]; then
    echo " ./system/world not exist, use xmake to build"
    exit 1
fi

world_files=$(find  ./system/world -name "*.wld")
if [ -z "$world_files" ]; then
    echo " ./system/world/*.wld not exist, use xmake to build"
    exit 1
fi

terraria_folder="${XDG_DATA_HOME}/Terraria/Worlds/"
tmodloader_folder="${XDG_DATA_HOME}/Terraria/tModLoader/Worlds/"

for world_file in $world_files; do
    world_filename=$(basename "$world_file")

    if [ -d "$terraria_folder" ]; then
        echo "Copy world to $terraria_folder$world_filename"
        cp "$world_file" "$terraria_folder$world_filename"
    fi

    if [ -d "$tmodloader_folder" ]; then
        echo "Copy world to $tmodloader_folder$world_filename"
        cp "$world_file" "$tmodloader_folder$world_filename"
    fi
done

echo "Copy world complete"
