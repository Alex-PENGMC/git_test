import argparse

def generate_file(output_file, data_byte, size):
    """生成指定大小的.hex文件，并填充指定数据"""
    # 将文件大小从字符串转换为字节数
    size_multiplier = {'B': 1, 'K': 1024, 'M': 1024**2}
    unit = size[-1].upper()
    if unit not in size_multiplier:
        raise ValueError("无效的大小单位，请使用 B, K, 或 M")
    num_size = int(size[:-1]) * size_multiplier[unit]

    # 填充内容
    data = bytes([data_byte]) * num_size

    # 写入文件
    with open(output_file, "wb") as f:
        f.write(data)

    print(f"文件生成成功: {output_file} ({num_size}字节)")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="生成指定大小的.hex文件")
    parser.add_argument("output_file", type=str, help="输出文件名，必须以.hex结尾")
    parser.add_argument("data_byte", type=lambda x: int(x, 16), help="填充数据（十六进制格式，例如 0xaa）")
    parser.add_argument("size", type=str, help="文件大小（例如 1K, 2K, 100B）")

    args = parser.parse_args()

    # 检查文件后缀
    if not args.output_file.endswith(".hex"):
        raise ValueError("输出文件名必须以 .hex 结尾")

    generate_file(args.output_file, args.data_byte, args.size)

# 生成固定内容的hex文件及大小
# 使用方法：
# python 20250101_generate_size_content_hex.py 0xaa.hex 0xaa 1K
