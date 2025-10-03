/////////////////////////////////////////////////
/////////////////INCLUDE_PART////////////////////
/////////////////////////////////////////////////
#include "module_app_core.h"
/////////////////////////////////////////////////
/////////////CUSTOM_DATA_PART////////////////////
/////////////////////////////////////////////////
// custom structs and global things here
/////////////////////////////////////////////////
///////////////////MAIN_PART/////////////////////
/////////////////////////////////////////////////
struct program_datas{
// Thread control flags
volatile int update_control_flag;
volatile int update_logic_flag; 
volatile int update_render_flag;
volatile int quit_signal;
    
// Thread handles (add your platform-specific types)
// pthread_t logic_thread;
// pthread_t render_thread;
    
// Shared data (protect with mutex if needed)
int frame_count;
////////////////////////
////////////////////////
// app data goes here
struct io_data io;

struct imgs test_sprite;
double test_x;
double test_y;
double scale_w;
double scale_h;
double test_angle_rad;

struct box_2d clip_box;

struct render_box_2d test_box;

struct clicks test_signal;
struct clicks test_signal_flip_h;
struct clicks test_signal_flip_v;
};

////////////////////////
/////FUNC_PROTOTYPE/////
////////////////////////
int init_program(struct program_datas* data);
int update_control(struct program_datas* data);
void update_logic(struct program_datas* data);
void update_render(struct program_datas* data);
void clean_up_program(struct program_datas* data);

// custom func prototype

////////////////////////
////////////////////////
////////////////////////

void run(){
struct program_datas data;
if(!init_program(&data)) return;

for(;;){
if(!update_control(&data)) break;
update_logic(&data);
update_render(&data);
}

clean_up_program(&data);
return;
}

////////////////////////
////////////////////////
//should implement with caution
//void run_multi_thread(){}
////////////////////////
////////////////////////

// Entry point - completely self-contained
int main(int argc, char** argv){
(void)argc; (void)argv; // suppress unused warnings
run();
return 0;
}

/////////////////////////////////////////////////
///////////CUSTOM_FUNC_IMPLEMENT/////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
///////////////FUNC_IMPLEMENT////////////////////
/////////////////////////////////////////////////
int init_program(struct program_datas* data){
data->update_control_flag = 1;
data->update_logic_flag = 1;
data->update_render_flag = 1;
data->quit_signal = 0;
data->frame_count = 0;
////////////////////////
////////////////////////
// code goes here
if( !init_module() ){ return 0; }
if( !init_window(1200, 900, "zombie_bonk") ){ return 0; }
load_img_vram(&data->test_sprite, "./resource/foo.png");
data->test_x = get_window_w() / 2;
data->test_y = get_window_h() / 2;
data->scale_w = 1.0f;
data->scale_h = 1.0f;
data->test_angle_rad = 0;

data->clip_box.x = 0.0f;
data->clip_box.y = 0.0f;
data->clip_box.w = data->test_sprite.w;
data->clip_box.h = data->test_sprite.h;

set_render_box_2d(&data->test_box, 200, 400, (struct colors){0xFF, 0x00, 0x00, 0xFF});

reset_click(&data->test_signal);
data->test_signal.state = 1;
reset_click(&data->test_signal_flip_h);
reset_click(&data->test_signal_flip_v);

set_window_fps(60);
////////////////////////
////////////////////////
return 1;
}




int update_control(struct program_datas* data){
if(!data->update_control_flag){ return 1; } // add app quit signal control here
////////////////////////
////////////////////////
// code goes here
// set data->quit_signal = 1 to exit
struct io_data* io = &data->io;
get_io_signal(io, 1, 1);

data->quit_signal = io->quit;


//if(io->key_arrow_left){ data->test_x -= 4.0f; }
//if(io->key_arrow_right){ data->test_x += 4.0f; }
//if(io->key_arrow_down){ data->test_y -= 4.0f; }
//if(io->key_arrow_up){ data->test_y += 4.0f; }

if(io->key_z){ data->test_angle_rad -= 0.1f; }
if(io->key_x){ data->test_angle_rad += 0.1f; }


if(io->key_a){ data->clip_box.x -= 4.0f; }
if(io->key_d){ data->clip_box.x += 4.0f; }
if(io->key_s){ data->clip_box.y -= 4.0f; }
if(io->key_w){ data->clip_box.y += 4.0f; }

if(io->key_arrow_left){ data->clip_box.w -= 4.0f; }
if(io->key_arrow_right){ data->clip_box.w += 4.0f; }
if(io->key_arrow_down){ data->clip_box.h -= 4.0f; }
if(io->key_arrow_up){ data->clip_box.h += 4.0f; }








update_click_switch(&data->test_signal, io->key_space);
update_click_switch(&data->test_signal_flip_h, io->key_f);
update_click_switch(&data->test_signal_flip_v, io->key_g);



if(io->key_h){ data->scale_w -= 0.1f; }
if(io->key_k){ data->scale_w += 0.1f; }
if(io->key_j){ data->scale_h += 0.1f; }
if(io->key_u){ data->scale_h -= 0.1f; }

data->test_x = io->mouse_x;
data->test_y = io->mouse_y;

printf("x %f y %f angle %f\n", data->test_x, data->test_y, data->test_angle_rad);
////////////////////////
////////////////////////
return !data->quit_signal;
}


void update_logic(struct program_datas* data){
if(!data->update_logic_flag){ return; } 
////////////////////////
////////////////////////
// code goes here

////////////////////////
////////////////////////
return;
}


void update_render(struct program_datas* data){
if(!data->update_render_flag){ return; } 
////////////////////////
////////////////////////
// code goes here

clear_window_screen();

//draw_render_box_2d(&data->test_box, data->test_x, data->test_y);
//draw_img(&data->test_sprite, data->test_x, data->test_y);

//draw_imgs(&data->test_sprite, data->test_x, data->test_y, data->scale_w, data->scale_h, data->test_angle_rad, data->test_signal_flip_h.state, data->test_signal_flip_v.state, data->test_signal.state);

draw_imgs_part(&data->test_sprite, data->test_x, data->test_y, data->scale_w, data->scale_h, data->test_angle_rad, data->test_signal_flip_h.state, data->test_signal_flip_v.state, data->test_signal.state, NULL//&data->clip_box
);
update_window_screen();
////////////////////////
////////////////////////
return;
}


void clean_up_program(struct program_datas* data){
////////////////////////
////////////////////////
// code goes here
cleanup_module();
////////////////////////
////////////////////////
return;
}






