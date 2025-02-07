import sys

def align_text(lines, symbol):
    # 找到所有行中符号的位置
    positions = [line.find(symbol) for line in lines if symbol in line]
    if not positions:
        print("未找到指定符号，保持原格式。")
        return lines
    
    max_pos = max(positions)  # 找到符号的最大索引
    
    aligned_lines = []
    for line in lines:
        if symbol in line:
            parts = line.split(symbol, 1)
            left_part = parts[0].rstrip()
            right_part = parts[1].lstrip()
            aligned_line = left_part.ljust(max_pos) + " " + symbol + " " + right_part
        else:
            aligned_line = line  # 保持无符号的行原样
        aligned_lines.append(aligned_line)
    
    return aligned_lines

def main():
    # symbol = input("请输入要对齐的符号（如 = 或 +）：").strip()
    symbol = '='
    print("请输入文本，输入空行结束输入：")
    
    lines = []
    while True:
        line = sys.stdin.readline().rstrip()
        if not line:
            break
        lines.append(line)
    
    aligned_lines = align_text(lines, symbol)
    print("\n对齐后的文本：")
    print("\n".join(aligned_lines))

if __name__ == "__main__":
    main()

# 使用脚本进行小规模的代码对齐，默认为‘=’，如有需要，可手动修改 symbol 参数值
# 使用方法：
# python 20250207_align_code.py