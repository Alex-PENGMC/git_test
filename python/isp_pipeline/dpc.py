import numpy as np
import cv2

# 支持 Bayer Pattern 的坏点修复
BAYER_PATTERNS = {
    'RGGB': ((0, 0, 'R'), (0, 1, 'Gr'), (1, 0, 'Gb'), (1, 1, 'B')),
    'BGGR': ((0, 0, 'B'), (0, 1, 'Gb'), (1, 0, 'Gr'), (1, 1, 'R')),
    'GRBG': ((0, 0, 'Gr'), (0, 1, 'R'), (1, 0, 'B'), (1, 1, 'Gb')),
    'GBRG': ((0, 0, 'Gb'), (0, 1, 'B'), (1, 0, 'R'), (1, 1, 'Gr')),
}

def read_raw_padded(path, width, height, bit_width):
    dtype = np.uint16 if bit_width > 8 else np.uint8
    with open(path, 'rb') as f:
        raw = np.frombuffer(f.read(), dtype=dtype)
    raw = raw[:width * height].reshape((height, width))
    return raw

def save_raw(path, raw, bit_width):
    raw = raw.astype(np.uint16 if bit_width > 8 else np.uint8)
    with open(path, 'wb') as f:
        f.write(raw.tobytes())

def split_bayer_channels(raw, pattern):
    map_pos = {info[2]: (info[0], info[1]) for info in BAYER_PATTERNS[pattern]}
    R  = raw[map_pos['R' ][0]::2, map_pos['R' ][1]::2]
    Gr = raw[map_pos['Gr'][0]::2, map_pos['Gr'][1]::2]
    Gb = raw[map_pos['Gb'][0]::2, map_pos['Gb'][1]::2]
    B  = raw[map_pos['B' ][0]::2, map_pos['B' ][1]::2]
    return R, Gr, Gb, B

def merge_bayer_channels(R, Gr, Gb, B, pattern):
    h, w = R.shape[0] * 2, R.shape[1] * 2
    raw = np.zeros((h, w), dtype=R.dtype)
    map_pos = {info[2]: (info[0], info[1]) for info in BAYER_PATTERNS[pattern]}
    raw[map_pos['R' ][0]::2, map_pos['R' ][1]::2] = R
    raw[map_pos['Gr'][0]::2, map_pos['Gr'][1]::2] = Gr
    raw[map_pos['Gb'][0]::2, map_pos['Gb'][1]::2] = Gb
    raw[map_pos['B' ][0]::2, map_pos['B' ][1]::2] = B
    return raw

def detect_defective_pixels(channel, threshold, var_threshold):
    median = cv2.medianBlur(channel, 3)
    diff = np.abs(channel.astype(np.int32) - median.astype(np.int32))

    # 使用拉普拉斯估算局部变化程度（纹理/边缘）
    laplacian = cv2.Laplacian(channel.astype(np.float32), cv2.CV_32F, ksize=3)
    local_var = cv2.GaussianBlur(laplacian ** 2, (3, 3), 0)

    # mask = (diff > threshold) & (local_var < var_threshold)
    mask = diff > threshold
   
    print(f"Bad pixel candidates (diff > {threshold}): {np.count_nonzero(diff > threshold)}")
    print(f"Final detected bad pixels: {np.count_nonzero(mask)}")
    # 像素偏离 + 区域平坦才认定为坏点
    return mask

def correct_defective_pixels(channel, mask, method='median'):
    corrected = channel.copy()
    if method == 'median':
        filtered = cv2.medianBlur(channel, 3)
    elif method == 'mean4':
        kernel = np.array([[0,1,0],[1,0,1],[0,1,0]])
        filtered = cv2.filter2D(channel.astype(np.float32), -1, kernel / kernel.sum())
    elif method == 'mean8':
        kernel = np.ones((3, 3), dtype=np.float32)
        kernel[1, 1] = 0
        filtered = cv2.filter2D(channel.astype(np.float32), -1, kernel / kernel.sum())
    else:
        raise ValueError(f"Unsupported method: {method}")
    
    corrected[mask] = filtered[mask].astype(channel.dtype)
    return corrected


def detect_defective_pixels_adaptive(channel, base_threshold, var_threshold):
    median = cv2.medianBlur(channel, 3)
    diff = np.abs(channel.astype(np.int32) - median.astype(np.int32))

    laplacian = cv2.Laplacian(channel.astype(np.float32), cv2.CV_32F, ksize=3)
    local_var = cv2.GaussianBlur(laplacian ** 2, (3, 3), 0)

    # 自适应阈值：阈值随局部方差线性变化
    adaptive_threshold = base_threshold * (1 + local_var / (var_threshold + 1e-5))
    
    # 初步坏点检测
    mask = diff > adaptive_threshold
    
    # 保护纹理边缘：局部方差高的区域保留较多
    mask = mask & (local_var < var_threshold * 2)

    # 形态学开操作去除孤立点
    kernel = np.ones((3,3), np.uint8)
    mask_clean = cv2.morphologyEx(mask.astype(np.uint8), cv2.MORPH_OPEN, kernel)

    return mask_clean.astype(bool)


def apply_dpc_bayer(raw, width, height, bit_width, bayer_pattern='RGGB', threshold=60, var_threshold=15, method='median', basename=None):
    raw = raw[:height, :width]
    R, Gr, Gb, B = split_bayer_channels(raw, bayer_pattern)

    R_mask  = detect_defective_pixels(R,  threshold, var_threshold)
    Gr_mask = detect_defective_pixels(Gr, threshold, var_threshold)
    Gb_mask = detect_defective_pixels(Gb, threshold, var_threshold)
    B_mask  = detect_defective_pixels(B,  threshold, var_threshold)

    R_corr  = correct_defective_pixels(R,  R_mask,  method)
    Gr_corr = correct_defective_pixels(Gr, Gr_mask, method)
    Gb_corr = correct_defective_pixels(Gb, Gb_mask, method)
    B_corr  = correct_defective_pixels(B,  B_mask,  method)
    
    return merge_bayer_channels(R_corr, Gr_corr, Gb_corr, B_corr, bayer_pattern)

if __name__ == '__main__':
    import argparse
    import os

    parser = argparse.ArgumentParser(description='Bayer DPC Tool')
    parser.add_argument('--input', required=True, help='Input RAW file')
    parser.add_argument('--output', required=False, help='Output RAW file')
    parser.add_argument('--width', type=int, required=True)
    parser.add_argument('--height', type=int, required=True)
    parser.add_argument('--bit', type=int, default=12)
    parser.add_argument('--pattern', type=str, default='RGGB', choices=list(BAYER_PATTERNS.keys()))
    parser.add_argument('--threshold', type=int, default=60, help='DPC threshold (intensity difference)')
    parser.add_argument('--var_threshold', type=float, default=15, help='Local variance threshold (edge protect)')
    parser.add_argument('--method', type=str, default='median', choices=['mean4', 'mean8', 'median'])
    args = parser.parse_args()

    raw = read_raw_padded(args.input, args.width, args.height, args.bit)
    basename = os.path.splitext(os.path.basename(args.input))[0]
    fixed = apply_dpc_bayer(
        raw,
        args.width,
        args.height,
        args.bit,
        args.pattern,
        args.threshold,
        args.var_threshold,
        args.method,
        basename=basename
    )

    output = args.output or os.path.splitext(args.input)[0] + '_dpc.raw'
    save_raw(output, fixed, args.bit)
    print(f"DPC done, saved to {output}")

# DPC被广泛用于修复sensor的坏点，主要分为静态坏点和动态坏点
# 1. 静态坏点：通过标定的方式，采集黑场、灰常、均匀图来看，确定静态坏点坐标图；
#    修复方法：周围邻域平均（3x3或5x5滤波）；方向性补偿：根据梯度方向选择合适的方向插值；
# 2. 动态坏点：在每帧图像中检测到亮度跳变突兀或差异极大的像素点；
#    修复方法：均值差法、方差检测、SNR/PSNR、高频突变剔除；