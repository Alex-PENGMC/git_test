import random
import matplotlib.pyplot as plt

def simulate_monty_hall(num_trials=10000):
    win_if_switch = 0
    win_if_stay = 0

    for _ in range(num_trials):
        car_door = random.randint(0, 2)
        player_choice = random.randint(0, 2)

        # 主持人打开一扇有羊的门
        possible_doors_to_open = [d for d in range(3) if d != car_door and d != player_choice]
        door_opened = random.choice(possible_doors_to_open)

        # 玩家换到剩下的一扇门
        switch_choice = [d for d in range(3) if d != player_choice and d != door_opened][0]

        if switch_choice == car_door:
            win_if_switch += 1
        if player_choice == car_door:
            win_if_stay += 1

    switch_win_rate = win_if_switch / num_trials
    stay_win_rate = win_if_stay / num_trials

    print(f"总模拟次数：{num_trials}")
    print(f"换门胜率：{switch_win_rate:.4f} ({win_if_switch} 次赢)")
    print(f"不换胜率：{stay_win_rate:.4f} ({win_if_stay} 次赢)")

    return switch_win_rate, stay_win_rate

def plot_results(switch_win_rate, stay_win_rate):
    methods = ['换门', '不换门']
    win_rates = [switch_win_rate, stay_win_rate]

    plt.figure(figsize=(6, 5))
    bars = plt.bar(methods, win_rates, color=['green', 'red'])
    plt.ylim(0, 1)
    plt.ylabel('胜率')
    plt.title('蒙提霍尔问题模拟结果')

    # 添加柱状图标签
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2, height + 0.02, f'{height:.2%}', ha='center', fontsize=12)

    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    switch_win_rate, stay_win_rate = simulate_monty_hall(10000)
    plot_results(switch_win_rate, stay_win_rate)
