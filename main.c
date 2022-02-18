#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include "config.h"

const wchar_t* rus_freq = L"оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъё";

wchar_t versions[MAX_BACK_UPS][MAX_LEN] = {'\0'};
int curr_version = 0;
wchar_t last_change[MAX_BACK_UPS][2] = {L'\0'};

char safe_input();
wchar_t safe_input_wchar();
int amount_uncrypt_letters(const wchar_t *word);
int comp_w(const void *val1, const void *val2);
int comp_w_uncrypt(const void *val1, const void *val2);
int is_letter(const wchar_t word);
void print_help_message();
void print_words_by_len();
void print_words_by_uncrypt_len();
void save_exit();
void change_letter();
void recomend_change();
void roll_back();
void auto_change();
void clean_stdin();

int main(){
    char choice;
    int i;
    setlocale(LC_ALL, "ru_RU.UTF-8");
    printf("Утилита для крипточастнотного анализа ver.0.1 запущена!\n");
    FILE *file_inp = fopen("test_in.txt", "r");
    for (i = 0; (versions[curr_version][i] = fgetwc(file_inp)) != EOF; i++);
	versions[curr_version][i] = '\0';
    fclose(file_inp);
    print_help_message();
    choice = safe_input();
    system("clear");
    while (choice != '8'){
        switch (choice){
        case '1':
            print_words_by_len();
            printf("\nНажмите enter, чтобы продолжить.");
            getchar();
            break;
        case '2':
            print_words_by_uncrypt_len();
            printf("\nНажмите enter, чтобы продолжить.");
            getchar();
            break;
        case '3':
            printf("%ls\n", versions[curr_version]);
            printf("\n| Было  | Стало |\n");
            printf("|-------|-------|\n");
            if(curr_version != 0){
                for(int i = 1; i <= curr_version; i++){
                    printf("|   %lc   |   %lc   |\n", last_change[i][0], last_change[i][1]);
                }
            }
            printf("|-------|-------|\n");
            printf("Нажмите enter, чтобы продолжить.");
            getchar();
            break;
        case '4':
            change_letter();
            break;
        case '5':
            roll_back();
            break;
        case '6':
            recomend_change();
            getchar();
            break;
        case '7':
            auto_change();
            getchar();
            break;
        default:
            printf("Введён неправильный вариант, пожалуйста,попробуйте ещё раз.\nНажмите enter, чтобы продолжить.");
            getchar();
            break;
        }
        system("clear");
        print_help_message();
        choice = safe_input();
    }
    save_exit();
    return 0;
}


void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

char safe_input() {
    char input[200] = {'\0'};
    fgets(input, sizeof(input), stdin);
    if(input[1] != 10){
        return 0;
    }
    
    return input[0];
}

wchar_t safe_input_wchar(){
    wchar_t input[200] = {'\0'};
    fgetws(input, sizeof(input), stdin);
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
7 - Автоматическая замена, на основе крипточастотного анализа.\n\
8 - Выход из программы.\n");
}

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

int amount_uncrypt_letters(const wchar_t *word){
    int count = 0;
    for(int i = 0; word[i] != '\0';i++){
        if(word[i] >= 1072 && word[i] <= 1103){
            count++;
        }
    }
    return count;
}

int comp_w(const void *val1, const void *val2){
    const wchar_t *x1 = val1;
    const wchar_t *x2 = val2;
    return (wcslen(x2) - wcslen(x1));
}

int comp_w_uncrypt(const void *val1, const void *val2){
    const wchar_t *x1 = val1;
    const wchar_t *x2 = val2;
    return (amount_uncrypt_letters(x2) - amount_uncrypt_letters(x1));
}

void print_words_by_len(){
    int i;
    wchar_t words[MAX_WORDS_COUNT][MAX_WORD_LEN];
    wchar_t line[MAX_LEN];
    wchar_t *piece, *pt;
    wcpcpy(line, versions[curr_version]);
    piece = wcstok(line, L" ,.-;()/\"\n", &pt);
    for (i = 0; piece != NULL;i++){
        wcpcpy(words[i], piece);
        piece = wcstok(NULL, L" ,.-;()/\"\n", &pt);
	}
    qsort(words, i, sizeof(wchar_t)*MAX_WORD_LEN , comp_w);
    i = 0;
    while(words[i][0] != '\0'){
        printf("%ls, ", words[i]);
        i++;
    }
}

void print_words_by_uncrypt_len(){
    int i;
    wchar_t words[MAX_WORDS_COUNT][MAX_WORD_LEN];
    wchar_t line[MAX_LEN];
    wchar_t *piece, *pt;
    wcpcpy(line, versions[curr_version]);
    piece = wcstok(line, L" ,.-;()/\"\n", &pt);
    for (i = 0; piece != NULL;i++){
        wcpcpy(words[i], piece);
        piece = wcstok(NULL, L" ,.-;()/\"\n", &pt);
    }
    qsort(words, i, sizeof(wchar_t)*MAX_WORD_LEN , comp_w_uncrypt);
    i = 0;
    while(words[i][0] != '\0'){
        printf("%ls, ", words[i]);
        i++;
    }

}

void save_exit(){
    char choice;
    int i;
    printf("Сохранить текущий вариант текста перед выходом?\n1 - Да\n2 - Нет\n");
    choice = safe_input();
    if (choice == '1'){
        FILE *file_out = fopen("test_out.txt", "w");
        for(i = 0;versions[curr_version][i];i++){
            fputwc(versions[curr_version][i], file_out);
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
        for(i = 0;versions[curr_version][i];i++){
            fputwc(versions[curr_version][i], file_out);
        }
        fclose(file_out);
        printf("Файл был сохранён.");
        return;
    }
}

void change_letter(){
    wchar_t letter_add, letter_remove;
    int i;
    wcscpy(versions[curr_version+1],versions[curr_version]);
    printf("%ls\n\n", versions[curr_version]);
    printf("Через пробел введите сначала букву, которую надо заменить, затем её замену или \"0 0 \"для отмены:\n");
    // clean_stdin();
    scanf("%lc %lc", &letter_remove, &letter_add);
    if(letter_add == 48 && letter_remove == 48){
        return;
    }
    if(!is_letter(letter_add) || !is_letter(letter_remove)){
        printf("Замена не случилась :(\n");
        printf("Нажмите enter, чтобы продолжить");
        getchar();
        getchar();
        return;
        }
    for(i = 0; versions[curr_version][i];i++){
        if(versions[curr_version][i] == letter_remove){ 
            versions[curr_version][i] = letter_add - 32;// -32 для получения верхнего регистра
        }
    }
    last_change[curr_version][0] = letter_remove;
    last_change[curr_version][1] = letter_add;
    curr_version++;
    printf("Нажмите enter, чтобы продолжить");
    getchar();
    getchar();
    
}

void auto_change(){
    int i, z;
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
        for(z = 0; versions[curr_version][z];z++){
            
            if(versions[curr_version][z] == max_index+1072){ 
                versions[curr_version][z] = rus_freq[index_rus_freq] - 32;// -32 для получения верхнего регистра
            }
        }
        freq[max_index] = 0;
        last_change = rus_freq[index_rus_freq];
        }
        printf("%ls\nНажмите enter, чтобы продолжить.", versions[curr_version]);
}

void recomend_change(){
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
        //printf("Буква: %lc, частота: %lf, рекомендуемая замена: %lc\n",max_index+1072, (double)max/i, rus_freq[index_rus_freq]);
        freq[max_index] = 0;
        last_change = rus_freq[index_rus_freq];
    }
    getchar();
}

int is_letter(const wchar_t letter){
    if(letter >= 1040 && letter <= 1103){
        return 1;
    }
    return 0;
}