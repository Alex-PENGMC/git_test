import matplotlib.pyplot as plt

# 提供的数据
x = [0x0, 0x40, 0x80, 0xc0, 0x100, 0x140, 0x180, 0x1c0, 0x200, 0x240, 0x280, 0x2c0]
y = [0x0, 0x400, 0x800, 0xc00, 0x1000, 0x1400, 0x1800, 0x1c00, 0x2000, 0x2400, 0x2800, 0x2c00]

# 绘制折线图
plt.figure(figsize=(8, 6))
plt.plot(x, y, marker='o', linestyle='-', color='b', label='Data Line')

# 标注点位
for i in range(len(x)):
    plt.text(x[i], y[i], f"({x[i]}, {y[i]})", fontsize=10, verticalalignment='bottom')

# 图表设置
plt.xlabel("X Values")
plt.ylabel("Y Values")
plt.title("Plot of Given X and Y Data")
plt.legend()
plt.grid(True)

# 显示图像
plt.show()
