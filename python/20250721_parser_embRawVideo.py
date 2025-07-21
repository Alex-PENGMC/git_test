import struct
import os

input_file = "RAW_VIDEO_1920x1082_30000_7_20250718_141021.raw"
width = 1920
height = 1082
bytes_per_pixel = 2
frame_size = width * height * bytes_per_pixel
line_size = width * bytes_per_pixel

log_file = "first_line_log.txt"

def parse_first_line(data_bytes, frame_index):
    lines = []

    # 16bit小端解包成单词序列
    words = struct.unpack('<' + 'H' * (len(data_bytes) // 2), data_bytes)

    data_list = []
    i = 0
    while i < len(words) - 1:
        tag = words[i]
        data = words[i + 1]
        if tag != 0x0DA0:
            print(f"[Warning] Frame {frame_index}: Invalid tag 0x{tag:04X} at index {i}")
            i += 2
            continue
        data_list.append(data >> 4)  # 右移4位，保留12bit有效数据
        i += 2

    if len(data_list) < 36:
        lines.append(f"Frame {frame_index}: Too few data points ({len(data_list)})")
        return lines

    # 前8个参数，两两组合
    param_names = ["flip", "active_w", "active_h", "hts", "vts", "shutter_dcg", "shutter_spd", "shutter_vs"]
    for idx in range(8):
        high = data_list[idx * 2]
        low = data_list[idx * 2 + 1]
        value = (high << 8) | low
        lines.append(f"  {param_names[idx]} = {value}")

    # 后4组，组名指定
    group_names = ["gain_hcg", "gain_lcg", "gain_spd", "gain_vs"]

    for grp in range(4):
        base = 16 + grp * 5
        high1, low1 = data_list[base], data_list[base + 1]
        field_A = (high1 << 8) | low1

        reg1 = data_list[base + 2] & 0xF       # bit[3:0]
        reg2 = data_list[base + 3] & 0xFF      # bit[7:0]
        reg3 = (data_list[base + 4] >> 6) & 0x3  # bit[7:6]
        field_B = (reg1 << 10) | (reg2 << 2) | reg3
        # b1, b2, b3 = data_list[base + 2], data_list[base + 3], data_list[base + 4]
        # field_B = (b1 << 16) | (b2 << 8) | b3
        lines.append(f"  {group_names[grp]}: Again = {field_A:04x}, Dgain = 0x{field_B:04x}")

    return lines

def main():
    if not os.path.exists(input_file):
        print(f"[Error] File not found: {input_file}")
        return

    with open(input_file, 'rb') as f, open(log_file, 'w') as log:
        frame_index = 0
        while True:
            frame_data = f.read(frame_size)
            if len(frame_data) < frame_size:
                break

            first_line = frame_data[:line_size]
            log_lines = parse_first_line(first_line, frame_index)
            log.write(f"Frame {frame_index}:\n")
            for line in log_lines:
                log.write(line + '\n')

            frame_index += 1

    print(f"✅ Done. Parsed data saved to: {log_file}")

if __name__ == "__main__":
    main()
