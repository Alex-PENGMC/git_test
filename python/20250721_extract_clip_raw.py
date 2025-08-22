import argparse
import os

def get_bytes_per_pixel(bitdepth, fmt):
    """
    :param bitdepth: 10 or 12
    :param fmt: 'padded' or 'packed'
    :return: bytes per pixel or per row unit
    """
    if fmt == 'padded':
        return 2  # always 2 bytes per pixel for 10/12bit padded
    elif fmt == 'packed':
        if bitdepth == 10:
            return 5 / 4  # 4 pixels = 5 bytes
        elif bitdepth == 12:
            return 3 / 2  # 2 pixels = 3 bytes
    raise ValueError("Unsupported bitdepth or format")


def crop_raw_video(input_path, output_path, width, height, crop_x, crop_y, crop_w, crop_h, bitdepth, fmt):
    bpp = get_bytes_per_pixel(bitdepth, fmt)

    if fmt == "padded":
        bytes_per_line = int(width * bpp)
        crop_line_size = int(crop_w * bpp)
        frame_size = bytes_per_line * height
        skip_bytes = crop_y * bytes_per_line + int(crop_x * bpp)

        with open(input_path, 'rb') as fin, open(output_path, 'wb') as fout:
            frame_idx = 0
            while True:
                raw_frame = fin.read(frame_size)
                if len(raw_frame) < frame_size:
                    break

                for i in range(crop_h):
                    line_offset = skip_bytes + i * bytes_per_line
                    fout.write(raw_frame[line_offset:line_offset + crop_line_size])

                frame_idx += 1
            print(f"[✓] 裁剪完成: 共处理 {frame_idx} 帧，每帧输出 {crop_w}x{crop_h}")

    elif fmt == "packed":
        # packed格式下，以行单位做偏移
        pixels_per_line = width
        bytes_per_line = int(pixels_per_line * bpp)
        crop_line_bytes = int(crop_w * bpp)
        frame_size = bytes_per_line * height
        crop_x_bytes = int(crop_x * bpp)

        with open(input_path, 'rb') as fin, open(output_path, 'wb') as fout:
            frame_idx = 0
            while True:
                raw_frame = fin.read(frame_size)
                if len(raw_frame) < frame_size:
                    break

                for i in range(crop_h):
                    line_offset = (crop_y + i) * bytes_per_line + crop_x_bytes
                    fout.write(raw_frame[line_offset:line_offset + crop_line_bytes])

                frame_idx += 1
            print(f"[✓] 裁剪完成: 共处理 {frame_idx} 帧，每帧输出 {crop_w}x{crop_h}")

    else:
        raise ValueError("Unknown format")


def main():
    parser = argparse.ArgumentParser(description="RAW 视频裁剪工具（支持 RAW10/RAW12，padded/packed）")
    parser.add_argument("--input", required=True, help="输入 RAW 文件路径")
    parser.add_argument("--output", required=True, help="输出 RAW 文件路径")
    parser.add_argument("--width", type=int, required=True, help="原始图宽")
    parser.add_argument("--height", type=int, required=True, help="原始图高")
    parser.add_argument("--crop_x", type=int, default=0, help="裁剪起始列")
    parser.add_argument("--crop_y", type=int, default=0, help="裁剪起始行")
    parser.add_argument("--crop_w", type=int, required=True, help="裁剪宽度")
    parser.add_argument("--crop_h", type=int, required=True, help="裁剪高度")
    parser.add_argument("--bitdepth", type=int, choices=[10, 12], default=12, help="RAW 位深")
    parser.add_argument("--format", choices=["padded", "packed"], default="padded", help="打包格式（padded 或 packed）")

    args = parser.parse_args()

    crop_raw_video(
        input_path  = args.input,
        output_path = args.output,
        width       = args.width,
        height      = args.height,
        crop_x      = args.crop_x,
        crop_y      = args.crop_y,
        crop_w      = args.crop_w,
        crop_h      = args.crop_h,
        bitdepth    = args.bitdepth,
        fmt         = args.format
    )

if __name__ == "__main__":
    main()

# python 20250721_extract_clip_raw.py \
#   --input RAW_VIDEO_1920x1082_30000_7_20250718_195228.raw \
#   --output out.raw \
#   --width 1920 --height 1082 \
#   --crop_x 0 --crop_y 2 \
#   --crop_w 1920 --crop_h 1080 \
#   --bitdepth 12 --format padded
