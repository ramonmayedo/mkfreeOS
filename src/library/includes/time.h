#ifndef TIME_H
#define TIME_H


typedef struct tm {
    int tm_sec;    /* Seconds. [0-60] (1 leap second) */
    int tm_min;    /* Minutes. [0-59] */
    int tm_hour;   /* Hours.   [0-23] */
    int tm_mday;   /* Day.     [1-31] */
    int tm_mon;    /* Month.   [0-11] */
    int tm_year;   /* Year [1970; ...] */
    int tm_wday;   /* Day of week. [0-6], 0=Sunday */
    int tm_yday;   /* Days in year. [0-365] */
    int tm_isdst;  /* Daylight saving [-1/0/1] */
};

typedef struct stime {
    int sec;    /* Seconds. [0-60] (1 leap second) */
    int min;    /* Minutes. [0-59] */
    int hour;   /* Hours.   [0-23] */
    int msec;   /* Mili Seconds*/
};

int time(int* timer);
void gmtime(const int timer,stime *time);
#endif

