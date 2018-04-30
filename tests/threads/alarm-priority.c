/* Checks that when the alarm clock wakes up threads, the
 higher-priority threads run first. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

static thread_func alarm_priority_thread;
static struct semaphore wait_sema;

void
test_alarm_priority (void)
{
    /* This test does not work with the MLFQS. */
    ASSERT (!thread_mlfqs);
    
    sema_init (&wait_sema, 0);
    
    for (int i = 0; i < 10; i++)
    {
        int priority = PRI_DEFAULT - (i + 5) % 10 - 1;
        char name[16];
        snprintf (name, sizeof name, "priority %d", priority);
//        printf("[alarm-priority.c] priority %d created\n", priority);
        thread_create (name, priority, alarm_priority_thread, NULL);
    }
    
//    printf ("\nbegin\n");
//    printf("priority of main: %d\n", thread_get_priority());
//    thread_set_priority(PRI_MAX);
    for (int i = 0; i < 10; i++)
        sema_up (&wait_sema);
    
    timer_sleep(2000);
//    printf ("\nend\n");
}
//
static void
alarm_priority_thread (void *aux UNUSED)
{
    /* Now we know we're at the very beginning of a timer tick, so
     we can call timer_sleep() without worrying about races
     between checking the time and a timer interrupt. */


//    msg("hello\n");
//    printf("Thread %d woke up.\n", thread_get_priority());
    sema_down (&wait_sema);
//    if(strcmp(thread_name(), "priority 28") == 0) while(1);
    msg ("Thread %s woke up.", thread_name());
    /* Print a message on wake-up. */
//    struct thread *cur = thread_current();
    int64_t t = timer_ticks ();
    for (int i = 0; i < 30; ) {
        if (timer_ticks () != t) {
            i++, t = timer_ticks ();
        }
    }
}

//
//static void
//alarm_priority_thread (void *aux UNUSED)
//{
//    /* Busy-wait until the current time changes. */
//    int64_t start_time = timer_ticks ();
//    while (timer_elapsed (start_time) == 0)
//        continue;
//
//    /* Now we know we're at the very beginning of a timer tick, so
//       we can call timer_sleep() without worrying about races
//       between checking the time and a timer interrupt. */
//    timer_sleep (wake_time - timer_ticks ());
//
//    /* Print a message on wake-up. */
//    msg ("Thread %s woke up.", thread_name ());
//
//    sema_up (&wait_sema);
//}