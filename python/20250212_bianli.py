def calculate_divisions(num):
    """
    输入一个数字，计算该数字除以从1到该数字之间所有数的结果，并返回结果列表。
    
    参数:
    num (int): 输入的数字
    
    返回:
    list: 包含所有除法结果的列表
    """
    results = []
    for i in range(1, num + 1):
        result = round(num / i, 6)  # 保留6位有效数字
        results.append(result)
    return results

def save_results_to_html(num, results):
    """
    将计算结果保存为HTML格式文件。
    
    参数:
    num (int): 输入的数字
    results (list): 计算结果列表
    """
    # 打开一个文件用于写入
    with open(f"results_{num}.html", "w") as f:
        f.write("<html><body><h2>计算结果</h2><table border='1'>")
        f.write(f"<tr><th>数字</th><th>除法结果</th></tr>")
        
        for i, result in enumerate(results, 1):
            # 如果小数点后小于等于两位，标红
            if len(str(result).split('.')[1]) <= 1:
                f.write(f"<tr style='color:red;'><td>{num} / {i}</td><td>{result}</td></tr>")
            else:
                f.write(f"<tr><td>{num} / {i}</td><td>{result}</td></tr>")
        
        f.write("</table></body></html>")
    print(f"结果已保存为HTML文件: results_{num}.html")

def main():
    # 主程序
    num = int(input("请输入一个数字："))
    results = calculate_divisions(num)
    
    # 输出所有结果为整数的除法结果到终端
    for i, result in enumerate(results, 1):
        if result.is_integer():  # 只输出结果为整数的行
            print(f"{num} / {i} = {int(result)}")


    # 将结果保存为HTML文件
    save_results_to_html(num, results)

# 确保脚本作为主程序执行
if __name__ == "__main__":
    main()
