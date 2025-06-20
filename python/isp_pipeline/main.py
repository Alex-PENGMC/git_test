import blc, dpc
import numpy as np
import matplotlib.pyplot as plt
import os

with open("sample.raw", "rb") as f:
    raw_data = f.read()

# 参数配置
width       = 1280
height      = 960
bit_depth   = 10
pattern     = "RGGB"
blc_offsets = (64, 64, 64, 64)
is_packed   = False  # True 如果是 packed 格式

# 调用模块处理
blc_result = blc.blc_process(
    raw_data,
    width     = width,
    height    = height,
    bit_depth = bit_depth,
    pattern   = pattern,
    blc       = blc_offsets,
    is_packed = is_packed
)

# 保存处理后的 RAW 数据
blc.save_raw("output_blc.raw", blc_result)





