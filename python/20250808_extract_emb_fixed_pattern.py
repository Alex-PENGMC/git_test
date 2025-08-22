def read_uyvy_first_line(filename, width=1920, height=1080, pixels_to_read=256):
        """
        读取UYVY格式YUV文件第一行的前N个像素值并保存到TXT文件
        
        参数:
            filename: YUV文件路径
            width: 图像宽度
            height: 图像高度
            pixels_to_read: 需要读取的像素数量
        """
        # UYVY格式每个像素占用2字节（因为是4:2:2采样）
        bytes_per_pixel = 2
        # 计算第一行的字节数
        bytes_per_line = width * bytes_per_pixel
        
        try:
            # 以二进制方式打开文件
            with open(filename, 'rb') as f:
                # 读取第一行数据
                first_line = f.read(bytes_per_line)
                
                # 只取前pixels_to_read个像素的数据
                data_to_save = first_line[:pixels_to_read * bytes_per_pixel]
                
                # 将字节数据转换为十六进制字符串，便于查看
                hex_values = ['0x{:02X}'.format(byte) for byte in data_to_save]
                
                # 保存到TXT文件
                output_filename = 'yuv_first_line_pixels.txt'
                with open(output_filename, 'w') as out_f:
                    # 每4个字节为一组（对应2个像素），便于阅读
                    for i in range(0, len(hex_values), 4):
                        line = ' '.join(hex_values[i:i+4])
                        out_f.write(line + '\n')
                
                print(f"成功读取第一行前{min(pixels_to_read, width)}个像素，结果已保存到 {output_filename}")
                
        except FileNotFoundError:
            print(f"错误：找不到文件 {filename}")
        except Exception as e:
            print(f"发生错误：{str(e)}")

    if __name__ == "__main__":
        # 替换为你的YUV文件路径
        yuv_file = "input.yuv"
        # 读取第一行的前256个像素
        read_uyvy_first_line(yuv_file, pixels_to_read=256)
    