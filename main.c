#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VERSION "1.1.0"
#define UPDATE_CMD "curl -o- https://raw.githubusercontent.com/ArdaGnsrn/pvm-ubuntu/master/install.sh | bash"

int is_php_package_available(char *version);

int install_apt_repository_if_not_exists();

void install_php_version(char *version);

void use_php_version(char *version);

void use_command(int argc, char *argv[]);

int is_php_version_installed(char *version);

void install_command(int argc, char *argv[]);

void list_command();

char *current_php_version();

void uninstall_command(int argc, char *argv[]);

void update_command();

void version_command();

int main(int argc, char *argv[]) {
    install_apt_repository_if_not_exists();
    if (argc < 2) {
        printf("\n");
        printf("Running version %s.\n", VERSION);
        printf("https://github.com/ArdaGnsrn/pvm-ubuntu/\n");
        printf("\n");
        printf("Usage:\n");
        printf("  pvm install <version>\t\t:\tInstall a PHP version.\n");
        printf("  pvm use <version>\t\t:\tUse a PHP version.\n");
        printf("  pvm list\t\t\t:\tList installed PHP versions.\n");
        printf("  pvm uninstall <version>\t:\tUninstall a PHP version.\n");
        printf("\n");
        printf("  pvm update\t\t\t:\tUpdate to the latest version.\n");
        printf("  pvm version\t\t\t:\tShow the current version.\n");
        printf("\n");
        return 1;
    }

    if (strcmp(argv[1], "install") == 0) {
        install_command(argc, argv);
    } else if (strcmp(argv[1], "uninstall") == 0) {
        uninstall_command(argc, argv);
    } else if (strcmp(argv[1], "use") == 0) {
        use_command(argc, argv);
    } else if (strcmp(argv[1], "list") == 0) {
        list_command();
    } else if (strcmp(argv[1], "update") == 0) {
        update_command();
    } else if (strcmp(argv[1], "version") == 0) {
        version_command();
    } else {
        return main(1, argv);
    }

    return 0;
}

char *current_php_version() {
    char command[1024];
    char output[1024];
    FILE *fp;

    sprintf(command, "php -v | grep PHP | awk '{print $2}'");
    fp = popen(command, "r");

    if (fp == NULL) {
        printf("There was an error while running the command.\n");
        return "";
    }

    while (fgets(output, sizeof(output), fp) != NULL) {
        char *version = strtok(output, "-");
        pclose(fp);
        return version;
    }

    pclose(fp);
    return "";
}

void version_command() {
    printf("Running version %s.", VERSION);
    printf("\n");
}

void update_command() {
    system(UPDATE_CMD);
    printf("Updated to the latest version.\n");
}

void list_command() {
    char command[1024];
    char output[1024];
    FILE *fp;

    if (access("/usr/bin/php", F_OK) == -1) {
        printf("No PHP versions found.\n");
        return;
    }

    sprintf(command, "ls -d /usr/bin/php* | awk -F'/' '{print $4}'");

    fp = popen(command, "r");

    if (fp == NULL) {
        printf("There was an error while running the command.\n");
        return;
    }

    char *current_version = current_php_version();

    // push array to store the versions
    char versions[1024][1024];
    while (fgets(output, sizeof(output), fp) != NULL) {
        memmove(output, output + 3, strlen(output));
        output[strcspn(output, "\n")] = 0;
        if (strspn(output, "0123456789.") != strlen(output)) continue;

        int i = 0;
        while (strlen(versions[i]) > 0) i++;
        strcpy(versions[i], output);
    }

    if (strlen(versions[0]) == 0) {
        printf("No PHP versions found.\n");
        return;
    }

    printf("\nAvailable PHP versions:\n");
    for (int i = 0; i < 1024; i++) {
        if (strlen(versions[i]) == 0) break;

        char *version;
        version = (char *) malloc(1024 * sizeof(char));

        if (strstr(current_version, versions[i]) != NULL) sprintf(version, "  * %s\n", versions[i]);
        else sprintf(version, "    %s\n", versions[i]);

        printf("%s", version);
    }

    pclose(fp);
    printf("\n");
}

void use_command(int argc, char *argv[]) {
    if (argc < 3) {
        printf("A version argument is required but missing.\n");
        return;
    }

    use_php_version(argv[2]);
}

void install_command(int argc, char *argv[]) {
    if (argc < 3) {
        printf("A version argument is required but missing.\n");
        return;
    }

    if (!is_php_package_available(argv[2])) {
        printf("PHP version %s is not available.\n", argv[2]);
        return;
    }

    install_php_version(argv[2]);

    printf("\nPHP version %s is installed.\n", argv[2]);
    printf("You can use it by running 'pvm use %s'.\n", argv[2]);
}

void uninstall_command(int argc, char *argv[]) {
    if (argc < 3) {
        printf("A version argument is required but missing.\n");
        return;
    }

    if (!is_php_version_installed(argv[2])) {
        printf("PHP version %s is not installed.\n", argv[2]);
        return;
    }

    char command[1024];
    sprintf(command, "sudo apt remove php%s -y", argv[2]);
    system(command);

    system("sudo apt autoremove -y");

    printf("PHP version %s is uninstalled.\n", argv[2]);
}

int is_php_package_available(char *version) {
    char command[1024];
    char output[1024];
    FILE *fp;

    sprintf(command, "apt-cache show php%s | grep Version", version);

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("There was an error while running the command.\n");
        return -1;
    }

    while (fgets(output, sizeof(output), fp) != NULL) {
        if (strstr(output, "deb.sury.org") != NULL) {
            pclose(fp);
            return 1;
        }
    }

    pclose(fp);
    return 0;
}

void install_php_version(char *version) {
    if (is_php_version_installed(version)) {
        printf("Version %s is already installed.\n", version);
        return;
    }

    char command[1024];
    sprintf(command, "sudo apt install php%s -y", version);
    system(command);
}

int is_php_version_installed(char *version) {
    char path[1024];
    sprintf(path, "/usr/bin/php%s", version);
    if (access(path, F_OK) != -1) {
        return 1;
    }
    return 0;
}

void use_php_version(char *version) {
    if (!is_php_version_installed(version)) {
        printf("PHP version %s is not installed.\n", version);
        return;
    }

    char command[1024];
    sprintf(command, "sudo update-alternatives --set php /usr/bin/php%s", version);
    system(command);

    printf("Now using PHP version %s.\n", version);
}

int install_apt_repository_if_not_exists() {
    char output[1024];
    FILE *fp;

    fp = popen("grep -r --include '*.list' '^deb ' /etc/apt/sources.list /etc/apt/sources.list.d/", "r");
    if (fp == NULL) {
        printf("There was an error while running the command.\n");
        return -1;
    }

    while (fgets(output, sizeof(output), fp) != NULL) {
        if (strstr(output, "ondrej-ubuntu-php-jammy.list:deb") != NULL) {
            pclose(fp);
            return 1;
        }
    }

    pclose(fp);

    printf("\n\nInstalling PHP repository...\n\n");

    system("sudo add-apt-repository ppa:ondrej/php -y");
    system("sudo apt-get update -y");

    printf("\n\nPHP repository installed.\n\n");

    return 0;
}
