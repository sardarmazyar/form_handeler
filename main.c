#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>


#define form_len 1000
#define name_len 100

char **list_forms;
int n_forms=0;
void read_forms(){
    list_forms = (char**) malloc(form_len * sizeof(char*));
    for(int i=0; i<form_len; i++)
        *(list_forms+i)=malloc(name_len);
    struct dirent *dp;
    DIR *dir = opendir(".");
    if (!dir)
        printw("we couldn't open the directory\n");
    while ((dp = readdir(dir)) != NULL){
        int len=strlen(dp->d_name);
        if (!strcmp(dp->d_name+len-4,".txt"))
            strcpy(list_forms[n_forms++], dp->d_name);
    }
    closedir(dir);
}
int print_forms(){
    for (int i=0;i<n_forms;i++)
        printw("%s\n",list_forms[i]);
    return 1;
}
int new_form(){
    printw("choose a name form your form:\n");
    refresh();
    echo();
    char name[100];
    wgetstr(stdscr,name);
    //getchar();
    strcat(name,".txt");
    for (int i=0;i<n_forms;i++)
        if (!strcmp(name,list_forms[i])){
            char c;
            char order[100];
            printw("a file with this name already exists. do want to \"choose another name\" or get \"back to main menu\"\n");
            refresh();
            wgetstr(stdscr,order);
            //getchar(); // \n
            while (strcmp(order,"choose another name") && strcmp(order,"back to main menu")){
                printw("wrong input. insert 'choose another name' to choose an other name or insert 'back to main menu' to get back to the main_menu?\n");
                refresh();
                wgetstr(stdscr,order);
                //getchar(); // \n
            }
            if (!strcmp(order,"choose another name"))
                return new_form();
            else
                return 1;
        }
    FILE *fp;
    fp= fopen(name,"w");
    int len= strlen(name);
    name[len -4 ]='\0';
    mkdir(name);
    fclose(fp);
    name[len -4 ]='.';
    strcpy(list_forms[n_forms++],name);
    printw("form created sucsessfully\n");
    refresh();
    return 1;
}

int main_menu(){

    printw("you are in main menu\n");
    refresh();
    char order[20];
    wgetstr(stdscr,order);
    //getchar();
    if (!(strcmp(order,"new form")))
        return new_form();
    else if (!(strcmp(order,"list of forms")))
        return print_forms();
}



int main() {
    int maxy,maxx;

    initscr();
    getmaxyx(stdscr,maxy,maxx);
    move(7*maxy/10,0);
    read_forms();
    while(main_menu());
    endwin();
    return 0;
}
