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
void read_form(FILE *fp){
    int n, size_numbers[4];
    char str[name_len];
    fscanf(fp,"%d",&n);
    WINDOW *win[n];
//    win[0]= newwin(5,5,0,0);
//    refresh();
//    box(win[0],0,0);
//    wrefresh(win[0]);
//    win[1]= newwin(5,5,13,17);
//    refresh();
//    box(win[1],0,0);
//    wrefresh(win[1]);
//    win[2]= newwin(20,30,1,1);
//    refresh();
//    box(win[2],0,0);
//    wrefresh(win[2]);
//    win[3]= newwin(30,40,1,1);
//    refresh();
//    box(win[3],0,0);
//    wrefresh(win[3]);
    for (int i=0; i<n; i++){
        fscanf(fp,"%s",str);
	    //printw("%s\n",str);
        if (!(strcmp(str,"border")) || !(strcmp(str,"textbox")) || !(strcmp(str,"label"))){
            fscanf(fp,"%d %d %d %d", &size_numbers[0],&size_numbers[1],&size_numbers[2],&size_numbers[3]);
            //wprintw(win[i],"%d %d %d %d\n",size_numbers[0],size_numbers[1],size_numbers[2],size_numbers[3]);
            win[i]= newwin(size_numbers[0],size_numbers[1],size_numbers[2],size_numbers[3]);
            refresh();
//            wrefresh(win[i]);
//	        wprintw(win[i],"aaaaaaaaa\n");
//            wrefresh(win[i]);
//	        refresh();

            fscanf(fp,"%s",str);
            //printw("%s\n",str);
            if (!strcmp(str,"normal")) {
                box(win[i],0, 0);
                move(size_numbers[2],size_numbers[3]);
                printw(win[i],"aaaaaaaaa\n");
                wrefresh(win[i]);
                //wprintw(win[i],"we are in\n");
            }
            else if (!strcmp(str,"none"))
                ;
            else
                box(win[i],str[0],str[0]);
        }
        if (!(strcmp(str,"label"))){
            fgets(str,name_len,fp);
            //wprintw(win[i],str);
        }
        else if (!(strcmp(str,"border"))){
            ;
        }
        else if (!(strcmp(str,"border"))){
            ;
        }
    }

 }
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
int design_form(FILE *fp){
    read_form(fp);
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
    printw("form created successfully\n");
    refresh();
    return design_form(fp);
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
    else if (!(strcmp(order,"edit form"))){
        //some code
        FILE *fp;
        return design_form(fp);}
}



int main() {
//    int maxy,maxx;
  initscr();
//    WINDOW *win= newwin(10,10,10,10);
//    refresh();
//    box(win,0,0);
//    wrefresh(win);
//    getch();
//    endwin();
//    return 0;
    FILE *fp= fopen("a.txt","r");
    read_form(fp);
    refresh();
//    WINDOW *win;
//    win=newwin(100,100,10,10);
//    WINDOW *win2=newwin(20,20,12,12);
//    refresh();
//    box(win,0,0);
//    box(win2,0,0);
//    wrefresh(win);
//    wrefresh(win2);

//    getmaxyx(stdscr,maxy,maxx);
//    move(0*maxy/10,0);
//    read_forms();
//    while(main_menu());
	getch();
    endwin();
    return 0;
}
