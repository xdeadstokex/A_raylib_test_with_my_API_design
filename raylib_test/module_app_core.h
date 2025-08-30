#ifndef MODULE_APP_CORE_H
#define MODULE_APP_CORE_H

// phase out later
#include <stdio.h>
#include <stdlib.h> // rand()
#include <string.h>
#include <math.h>
#include <unistd.h>  // Include for usleep

#include <time.h>


// default text, better struct style { int a[64];...
// move todo to end of file


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
/////////////////////////////COMMON_LOGIC_DATA_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#define PI 3.14159265358979323846
#define PI_X2 2 * 3.14159265358979323846
#define PI_HALF 0.5 * 3.14159265358979323846

struct box_2d{
double x; double y;
double w; double h;
};

struct pos_2d{
double x;
double y;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////COMMON_LOGIC_FUNC_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
int check_point_in_area_2d_debug(register double x, register double y, register double x1_begin, register double y1_begin, register double x1_end, register double y1_end);

int check_area_overlap_2d_debug(register double x1_begin, register double y1_begin, register double x1_end, register double y1_end, register double x2_begin, register double y2_begin, register double x2_end, register double y2_end);

int check_point_in_area_2d(struct pos_2d pos, struct box_2d box);

int check_area_overlap_2d(struct pos_2d area1_start, struct pos_2d area1_end, struct pos_2d area2_start, struct pos_2d area2_end);

int check_two_box_2d_hit_centralized(const struct box_2d* box1, const struct box_2d* box2);

void set_box_2d(struct box_2d* box, register double x, register double y, register double w, register double h);
void set_pos_2d(struct pos_2d* pos, register double x, register double y);
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////RENDER_DATA_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
struct colors{
char r; char g; char b;
char a;
};

struct imgs{
void* data_vram; // vram
void* data_ram; // in pixel manipulating case
double w; double h;
double scale_w; double scale_h;

int reset_flip_mode; // for case when flip img is temporary
int flip_mode; // 0: non, 1: horizontal, 2: vertical

double rotation_angle_rad; // default: 0

int scale_mode; //
int blend_mode; //

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


struct colors get_color(char r, char g, char b, char a);
struct colors get_color_from_int32(int color);

void draw_begin();
void draw_end();
void draw_box(const struct box_2d* box, struct colors color);
void draw_box_rotate(const struct box_2d* box, double angle_rad, struct colors color);
void draw_box_centralized(struct box_2d* box, struct colors color);
void draw_box_basic(register double x, register double y, register double w, register double h, struct colors color);

////////////////////////IMGS_PROCESS_SECTION////////////////////////
int load_img_vram(struct imgs* img, const char* file_path); // end_users should use this only
int load_img_ram(struct imgs* img, const char* file_path);
////////////////////////IMGS_USAGE_SECTION////////////////////////
void draw_img(struct imgs* img, double x, double y);
void draw_img_centralized(struct imgs* img, double x, double y);
void draw_img_centralized_rotate(struct imgs* img, double x, double y, double angle_rad);
////////////////////////TEXT_RENDER_SECTION////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////GUI_DATA_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
struct buttons{
struct box_2d hit_area;
struct box_2d visual_area;

double visual_area_offset_x;
double visual_area_offset_y;

int state;
int signal_last;

double value;  // multipurpose: hold duration, haptic intensity, slider position, etc.
};


struct sliders{
struct box_2d hit_area;
struct box_2d visual_area;
struct box_2d bounding_area;

double x_offset_begin;
double y_offset_begin;
double x_offset_end;
double y_offset_end;

double visual_area_offset_x;
double visual_area_offset_y;

int state;

double value_x;
double value_y;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////IO_DATA_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////IO_FUNC_AREA////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void init_io_data(struct io_data* io);
void get_io_signal(struct io_data* io, int mouse_signal, int kb_signal);
#endif

