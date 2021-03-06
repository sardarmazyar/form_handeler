#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>

#define form_len 1000
#define name_len 100

char **list_forms;
int n_forms = 0;

void read_form(FILE *fp) {
    int n, inside_win, size_numbers[4];
    char str[name_len], str2[name_len];
    fscanf(fp, "%d", &n);
    WINDOW *win_in[n], *win_out[n];
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s", str);
        fscanf(fp, "%d %d %d %d %d", &inside_win, &size_numbers[0], &size_numbers[1], &size_numbers[2],
               &size_numbers[3]);
        if (inside_win) {
            win_in[i] = newwin(size_numbers[0], size_numbers[1], size_numbers[2], size_numbers[3]);
            refresh();
            fscanf(fp, "%d %d %d %d", &size_numbers[0], &size_numbers[1], &size_numbers[2], &size_numbers[3]);
        } else
            win_in[i] = newwin(size_numbers[0] - 2, size_numbers[1] - 2, size_numbers[2] + 1, size_numbers[3] + 1);
        win_out[i] = newwin(size_numbers[0], size_numbers[1], size_numbers[2], size_numbers[3]);
        refresh();
        fscanf(fp, "%s", str2);
        if (!strcmp(str2, "normal")) {
            box(win_out[i], 0, 0);
        } else if
                (!strcmp(str2, "none"));
        else
            box(win_out[i], str2[0], str2[0]);
        wrefresh(win_out[i]);
        fgetc(fp);
        if (!(strcmp(str, "button")) || !(strcmp(str, "label"))) {
            fscanf(fp, "%[^\n]s", str);
            fgetc(fp);
            wprintw(win_in[i], "%s\n", str);
            wrefresh(win_in[i]);
        } else if (!(strcmp(str, "combobox"))) {
            wprintw(win_in[i], "Choose one\n");
            fscanf(fp, "%d", &n);
            fgetc(fp);
            for (int j = 0; j < n; j++) {
                fscanf(fp, "%s", str);
                fgetc(fp);
            }
        } else if (!(strcmp(str, "checkbox"))) {
            fscanf(fp, "%d", &n);
            fgetc(fp);// \n
            for (int j = 0; j < n; j++) {
                fscanf(fp, "%s", str);
                fgetc(fp);
                wprintw(win_in[i], "%s\n", str);
            }
        }
    }
}

void read_forms(WINDOW *command_win) {
    list_forms = (char **) malloc(form_len * sizeof(char *));
    for (int i = 0; i < form_len; i++)
        *(list_forms + i) = malloc(name_len);
    struct dirent *dp;
    DIR *dir = opendir(".");
    if (!dir)
        wprintw(command_win, "we couldn't open the directory\n");
    while ((dp = readdir(dir)) != NULL) {
        int len = strlen(dp->d_name);
        if (!strcmp(dp->d_name + len - 4, ".txt"))
            strcpy(list_forms[n_forms++], dp->d_name);
    }
    closedir(dir);
}

int print_forms(WINDOW *command_win) {
    for (int i = 0; i < n_forms; i++)
        wprintw(command_win, "%s\t", list_forms[i]);
    wrefresh(command_win);
    wprintw(command_win, "\n");
    return 1;
}

int design_form(WINDOW *command_win, FILE *fp) {
    wclear(command_win);
    read_form(fp);
    char order[name_len];
    wprintw(command_win, "insert your order:\n");
    wgetstr(command_win, order);
    int border_info[9], n;
    char border_type[name_len];
    while (strcmp(order, "end")) {
        fseek(fp, 0, SEEK_SET);
        fscanf(fp, "%d", &n);
        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "%d", n + 1);
        fseek(fp, 0, SEEK_END);
        wscanw(command_win, "%d", &border_info[0]);
        if (border_info[0]) {
            for (int i = 1; i < 9; i++) {
                wclear(command_win);
                wscanw(command_win, "%d", &border_info[i]);
            }
            fprintf(fp, "%s %d %d %d %d %d %d %d %d %d ", order, border_info[0], border_info[1], border_info[2],
                    border_info[3], border_info[4],
                    border_info[5], border_info[6], border_info[7], border_info[8]);
        } else {
            for (int i = 1; i < 5; i++) {
                wclear(command_win);
                wscanw(command_win, "%d", &border_info[i]);
            }
            fprintf(fp, "%s %d %d %d %d %d ", order, border_info[0], border_info[1], border_info[2], border_info[3],
                    border_info[4]);
        }
        wclear(command_win);
        wscanw(command_win, "%s", border_type);
        fprintf(fp, "%s\n", border_type);
        if (!strcmp(order, "label")) {
            char label_text[name_len];
            wprintw(command_win, "insert the text inside the label:\n");
            wgetstr(command_win, label_text);
            fprintf(fp, "%s\n", label_text);
        } else if (!strcmp(order, "button")) {
            char button[name_len];
            wprintw(command_win, "insert the text inside the button:\n");
            wgetstr(command_win, button);
            fprintf(fp, "%s\n", button);
        }
        fseek(fp, 0, SEEK_SET);
        read_form(fp);
        wprintw(command_win, "insert your order:\n");
        wgetstr(command_win, order);
    }
    return 1;
}

int new_form(WINDOW *command_win) {
    wprintw(command_win, "choose a name for your form:\n");
    char name[name_len];
    wgetstr(command_win, name);
    strcat(name, ".txt");
    for (int i = 0; i < n_forms; i++)
        if (!strcmp(name, list_forms[i])) {
            char order[100];
            wprintw(command_win,
                    "a file with this name already exists. do want to \"choose another name\" or get \"back to main menu\"\n");
            wgetstr(command_win, order);
            wclear(command_win);
            while (strcmp(order, "choose another name") && strcmp(order, "back to main menu")) {
                wprintw(command_win,
                        "wrong input. insert 'choose another name' to choose an other name or insert 'back to main menu' to get back to the main_menu?\n");
                refresh();
                wgetstr(command_win, order);
                wclear(command_win);
            }
            if (!strcmp(order, "choose another name"))
                return new_form(command_win);
            else
                return 1;
        }
    FILE *fp;
    fp = fopen(name, "w+");
    int len = strlen(name);
    name[len - 4] = '\0';
    mkdir(name);
    fprintf(fp, "%d\n", 1);
    wclear(command_win);
    wprintw(command_win, "insert border information:\n");
    int border_info[10];
    wscanw(command_win, "%d", &border_info[0]);
    if (!border_info[0]) {
        // wprintw(command_win,"Im in");
        for (int i = 1; i < 5; i++) {
            wscanw(command_win, "%d", &border_info[i]);
            wclear(command_win);
        }
            fprintf(fp, "border %d %d %d %d %d ", border_info[0], border_info[1], border_info[2], border_info[3],
                border_info[4]);
    } else {
        for (int i = 1; i < 9; i++) {
            wscanw(command_win, "%d", &border_info[i]);
            wclear(command_win);
        }
            fprintf(fp, "border %d %d %d %d %d %d %d %d %d ", border_info[0], border_info[1], border_info[2],
                border_info[3],
                border_info[4], border_info[5], border_info[6], border_info[7], border_info[8]);
    }
    char border_type[name_len];
    wscanw(command_win, "%s", border_type);
    fprintf(fp, "%s\n", border_type);
    name[len - 4] = '.';
    strcpy(list_forms[n_forms++], name);
    wprintw(command_win, "form created successfully\n");
    fclose(fp);
    fp = fopen(name, "r+");
    return design_form(command_win, fp);
}


int main_menu(WINDOW *command_win) {
    wprintw(command_win, "you are in main menu\n");
    wrefresh(command_win);
    char order[name_len];
    wgetstr(command_win, order);
    wclear(command_win);
    if (!(strcmp(order, "new form")))
        return new_form(command_win);
    else if (!(strcmp(order, "list of forms")))
        return print_forms(command_win);
    else if (!(strcmp(order, "edit form"))) {
        wprintw(command_win, "insert name of the form:\n");
        wgetstr(command_win, order);
        FILE *fp = fopen(order, "r+");
        if (fp == NULL) {
            wprintw(command_win, "file doesn't exist or we can not open it.\n");
            return 1;
        }
        return design_form(command_win, fp);
    }
    else if (!strcmp(order, "exit"))
        return 0;
    else {
        wprintw(command_win, "command \"%s\" not found, insert again\n",order);
        return 1;
    }
}


int main() {
    int maxy, maxx;
    initscr();
    getmaxyx(stdscr, maxy, maxx);
    WINDOW *command_win = newwin(maxy - (8 * maxy / 10), maxx, 8 * maxy / 10, 0);
    refresh();
    read_forms(command_win);
    while (main_menu(command_win));
    endwin();
    return 0;
}
