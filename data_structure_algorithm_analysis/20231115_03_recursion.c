#include <dirent.h>   // 包含opendir()、readdir()和closedir()
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // 包含stat()

// 递归遍历目录
void traverse_dir(const char *dir_path) 
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL) 
    {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        // 忽略当前目录(".")和上一级目录("..")
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // 构建完整路径
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        // 检查是目录还是文件
        struct stat path_stat;
        stat(full_path, &path_stat);
        if (S_ISDIR(path_stat.st_mode)) 
        {
            printf("Directory: %s\n", full_path);
            traverse_dir(full_path); // 递归调用
        } 
        else 
            printf("File: %s\n", full_path);
    }

    closedir(dir);
}

int main() 
{
    const char *start_dir = "."; // 当前目录
    traverse_dir(start_dir);

    return 0;
}
