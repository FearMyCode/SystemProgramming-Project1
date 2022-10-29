//  CITS2002 Project 1 2022
//  Student1:   23268041   Yip , Kang Chau
//gcc -std=c11 -Wall -Werror -o estimatecron estimatecron.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define LONGEST_COMMAND_LINE 20
#define LONGEST_COMMAND_NAME 40
#define LONGEST_CHARACTER_IN_EACH_LINE 100
#define MAX_PROCESSES_AT_SAME_TIME 20
#define arbitrary_num -1

/* Constant number of month declarations */
const char *MONTHS_NAME[] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
const int MONTHS[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const char *WEEKDAYS[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

struct Commands{    
    int minutes;
    int hour;
    int day;
    int month; //MONTHS[month]
    int weekday; //WEEKDAYS[weekday]
    char command_name[LONGEST_COMMAND_NAME];
    int duration;
    int frequencies;
    bool freq_included;
};

struct Commands data[LONGEST_COMMAND_LINE];
int total_commands = 0;

int check_command_month(char month[]){
    int result = 0;
    if (isdigit(month[0])){//check whether the first char is digit
        int size = strlen(month);
        //check validation of each character
        for(int i = 0; i < size; i++){
            if ('0' <= month[i] && month[i] <= '9'){
                continue;
            }else{
                fprintf(stderr, "Invalid month \"%s\".\n", month);
                exit(EXIT_FAILURE);
            }
        }

        int months = atoi(month);
        //check if month is valid
        if(0 <= months && months <= 11){
            result = months;
        }else{
            fprintf(stderr, "Invalid month \"%d\".\n", months);
            exit(EXIT_FAILURE);
        }
    }else{//3-letter lowercase abbreviations
        int is_valid = 0;
        int total_months = sizeof(MONTHS_NAME) / sizeof(MONTHS_NAME[0]);
        for (int months = 0; months < total_months ; months++){
            if (strcmp(month, MONTHS_NAME[months]) == 0){
                is_valid = 1;
                result = months;
            }
        }
        if (is_valid == 0){
            fprintf(stderr, "Invalid month \"%s\".\n", month);
            exit(EXIT_FAILURE);
        }
    }
    return result;
}

void trim_line(char line[])
{
    int i = 0;

    //  LOOP UNTIL WE REACH THE END OF line
    while (line[i] != '\0')
    {
        //  CHECK FOR CARRIAGE-RETURN OR NEWLINE
        if (line[i] == '\r' || line[i] == '\n')
        {
            line[i] = '\0'; // overwrite with nul-byte
            break;
        }
        i = i + 1; // iterate through character array
    }
}

void check_store_mintues(int command_line, char temp[]){
    if (temp[0] == '*'){
        data[command_line].minutes = arbitrary_num;
        //if *@#%
        if (strlen(temp) != 1){
            fprintf(stderr, "Invalid min \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }else{
        int size = strlen(temp);
        //check validation of each character
        for(int i = 0; i < size; i++){
            if (temp[i] >= '0' && temp[i] <= '9'){
                continue;
            }else{
                fprintf(stderr, "Invalid min \"%s\" at command Line %d.\n", temp, command_line);
                exit(EXIT_FAILURE);
            }
        }

        int min = atoi(temp);
        //check if min is valid
        if(0 <= min && min <= 59){
            data[command_line].minutes = min;
        }else{
            fprintf(stderr, "Invalid min \"%d\" at command Line %d.\n", min, command_line);
            exit(EXIT_FAILURE);
        }
        
    }
}

void check_store_hour(int command_line, char temp[]){
    if (temp[0] == '*'){
        data[command_line].hour = arbitrary_num;
        //if *@#%
        if (strlen(temp) != 1){
            fprintf(stderr, "Invalid hour \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }else{
        int size = strlen(temp);
        //check validation of each character
        for(int i = 0; i < size; i++){
            if ('0' <= temp[i] && temp[i] <= '9'){
                continue;
            }else{
                fprintf(stderr, "Invalid hour \"%s\" at command Line %d.\n", temp, command_line);
                exit(EXIT_FAILURE);
            }
        }

        int hr = atoi(temp);
        //check if hour is valid
        if(0 <= hr && hr <= 23){
            data[command_line].hour = hr;
        }else{
            fprintf(stderr, "Invalid hour \"%d\" at command Line %d.\n", hr, command_line);
            exit(EXIT_FAILURE);
        }
    }
}

void check_store_day(int command_line, char temp[]){
    if (temp[0] == '*'){
        data[command_line].day = arbitrary_num;
        //if *@#%
        if (strlen(temp) != 1){
            fprintf(stderr, "Invalid day \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }else{
        int size = strlen(temp);
        //check validation of each character
        for(int i = 0; i < size; i++){
            if ('0' <= temp[i] && temp[i] <= '9'){
                continue;
            }else{
                fprintf(stderr, "Invalid day \"%s\" at command Line %d.\n", temp, command_line);
                exit(EXIT_FAILURE);
            }
        }

        int day = atoi(temp);
        //check if day is valid
        if(1 <= day && day <= 31){
            data[command_line].day = day;
        }else{
            fprintf(stderr, "Invalid day \"%d\" at command Line %d.\n", day, command_line);
            exit(EXIT_FAILURE);
        }
    }
}

void check_store_month(int command_line, char temp[]){
    if (temp[0] == '*'){
        data[command_line].month = arbitrary_num;
        //if *@#%
        if (strlen(temp) != 1){
            fprintf(stderr, "Invalid month \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }else if (isdigit(temp[0])){//check whether the first char is digit
        int size = strlen(temp);
        //check validation of each character
        for(int i = 0; i < size; i++){
            if ('0' <= temp[i] && temp[i] <= '9'){
                continue;
            }else{
                fprintf(stderr, "Invalid month \"%s\" at command Line %d.\n", temp, command_line);
                exit(EXIT_FAILURE);
            }
        }

        int month = atoi(temp);
        //check if month is valid
        if(0 <= month && month <= 11){
            data[command_line].month = month;
        }else{
            fprintf(stderr, "Invalid month \"%d\" at command Line %d.\n", month, command_line);
            exit(EXIT_FAILURE);
        }
    }else{//3-letter lowercase abbreviations
        int is_valid = 0;
        int total_months = sizeof(MONTHS_NAME) / sizeof(MONTHS_NAME[0]);
        for (int months = 0; months < total_months ; months++){
            if (strcmp(temp, MONTHS_NAME[months]) == 0){
                is_valid = 1;
                data[command_line].month = months;
            }
        }
        if (is_valid == 0){
            fprintf(stderr, "Invalid month \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }

    //extra check of day and month
    if (data[command_line].day != arbitrary_num && data[command_line].month != arbitrary_num
        && data[command_line].day > MONTHS[data[command_line].month]){
        fprintf(stderr, "day \"%d\" and month \"%d\" are not matching with calendar at command Line %d.\n"
        , data[command_line].day, data[command_line].month, command_line);
        exit(EXIT_FAILURE);
    }
}

void check_store_weekday(int command_line, char temp[]){
    if (temp[0] == '*'){
        data[command_line].weekday = arbitrary_num;
        //if *@#%
        if (strlen(temp) != 1){
            fprintf(stderr, "Invalid weekday \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }else if (isdigit(temp[0])){//check whether the first char is digit
        int size = strlen(temp);
        //check validation of each character
        for(int i = 0; i < size; i++){
            if ('0' <= temp[i] && temp[i] <= '6'){
                continue;
            }else{
                fprintf(stderr, "Invalid weekday \"%s\" at command Line %d.\n", temp, command_line);
                exit(EXIT_FAILURE);
            }
        }

        int weekday = atoi(temp);
        //check if weekday is valid
        if(0 <= weekday && weekday <= 6){
            data[command_line].weekday = weekday;
        }else{
            fprintf(stderr, "Invalid weekday \"%d\" at command Line %d.\n", weekday, command_line);
            exit(EXIT_FAILURE);
        }
    }else{//3-letter lowercase abbreviations
        int is_valid = 0;
        int total_weekdays = sizeof(WEEKDAYS) / sizeof(WEEKDAYS[0]);
        for (int weekdays = 0; weekdays < total_weekdays ; weekdays++){
            if (strcmp(temp, WEEKDAYS[weekdays]) == 0){
                is_valid = 1;
                data[command_line].weekday = weekdays;
            }
        }
        if (is_valid == 0){
            fprintf(stderr, "Invalid weekday \"%s\" at command Line %d.\n", temp, command_line);
            exit(EXIT_FAILURE);
        }
    }
}

void check_store_command_name(int command_line, char temp[]){
    if(strlen(temp) > LONGEST_COMMAND_NAME){
        fprintf(stderr, "Invalid command_name \"%s\" at command Line %d. Each command name will be at most %d characters long.\n"
        , temp, command_line, LONGEST_COMMAND_NAME);
        exit(EXIT_FAILURE);
    }else{
        strcpy(data[command_line].command_name, temp);
    }
    
}

bool check_duration(int estimate_line, char temp[]){
    bool result = true;
    int size = strlen(temp);
    // check validation of each character
    for (int i = 0; i < size; i++){
        if (temp[i] >= '0' && temp[i] <= '9'){
            continue;
        }else{
            result = false;
            fprintf(stderr, "Invalid duration \"%s\" at estimate line %d", temp, estimate_line);
            exit(EXIT_FAILURE);
        }
    }

    int duration = atoi(temp);
    if (duration <= 0){
        result = false;
        fprintf(stderr, "Invalid duration \"%d\" at estimate line %d", duration, estimate_line);
        exit(EXIT_FAILURE);
    }
    return result;
}

void store_crontab(char line[], int command_line){
    int i = 0;
    int ith_words = 0;
    char temp[LONGEST_COMMAND_NAME + 1];
    int temp_index = 0;

    // printf("%s\n", line);
    //  LOOP UNTIL WE REACH THE END OF line
    while (line[i] != '\0'){
        // reset the temp string
        memset(temp, 0, sizeof(temp));
        temp_index = 0;

        //skip all the while space before first word
        while(line[i] == ' ' || line[i] == '\t'){
            i++;
        }

        while (line[i] != ' '){ 
            if(line[i] == '\t'){
                break;
            }
            temp[temp_index] = line[i];
            temp_index++;

            // break this loop before reaching the end of LINE
            if (line[i + 1] == '\0'){
                break;
            }
            i++;
        }
        // printf("%d\n", strlen(temp));
        printf("the temp string is |%s|\n", temp);
        temp[temp_index] = '\0';

        // printf("the temp index is %d\n", i);
        // printf("switch statement condition is %d\n", ith_words);

        switch (ith_words){
        case 0: // the minute
            check_store_mintues(command_line, temp);
            // printf("minutes is %d\n", data[command_line].minutes);
            break;
        case 1: // the hour
            check_store_hour(command_line, temp);
            // printf("hour is %d\n", data[command_line].hour);
            break;
        case 2: // the day of the month
            check_store_day(command_line, temp);
            // printf("day is %d\n", data[command_line].day);
            break;
        case 3: // the month
            check_store_month(command_line, temp);
            // printf("month is %d\n", data[command_line].month);
            break;
        case 4: // the day of the week
            check_store_weekday(command_line, temp);
            break;
        case 5: // the name of the command
            check_store_command_name(command_line, temp);
            // printf("the command name is %s\n", data[command_line].command_name);
            break;
        }
        ith_words++;

        i++;
    }

    // command lines consist of at least 6 'words'
    if (ith_words != 6){
        fprintf(stderr, "Invalid Command Line %d. It should be 6 arguments.\n", command_line);
        exit(EXIT_FAILURE);
    }
}

void read_crontab_file(char filename[]){
    FILE *fp = fopen(filename, "r");
    // ENSURE THAT WE CAN OPEN (WITH READ-ACCESS) THE FILE
    if(fp == NULL){
        fprintf(stderr, "\"%s\": No such file or directory.\n",filename);
        exit(EXIT_FAILURE);
    }else{
        char command_line[400];

        int command_line_index = 0;
        // READ EACH LINE OF THE FILE
        while (fgets(command_line, sizeof command_line, fp)){
            //  Remove non-command lines
            if (command_line[0] == '#'){
                continue;
            }else if(command_line[0] == '\n'){
                fprintf(stderr, "Invalid command Line '' in %s.\n", filename);
                exit(EXIT_FAILURE);
            }

            if(strlen(command_line) > LONGEST_CHARACTER_IN_EACH_LINE){
                fprintf(stderr, "Invalid command Line %d, each line will be at most %d characters long.\n"
                , command_line_index, LONGEST_CHARACTER_IN_EACH_LINE);
                exit(EXIT_FAILURE);
            }

            trim_line(command_line);
            // printf("each line is %s\n", command_line);

            // store the command in the array
            store_crontab(command_line, command_line_index);
            command_line_index++;

            total_commands++;

            if(command_line_index > LONGEST_COMMAND_LINE){
                fprintf(stderr, "%s contain at most %d non-comment lines.\n", filename, LONGEST_COMMAND_LINE);
                exit(EXIT_FAILURE);
            }
        }

        fclose(fp);
    }
}

void store_estimates(char line[], int estimate_line){
    int i = 0;
    int ith_words = 0;
    char temp[LONGEST_COMMAND_NAME];
    int temp_index = 0;

    char temp_command_name[LONGEST_COMMAND_NAME];
    int duration = 0; 

    // printf("%s\n", line);
    //  LOOP UNTIL WE REACH THE END OF line
    while (line[i] != '\0'){
        // reset the temp string
        memset(temp, 0, sizeof(temp));
        temp_index = 0;

        //skip all the while space before first word or between word
        while(line[i] == ' ' || line[i] == '\t'){
            i++;
        }

        while (line[i] != ' '){
            if(line[i] == '\t'){
                break;
            }
            temp[temp_index] = line[i];
            temp_index++;

            // break this loop before reaching the end of LINE
            if (line[i + 1] == '\0'){
                break;
            }
            i++;
        }

        // printf("the estimates temp string is |%s|\n", temp);
        // printf("the temp index is %d\n", i);
        // printf("switch statement condition is %d\n", ith_words);

        // need to find the duration first before storing into struct
        switch (ith_words){
        case 0:
            strcpy(temp_command_name, temp);
            // printf("the command name is %s\n", temp_command_name);
            break;
        case 1:
            if(check_duration(estimate_line, temp))
                duration = atoi(temp);
            break;
        }
        // move to the next word(in the same command line)
        ith_words++;

        i++;
    }

    // printf("the ith_words is %d\n", ith_words);
    //sotre duration after find both value
    //iterate through the commands to find the command's strict using command_name
    if(ith_words == 2){
        int c = 0;
        while (c < total_commands){
            if (strcmp(temp_command_name, data[c].command_name) == 0){
                data[c].duration = duration;
                // printf("duration is %d\n", duration);
                // printf("temp_command_name is %s\n", temp_command_name);
            }
            c++;
        }
    }else{
        fprintf(stderr, "Invalid Estimate Line %d. It should be 2 arguments.\n", estimate_line);
        exit(EXIT_FAILURE);
    }   
}

void read_estimates_file(char filename[]){
    FILE *fp = fopen(filename, "r");
    // ENSURE THAT WE CAN OPEN (WITH READ-ACCESS) THE FILE
    if(fp == NULL){
        fprintf(stderr, "\"%s\": No such file or directory.\n",filename);
        exit(EXIT_FAILURE);
    }else{
        char estimate_line[400];
        int estimate_line_index = 0;
        // READ EACH LINE OF THE FILE
        while (fgets(estimate_line, sizeof estimate_line, fp))
        {
            //  Remove non-command lines
            if (estimate_line[0] == '#'){
                continue;
            }else if(estimate_line[0] == '\n'){
                fprintf(stderr, "Invalid Estimate line '' in %s.\n", filename);
                exit(EXIT_FAILURE);
            }

            if(strlen(estimate_line) > LONGEST_CHARACTER_IN_EACH_LINE){
                fprintf(stderr, "Invalid Estimate Line %d, each line will be at most %d characters long.\n"
                , estimate_line_index, LONGEST_CHARACTER_IN_EACH_LINE);
                exit(EXIT_FAILURE);
            }

            trim_line(estimate_line);

            // printf("the wholle command line is %s\n", estimate_line);
            // store the command in the array
            store_estimates(estimate_line, estimate_line_index);
            estimate_line_index++;

            if(estimate_line_index > LONGEST_COMMAND_LINE){
                fprintf(stderr, "%s contain at most %d non-comment lines.\n", filename, LONGEST_COMMAND_LINE);
                exit(EXIT_FAILURE);
            }
        }

        fclose(fp);
    }
}

void check_crontab_file_command(){
    for(int command_line = 0; command_line < total_commands; command_line++){
        //command's duration is 0 means the duration is not assigned 
        if(data[command_line].duration == 0){
            fprintf(stderr, "Unknown command name \"%s\" at command Line %d.\n"
            , data[command_line].command_name, command_line);
            exit(EXIT_FAILURE);
        }
    }
}

//  written by Chris.McDonald@uwa.edu.au
//  RETURNS  0=Sun, 1=Mon, .....
int first_day_of_month(int month, int year)
{
    struct tm   tm;

//  SET ALL FIELDS OF tm TO ZERO TO MAKE SOME FIELDS 'UNKNOWN'
    memset(&tm, 0, sizeof(tm));

//  INITIALIZE THE FILEDS THAT WE ALREADY KNOW
    tm.tm_mday  = 1;
    tm.tm_mon   = month-1;              // 0=Jan, 1=Feb, ....
    tm.tm_year  = year-1900;

//  ASK THE POSIX FUNCTION mktime TO DETERMINE THE 'UNKNOWN' FIELDS
//  See: https://pubs.opengroup.org/onlinepubs/9699919799/
    mktime(&tm);

//  RETURN THE INTEGER MONTH VALUE
    return tm.tm_wday;                  // 0=Sun, 1=Mon, .....
}

bool is_same_min(int command_lines, int weekday, int month, int day, int hour, int min){
    return (data[command_lines].weekday == arbitrary_num || data[command_lines].weekday == weekday) 
        && (data[command_lines].month == arbitrary_num || data[command_lines].month == month) 
        && (data[command_lines].day == arbitrary_num || data[command_lines].day == day)
        && (data[command_lines].hour == arbitrary_num || data[command_lines].hour == hour)
        && (data[command_lines].minutes == arbitrary_num || data[command_lines].minutes == min);
}

int find_max_running(int month){
    //example
    //month = 0, Jan
    /*
        January 2022
    0  1  2  3  4  5  6    
    Su Mo Tu We Th Fr Sa
                      1
    2  3  4  5  6  7  8
    9  10 11 12 13 14 15
    16 17 18 19 20 21 22
    23 24 25 26 27 28 29
    30 31
    */

    int days = MONTHS[month];
    int weekday = first_day_of_month(month + 1 ,2022); // first day of month method()
    // printf("weekday is %d\n", weekday);

    int max_mins_in_a_month = 24 * 60*days;
    int time_line[max_mins_in_a_month];
    for (int t = 0; t < max_mins_in_a_month; t++){
        time_line[t] = 0;
    }

    //int command_lines = 0;

    int max_running = 0;

    int max_hours = 23;
    int max_mins = 59;
    //check every minutes
    for(int day = 1; day <= days; day++){
        weekday = weekday % 7;
        for(int hour = 0; hour <= max_hours; hour++){
            for(int min = 0; min <= max_mins; min++){
                // iterate through all the commands
                for(int command_lines = 0; command_lines < total_commands; command_lines++){
                    // check whether this command is executed on this specific day
                    // int command_month = data[command_lines].month;
                    // int command_day = data[command_lines].day;
                    // int command_hour = data[command_lines].hour;
                    // int command_min = data[command_lines].minutes;
                    if(is_same_min(command_lines, weekday, month, day, hour, min)){ 
                        // store the mins in timeline
                        data[command_lines].frequencies++;

                        int start_time = (day-1)*24*60 + hour*60 + min;
                        int duration = data[command_lines].duration;
                        int end_time = start_time + duration; // or do it in data[command_lines].end_time
                        if(end_time > max_mins_in_a_month){
                            end_time = max_mins_in_a_month;//end time stop at max
                        }

                        for(int d = start_time; d < end_time; d++){//if duration is over them moth,need to terminate
                            if(time_line[d] == MAX_PROCESSES_AT_SAME_TIME){
                                //if exceed max, decrement to not count the command
                                data[command_lines].frequencies--;
                                //do not update anything
                                // printf("%s\n", data[command_lines].command_name);
                                // printf("time is %d\n", d);
                                // fprintf(stderr,"Cannot invole new process, max=%d.\n", MAX_PROCESSES_AT_SAME_TIME);
                                break;
                            }else{
                                time_line[d] += 1;
                                if (time_line[d] >= max_running){
                                    max_running = time_line[d];
                                    
                                    // maybe store in global:
                                    // int max_running_time_min = d;
                                }
                                
                            }
                        }
                    }
                }
            }
        }
        weekday++;
        //command_lines = 0; //reseet command_lines so it can iterate again
    }

    return max_running;
}

char * find_most_freq(char *max_name){
    printf("\n");
    printf("Command Invocations:\n");
    int max = data[0].frequencies;
    max_name = data[0].command_name;
    int size = strlen(data[0].command_name);
    max_name[size] = '\0';

    //start finding the same name from 0
    for(int command_line = 0; command_line < total_commands; command_line++){
        if(data[command_line].freq_included == true){
            continue;
        }
        //initialize every time for each command
        int total_freq = 0;
        total_freq += data[command_line].frequencies;
        data[command_line].freq_included = true;

        //check if there is any other same command name in the data
        for(int x = command_line + 1; x < total_commands; x++){
            if(strcmp(data[command_line].command_name, data[x].command_name) == 0){
                total_freq += data[x].frequencies;
                data[x].freq_included = true;
            }
        }

        printf("Command name is %s, total total frequencies are %d.\n", data[command_line].command_name, total_freq);

        //compare with max
        if(total_freq > max){
            max = total_freq;
            max_name = data[command_line].command_name;
            int size = strlen(data[command_line].command_name);
            max_name[size] = '\0';
        }
    }
    return max_name;

}

int cal_totoal_running(){
    int total_invoked = 0;
    for(int command_line = 0; command_line < total_commands; command_line++){
        total_invoked += data[command_line].frequencies;
    }
    return total_invoked;
}

int main(int argc, char *argv[]){
    //  ENSURE WE HAVE SUFFICIENT ARGUMENTS
    if (argc != 4){
        fprintf(stderr, "Usage : %s month crontab-file estimates-file.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int month = check_command_month(argv[1]);
    
    read_crontab_file(argv[2]);
    read_estimates_file(argv[3]);
    check_crontab_file_command();

    int max_running = 0;
    max_running = find_max_running(month);

    // print all details of the structs of Commands
    printf("minutes, hour, day, month, weekday, command name, duration, frequencies\n");
    int i = 0;
    while (i < total_commands){
        printf("%d, %d, %d, %d, %d, %s, %d, %d\n", data[i].minutes, data[i].hour, 
        data[i].day, data[i].month, data[i].weekday, data[i].command_name, 
        data[i].duration, data[i].frequencies);
        i++;
    }

    char *max_name = data[0].command_name;
    max_name = find_most_freq(max_name);

    int total_invoked = cal_totoal_running();

    printf("\n");
    printf("The name of the most frequently executed command is %s.\n", max_name);
    printf("Total commands run in month %d is %d.\n", month, total_invoked);
    printf("max running commands at the same time is %d.\n", max_running);

    printf("\n");
    printf("%s %d %d\n", max_name, total_invoked, max_running);

    return EXIT_SUCCESS;
}