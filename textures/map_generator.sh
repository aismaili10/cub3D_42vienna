#!/bin/bash

# ANSI COLORS
BLACK='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[0;37m'
NC='\033[0m' # No Color

# Function definitions
is_yes() {
    case "$1" in
        [yY] | [yY][eE][sS]) true ;;
        *) false ;;
    esac
}

is_no() {
    case "$1" in
        [nN] | [nN][oO]) true ;;
        *) false ;;
    esac
}

transform_orientation() {
    case "$1" in
        [nN] | [nN][oO][rR][tT][hH]) echo "N" ;;
        [sS] | [sS][oO][uU][tT][hH]) echo "S" ;;
        [eE] | [eE][aA][sS][tT]) echo "E" ;;
        [wW] | [wW][eE][sS][tT]) echo "W" ;;
        *) echo "error" ;;
    esac
}

# Validate player position
validate_position() {
    local x=$1 y=$2 w=$3 h=$4
    [[ $x -ge 1 && $x -le $((w - 2)) && $y -ge 1 && $y -le $((h - 2)) ]]
}

# Create 'generated_maps' directory if it doesn't exist
mkdir -p generated_maps

# Initial prompt
echo -e "${CYAN}Generate a valid map (Y/n)? (Default: yes):${NC}"
read valid
if is_no "$valid"; then
    valid="no"
else
    valid="yes"
fi

# Map generation parameters
echo -e "${CYAN}Enter map width:${NC}"
read width
echo -e "${CYAN}Enter map height:${NC}"
read height

if [[ "$valid" == "yes" && ($width -lt 3 || $height -lt 3) ]]; then
    echo -e "${RED}Error: Width and height must be at least 4 for a valid map.${NC}"
    exit 1
fi

echo -e "${CYAN}Enter player orientation (N,S,E,W or full name):${NC}"
read orientation_input
orientation=$(transform_orientation "$orientation_input")

if [[ "$valid" == "yes" && "$orientation" == "error" ]]; then
    echo -e "${RED}Error: Invalid orientation. Please enter N, S, E, W, North, South, East, or West.${NC}"
    exit 1
fi

echo -e "${CYAN}Enter player position. Available options are:${NC}"
echo -e "${CYAN}1 = Center of the map${NC}"
echo -e "${CYAN}2 = Top-left corner${NC}"
echo -e "${CYAN}3 = Bottom-left corner${NC}"
echo -e "${CYAN}4 = Top-right corner${NC}"
echo -e "${CYAN}5 = Bottom-right corner${NC}"
echo -e "${CYAN}6 = Specified position (x,y --> col,row)${NC}"
read position
if [[ "$position" == "6" ]]; then
    echo -e "${CYAN}Enter x or column (x = 0 is col = 0):${NC}"
    read player_x
    echo -e "${CYAN}Enter y or row (y = 0 is row = 0):${NC}"
    read player_y
    player_x=$((player_x))
    player_y=$((player_y))
else
    case $position in
        1) player_x=$((width / 2)); player_y=$((height / 2)) ;;
        2) player_x=1; player_y=1 ;;
        3) player_x=1; player_y=$((height - 2)) ;;
        4) player_x=$((width - 2)); player_y=1 ;;
        5) player_x=$((width - 2)); player_y=$((height - 2)) ;;
        *) echo -e "${RED}Invalid position option.${NC}"; exit 1 ;;
    esac
fi

if [[ "$valid" == "yes" ]]; then
    if ! validate_position "$player_x" "$player_y" "$width" "$height"; then
        echo -e "${RED}Error: Player position must be inside the map and not on the border.${NC}"
        exit 1
    fi
fi

# Filename setup
filename="generated_maps/map_${width}_x_${height}.cub"
version=0
while [[ -f "$filename" ]]; do
    ((version++))
    filename="generated_maps/map_${width}_x_${height}-$(printf "%02d" $version).cub"
done

# Generate the map and write to the file
{
    for (( y=0; y<$height; y++ )); do
        for (( x=0; x<$width; x++ )); do
            if [ $y -eq 0 ] || [ $y -eq $((height - 1)) ] || [ $x -eq 0 ] || [ $x -eq $((width - 1)) ]; then
                printf "1"
            elif [ $x -eq $player_x ] && [ $y -eq $player_y ]; then
                printf "%s" "$orientation"
            else
                printf "0"
            fi
        done
        printf "\n"
    done
} > "$filename"

echo -e "${GREEN}Map generated and saved as $filename${NC}"

