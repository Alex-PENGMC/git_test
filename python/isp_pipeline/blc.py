import numpy as np

def unpack_packed_raw(data, width, height, bit_depth):
    """
    解包 packed RAW 图像
    支持 bit_depth = 10 或 12
    """
    if bit_depth == 10:
        row_size = width * 10 // 8
        img = np.zeros((height, width), dtype=np.uint16)
        for h in range(height):
            row_bytes = data[h * row_size : (h + 1) * row_size]
            for i in range(0, len(row_bytes), 5):
                if i + 5 > len(row_bytes):
                    break
                b = row_bytes[i:i+5]
                p0 = b[0] + ((b[4] & 0b00000011) << 8)
                p1 = b[1] + ((b[4] & 0b00001100) << 6)
                p2 = b[2] + ((b[4] & 0b00110000) << 4)
                p3 = b[3] + ((b[4] & 0b11000000) << 2)
                base = (i // 5) * 4
                if base + 4 <= width:
                    img[h, base + 0] = p0
                    img[h, base + 1] = p1
                    img[h, base + 2] = p2
                    img[h, base + 3] = p3
        return img

    elif bit_depth == 12:
        row_size = width * 12 // 8
        img = np.zeros((height, width), dtype=np.uint16)
        for h in range(height):
            row_bytes = data[h * row_size : (h + 1) * row_size]
            for i in range(0, len(row_bytes), 3):
                if i + 3 > len(row_bytes):
                    break
                b0, b1, b2 = row_bytes[i:i+3]
                p0 = b0 + ((b1 & 0x0F) << 8)
                p1 = ((b1 >> 4) & 0x0F) + (b2 << 4)
                base = (i // 3) * 2
                if base + 2 <= width:
                    img[h, base + 0] = p0
                    img[h, base + 1] = p1
        return img

    else:
        raise ValueError("仅支持 10bit 和 12bit packed 格式")

def load_padded_raw(data, width, height, bit_depth):
    dtype = np.uint8 if bit_depth <= 8 else np.uint16
    img = np.frombuffer(data, dtype=dtype)
    expected_pixels = width * height
    if img.size != expected_pixels:
        raise ValueError(f"尺寸不匹配：期望 {expected_pixels} 像素，实际为 {img.size}")
    return img.reshape((height, width))

def apply_blc(raw_img, bit_depth=12, blc_offsets=(64, 64, 64, 64), pattern='RGGB'):
    corrected_img = raw_img.astype(np.int32)
    pattern = pattern.upper()

    ch_map = {
        'RGGB': {'R': (0, 0), 'Gr': (0, 1), 'Gb': (1, 0), 'B': (1, 1)},
        'BGGR': {'B': (0, 0), 'Gb': (0, 1), 'Gr': (1, 0), 'R': (1, 1)},
        'GRBG': {'Gr': (0, 0), 'R': (0, 1), 'B': (1, 0), 'Gb': (1, 1)},
        'GBRG': {'Gb': (0, 0), 'B': (0, 1), 'R': (1, 0), 'Gr': (1, 1)},
    }

    coords = ch_map.get(pattern)
    if coords is None:
        raise ValueError(f"不支持的 pattern: {pattern}")

    for ch, offset in zip(['R', 'Gr', 'Gb', 'B'], blc_offsets):
        y, x = coords[ch]
        corrected_img[y::2, x::2] -= offset

    corrected_img = np.clip(corrected_img, 0, (1 << bit_depth) - 1)
    return corrected_img.astype(raw_img.dtype)

def save_raw(filepath, image):
    image.tofile(filepath)

def blc_process(raw_bytes, width, height, bit_depth, pattern, blc, is_packed):
    """
    供外部调用的主函数接口
    """
    if is_packed:
        raw_img = unpack_packed_raw(raw_bytes, width, height, bit_depth)
    else:
        raw_img = load_padded_raw(raw_bytes, width, height, bit_depth)

    blc_img = apply_blc(raw_img, bit_depth=bit_depth, blc_offsets=blc, pattern=pattern)
    return blc_img

# # ---------- 通用主程序 ----------
# if __name__ == "__main__":
#     import argparse

#     parser = argparse.ArgumentParser(description="支持 Packed 和 Padded 的通用 RAW BLC 处理器")
#     parser.add_argument("input", help="输入 .raw 文件路径")
#     parser.add_argument("--width", type=int, required=True, help="图像宽度")
#     parser.add_argument("--height", type=int, required=True, help="图像高度")
#     parser.add_argument("--bit", type=int, required=True, help="位宽：支持 8/10/12/14/16")
#     parser.add_argument("--pattern", type=str, default="RGGB", help="Bayer 格式（RGGB/BGGR/GRBG/GBRG）")
#     parser.add_argument("--blc", nargs=4, type=int, default=[64, 64, 64, 64], help="四通道黑电平值 R Gr Gb B")
#     parser.add_argument("--packed", action="store_true", help="启用 Packed 格式；默认使用 Padded")
#     parser.add_argument("--output", default="blc_output.raw", help="输出文件路径")
#     args = parser.parse_args()

#     with open(args.input, "rb") as f:
#         raw_data = f.read()

#     if args.packed:
#         raw_img = unpack_packed_raw(raw_data, args.width, args.height, args.bit)
#     else:
#         raw_img = load_padded_raw(raw_data, args.width, args.height, args.bit)

#     blc_img = apply_blc(
#         raw_img,
#         bit_depth=args.bit,
#         blc_offsets=tuple(args.blc),
#         pattern=args.pattern
#     )

#     save_raw(args.output, blc_img)
#     print(f"[INFO] BLC 处理完成，输出文件: {args.output}")


# 由于暗电流、放大器偏置、ADC零点漂移等原因，导致图像在全黑场景下输出的raw数据不全为0，需要分通道减去全黑场景下的值，使得
# 从零开始输出。