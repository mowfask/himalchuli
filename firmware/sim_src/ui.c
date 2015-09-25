#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include "ui.h"

ui_inputs input_state;
pthread_mutex_t input_mutex;

ui_outputs output_state;
pthread_mutex_t output_mutex;

pthread_t ui_thread;
//The UI thread will try to lock this mutex every now and then. If it
//succeeds, it shall terminate.
pthread_mutex_t ui_thread_lock;

static void ui_update(void)
{
    const char motor_states[][6] = {"off  ", "left ", "right"};
    const char led_states[][4]   = {"off", "on "};
    const char btn_states[][9]   = {"released", "pressed "};
    ui_outputs outp;
    ui_inputs inp;

    pthread_mutex_lock(&output_mutex);
    outp = output_state;
    pthread_mutex_unlock(&output_mutex);

    pthread_mutex_lock(&input_mutex);
    inp = input_state;
    pthread_mutex_unlock(&input_mutex);

    mvprintw(2, 18, "%s", motor_states[outp.motor]);
    mvprintw(3, 18, "%s", led_states[outp.led_green]);
    mvprintw(4, 18, "%s", led_states[outp.led_red]);

    mvprintw(2, 60, "%s", btn_states[inp.manual_enable]);
    mvprintw(3, 60, "%s", btn_states[inp.manual_up]);
    mvprintw(4, 60, "%s", btn_states[inp.manual_down]);
    mvprintw(6, 60, "%s", btn_states[inp.motor_rot]);
    mvprintw(7, 60, "%3d", inp.current);
    mvprintw(8, 60, "%3d", inp.distance);

    mvaddch(23,0,'>');
    refresh();
}

void ui_setoutp(ui_outputs outp)
{
    pthread_mutex_lock(&output_mutex);
    output_state = outp;
    pthread_mutex_unlock(&output_mutex);
    ui_update();
}

static void ui_setinp(ui_inputs inp)
{
    pthread_mutex_lock(&input_mutex);
    input_state = inp;
    pthread_mutex_unlock(&input_mutex);
    ui_update();
}

ui_inputs ui_getinp(void)
{
    ui_inputs result;
    pthread_mutex_lock(&input_mutex);
    result = input_state;
    pthread_mutex_unlock(&input_mutex);

    return(result);
}

static void print_template(void)
//Assuming window size of 80x24
{
    int i;
    const ui_outputs outp_defaults = {motor_off, ledg_off, ledr_off};
    const ui_inputs inp_defaults = {btn_released, btn_released,
                                    btn_released, btn_released, 0, 0};
    //Headings and divisions:
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(0, 1, "%s", "Outputs");
    mvprintw(0, 40, "%s", "Inputs");
    mvprintw(19, 1, "%s", "Controls");
    attroff(A_UNDERLINE);
    for(i=0; i<80; ++i)
    {
        mvaddch(18, i, '#');
    }
    for(i=0; i<18; ++i)
    {
        mvaddch(i, 37, '#');
    }
    attroff(A_BOLD);

    //Outputs
    mvprintw(2, 3, "%s", "Motor:");
    mvprintw(3, 3, "%s", "LED green:");
    mvprintw(4, 3, "%s", "LED red:");
    ui_setoutp(outp_defaults);

    //Inputs
    mvprintw(2, 42, "%s", "Manual enable:");
    mvprintw(3, 42, "%s", "Manual up:");
    mvprintw(4, 42, "%s", "Manual down:");
    mvprintw(6, 42, "%s", "Motor rotation:");
    mvprintw(7, 42, "%s", "Current:");
    mvprintw(8, 42, "%s", "Distance:");
    ui_setinp(inp_defaults);

    //Controls
    mvprintw(19, 11, "%s", "e    : Manual enable");
    mvprintw(20, 11, "%s", "UP   : Manual up");
    mvprintw(21, 11, "%s", "DOWN : Manual down");
    mvprintw(19, 35, "%s", "r : Motor rotation");
    mvprintw(20, 35, "%s", "i : Current+");
    mvprintw(21, 35, "%s", "k : Current-");
    mvprintw(22, 35, "%s", "o : Distance+");
    mvprintw(23, 35, "%s", "l : Distance-");
    mvprintw(19, 59, "%s", "q : Quit");

    ui_update();
}

void ui_handle_inp(int inp)
{
    ui_inputs inps;
    inps = ui_getinp();

    switch(inp)
    {
        case 'e':
            inps.manual_enable = 1-inps.manual_enable;
            break;
        case KEY_UP:
            inps.manual_up = 1-inps.manual_up;
            break;
        case KEY_DOWN:
            inps.manual_down = 1-inps.manual_down;
            break;
        case 'r':
            inps.motor_rot = 1-inps.motor_rot;
            break;
        case 'i':
            if(inps.current <= 245)
            {
                inps.current += 10;
            }
            else
            {
                inps.current = 255;
            }
            break;
        case 'k':
            if(inps.current >= 10)
            {
                inps.current -= 10;
            }
            else
            {
                inps.current = 0;
            }
            break;
        case 'o':
            if(inps.distance <= 245)
            {
                inps.distance += 10;
            }
            else
            {
                inps.distance = 255;
            }
            break;
        case 'l':
            if(inps.distance >= 10)
            {
                inps.distance -= 10;
            }
            else
            {
                inps.distance = 0;
            }
            break;
        case 'q':
            endwin();
            exit(0);
    }
    ui_setinp(inps);
}

void *ui_thread_func(void *arg)
{
    int inp;

    while(1)
    {
        inp = getch();
        ui_handle_inp(inp);

        if(pthread_mutex_trylock(&ui_thread_lock) == 0)
        {
            return NULL;
        }
    }
}

void ui_init(void)
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    print_template();
    timeout(20);

    pthread_mutex_init(&input_mutex, NULL);
    pthread_mutex_init(&output_mutex, NULL);
    pthread_mutex_init(&ui_thread_lock, NULL);

    pthread_mutex_lock(&ui_thread_lock);
    pthread_create(&ui_thread, NULL, ui_thread_func, NULL);
}

void ui_deinit(void)
{
    pthread_mutex_unlock(&ui_thread_lock);
    pthread_join(ui_thread, NULL);

    timeout(-1);
    endwin();

    pthread_mutex_destroy(&input_mutex);
    pthread_mutex_destroy(&output_mutex);
    pthread_mutex_destroy(&ui_thread_lock);
}

