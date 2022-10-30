#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _data{
    unsigned int year;     //год чеканки
    unsigned int value;    //номинал
    unsigned int count;    //количество
    char country[20];      //страна
    char material[20];     //материал
    char type;             //Тип
    union {                //Цена
        float cost_f;
        unsigned int cost_i;
    };
} data;
typedef struct _node{
    data info;
    struct _node *next, *prev;
} node;
typedef struct _roster{
    node *begin, *end;
    unsigned int count;
} roster;

void   view_list_direct (roster);
void   view_list_reverse(roster);
void   view_early_centry(roster);
void   delete_of_list   (roster*);
void   add_in_list      (roster*);
void   cost             (roster);

data   read_info     ();
void   read_str      (char*, char*);
float  read_num      (char*, char);
char   check_num_f   (char*);
char   check_num_i   (char*);

void   add_to_end    (roster*, data);
void   add_to_begin  (roster*, data);
void   delete_element(roster*, node*);

void   test_open     (char*);
void   create_file   (char*);
void   generate_file (char*);
roster create_list   (char*);
void   save_file     (roster, char*);

void   print         (int, data);

//==========================================================================================================================================================================================================================
int main(int argc, char* argv[]) {
    char way[100] = {0}, menu;
    printf("1");
    if(argc > 1)
        strcpy(way, argv[1]);
    test_open(way);
    puts("Ok open or create");
    system("pause");

    roster list = create_list(way);

    do{
        system("cls");

        puts("1) View list in direct order");
        puts("2) View list in reverse order");
        puts("3) View early specified century");
        puts("4) Add element");
        puts("5) Delete element");
        puts("6) Collection cost");

        puts("\n7) ReGenerate file");
        puts("0) Exit");
        printf("\nInput number menu: ");

        menu = getchar();
        switch (menu){
            case '1': view_list_direct(list); break;
            case '2': view_list_reverse(list); break;
            case '3': view_early_centry(list); break;
            case '4': add_in_list(&list); break;
            case '5': delete_of_list(&list); break;
            case '6': cost(list); break;
            case '7':
                system("cls");
                generate_file(way);
                list = create_list(way);
                break;
        }
    } while(menu != '0');

    save_file(list, way);
    return 0;
}
//==========================================================================================================================================================================================================================

void view_list_direct (roster list){   //Просмотр списка в прямом порядке (по странице)
    if(list.begin == NULL){
        system("cls");
        puts("Empty list");
        system("pause");
        return;
    }

    int i = 0, j = 0;
    int count_str = list.count % 14 ? list.count/14+1 : list.count/14;
    int count = list.count, count_last = 1;
    node **mass = (node**)malloc(count_str * sizeof(node*));

    while(i < count){
        if(!(i++ % 14))
            mass[j++] = list.begin;
        list.begin = list.begin->next;
    }

    j = 0;
    char menu;
    node *cur;
    do{
        count = count_last;
        cur = mass[j];

        system("cls");
        puts("|  # | value | count |      country      |      material     | cost |year|");
        puts("+----+-------+-------+-------------------+-------------------+------+----+");

        while(cur != NULL && count - count_last < 14){
            print(count++, cur->info);
            cur = cur->next;
        }
        puts("+----+-------+-------+-------------------+-------------------+------+----+");
        puts("\nup - w, down - s, exit - 0");
        printf("menu: ");

        menu = getchar();
        switch(menu){
            case 'W':
            case 'w': if(j>0) { j--; count_last -= 14; } break;
            case 'S':
            case 's': if(j<count_str-1) { j++; count_last += 14; } break;
        }
    } while(menu != '0');
}
/*void view_list_direct (roster list){   //Просмотр списка в прямом порядке
    int count=1, count_last = 1;
    char menu;
    data element;
    node *start = list.begin, *cur;

    if(list.begin == NULL){
        system("cls");
        puts("Empty list");
        system("pause");
        return;
    }

    do{
        system("cls");
        puts("|  # | value | count |      country      |      material     | cost |year|");
        puts("+----+-------+-------+-------------------+-------------------+------+----+");

        cur = start;
        count = count_last;
        while(count-count_last < 15 && cur!=NULL){
            element = cur->info;
            print(count++, element);
            cur = cur->next;
        }

        puts("+----+-------+-------+-------------------+-------------------+------+----+");
        puts("\nup - w, down - s, exit - 0");
        printf("menu: ");

        menu = getchar();
        switch(menu){
            case 'W':
            case 'w': if(start->prev != NULL) {start = start->prev; count_last--;} break;
            case 'S':
            case 's': if(start->next != NULL && list.count - count_last > 14) {start = start->next; count_last++;} break;
        }
    } while(menu != '0');
}*/
void view_list_reverse(roster list){   //Просмотр списка в обратном порядке
    int count = list.count, count_last = list.count;
    char menu;
    data element;
    node *end = list.end, *cur;

    if(list.begin == NULL){
        system("cls");
        puts("Empty list");
        system("pause");
        return;
    }

    do{
        system("cls");
        puts("|  # | value | count |      country      |      material     | cost |year|");
        puts("+----+-------+-------+-------------------+-------------------+------+----+");

        cur = end;
        count = count_last;

        while(cur!=NULL && count_last - count < 15) {
            element = cur->info;
            print(count--, element);
            cur = cur->prev;
        }

        puts("+----+-------+-------+-------------------+-------------------+------+----+");

        puts("\nup - w, down - s, exit - 0");
        printf("menu: ");

        menu = getchar();
        switch(menu){
            case 'W':
            case 'w': if(end->next != NULL) {end = end->next; count_last++;} break;
            case 'S':
            case 's': if(end->prev != NULL && count_last > 15) {end = end->prev; count_last--;} break;
        }
    } while(menu != '0');
}
void view_early_centry(roster list){   //Вывод информации о монетах выпущенных ранее указанного века
    int count=1, count_last = 1, year;
    char menu;
    data element;
    node *start = list.begin, *cur;

    if(list.begin == NULL){
        system("cls");
        puts("Empty list");
        system("pause");
        return;
    }

    system("cls");
    year = (int)read_num("century", 1);
    year *= 100;

    do{
        system("cls");
        puts("|  # | value | count |      country      |      material     | cost |year|");
        puts("+----+-------+-------+-------------------+-------------------+------+----+");

        cur = start;
        count = count_last;
        element = cur->info;

        while(count-count_last < 15 && cur!=NULL && element.year < year){
            print(count++, element);
            element = cur->info;
            cur = cur->next;
        }

        puts("+----+-------+-------+-------------------+-------------------+------+----+");
        puts("\nup - w, down - s, exit - 0");
        printf("menu: ");

        menu = getchar();
        switch(menu){
            case 'W':
            case 'w': if(start->prev != NULL) {start = start->prev; count_last--;} break;
            case 'S':
            case 's': if(start->next != NULL && list.count - count_last > 14 && (*cur).info.year < year) {start = start->next; count_last++;} break;
        }
    } while(menu != '0');
}
void delete_of_list   (roster *list){  //Удаление узла из списка (поиск узла и вывод сообщения)
    system("cls");
    if(list->begin == NULL){
        puts("Empty list");
        system("pause");
        return;
    }

    char menu;
    int count = list->count;
    data info;
    info.year = (int)read_num("year", 1);

    node *cur = list->begin;
    while(count-- && cur->info.year <= info.year){
        if(cur->info.year == info.year){
            info = cur->info;
            do {
                system("cls");
                puts("|    | value | count |      country      |      material     | cost |year|");
                puts("+----+-------+-------+-------------------+-------------------+------+----+");
                print(0, info);
                puts("+----+-------+-------+-------------------+-------------------+------+----+");
                printf("\nIs that element? (y/n): ");
                menu = getchar();
                switch(menu){
                    case 'Y':
                    case 'y': delete_element(list, cur); return;
                }
            } while (menu != 'n' && menu != 'N');
        }

        cur = cur->next;
    }
}
void add_in_list      (roster *list){  //Добавление узла в список
    data info = read_info();
    if(list->begin == NULL || list->begin->info.year >= info.year)
        add_to_begin(list, info);
    else if(list->end->info.year <= info.year)
        add_to_end(list, info);
    else {
        node *cur = list->begin;
        while(cur->info.year < info.year)
            cur = cur->next;
        node *element = (node*)malloc(sizeof(node));

        element->info = info;

        element->next = cur;
        element->prev = cur->prev;
        cur->prev = element;
        cur->prev->prev->next = element;
        list->count++;
    }
}
void cost             (roster list){   //Вывод стоимости коллекции
    system("cls");

    if(list.begin == NULL){
        printf("Empty list\n");
        system("pause");
        return;
    }

    double cost = 0, temp = 0;
    while (list.begin != NULL) {
        if(list.begin->info.type) temp = list.begin->info.count * list.begin->info.cost_i;
        else temp = list.begin->info.count * list.begin->info.cost_f;
        cost += temp;
        list.begin = list.begin->next;
    }

    printf("Cost: %.2lf\n", cost);
    system("pause");
}
//==========================================================================================================================================================================================================================

data   read_info      (){                           //Чтение данных
    system("cls");
    data info;
    float num;

    info.value = (int)read_num("value", 1);
    info.count = (int)read_num("count", 1);
    num = read_num("cost", 0);

    if((float)((int)num) == num) {
        info.type = 1;
        info.cost_i = (int)num;
    } else {
        info.type = 0;
        info.cost_f = num;
    }

    info.year = (int)read_num("year", 1);
    read_str("country", info.country);
    read_str("material", info.material);

    return info;
}
void   read_str       (char *message, char *data){  //Чтение строки из консоли
    char *temp = (char*)malloc(100*sizeof(char));
    printf("Input %s: ", message);
    fflush(stdin);
    fgets(temp, 100, stdin);

    while(strchr(temp, ' ') || strlen(temp) > 20 || strlen(temp) < 2){
        printf("So long or bad str. Input new %s: ", message);
        fflush(stdin);
        fgets(temp, 100, stdin);
    }

    temp[strlen(temp)-1] = 0;
    strcpy(data, temp);
}
float  read_num       (char *message, char type){   //Чтение строки из коносли и преобразование в число
    char str[100], t;
    printf("Input %s: ", message);
    fflush(stdin);
    fgets(str, 100, stdin);

    while(strlen(str) < 2 || !(t = type ? check_num_i(str) : check_num_f(str))){
        printf("Bad num. Input new %s: ", message);
        fflush(stdin);
        fgets(str, 100, stdin);
    }

    return atof(str);
}
char   check_num_f    (char *str){                  //Проверка считанной строки на то является ли она вещественным числом или набором символов
    while(*str && *str != '\n')
        if(!strchr("1234567890.", *str++))
            return 0;
    return 1;
}
char   check_num_i    (char *str){                  //Проверка считанной строки на то является ли она целым числом или набором символов
    while(*str && *str != '\n')
        if(!strchr("1234567890", *str++))
            return 0;
    return 1;
}

void   add_to_end     (roster *list, data info){    //Добавление узла в конец списка
    node *element = (node*)malloc(sizeof(node));
    element->next = NULL;
    element->info = info;

    if(list->end != NULL){
        element->prev = list->end;
        list->end->next = element;
        list->end = element;
    } else {
        list->begin = element;
        list->end = element;
        element->prev = NULL;
        list->count=0;
    }
    list->count++;
}
void   add_to_begin   (roster *list, data info){    //Добавление узла в начало списка
    node *element = (node*)malloc(sizeof(node));
    element->prev = NULL;
    element->info = info;

    if(list->begin != NULL){
        element->next = list->begin;
        list->begin->prev = element;
        list->begin = element;
    } else {
        list->begin = element;
        list->end = element;
        element->next = NULL;
        list->count=0;
    }
    list->count++;
}
void   delete_element (roster *list, node *cur){    //Удаление узла из списка (переназначение указателей и очистка памяти)
    node *prev, *next;
    if(list->count == 1)                             //Удаление если в списке 1 элемент
      list->begin = list->end = NULL;
    else if(cur != list->begin && cur != list->end){ //Удаление из середины списка
        next = cur->next;
        prev = cur->prev;
        next->prev = prev;
        prev->next = next;
    } else if(cur != list->begin && cur == list->end){  //Удаление из конца списка
        prev = cur->prev;
        prev->next = NULL;
        list->end = prev;
    } else {                                            //Удаление из начала списка
        next = cur->next;
        next->prev = NULL;
        list->begin = next;
    }
    list->count--;
    free(cur);
}
void   test_open      (char *way){                  //Тестовое открытие файла (создание файла и генерация информаци в нем если его не удалось открыть)
    FILE *file;

    if((file = fopen(way, "rb")) != NULL){
        return;
    } else {
        char menu;
        do{
            system("cls");
            puts("Error open");
            printf("Do you want create a new file? (y/n): ");
            menu = getchar();
            getchar();
            if(menu == 'y' || menu == 'Y') {
                create_file(way);
                return;
            }
        } while(menu != 'y' && menu != 'n' && menu != 'Y' && menu != 'N');

        printf("Input new way: ");
        gets(way);
        test_open(way);
    }
    fclose(file);
}
void   create_file    (char *way){                  //Создание файла с возможностью генерации информации в нем
    FILE *file;
    char menu;
    while((file = fopen(way, "wb")) == NULL){
        puts("Error create file");
        printf("Input new way: ");
        gets(way);
    }

    fclose(file);

    printf("Do you wand generate info in file? (y/n): ");
    do{
        menu=getchar();
        getchar();
        if(menu == 'y' || menu == 'Y')
            generate_file(way);
    } while(menu != 'y' && menu != 'n' && menu != 'Y' && menu != 'N');
}
void   generate_file  (char *way){                  //Генерация и сортировка данных в файле
    srand(time(NULL));

    char *country[] = {"England", "Finland", "Iceland", "Norway", "Scotland", "Sweden", "Germany", "Russia", "Greece", "Poland"};
    char *material[] = {"Aluminium", "Brass", "Copper", "Gold", "Bronze", "Iron", "Lead", "Silver", "Tin", "Nickel"};
    FILE *file;
    data element;

    if((file = fopen(way, "wb")) == NULL){
        puts("Error open/create file");
        create_file(way);
    }

    int count, count_cur, i, j;
    puts("How many elements create?");
    count = (int)read_num("count", 1);

    data *a = (data*)malloc(count * sizeof(data)), *an = a;
    count_cur = count;

    while(count_cur--){
        strcpy(element.country, country[rand()%10]);
        strcpy(element.material, material[rand()%9]);
        element.count = rand()%50+1;
        element.value = rand()%100*10+10;
        element.year = rand()%2020+1;
        element.type = rand()%2;

        if(element.type) element.cost_i = rand()%100*10;
        else             element.cost_f = rand()%1000 + rand()%1000/(float)100;

        *a++ = element;
    }

    for(i=0; i<count; i++)
        for(j=count; j>i; j--)
            if(an[j].year < an[j-1].year){
                element = an[j-1];
                an[j-1] = an[j];
                an[j] = element;
            }

    fwrite(an, sizeof(data)*count, 1, file);
    free(an);

    puts("Ok generate file");
    fclose(file);
}
roster create_list    (char *way){                  //Создание списка на основании файла
    FILE *file;
    roster list;
    list.begin=NULL; list.end = NULL; list.count=0;

    if((file = fopen(way, "rb")) == NULL){
        puts("Error create list");
        system("pause");
        return list;
    }

    data info;

    fseek(file, 0, SEEK_END);
    if(ftell(file) < sizeof(info)){
        puts("Empty file");
        system("pause");
        return list;
    }

    fseek(file, 0, SEEK_SET);
    while(fread(&info, sizeof(info), 1, file))
        add_to_end(&list, info);

    return list;
}
void   save_file      (roster list, char *way){     //Сохранение файла после завершения работы программы и выгрузка из памяти СХ
    FILE *file;
    data info;

    if((file = fopen(way, "wb")) == NULL){
        puts("Error save file");
        system("pause");
        return;
    }

    while(list.begin != NULL){
        info = list.begin->info;
        fwrite(&info, sizeof(info), 1, file);
        list.end = list.begin;
        list.begin = list.begin->next;
        free(list.end);
    }

    fclose(file);
}
void   print          (int count, data element){    //Печать в консоль содержимого полей
    if(element.type) printf("|%3d)|%7d|%7d|%-19s|%-19s|%-6d|%4d|\n",  count, element.value, element.count, element.country, element.material, element.cost_i, element.year);
    else             printf("|%3d)|%7d|%7d|%-19s|%-19s|%6.2f|%4d|\n", count, element.value, element.count, element.country, element.material, element.cost_f, element.year);
}
