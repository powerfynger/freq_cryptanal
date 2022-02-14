#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
//Максимальная длина текста
#define MAX_LEN 5000
//Максимальное колличсетво сохранений, чуть больше мощности алфавита 
#define MAX_BACK_UPS 36
//Максимальная длина строки
#define MAX_LINE_LENGTH 200
//Максимальное кол-во слов = MAX_LEN/5, 4.9-5.9 средняя длина слова в русс яз
#define MAX_WORDS_COUNT 500
//Максимальная длина слова
#define MAX_WORD_LEN 35

const wchar_t* rus_freq = L"оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъё";
const char* separators = " ";

wchar_t versions[MAX_BACK_UPS][MAX_LEN] = {'\0'};
int curr_version = 0;
wchar_t last_change[MAX_BACK_UPS][2] = {L'\0'};

char safeInput() {
    char input[200] = {'\0'};
    fgets(input, sizeof(input), stdin);
    if(input[1] != 10){
        return 0;
    }
    
    return input[0];
}

void print_help_message(){
    printf("\
1 - Вывод на экран всех слов, сгрупированных по колличеству букв.\n\
2 - Вывод на экран всех слов, сгрупированных по колличеству нерасшифрованных букв.\n\
3 - Отображение криптограммы и текста.\n\
4 - Замена букв в криптограмме.\n\
5 - Откат к прошлой версии криптограммы.\n\
6 - Показать рекомендуемые замены.\n\
7 - Выход из программы.\n");
}
// 5 - 1
// 4 - 1
// 1 - 0
// 2 - 0
// 3 - 0
// 6 - 0


//Функция откатов текста
void roll_back(){
    int change;
    printf("Текущий шаг %d, последнее изменение с %lc на %lc\n\
Введите число шагов, на которое нужно вернуться: ", curr_version, last_change[curr_version][0], last_change[curr_version][1]);
    scanf("%d", &change);
    getchar();
    if(change >= curr_version){
        curr_version = 0;
        printf("Текущий шаг: %d\n", curr_version);
        return;
    }
    curr_version -= change;
    printf("Текущий шаг: %d\n", curr_version);
}

// void print_words_by_len(){
//     char words[MAX_WORDS_COUNT][MAX_WORD_LEN];
//     char *piece;
//     piece = strtok()
// }

//Функция выхода и сохранения текущего текста в файл
void save_exit(){
    char choice;
    int i;
    printf("Сохранить текущий вариант текста перед выходом?\n1 - Да\n2 - Нет\n");
    choice = safeInput();
    if (choice == '1'){
        FILE *file_out = fopen("test_out.txt", "w");
        for(i = 0;versions[curr_version][i];i++){
            fputc(versions[curr_version][i], file_out);
        }
        printf("Файл сохранён!\nНажмите enter.");
        getchar();
        return;
    }
    if(choice == '2'){
        printf("Изменения не были сохранены!\nНажмите enter.");
        getchar();
        return;
    }
    else{
        FILE *file_out = fopen("test_out.txt", "w");
        printf("Файл был сохранён.");
    }
}

//Основная функиция замены
void change_letter(){
    wcscpy(versions[curr_version+1],versions[curr_version]);
    curr_version++;
    wchar_t letter_add, letter_remove;
    int i;
    printf("%ls\n\n", versions[curr_version]);
    printf("Через пробел введите сначала букву, которую надо заменить, затем её замену:\n");
    scanf("%lc %lc", &letter_remove, &letter_add);
    for(i = 0; versions[curr_version][i];i++){
        if(versions[curr_version][i] == letter_remove){ 
            versions[curr_version][i] = letter_add - 32;// -32 для получения верхнего регистра
        }
    }
    last_change[curr_version][0] = letter_remove;
    last_change[curr_version][1] = letter_add;
    printf("Нажмите enter, чтобы продолжить");
    getchar();
    getchar();
    
}

//Функция рекомендации замен
void recommend_change(){
    //-1072
    int i;
    int freq[33] = {0};
    int freq_top[33] = {0};
    wchar_t last_change = 0;
    for(i = 0; versions[curr_version][i];i++){
        if(versions[curr_version][i] >= 1072 && versions[curr_version][i] <= 1103){
            freq[versions[curr_version][i] - 1072]++;
        }
        if(versions[curr_version][i] >= 1040 && versions[curr_version][i] <= 1071){
            freq_top[versions[curr_version][i] - 1040]++;
        }
    }
   for(int j = 0; j < 33; j++){
        int max = -1;
        int max_index = -1;
        for(int k = 0; k < 33; k++){
            if(freq[k] > max){
                max = freq[k];
                max_index = k;
            }
        }
        int index_rus_freq = j;
        if(max == 0) continue;
        while(freq_top[rus_freq[index_rus_freq] - 32 - 1040] != 0) index_rus_freq++;
        if(rus_freq[index_rus_freq] == last_change) index_rus_freq++;
        printf("Буква: %lc, частота: %lf, рекомендуемая замена: %lc\n",max_index+1072, (double)max/i, rus_freq[index_rus_freq]);
        freq[max_index] = 0;
        last_change = rus_freq[index_rus_freq];
    }
    getchar();
}

int main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");
    printf("Утилита для крипточастнотного анализа ver.0.1 запущена!\n");
    char choice;
    int i;
    FILE *file_inp = fopen("test_in.txt", "r");
    for (i = 0; (versions[curr_version][i] = fgetwc(file_inp)) != EOF; i++);
	versions[curr_version][i] = '\0';
    fclose(file_inp);
    print_help_message();
    choice = safeInput();
    system("clear");
    while (choice != '7'){
        switch (choice){
        case '1':
            break;
        case '2':
            break;
        case '3':
            printf("%ls\n\nНажмите enter,чтобы продолжить.", versions[curr_version]);
            getchar();
            break;
        case '4':
            change_letter();
            break;
        case '5':
            roll_back();
            break;
        case '6':
            recommend_change();
        default:
            printf("Введён неправильный вариант, пожалуйста,попробуйте ещё раз.\n");
            break;
        }
        system("clear");
        print_help_message();
        choice = safeInput();
    }
    save_exit();
    return 0;
}