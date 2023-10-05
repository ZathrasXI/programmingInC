#include "stdio.h"

int main(void) {
    int start_hour, start_minute, end_hour, end_minute;
    int diff_hour, diff_minute;
    printf("Enter two times: ");
    scanf("%02d:%02d %02d:%02d", &start_hour, &start_minute, &end_hour, &end_minute);

    // Is start time earlier or later than end time? e.g. 19:00 22:00 vs 23:00 04:15
    if (start_hour < end_hour) {
         // When start time is earlier than end time
        // subtract time from end time
        diff_hour = end_hour - start_hour;
        if (end_minute < start_minute) {
            diff_minute = 60 - diff_minute + end_minute;
            diff_hour -= 1;
        } else {
            diff_minute = end_minute - start_minute;
        }
    }
    printf("%02d:%02d\n", diff_hour, diff_minute);
    //} else {
    // When end time is earlier than start time
    // subtract hours away from 24
    // subtract minutes away from 60
    //}
    
}