#ifndef MODULE_APP_CORE_H
#define MODULE_APP_CORE_H

// phase out later
#include <stdio.h>
#include <stdlib.h> // rand()
#include <string.h>
#include <math.h>
#include <unistd.h>  // Include for usleep

#include <time.h>

#include "module_common_logic.h"

// NOTE
// naming convention:
// "signal": input 0,1 : off,on

int init_module();
void cleanup_module();
/////////////////////////////////////////////////
//////////////WINDOW_SECTION/////////////////////
/////////////////////////////////////////////////
struct graphic_window{
void* data_pointer;
int w;
int h;
int graphic_size;

void* gpu_handler_0; // main, other are backup for other framework
void* gpu_handler_1;
void* gpu_handler_2;
void* gpu_handler_3;

int full_screen_signal_last;
int full_screen_mode; // 0: none, 1:full

int mode_resizable;
char name[1024];
};


int init_window(int w, int h, const char* window_name);
int get_window_w();
int get_window_h();
void set_window_fps(int fps);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////RENDER_DATA_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
struct colors{
char r; char g; char b;
char a;
};

struct render_box_2d{
double w; double h;

double angle_rad; // default: 0.0f
double rotation_origin_x; double rotation_origin_y; // default: 0.5f, 0.5f (middle of img, use relatively value scale to clip w, h)
int flip_h_mode; int flip_v_mode; // defauft: 0, 0
int draw_centralized_mode; // defauft: 1

double clip_box_x; double clip_box_y; // defauft: 0, 0
double clip_box_w; double clip_box_h; // defauft: img_w, img_h

double offset_x; double offset_y; // default: 0.0f, 0.0f

int scale_mode; //
double scale_w; double scale_h; // defauft: 1.0f, 1.0f
int blend_mode; //

struct colors color;
};

struct imgs{
void* data_vram; // vram
void* data_ram; // in pixel manipulating case
double w; double h; // const

/*
double angle_rad; // default: 0.0f
double rotation_origin_x; double rotation_origin_y; // default: 0.5f, 0.5f (middle of img, use relatively value scale to clip w, h)
int flip_h_mode; int flip_v_mode; // defauft: 0, 0
int draw_centralized_mode; // defauft: 1

double clip_box_x; double clip_box_y; // defauft: 0, 0
double clip_box_w; double clip_box_h; // defauft: img_w, img_h

double offset_x; double offset_y; // default: 0.0f, 0.0f


double scale_w; double scale_h; // defauft: 1.0f, 1.0f


struct colors tint_color;
*/
//int scale_mode; //
//int blend_mode; //

// default should be either RGB or RGBA
int r_offset;  // 0xFF000000 for RGBA32 (byte offset mapping)
int g_offset;  // 0x00FF0000
int b_offset;  // 0x0000FF00
int a_offset;  // 0x000000FF or 0 if no alpha
int bytes_per_pixel; // probably needed too



int type; // 0 : 1 (static : modifiable) (can change to suit dev need)
int draw_mode; // 0 : 1 (cpu : gpu)
int id; // for advance devs
};
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////RENDER_FUNC_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////BASIC_FUNC_SECTION////////////////////////
void clear_window_screen();
void clear_window_screen_color(struct colors color);
void update_window_screen();

struct colors get_color(char r, char g, char b, char a);
struct colors get_color_from_int32(int color);

void set_render_box_2d(struct render_box_2d* box, double w, double h, struct colors color);
void draw_render_box_2d(const struct render_box_2d* box, double x, double y);
void draw_box(const struct box_2d* box, struct colors color);
void draw_box_rotate(const struct box_2d* box, double angle_rad, struct colors color);
void draw_box_centralized(struct box_2d* box, struct colors color);
void draw_box_basic(register double x, register double y, register double w, register double h, struct colors color);


////////////////////////IMGS_PROCESS_SECTION////////////////////////
int load_img_vram(struct imgs* img, const char* file_path); // end_users should use this only
int load_img_ram(struct imgs* img, const char* file_path);
////////////////////////IMGS_USAGE_SECTION////////////////////////
void draw_img(struct imgs* img, double x, double y);

void draw_imgs(struct imgs* img, double x, double y, double scale_w, double scale_h, double angle_rad, int flip_h, int flip_v, int center_draw);
void draw_imgs_part(struct imgs* img, double x, double y, double scale_w, double scale_h, double angle_rad, int flip_h, int flip_v, int center_draw, struct box_2d* clip);
////////////////////////TEXT_RENDER_SECTION////////////////////////
// WIP

/////////////////////////////////////////////////
//////////////GUI_HELPER_SECTION/////////////////
/////////////////////////////////////////////////
struct clicks{
int state;
int signal_last;
};

void reset_click(struct clicks* click);
void update_click_hold(struct clicks* click, int signal);
void update_click_switch(struct clicks* click, int signal);

void update_click_once(struct clicks* click, int signal);
/////////////////////////////////////////////////
//////////////////IO_SECTION/////////////////////
/////////////////////////////////////////////////
struct io_data{
int quit;

// key section
int key_a; int key_b; int key_c;
int key_d; int key_e; int key_f;
int key_g; int key_h; int key_i;
int key_j; int key_k; int key_l;
int key_m; int key_n; int key_o;
int key_p; int key_q; int key_r;
int key_s; int key_t; int key_u;
int key_v; int key_w; int key_x;
int key_y; int key_z;

int key_0; int key_1; int key_2;
int key_3; int key_4; int key_5;
int key_6; int key_7; int key_8;
int key_9;

int key_arrow_up; int key_arrow_down;
int key_arrow_left; int key_arrow_right;

int key_space;
int key_enter;
int key_escape;
int key_tab;
int key_backspace;
int key_capslock;
int key_shift_left; int key_shift_right;
int key_ctrl_left; int key_ctrl_right;
int key_alt_left; int key_alt_right;

// mouse section
int mouse_motion;
double mouse_x; double mouse_y;

int mouse_click_left; int mouse_click_right;

double mouse_click_up_x; double mouse_click_up_y;
double mouse_click_down_x; double mouse_click_down_y;

int mouse_scroll;
int mouse_scroll_horizontal;
int mouse_scroll_vertical;
int mouse_scroll_x;
int mouse_scroll_y;
};

void init_io_data(struct io_data* io);
void get_io_signal(struct io_data* io, int mouse_signal, int kb_signal);

/////////////////////////////////////////////////
////////////////TIME_SECTION/////////////////////
/////////////////////////////////////////////////
struct timers{
double time_current;
double time_last;
double delta_time;
};

void reset_timer(struct timers* timer);
void update_timer(struct timers* timer);
int check_timer_delta_time_passed(struct timers* timer, double tagret_delta_time); // since last call of this func

#endif

