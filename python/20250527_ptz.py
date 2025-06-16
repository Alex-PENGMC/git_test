import matplotlib.pyplot as plt

# 参数（单位：采样点）
sync_pulse   = 0xf0
offset_left  = 0x12d
bit_number   = 54       # 已拆分后的脉冲数量（高低交替）
bit_width    = 37       # 单个脉冲宽度（采样点）
offset_right = 0x295

# 采样偏移（点单位）
sample_offsets = [13, 25, 37]

# 一个采样点 = 1 / 72MHz ≈ 13.888889 ns
tick_ns = 1_000_000_000 / 72_000_000  # ≈13.888889

# 波形生成函数，offset_left/bit_width 接受纳秒单位
def generate_wave_points_ns(sync_pulse, offset_left_ns, bit_number, bit_width_ns, offset_right):
    xs, ys = [], []
    x, y = 0, 0

    # 起始低电平段
    xs.append(x); ys.append(y)
    y -= 0.5
    xs.append(x); ys.append(y)

    x += sync_pulse * tick_ns
    xs.append(x); ys.append(y)

    y += 0.5
    xs.append(x); ys.append(y)

    x += offset_left_ns
    xs.append(x); ys.append(y)

    for i in range(int(bit_number)):
        y = 1 if i % 2 == 0 else 0
        xs.append(x); ys.append(y)
        x += bit_width_ns
        xs.append(x); ys.append(y)

    xs.append(x); ys.append(y)
    x += offset_right * tick_ns
    xs.append(x); ys.append(y)

    return xs, ys

# 插值获取指定x的y值
def get_y_by_x(xs, ys, xq):
    for i in range(len(xs) - 1):
        if xs[i] <= xq <= xs[i+1]:
            x0, y0 = xs[i], ys[i]
            x1, y1 = xs[i+1], ys[i+1]
            if x1 == x0:
                return y0
            return y0 + (y1 - y0) * (xq - x0) / (x1 - x0)
    return None

# 获取三组采样点横坐标（单位 ns）
def get_all_sample_xs(sync_pulse, offset_left, bit_number, bit_width):
    base = sync_pulse + offset_left
    group1, group2, group3 = [], [], []
    for i in range(int(bit_number)):
        start = base + i * bit_width
        group1.append((start + sample_offsets[0]) * tick_ns)
        group2.append((start + sample_offsets[1]) * tick_ns)
        group3.append((start + sample_offsets[2]) * tick_ns)
    return [group1, group2, group3]

# 提取采样值
def extract_y_samples(xs, ys, sample_x_groups):
    return [[get_y_by_x(xs, ys, xq) for xq in group] for group in sample_x_groups]

# 判断是否匹配参考采样
def compare_with_reference(ref_ys_groups, new_ys_groups):
    for new_group in new_ys_groups:
        for ref_group in ref_ys_groups:
            if new_group == ref_group:
                return True
    return False

# 绘图
def plot_wave(xs, ys, sample_points=None):
    plt.figure(figsize=(12, 4))
    plt.plot(xs, ys, marker='o', label="Waveform")
    if sample_points:
        for i, group in enumerate(sample_points):
            plt.plot(group, [get_y_by_x(xs, ys, xq) for xq in group], 'x', label=f"Sample Group {i+1}")
    plt.grid(True)
    plt.legend()
    plt.title("Waveform with Sampling Points (Time in ns)")
    plt.xlabel("Time (ns)")
    plt.ylabel("Amplitude")
    plt.show()

def run_test_range():
    bit_width_ns       = bit_width * tick_ns
    offset_left_ns     = offset_left * tick_ns

    # 生成参考波形和采样
    xs_ref, ys_ref       = generate_wave_points_ns(sync_pulse, offset_left_ns, bit_number, bit_width_ns, offset_right)
    ref_sample_xs_groups = get_all_sample_xs(sync_pulse, offset_left, bit_number, bit_width)
    ref_sample_ys_groups = extract_y_samples(xs_ref, ys_ref, ref_sample_xs_groups)

    print("测试结果（单位：ns）：")
    print("offset_left_delta_ns, bit_width_delta_ns, match")

    for offset_delta in range(-10, 11):
        for width_delta in range(-10, 11):
            offset_left_ns_new = offset_left_ns + offset_delta
            bit_width_ns_new   = bit_width_ns + width_delta

            xs_new, ys_new = generate_wave_points_ns(sync_pulse, offset_left_ns_new, bit_number, bit_width_ns_new, offset_right)
            new_sample_ys_groups = extract_y_samples(xs_new, ys_new, ref_sample_xs_groups)

            match = compare_with_reference(ref_sample_ys_groups, new_sample_ys_groups)
            print(f"{offset_delta:>+3}, {width_delta:>+3}, {'✅' if match else '❌'}")


# 主程序
if __name__ == "__main__":
    # 初始值换算为 ns
    bit_width_ns       = bit_width * tick_ns
    offset_left_ns     = offset_left * tick_ns
    ns_variation       = 0  # 模拟脉宽变化，例如增宽3ns
    offset_left_ns_new = offset_left_ns - 0 # 模拟offset_left发生变化，例如增大10ns
    # 生成参考波形
    xs_ref, ys_ref       = generate_wave_points_ns(sync_pulse, offset_left_ns, bit_number, bit_width_ns, offset_right)
    ref_sample_xs_groups = get_all_sample_xs(sync_pulse, offset_left, bit_number, bit_width)
    ref_sample_ys_groups = extract_y_samples(xs_ref, ys_ref, ref_sample_xs_groups)

    # 生成新的波形（模拟变化）
    xs_new, ys_new = generate_wave_points_ns(sync_pulse, offset_left_ns_new, bit_number, bit_width_ns + ns_variation, offset_right)
    new_sample_ys_groups = extract_y_samples(xs_new, ys_new, ref_sample_xs_groups)

    # 判断是否匹配
    success = compare_with_reference(ref_sample_ys_groups, new_sample_ys_groups)
    print(f"脉宽增宽 {ns_variation} ns 采样结果：", "✅ 匹配成功" if success else "❌ 匹配失败")

    # 绘图
    plot_wave(xs_new, ys_new, ref_sample_xs_groups)

    # run_test_range()