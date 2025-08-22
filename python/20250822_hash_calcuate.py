import os
import hashlib
import sys
import subprocess
import argparse
import json
import configparser
from datetime import datetime
from typing import Tuple, List, Dict, Optional
try:
    import requests
except ImportError:
    requests = None
try:
    from tqdm import tqdm
except ImportError:
    tqdm = None


# 需要自动排除的特殊文件（配置文件和哈希清单）
EXCLUDED_FILES = {
    'hash_config.ini',       # 配置文件
    'hash_list.txt'          # 默认哈希清单
}


# 配置文件默认内容
DEFAULT_CONFIG = """
[DEFAULT]
# 哈希算法：md5/sha256/sha512
algorithm = sha256
# 忽略的文件后缀（用逗号分隔）
ignore_suffixes = .log,.tmp,.swp
# 分块大小（KB）
block_size = 1024
# 默认输出清单路径
output_list = hash_list.txt
# Git模式开关（yes/no）
use_git = no
# 缓存文件路径（用于增量计算）
cache_file = .hash_cache.json
# 远程校验服务器地址（可选）
remote_server = 
# 要排除的文件或目录（用逗号分隔，支持相对路径和通配符）
exclude_paths = 

[PATHS]
# 常用项目路径别名（可选）
# project1 = D:/work/project1
# project2 = E:/code/project2
"""


def load_config(config_path: str) -> Dict:
    """加载配置文件，返回配置字典"""
    config = {
        'algorithm': 'sha256',
        'ignore_suffixes': [],
        'block_size': 1024,
        'output_list': 'hash_list.txt',
        'use_git': False,
        'cache_file': '.hash_cache.json',
        'remote_server': '',
        'exclude_paths': []  # 新增：要排除的文件或目录
    }
    
    if not os.path.exists(config_path):
        # 生成默认配置文件
        with open(config_path, 'w', encoding='utf-8') as f:
            f.write(DEFAULT_CONFIG.strip())
        print(f"已生成默认配置文件：{config_path}")
        return config
    
    # 解析配置文件
    cp = configparser.ConfigParser()
    cp.read(config_path, encoding='utf-8')
    
    if 'DEFAULT' in cp:
        # 哈希算法
        if cp['DEFAULT'].get('algorithm') in ['md5', 'sha256', 'sha512']:
            config['algorithm'] = cp['DEFAULT']['algorithm']
        
        # 忽略的后缀
        suffixes = cp['DEFAULT'].get('ignore_suffixes', '')
        config['ignore_suffixes'] = [s.strip() for s in suffixes.split(',') if s.strip()]
        
        # 分块大小
        try:
            config['block_size'] = int(cp['DEFAULT'].get('block_size', 1024))
        except ValueError:
            pass
        
        # 输出清单路径
        if cp['DEFAULT'].get('output_list'):
            config['output_list'] = cp['DEFAULT']['output_list']
            # 将用户配置的输出清单也加入排除列表
            EXCLUDED_FILES.add(os.path.basename(config['output_list']))
        
        # Git模式
        config['use_git'] = cp['DEFAULT'].get('use_git', 'no').lower() == 'yes'
        
        # 缓存文件路径
        if cp['DEFAULT'].get('cache_file'):
            config['cache_file'] = cp['DEFAULT']['cache_file']
        
        # 远程服务器
        if cp['DEFAULT'].get('remote_server'):
            config['remote_server'] = cp['DEFAULT']['remote_server']
        
        # 新增：要排除的文件或目录
        exclude_paths = cp['DEFAULT'].get('exclude_paths', '')
        config['exclude_paths'] = [p.strip() for p in exclude_paths.split(',') if p.strip()]
    
    return config


def is_excluded(path: str, root_dir: str, exclude_paths: List[str]) -> bool:
    """检查路径是否应该被排除"""
    # 转换为绝对路径
    abs_path = os.path.abspath(os.path.join(root_dir, path))
    root_abs = os.path.abspath(root_dir)
    
    # 检查是否匹配排除路径
    for pattern in exclude_paths:
        # 处理相对路径模式
        pattern_abs = os.path.abspath(os.path.join(root_abs, pattern))
        
        # 检查是否是目录匹配
        if os.path.isdir(pattern_abs) and abs_path.startswith(pattern_abs + os.sep):
            return True
            
        # 检查是否是文件匹配
        if os.path.isfile(pattern_abs) and abs_path == pattern_abs:
            return True
            
        # 检查通配符匹配（简单实现 * 匹配）
        if '*' in pattern:
            # 简单的通配符处理，仅支持末尾的*
            if pattern.endswith('*'):
                prefix = pattern[:-1]
                if path.startswith(prefix) or abs_path.startswith(prefix):
                    return True
            # 支持开头的*
            elif pattern.startswith('*'):
                suffix = pattern[1:]
                if path.endswith(suffix) or abs_path.endswith(suffix):
                    return True
    
    return False


def get_git_tracked_files(root_dir: str, exclude_paths: List[str]) -> List[str]:
    """获取Git仓库中被追踪的文件列表（跳过软链接、特殊文件和排除路径）"""
    try:
        current_dir = os.getcwd()
        os.chdir(root_dir)
        
        result = subprocess.run(
            ['git', 'ls-files', '--full-name', '--cached', '--no-empty-directory'],
            capture_output=True,
            check=True
        )
        
        os.chdir(current_dir)
        output = result.stdout.decode('utf-8', errors='ignore')
        files = [f.strip() for f in output.splitlines() if f.strip()]
        
        # 过滤软链接、特殊文件和排除路径
        tracked_files = []
        for rel_path in files:
            # 排除特殊文件（只看文件名，不看路径）
            filename = os.path.basename(rel_path)
            if filename in EXCLUDED_FILES:
                print(f"自动排除特殊文件（Git模式）：{rel_path}")
                continue
                
            # 检查是否在排除路径中
            if is_excluded(rel_path, root_dir, exclude_paths):
                print(f"排除文件（配置指定）：{rel_path}")
                continue
                
            abs_path = os.path.join(root_dir, rel_path)
            if not os.path.islink(abs_path) and os.path.isfile(abs_path):
                tracked_files.append(rel_path)
        
        return tracked_files
        
    except subprocess.CalledProcessError as e:
        print(f"Git命令错误: {e.stderr.decode('utf-8', errors='ignore')}")
        return []
    except Exception as e:
        print(f"获取Git文件失败: {str(e)}")
        return []


def get_all_files(
    root_dir: str, 
    ignore_suffixes: List[str] = None,
    exclude_paths: List[str] = None
) -> List[str]:
    """获取目录下所有非隐藏文件（跳过.git目录/软链接/指定后缀/特殊文件/排除路径）"""
    ignore_suffixes = ignore_suffixes or []
    exclude_paths = exclude_paths or []
    file_list = []
    
    for dirpath, dirnames, filenames in os.walk(root_dir):
        # 跳过.git目录
        if '.git' in dirpath.split(os.sep):
            continue
            
        # 检查当前目录是否需要排除
        rel_dir = os.path.relpath(dirpath, root_dir)
        if is_excluded(rel_dir, root_dir, exclude_paths):
            print(f"排除目录（配置指定）：{rel_dir}")
            continue
            
        for filename in filenames:
            # 排除特殊文件
            if filename in EXCLUDED_FILES:
                print(f"自动排除特殊文件：{os.path.join(rel_dir, filename)}")
                continue
                
            # 跳过隐藏文件
            if filename.startswith('.'):
                continue
                
            # 跳过指定后缀的文件
            if any(filename.endswith(suffix) for suffix in ignore_suffixes):
                continue
            
            rel_path = os.path.join(rel_dir, filename)
            
            # 检查是否需要排除该文件
            if is_excluded(rel_path, root_dir, exclude_paths):
                print(f"排除文件（配置指定）：{rel_path}")
                continue
            
            abs_path = os.path.join(root_dir, rel_path)
            if not os.path.islink(abs_path) and os.path.isfile(abs_path):
                file_list.append(rel_path)
    
    return file_list


def calculate_file_hash(
    file_path: str, 
    hash_alg: str = 'sha256', 
    block_size: int = 1024 * 1024
) -> str:
    """分块计算单个文件的哈希值"""
    try:
        if hash_alg.lower() == 'sha256':
            hash_obj = hashlib.sha256()
        elif hash_alg.lower() == 'sha512':
            hash_obj = hashlib.sha512()
        else:
            hash_obj = hashlib.md5()
        
        with open(file_path, 'rb') as f:
            while chunk := f.read(block_size):
                hash_obj.update(chunk)
        
        return hash_obj.hexdigest()
    
    except PermissionError:
        print(f"权限不足，跳过文件: {file_path}")
        return ""
    except Exception as e:
        print(f"计算文件哈希失败 {file_path}: {str(e)}")
        return ""


def load_cache(cache_path: str) -> Dict:
    """加载缓存文件（用于增量计算）"""
    # 确保缓存文件本身不被计入哈希计算
    if os.path.basename(cache_path) not in EXCLUDED_FILES:
        EXCLUDED_FILES.add(os.path.basename(cache_path))
        
    if os.path.exists(cache_path):
        try:
            with open(cache_path, 'r', encoding='utf-8') as f:
                return json.load(f)
        except (json.JSONDecodeError, Exception) as e:
            print(f"缓存文件损坏，将重新生成：{str(e)}")
    return {'files': {}, 'project_hash': '', 'timestamp': ''}


def save_cache(cache_path: str, cache_data: Dict):
    """保存缓存数据"""
    try:
        # 添加时间戳
        cache_data['timestamp'] = datetime.now().isoformat()
        with open(cache_path, 'w', encoding='utf-8') as f:
            json.dump(cache_data, f, ensure_ascii=False, indent=2)
    except Exception as e:
        print(f"保存缓存失败：{str(e)}")


def get_changed_files(
      root_dir    : str,
      file_list   : List[str],
      cache       : Dict
    ) -> List[str]: 
    """对比缓存，获取变更的文件列表（内容或修改时间变化）"""
    changed = []
    for rel_path in file_list:
        abs_path = os.path.join(root_dir, rel_path)
        # 获取当前文件修改时间（秒级）
        mtime = int(os.path.getmtime(abs_path))
        
        # 检查缓存
        if rel_path in cache['files']:
            cached = cache['files'][rel_path]
            # 时间或哈希变化都视为变更
            if cached.get('mtime') != mtime or cached.get('hash') == '':
                changed.append(rel_path)
        else:
            # 新文件
            changed.append(rel_path)
    return changed


def remote_verify(remote_url: str, project_hash: str, project_dir: str) -> bool:
    """远程校验哈希值（需要服务器端配合）"""
    if not requests:
        print("请安装requests库以使用远程校验：pip install requests")
        return False
        
    if not remote_url:
        print("未配置远程服务器地址，跳过远程校验")
        return False
        
    try:
        # 发送校验请求（示例格式，需与服务器端一致）
        response = requests.post(
            f"{remote_url}/verify",
            json={
                'project_path': project_dir,
                'hash_value': project_hash,
                'timestamp': datetime.now().isoformat()
            },
            timeout=10
        )
        
        if response.status_code == 200:
            result = response.json()
            if result.get('status') == 'valid':
                print(f"远程校验成功：{result.get('message', '哈希值匹配')}")
                return True
            else:
                print(f"远程校验失败：{result.get('message', '哈希值不匹配')}")
        else:
            print(f"远程请求失败，状态码：{response.status_code}")
            
    except Exception as e:
        print(f"远程校验出错：{str(e)}")
    return False


def calculate_project_hash(
      root_dir                                 : str,
      use_git                                  : bool = False,
      hash_alg                                 : str = 'sha256',
      ignore_suffixes                          : List[str] = None,
      exclude_paths                            : List[str] = None,         # 新增：排除路径参数
      block_size                               : int = 1024 * 1024,
      output_list                              : Optional[str] = None,
      use_cache                                : bool = False,
      cache_path                               : str = '.hash_cache.json',
      remote_server                            : str = ''
    ) -> Tuple[str, str, List[Tuple[str, str]]]: 
    """计算工程哈希（支持增量计算、缓存、远程校验）"""
    ignore_suffixes = ignore_suffixes or []
    exclude_paths   = exclude_paths or []
    mode_name       = ""
    file_list       = []
    
    # 获取文件列表
    if use_git:
        try:
            subprocess.run(
                ['git', 'rev-parse', '--is-inside-work-tree'],
                cwd            = root_dir,
                capture_output = True,
                check          = True
            )
            file_list = get_git_tracked_files(root_dir, exclude_paths)
            mode_name = f"Git追踪文件（{hash_alg.upper()}）"
        except subprocess.CalledProcessError:
            print(f"警告：{root_dir} 不是Git仓库，自动切换到普通模式")
            use_git = False
    
    if not use_git:
        file_list = get_all_files(root_dir, ignore_suffixes, exclude_paths)
        mode_name = f"所有非隐藏文件（排除{','.join(ignore_suffixes)}，{hash_alg.upper()}）"
    
    if not file_list:
        print(f"未找到符合条件的文件（模式：{mode_name}）")
        return ("", mode_name, [])
    
    # 加载缓存（用于增量计算）
    cache = load_cache(cache_path) if use_cache else {'files': {}}
    
    # 确定需要重新计算的文件
    if use_cache:
        changed_files = get_changed_files(root_dir, file_list, cache)
        print(f"增量计算模式：共{len(file_list)}个文件，{len(changed_files)}个需要更新")
    else:
        changed_files = file_list
        print(f"全量计算模式：共{len(file_list)}个文件")
    
    # 计算文件哈希（带进度条）
    file_hash_list = []
    valid_entries  = []
    progress_iter  = changed_files
    # 如果有tqdm库，显示进度条
    if tqdm and len(changed_files) > 10:
        progress_iter = tqdm(changed_files, desc="计算文件哈希")
    
    for rel_path in progress_iter:
        abs_path  = os.path.join(root_dir, rel_path)
        file_hash = calculate_file_hash(abs_path, hash_alg, block_size)
        
        if file_hash:
            # 更新缓存数据
            mtime = int(os.path.getmtime(abs_path))
            cache['files'][rel_path] = {
                'mtime': mtime,
                'hash' : file_hash
            }
            # 记录有效条目
            file_hash_list.append((rel_path, file_hash))
            valid_entries.append(f"{rel_path}:{file_hash}")
    
    # 保存缓存
    if use_cache:
        save_cache(cache_path, cache)
    
    # 计算综合哈希（包含所有文件，不仅是变更的）
    all_valid_entries = []
    for rel_path in file_list:
        if rel_path in cache['files'] and cache['files'][rel_path]['hash']:
            all_valid_entries.append(f"{rel_path}:{cache['files'][rel_path]['hash']}")
    
    all_valid_entries.sort()
    combined_data = '\n'.join(all_valid_entries).encode('utf-8')
    if hash_alg.lower() == 'sha256':
        project_hash = hashlib.sha256(combined_data).hexdigest()
    elif hash_alg.lower() == 'sha512':
        project_hash = hashlib.sha512(combined_data).hexdigest()
    else:
        project_hash = hashlib.md5(combined_data).hexdigest()
    
    # 更新缓存中的工程哈希
    if use_cache:
        cache['project_hash'] = project_hash
        save_cache(cache_path, cache)
    
    # 输出文件清单
    if output_list and file_hash_list:
        try:
            with open(output_list, 'w', encoding='utf-8') as f:
                f.write(f"# 工程文件哈希清单（模式：{mode_name}）\n")
                f.write(f"# 工程目录：{os.path.abspath(root_dir)}\n")
                f.write(f"# 综合哈希值：{project_hash}\n")
                f.write(f"# 生成时间：{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
                f.write(f"# 总文件数：{len(file_list)}，变更文件数：{len(changed_files)}\n")
                f.write("---\n")
                f.write(f"{'文件路径':<50} | {'哈希值':<64}\n")
                f.write("-" * (50 + 3 + 64) + "\n")
                for rel_path, file_hash in file_hash_list:
                    f.write(f"{rel_path:<50.47}... | {file_hash:<64}\n")  # 长路径省略
            print(f"\n文件哈希清单已保存到：{os.path.abspath(output_list)}")
        except Exception as e:
            print(f"保存文件清单失败：{str(e)}")

    # 找到 calculate_project_hash 函数中“输出文件清单”的代码块，修改为：
    # if output_list and file_hash_list:
    #     try:
    #         # 强化：写入前最后一次过滤，彻底排除含objs的路径
    #         filtered_hash_list = []
    #         for rel_path, file_hash in file_hash_list:
    #             # 检查路径中是否包含objs（无论层级）
    #             if 'objs' in rel_path.split(os.sep):
    #                 print(f"最终过滤：排除objs文件 {rel_path}")  # 打印日志确认
    #                 continue
    #             filtered_hash_list.append((rel_path, file_hash))
            
    #         # 写入过滤后的列表
    #         with open(output_list, 'w', encoding='utf-8') as f:
    #             f.write(f"# 工程文件哈希清单（模式：{mode_name}）\n")
    #             f.write(f"# 工程目录：{os.path.abspath(root_dir)}\n")
    #             f.write(f"# 综合哈希值：{project_hash}\n")
    #             f.write(f"# 生成时间：{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
    #             f.write(f"# 总文件数：{len(file_list)}，变更文件数：{len(changed_files)}\n")
    #             f.write("---\n")
    #             f.write(f"{'文件路径':<50} | {'哈希值':<64}\n")
    #             f.write("-" * (50 + 3 + 64) + "\n")
    #             for rel_path, file_hash in filtered_hash_list:  # 使用过滤后的列表
    #                 f.write(f"{rel_path:<50.47}... | {file_hash:<64}\n")
    #         print(f"\n文件哈希清单已保存到：{os.path.abspath(output_list)}")
    #     except Exception as e:
    #         print(f"保存文件清单失败：{str(e)}")
        
    # 远程校验
    if remote_server:
        remote_verify(remote_server, project_hash, root_dir)
    
    return (project_hash, mode_name, file_hash_list)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="工程文件综合哈希计算器（支持自定义排除文件和目录）",
        formatter_class=argparse.RawTextHelpFormatter
    )
    
    parser.add_argument(
        'directory', 
        nargs='?', 
        default=os.getcwd(),
        help='指定工程目录（默认：当前目录）'
    )
    parser.add_argument(
        '--git', '-g', 
        action='store_true',
        help='只计算Git追踪的文件'
    )
    parser.add_argument(
        '--alg', '-a', 
        choices=['md5', 'sha256', 'sha512'],
        help='哈希算法（默认由配置文件指定）'
    )
    parser.add_argument(
        '--ignore', '-i', 
        nargs='+', 
        default=[],
        help='忽略指定后缀的文件（例：--ignore .log .tmp）'
    )
    parser.add_argument(
        '--exclude', '-x', 
        nargs='+', 
        default=[],
        help='额外排除的文件或目录（例：--exclude temp/ test.log）'
    )
    parser.add_argument(
        '--block', '-b', 
        type=int, 
        help='分块大小（KB，默认由配置文件指定）'
    )
    parser.add_argument(
        '--output', '-o', 
        help='输出单文件哈希清单（默认由配置文件指定）'
    )
    parser.add_argument(
        '--cache', '-c', 
        action='store_true',
        help='启用增量计算（基于文件修改时间）'
    )
    parser.add_argument(
        '--config', '-f', 
        default='hash_config.ini',
        help='配置文件路径（默认：hash_config.ini）'
    )
    parser.add_argument(
        '--remote', '-r', 
        action='store_true',
        help='启用远程校验（需要配置服务器地址）'
    )
    
    args        = parser.parse_args()
    project_dir = os.path.abspath(args.directory)
    
    # 验证目录
    if not os.path.isdir(project_dir):
        print(f"错误：{project_dir} 不是有效的目录")
        sys.exit(1)
    
    # 加载配置文件
    config = load_config(args.config)
    
    # 合并配置（命令行参数优先于配置文件）
    hash_alg        = args.alg or config['algorithm']
    block_size      = (args.block or config['block_size']) * 1024  # 转换为字节
    output_list     = args.output or config['output_list']
    use_git         = args.git or config['use_git']
    ignore_suffixes = args.ignore or config['ignore_suffixes']
    # 合并配置文件和命令行的排除路径
    exclude_paths = config['exclude_paths'] + args.exclude
    cache_path    = os.path.join(project_dir, config['cache_file'])  # 缓存文件放在项目目录
    remote_server = config['remote_server'] if args.remote else ''
    
    # 验证分块大小
    if block_size <= 0:
        print(f"错误：分块大小必须为正数")
        sys.exit(1)
    
    # 执行计算
    project_hash, mode_name, _ = calculate_project_hash(
        root_dir        = project_dir,
        use_git         = use_git,
        hash_alg        = hash_alg,
        ignore_suffixes = ignore_suffixes,
        exclude_paths   = exclude_paths,    # 传递排除路径
        block_size      = block_size,
        output_list     = output_list if (args.output or config.get('output_list')) else None,
        use_cache       = args.cache,
        cache_path      = cache_path,
        remote_server   = remote_server
    )
    
    # 输出结果
    if project_hash:
        print(f"\n{'='*64}")
        print(f"工程综合哈希值（{mode_name}）")
        print(f"工程目录：{project_dir}")
        print(f"{project_hash}")
        print(f"{'='*64}")
