#ifndef __ADIA64_H_
#define __ADIA64_H_

typedef struct
{
    /* data */
    int cpu_rate;
    int cpu_temp;
    int mem_rate;
    int mem_use;
}aida64_data_t;


void aida64_monitor_start(char *ip);
void aia64_monitor_stop(void);
int aida64_monitor_isconnect(void);

#endif