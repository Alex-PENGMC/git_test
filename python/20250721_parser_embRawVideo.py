import struct
import os
import glob

# 视频文件参数配置
width = 1920
height = 1082
bytes_per_pixel = 2
frame_size = width * height * bytes_per_pixel
line_size = width * bytes_per_pixel

def parse_first_line(data_bytes, frame_index):
    """解析每帧数据，计算白平衡通道实际值（整数部分+小数部分/1024）"""
    lines = []

    # 16bit小端格式解包
    words = struct.unpack('<' + 'H' * (len(data_bytes) // 2), data_bytes)

    data_list = []
    i = 0
    while i < len(words) - 1:
        tag = words[i]
        data = words[i + 1]
        if tag != 0x0DA0:
            i += 2
            continue
        data_list.append(data >> 4)  # 右移4位，保留12bit有效数据
        i += 2

    # 检查数据点是否充足（原有36个 + 32个白平衡 = 68个）
    if len(data_list) < 68:
        lines.append(f"Frame {frame_index}: Too few data points ({len(data_list)}/68 required)")
        return lines

    # 解析前8个参数
    param_names = ["flip", "active_w", "active_h", "hts", "vts", 
                  "shutter_dcg", "shutter_spd", "shutter_vs"]
    for idx in range(8):
        high = data_list[idx * 2]
        low = data_list[idx * 2 + 1]
        value = (high << 8) | low
        lines.append(f"  {param_names[idx]} = {value}")

    # 解析4组增益参数
    group_names = ["gain_hcg", "gain_lcg", "gain_spd", "gain_vs"]
    gain_conversions = []

    for grp in range(4):
        base = 16 + grp * 5
        high1, low1 = data_list[base] & 0xF, (data_list[base + 1] >> 4) & 0xF
        field_A = (high1 << 4) | low1

        reg1 = data_list[base + 2] & 0xF
        reg2 = data_list[base + 3] & 0xFF
        reg3 = (data_list[base + 4] >> 6) & 0x3
        reg2_reg3 = (reg2 << 2) | reg3
        
        again_integer = (field_A >> 4)
        again_fraction = (field_A & 0x0F)
        again_value = again_integer + (again_fraction / 16)
        
        dgain_integer = reg1
        dgain_fraction = reg2_reg3
        dgain_value = dgain_integer + (dgain_fraction / 1024)
        
        gain_conversions.append({
            "name": group_names[grp],
            "again_value": again_value,
            "dgain_value": dgain_value
        })
        
        lines.append(f"  {group_names[grp]}: Again = {field_A:02x}, Dgain = {reg1:01x} {reg2_reg3:04x}")
    
    # 增益转换结果
    lines.append("")
    for conv in gain_conversions:
        lines.append(f"  {conv['name']} converted: Again = {conv['again_value']:.3f}X, Dgain = {conv['dgain_value']:.3f}X")

    # 解析四组白平衡数据（含实际值计算）
    lines.append("")
    wb_group_names = ["hcg_wb", "lcg_wb", "spd_wb", "vs_wb"]
    # 通道定义：(名称, h索引, l索引)
    channels = [("B", 0, 1), ("Gb", 2, 3), ("Gr", 4, 5), ("R", 6, 7)]
    
    for wb_grp in range(4):
        wb_base = 36 + wb_grp * 8
        channel_parts = []
        
        for channel_name, h_idx, l_idx in channels:
            # 1. 合并高低位得到16位值（h是高8位，l是低8位）
            h_val = data_list[wb_base + h_idx]
            l_val = data_list[wb_base + l_idx]
            combined = (h_val << 8) | l_val  # 合并为16位值（0xHHLL）
            
            # 2. 提取[14:10]位作为整数部分（5位，范围0-31）
            # 右移10位后取低5位（0x1F是5位掩码）
            integer_part = (combined >> 10) & 0x1F
            
            # 3. 提取低10位作为小数部分
            fractional_part = combined & 0x3FF  # 0x3FF是10位掩码（0-1023）
            
            # 4. 计算实际值
            actual_value = integer_part + (fractional_part / 1024)
            
            # 5. 格式化显示（高低位+实际值）
            channel_str = f"{channel_name}:{h_val:02x} {l_val:02x} ({actual_value:.3f}X)"
            channel_parts.append(channel_str)
        
        # 合并通道为一行
        lines.append(f"  {wb_group_names[wb_grp]}: {', '.join(channel_parts)}")

    return lines

def process_file(input_file):
    log_file = os.path.splitext(input_file)[0] + ".txt"
    
    if not os.path.exists(input_file):
        print(f"[Error] File not found: {input_file}")
        return
    
    print(f"Processing {input_file}...")
    
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

    print(f"✅ Completed. Results saved to: {log_file}")

def main():
    raw_files = glob.glob("*.raw")
    
    if not raw_files:
        print("No .raw files found in the current directory.")
        return
    
    for raw_file in raw_files:
        process_file(raw_file)

if __name__ == "__main__":
    main()
    