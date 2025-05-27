import os
import argparse

def get_raw_frame_size(width, height, bit_depth, packing):
    pixels = width * height
    if packing == "packed":
        total_bits = pixels * bit_depth
        return (total_bits + 7) // 8
    elif packing == "padded":
        bytes_per_pixel = (bit_depth + 7) // 8
        return pixels * bytes_per_pixel
    else:
        raise ValueError("未知打包格式，应为 'packed' 或 'padded'")

def read_raw_frame(f, width, height, frame_index, bit_depth, packing):
    frame_size = get_raw_frame_size(width, height, bit_depth, packing)
    offset = frame_size * frame_index
    f.seek(offset)
    data = f.read(frame_size)
    if len(data) != frame_size:
        raise ValueError(f"读取第 {frame_index} 帧失败：数据不足")
    return data

def main():
    parser = argparse.ArgumentParser(description="从 RAW 视频中提取帧并保存为 .raw 文件")
    parser.add_argument("file", help="输入 RAW 视频文件路径")
    parser.add_argument("width", type=int, help="图像宽度")
    parser.add_argument("height", type=int, help="图像高度")
    parser.add_argument("bit_depth", type=int, choices=[10, 12], help="位深 (10 或 12)")
    parser.add_argument("start_frame", type=int, help="起始帧编号（包含）")
    parser.add_argument("end_frame", type=int, help="结束帧编号（包含）")
    parser.add_argument("packing", choices=["packed", "padded"], help="打包格式")
    args = parser.parse_args()

    if not os.path.exists(args.file):
        print("❌ 输入文件不存在")
        return

    # 创建输出目录，使用原始文件名（去除扩展名）
    base_name = os.path.splitext(os.path.basename(args.file))[0]
    output_dir = os.path.join(os.path.dirname(args.file), base_name)
    os.makedirs(output_dir, exist_ok=True)

    with open(args.file, 'rb') as fin:
        for i in range(args.start_frame, args.end_frame + 1):
            try:
                data = read_raw_frame(fin, args.width, args.height, i, args.bit_depth, args.packing)
                out_name = f"{args.width}_{args.height}_{args.bit_depth}_{i}.raw"
                out_path = os.path.join(output_dir, out_name)
                with open(out_path, 'wb') as fout:
                    fout.write(data)
                print(f"✅ 保存帧 {i} 为 {out_name}")
            except Exception as e:
                print(f"⚠️ 第 {i} 帧出错：{e}")
                break

    print(f"\n🎉 所有帧已保存至：{output_dir}")

if __name__ == "__main__":
    main()
