import os

def read_uyvy_frame(f, width, height, frame_index):
    frame_size = width * height * 2  # UYVY 每像素 16bit
    offset = frame_size * frame_index
    f.seek(offset)
    data = f.read(frame_size)
    if len(data) != frame_size:
        raise ValueError(f"UYVY 第 {frame_index} 帧数据不足")
    return data

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
        raise ValueError(f"RAW{bit_depth} 第 {frame_index} 帧数据不足 ({packing})")
    return data

def main():
    import argparse
    parser = argparse.ArgumentParser(description="提取 YUV(UYVY) 或 RAW 视频帧段并保存为子视频")
    parser.add_argument("file", help="输入视频文件路径")
    parser.add_argument("format", choices=["uyvy", "raw10", "raw12"], help="视频格式")
    parser.add_argument("width", type=int, help="图像宽度")
    parser.add_argument("height", type=int, help="图像高度")
    parser.add_argument("start_frame", type=int, help="起始帧编号（包含）")
    parser.add_argument("end_frame", type=int, help="结束帧编号（包含）")
    parser.add_argument("--output", help="输出文件路径（可省略）")
    parser.add_argument("--packing", choices=["packed", "padded"], default="packed", help="RAW 格式打包方式，默认为 packed")
    args = parser.parse_args()

    if not os.path.exists(args.file):
        print("❌ 输入文件不存在")
        return

    output_path = args.output
    if not output_path:
        output_ext = args.format
        output_path = f"output_clip.{output_ext}"

    with open(args.file, 'rb') as fin, open(output_path, 'wb') as fout:
        for i in range(args.start_frame, args.end_frame + 1):
            try:
                if args.format == "uyvy":
                    frame_data = read_uyvy_frame(fin, args.width, args.height, i)
                else:
                    bit_depth = int(args.format[-2:])
                    frame_data = read_raw_frame(fin, args.width, args.height, i, bit_depth, args.packing)
                fout.write(frame_data)
                print(f"✅ 已写入第 {i} 帧")
            except ValueError as e:
                print(f"⚠️ 错误读取第 {i} 帧：{e}")
                break

    print(f"\n🎉 完成！输出文件保存为: {output_path}")

if __name__ == "__main__":
    main()


#python 20250512_video_clipping.py .\RAW_VIDEO_1280x960_cv1371.raw raw10 1280 960  20  79 --packing padded --output clip_part.raw