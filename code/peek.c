
#include "peek.h"
#include "headers.h"



void print_file_info(const char *filename, const struct stat *file_info) {
    
    printf("%s\t", filename);
    printf((S_ISDIR(file_info->st_mode)) ? "d" : "-");
    printf((file_info->st_mode & S_IRUSR) ? "r" : "-");
    printf((file_info->st_mode & S_IWUSR) ? "w" : "-");
    printf((file_info->st_mode & S_IXUSR) ? "x" : "-");
    printf((file_info->st_mode & S_IRGRP) ? "r" : "-");
    printf((file_info->st_mode & S_IWGRP) ? "w" : "-");
    printf((file_info->st_mode & S_IXGRP) ? "x" : "-");
    printf((file_info->st_mode & S_IROTH) ? "r" : "-");
    printf((file_info->st_mode & S_IWOTH) ? "w" : "-");
    printf((file_info->st_mode & S_IXOTH) ? "x" : "-");
    printf("\t%ld\t", (long)file_info->st_nlink);

    struct passwd *pw = getpwuid(file_info->st_uid);
    struct group *gr = getgrgid(file_info->st_gid);
    printf("%s\t%s\t", pw->pw_name, gr->gr_name);
    printf("%ld\t", (long)file_info->st_size);

    char date[20];
    strftime(date, 20, "%b %d %H:%M", localtime(&file_info->st_mtime));
    printf("%s\t", date);
}

void peek(const char *path, int show_hidden, int show_details) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue; // Skip hidden files
        }
          char full_path[4096];
snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // char full_path[PATH_MAX];
        // snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat file_info;
        if (lstat(full_path, &file_info) == -1) {
            perror("lstat");
            continue;
        }

        if (show_details) {
            print_file_info(entry->d_name, &file_info);
        }
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}
