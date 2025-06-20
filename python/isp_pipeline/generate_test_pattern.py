import numpy as np
import os

# 参数配置
width, height = 1280, 960
bit_depth = 10
max_value = (1 << bit_depth) - 1
pattern = 'RGGB'  # 支持 RGGB Bayer 排列

# 彩色 colorbar（分成 8 色）
def generate_colorbar_rgb(width, height):
    bar_width = width // 8
    color_list = [
            (max_value, max_value, max_value),  # 白 White (W)
            (max_value, max_value, 0),          # 黄 Yellow (Y)
            (0, max_value, 0),                  # 绿 Green (G)
            (0, max_value, max_value),          # 青 Cyan (C)
            (max_value, 0, max_value),          # 品红 Magenta (M)
            (max_value, 0, 0),                  # 红 Red (R)
            (0, 0, max_value),                  # 蓝 Blue (B)
            (0, 0, 0),                          # 黑 Black (K)
        ]
    rgb = np.zeros((height, width, 3), dtype=np.uint16)
    for i in range(8):
        rgb[:, i*bar_width:(i+1)*bar_width] = color_list[i]
    return rgb

# 灰度 colorbar（从左到右线性渐变）
def generate_colorbar_gray(width, height):
    gradient = np.tile(np.linspace(0, max_value, width, dtype=np.uint16), (height, 1))
    rgb = np.stack([gradient] * 3, axis=-1)  # replicate to R, G, B
    return rgb

# RGB -> Bayer RAW（RGGB）
def rgb_to_bayer(raw_rgb, pattern='RGGB'):
    R = raw_rgb[:, :, 0]
    G = raw_rgb[:, :, 1]
    B = raw_rgb[:, :, 2]
    raw = np.zeros((height, width), dtype=np.uint16)
    if pattern == 'RGGB':
        raw[0::2, 0::2] = R[0::2, 0::2]  # R
        raw[0::2, 1::2] = G[0::2, 1::2]  # G on R line
        raw[1::2, 0::2] = G[1::2, 0::2]  # G on B line
        raw[1::2, 1::2] = B[1::2, 1::2]  # B
    else:
        raise NotImplementedError(f"Pattern {pattern} not supported")
    return raw

# 保存 RAW 文件（按 little-endian 写入）
def save_raw_image(filename, raw):
    raw_clipped = np.clip(raw, 0, max_value).astype(np.uint16)
    raw_clipped.tofile(filename)

# 主流程
if __name__ == '__main__':
    color_rgb = generate_colorbar_rgb(width, height)
    gray_rgb = generate_colorbar_gray(width, height)

    color_raw = rgb_to_bayer(color_rgb, pattern)
    gray_raw = rgb_to_bayer(gray_rgb, pattern)

    save_raw_image('colorbar_color.raw', color_raw)
    save_raw_image('colorbar_gray.raw', gray_raw)

    print("已生成 colorbar_color.raw 和 colorbar_gray.raw")