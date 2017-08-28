#include <iostream>
#include <sstream>
#include <iomanip>
#include <windows.h>

#ifdef _MSC_VER  
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  
#pragma comment( linker, "/subsystem:\"console\" /entry:\"wWinMainCRTStartup\"" )  
#endif  

#define SCHEDULE_DAY_60 0
#define SCHEDULE_DAY_75 1


void PrintSchedule() {

    char* task_name[] = {
#if SCHEDULE_DAY_60
        "F1|01A-05A|", "F1|06A-10A|", "F1|11A-15A|", "F1|16A-20A|",
        "F2|21A-25A|", "F2|26A-30A|", "F3|31A-35A|", "F2|36A-40A|",
        "F3|41A-45A|", "F3|46A-50A|", "F3|51A-55A|", "F3|56A-60A|",
        "F1|01C-05C|", "F1|06C-10C|", "F1|11C-15C|", "F1|16C-20C|",
        "F1|01B-05B|", "F1|06B-10B|", "F1|11B-15B|", "F1|16B-20B|",
        "F2|21A-25A|", "F2|26A-30A|", "F2|31A-35A|", "F2|36A-40A|",
        "F3|41A-45A|", "F3|46A-50A|", "F3|51A-55A|", "F3|56A-60A|",
        "F2|21C-25C|", "F2|26C-30C|", "F2|31C-35C|", "F2|36C-40C|",
        "F1|01B-05B|", "F1|06B-10B|", "F1|11B-15B|", "F1|16B-20B|",
        "F2|21B-25B|", "F2|26B-30B|", "F2|31B-35B|", "F2|36B-40B|",
        "F3|41A-45A|", "F3|46A-50A|", "F3|51A-55A|", "F3|56A-60A|",
        "F3|41C-45C|", "F3|46C-50C|", "F3|51C-55C|", "F3|56C-60C|",
        "F1|01B-05B|", "F1|06B-10B|", "F1|11B-15B|", "F1|16B-20B|",
        "F2|21B-25B|", "F2|26B-30B|", "F2|31B-35B|", "F2|36B-40B|",
        "F3|41B-45B|", "F3|46B-50B|", "F3|51B-55B|", "F3|56B-60B|",
#else//schedule day 75
        "F1|01A-04A|", "F1|05A-08A|", "F1|09A-12A|", "F1|13A-16A|", "F1|17A-20A|",
        "F2|21A-24A|", "F2|25A-28A|", "F2|29A-32A|", "F2|33A-36A|", "F2|37A-40A|",
        "F3|41A-44A|", "F3|45A-48A|", "F3|49A-52A|", "F3|53A-56A|", "F3|57A-60A|",
        "F1|01C-04C|", "F1|05C-08C|", "F1|09C-12C|", "F1|13C-16C|", "F1|17C-20C|",
        "F1|01B-04B|", "F1|05B-08B|", "F1|09B-12B|", "F1|13B-16B|", "F1|17B-20B|",
        "F2|21A-24A|", "F2|25A-28A|", "F2|29A-32A|", "F2|33A-36A|", "F2|37A-40A|",
        "F3|41A-44A|", "F3|45A-48A|", "F3|49A-52A|", "F3|53A-56A|", "F3|57A-60A|",
        "F2|21C-24C|", "F2|25C-28C|", "F2|29C-32C|", "F2|33C-36C|", "F2|37C-40C|",
        "F1|01B-04B|", "F1|05B-08B|", "F1|09B-12B|", "F1|13B-16B|", "F1|17B-20B|",
        "F2|21B-24B|", "F2|25B-28B|", "F2|29B-32B|", "F2|33B-36B|", "F2|37B-40B|",
        "F3|41A-44A|", "F3|45A-48A|", "F3|49A-52A|", "F3|53A-56A|", "F3|57A-60A|",
        "F3|41C-44C|", "F3|45C-48C|", "F3|49C-52C|", "F3|53C-56C|", "F3|57C-60C|",
        "F1|01B-04B|", "F1|05B-08B|", "F1|09B-12B|", "F1|13B-16B|", "F1|17B-20B|",
        "F2|21B-24B|", "F2|25B-28B|", "F2|29B-32B|", "F2|33B-36B|", "F2|37B-40B|",
        "F3|41B-44B|", "F3|45B-48B|", "F3|49B-52B|", "F3|53B-56B|", "F3|57B-60B|",
#endif
        " "
    };
    char* sr_task_name[] = {
        //1,2,3
        //1,1,2,3
        //2,1,2,3
        //3,1,2,3

#if SCHEDULE_DAY_60
        "SR|01-034|", "SR|35-058|", "SR|59-081|", "SR|86-104|",
        "SR|01-027|", "SR|28-053|", "SR|54-079|", "SR|80-104|",
        "SR|01-025|", "SR|26-055|", "SR|56-081|", "SR|82-104|",

        "SR|01-034|", "SR|35-058|", "SR|59-081|", "SR|86-104|",
        "SR|01-034|", "SR|35-058|", "SR|59-081|", "SR|86-104|",
        "SR|01-027|", "SR|28-053|", "SR|54-079|", "SR|80-104|",
        "SR|01-025|", "SR|26-055|", "SR|56-081|", "SR|82-104|",

        "SR|01-027|", "SR|28-053|", "SR|54-079|", "SR|80-104|",
        "SR|01-034|", "SR|35-058|", "SR|59-081|", "SR|86-104|",
        "SR|01-027|", "SR|28-053|", "SR|54-079|", "SR|80-104|",
        "SR|01-025|", "SR|26-055|", "SR|56-081|", "SR|82-104|",

        "SR|01-025|", "SR|26-055|", "SR|56-081|", "SR|82-104|",
        "SR|01-034|", "SR|35-058|", "SR|59-081|", "SR|86-104|",
        "SR|01-027|", "SR|28-053|", "SR|54-079|", "SR|80-104|",
        "SR|01-025|", "SR|26-055|", "SR|56-081|", "SR|82-104|",
#else//schedule day 75
        "SR|01-029|", "SR|30-049|", "SR|50-067|", "SR|68-086|", "SR|87-104|",
        "SR|01-021|", "SR|22-043|", "SR|44-064|", "SR|65-084|", "SR|85-104|",
        "SR|01-019|", "SR|20-044|", "SR|45-066|", "SR|67-086|", "SR|87-104|",

        "SR|01-029|", "SR|30-049|", "SR|50-067|", "SR|68-086|", "SR|87-104|",
        "SR|01-029|", "SR|30-049|", "SR|50-067|", "SR|68-086|", "SR|87-104|",
        "SR|01-021|", "SR|22-043|", "SR|44-064|", "SR|65-084|", "SR|85-104|",
        "SR|01-019|", "SR|20-044|", "SR|45-066|", "SR|67-086|", "SR|87-104|",

        "SR|01-021|", "SR|22-043|", "SR|44-064|", "SR|65-084|", "SR|85-104|",
        "SR|01-029|", "SR|30-049|", "SR|50-067|", "SR|68-086|", "SR|87-104|",
        "SR|01-021|", "SR|22-043|", "SR|44-064|", "SR|65-084|", "SR|85-104|",
        "SR|01-019|", "SR|20-044|", "SR|45-066|", "SR|67-086|", "SR|87-104|",

        "SR|01-019|", "SR|20-044|", "SR|45-066|", "SR|67-086|", "SR|87-104|",
        "SR|01-029|", "SR|30-049|", "SR|50-067|", "SR|68-086|", "SR|87-104|",
        "SR|01-021|", "SR|22-043|", "SR|44-064|", "SR|65-084|", "SR|85-104|",
        "SR|01-019|", "SR|20-044|", "SR|45-066|", "SR|67-086|", "SR|87-104|",
#endif
        " "
    };

    int task_count = sizeof(task_name) / sizeof(char*);

    //    int repeat_date[] = {1, 2, 4, 7, 15, 31};
    int repeat_date[] = { 1, 4, 7, 15, 31 };
    int repeat_count = sizeof(repeat_date) / sizeof(int);

    int finish_days = repeat_date[repeat_count - 1]
        - repeat_date[0]
        + task_count;

    auto get_task_name = [&task_name, &sr_task_name](int cur_date,
        int task_start_date,
        int task_end_date)->std::string {
        if (cur_date < task_start_date)
            return std::string(" ");
        else if ((cur_date - task_start_date) < task_end_date) {
            int task_index = cur_date - task_start_date;
            if (task_name[task_index] != " ") {
                std::ostringstream os;
                os << task_name[task_index] << "_" << sr_task_name[task_index];
                return os.str();
            }
        }

        return std::string(" ");
        int task_index = ((cur_date - task_start_date) == 0) ? 0 :
            cur_date - task_start_date;

        std::ostringstream os;
        os << task_name[task_index] << "_" << sr_task_name[task_index];
        return os.str();
    };

    std::cout << std::endl;

    for (int i = 1; i < finish_days; i++) {
        std::ostringstream os;
        os << "Day_" << std::to_string(i) << ":   ";
        std::cout << std::setiosflags(std::ios::left);
        std::cout << " " << std::setw(12) << os.str();
        for (int j = 0; j < repeat_count; j++) {
            std::cout << std::setw(24) << get_task_name(i, repeat_date[j], task_count);
        }
        std::cout << std::endl;
    }

}

void PrintTask() {

    char* fluency_index[] = {
        "F1", "F2", "F3",
        "F1", "F1", "F2", "F3",
        "F2", "F1", "F2", "F3",
        "F3", "F1", "F2", "F3"
    };
    char* file_abc_index[] = {
        "A", "A", "A", "C", "B",
        "A", "A", "C", "B", "B",
        "A", "C", "B", "B", "B"
    };

    int group_count = sizeof(fluency_index) / sizeof(char*);
    int day_index = 1;
    for (int i = 0; i < group_count; i++) {

        int start_fluency_index = 1;
        if (fluency_index[i][1] == '2') {
            start_fluency_index = 21;
        } else if (fluency_index[i][1] == '3') {
            start_fluency_index = 41;
        }

        int end_fluency_index = start_fluency_index + 19;
        for (int j = start_fluency_index; j <= end_fluency_index; j++) {
            if (j % 4 == 0) {
                std::ostringstream day_index_os, start_os, end_os;
                day_index_os << std::setfill(' ') << std::setw(12) << "y_" << std::to_string(day_index++) << ": ";
                start_os << std::setfill('0') << std::setw(2) << std::to_string(j - 3);
                end_os << std::setfill('0') << std::setw(2) << std::to_string(j);
                std::cout << day_index_os.str() << fluency_index[i] << "-"
                    << start_os.str() << file_abc_index[i] << "-"
                    << end_os.str() << file_abc_index[i] << " "
                    << std::endl;
            }
            //            std::cout << fluency_index[i] <<"_U" << os.str() << "_" << file_abc_index[i] << std::endl;

            //            std::ostringstream os;
            //            os << std::setfill ('0') << std::setw (4) << std::to_string( (j-1)*50+1 );
            //            std::cout << fluency_index[i] << "-GMS-" << file_abc_index[i] << "-" << os.str() << std::endl;
        }
    }
}

int APIENTRY wWinMain(
    HINSTANCE, HINSTANCE, wchar_t*, int) {

    std::cout << std::endl;

    //PrintTask();
    PrintSchedule();

    std::cout << std::endl;
    system("pause");
    return 0;

}


////1 == =
//
//| 01 - 008 | | 09 - 016 | | 17 - 022 | | 23 - 029 |
//| 30 - 034 | | 35 - 039 | | 40 - 044 | | 45 - 049 |
//| 50 - 054 | | 55 - 058 | | 59 - 062 | | 63 - 067 |
//| 68 - 072 | | 73 - 077 | | 78 - 081 | | 82 - 086 |
//| 87 - 090 | | 91 - 094 | | 95 - 098 | | 99 - 104 |
////
////2 == ==
//| 01 - 005 | | 06 - 010 | | 11 - 016 | | 17 - 021 |
//| 22 - 027 | | 28 - 033 | | 34 - 038 | | 39 - 043 |
//| 44 - 048 | | 49 - 053 | | 54 - 059 | | 60 - 064 |
//| 65 - 069 | | 70 - 074 | | 75 - 079 | | 80 - 084 |
//| 85 - 088 | | 89 - 092 | | 93 - 097 | | 98 - 104 |
////
////
////3 == == =
////
//| 01 - 004 | | 05 - 009 | | 10 - 014 | | 15 - 019 |
//| 20 - 025 | | 26 - 032 | | 33 - 038 | | 39 - 044 |
//| 45 - 050 | | 51 - 055 | | 56 - 060 | | 61 - 066 |
//| 67 - 071 | | 72 - 076 | | 77 - 081 | | 82 - 086 |
//| 87 - 091 | | 92 - 095 | | 96 - 099 | | 100 - 104 |

//2,3     1,4     5       6       7,8
//9,10    11,12   13,14   15      16
//17,18   19      20      21      22
//23,24   25,26   27      28      29
//30      31      32      33      34
//
//35      36      37      38      39
//40      41      42      43      44
//45      46      47      48      49
//50      51      52      53      54
//55      56      57      58
//
//59      60      61      62
//63      64      65      66      67
//68      68      70      71      72
//73      74      75      76      77
//78      79      80      81
//
//82      83      84      85      86
//87      88      89      90
//91      92      93      94
//95      96      97      98
//99      100     101     102     103,104

////// ////// ////// //////

//1       2       3       4       5
//6       7       8       9       10
//11      12      13      14      15,16
//17      18      19      20      21
//22      23      24      25      26,27
//
//28      29      30      31      32,33
//34      35      36      37      38
//39      40      41      43      43
//49      50      51      52      53
//54      55      56      57      58,59
//
//60      61      62      63      64
//65      66      67      68      69
//70      71      72      73      74
//75      76      77      78      79
//80      81      82      83      84
//
//85      86      87      88
//89      90      91      92
//93      94      95      96      97
//98      99      100     101     102,103,104

////// ////// ////// //////

//1       2       3       4
//5       6       7       8       9
//10      11      12      13      14
//15      16      17      18      19
//20      21      22      23      24,25
//
//26      27      28      29,30   31,32
//33      34      35      36      37,38
//39      40      41      42      43,44
//45      46      47      48      49,50
//51      52      53      54      55
//
//56      57      58      59      60
//61      62      63      64      65,66
//67      68      69      70      71
//72      73      74      75      76
//77      78      79      80      81
//
//82      83      84      85      86
//87      88      89      90      91
//92      93      94      95
//96      97      98      99
//100     101     102     103     104


        //            "F1_U01_SR_01-008", "F1_U02_SR_09-016", "F1_U03_SR_17-022", "F1_U04_SR_23-029", "F1_U05_SR_30-034",
        //            "F1_U06_SR_35-039", "F1_U07_SR_40-044", "F1_U08_SR_45-049", "F1_U09_SR_50-054", "F1_U10_SR_55-058",
        //            "F1_U11_SR_59-062", "F1_U12_SR_63-067", "F1_U13_SR_68-072", "F1_U14_SR_73-077", "F1_U15_SR_78-081",
        //            "F1_U16_SR_82-086", "F1_U17_SR_87-090", "F1_U18_SR_91-094", "F1_U19_SR_95-098", "F1_U20_SR_99-104",
        //
        //            "F2_U01_SR_01-005", "F2_U02_SR_06-010", "F2_U03_SR_11-016", "F2_U04_SR_17-021", "F2_U05_SR_22-027",
        //            "F2_U06_SR_28-033", "F2_U07_SR_34-038", "F2_U08_SR_39-043", "F2_U09_SR_44-048", "F2_U10_SR_49-053",
        //            "F2_U11_SR_54-059", "F2_U12_SR_60-064", "F2_U13_SR_65-069", "F2_U14_SR_70-074", "F2_U15_SR_75-079",
        //            "F2_U16_SR_80-084", "F2_U17_SR_85-088", "F2_U18_SR_89-092", "F2_U19_SR_93-097", "F2_U20_SR_98-104",
        //
        //            "F3_U01_SR_01-004", "F3_U02_SR_05-009", "F3_U03_SR_10-014", "F3_U04_SR_15-019", "F3_U05_SR_20-025",
        //            "F3_U06_SR_26-032", "F3_U07_SR_33-038", "F3_U08_SR_39-044", "F3_U09_SR_45-050", "F3_U10_SR_51-055",
        //            "F3_U10_SR_56-060", "F3_U10_SR_61-066", "F3_U10_SR_67-071", "F3_U10_SR_72-076", "F3_U10_SR_77-081",
        //            "F3_U10_SR_82-086", "F3_U10_SR_87-091", "F3_U10_SR_92-095", "F3_U10_SR_96-099", "F3_U20_SR_00-104",