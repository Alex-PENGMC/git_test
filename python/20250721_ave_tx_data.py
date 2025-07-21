import argparse

def reverse_bits(byte):
    """反转单个字节的 bit 顺序（MSB ↔ LSB）"""
    return int('{:08b}'.format(byte)[::-1], 2)

def encode_reg_value(val):
    """将 32-bit 寄存器值转换为发送波形（小端 + 每字节 bit 翻转）"""
    if not (0 <= val <= 0xFFFFFFFF):
        raise ValueError("Value must be a 32-bit unsigned integer")
    
    bytes_be = val.to_bytes(4, 'big')
    bytes_le = bytes_be[::-1]
    encoded = [reverse_bits(b) for b in bytes_le]
    return encoded

def decode_waveform(wave):
    """将发送波形（4字节）解码还原为原始 32-bit 寄存器值"""
    if not (isinstance(wave, (list, tuple)) and len(wave) == 4):
        raise ValueError("Waveform must be a list or tuple of 4 bytes")
    
    reversed_bits = [reverse_bits(b) for b in wave]
    bytes_be = reversed_bits[::-1]
    return int.from_bytes(bytes_be, 'big')


def main():
    parser = argparse.ArgumentParser(description="寄存器值与发送波形转换工具")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--encode', type=str, help='输入寄存器值（例如：0x1234ABCD）')
    group.add_argument('--decode', type=str, help='输入发送波形（例如：b3 d5 2c 48）')

    args = parser.parse_args()

    if args.encode:
        val = int(args.encode, 16)
        encoded = encode_reg_value(val)
        print(f"寄存器值: 0x{val:08X}")
        print("发送波形:", ' '.join(f"{b:02X}" for b in encoded))

    elif args.decode:
        try:
            bytes_list = [int(b, 16) for b in args.decode.strip().split()]
            if len(bytes_list) != 4:
                raise ValueError
        except ValueError:
            print("输入格式错误，发送波形应为 4 个十六进制字节，例如：b3 d5 2c 48")
            return
        decoded = decode_waveform(bytes_list)
        print(f"发送波形: {' '.join(f'{b:02X}' for b in bytes_list)}")
        print(f"还原寄存器值: 0x{decoded:08X}")


if __name__ == "__main__":
    main()


# python reg_encoder.py --encode 0x1234ABCD
# 1. 字节反转；2. bit翻转；
# python reg_encoder.py --decode "b3 d5 2c 48"
