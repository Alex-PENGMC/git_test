import os

def delete_suffix_duplicates_recursive(root="."):
    for dirpath, dirnames, filenames in os.walk(root):
        base_set = set()
        # 先收集所有非 _1 的主文件
        for f in filenames:
            name, ext = os.path.splitext(f)
            if not name.endswith('_1'):
                base_set.add((name, ext))

        # 再处理以 _1 结尾的文件
        for f in filenames:
            name, ext = os.path.splitext(f)
            if name.endswith('_1'):
                base_name = name[:-2]
                if (base_name, ext) in base_set:
                    full_path = os.path.join(dirpath, f)
                    print(f"Deleting: {full_path}")
                    os.remove(full_path)

if __name__ == "__main__":
    delete_suffix_duplicates_recursive()
