import os

# Gain 和 Exposure 数组
gain_values = [
    64, 72, 80, 88, 96, 104, 112, 120, 128, 144, 160, 176, 192, 208,
    224, 240, 244, 274, 305, 335, 366, 396, 427, 457, 488, 549, 
    610, 671, 732, 793, 854, 915, 976, 1098
]

exposure_values = [
    1011, 909, 824, 750, 690, 630, 582, 546, 285, 255, 228, 204, 189,
    177, 162, 153, 150, 132, 120, 108, 99, 90, 84, 78, 72, 66, 60, 54, 51, 45, 42, 39, 36, 33
]

def rename_raw_files():
    for filename in os.listdir('.'):
        name, ext = os.path.splitext(filename)
        if ext.lower() == '.raw' and name.isdigit():
            index = int(name)
            if 1 <= index <= len(gain_values):
                gain = gain_values[index - 1]
                exposure = exposure_values[index - 1]
                new_filename = f"黑帧_case{index:02}_1280x960_10b_Gain{gain}_1_{exposure}_82lx.raw"

                if not os.path.exists(new_filename):
                    os.rename(filename, new_filename)
                    print(f"Renamed: {filename} → {new_filename}")
                else:
                    print(f"Skipped (target exists): {new_filename}")
            else:
                print(f"Index out of range: {filename}")
        else:
            print(f"Ignored: {filename}")

if __name__ == "__main__":
    rename_raw_files()
