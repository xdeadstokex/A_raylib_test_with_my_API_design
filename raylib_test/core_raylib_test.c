/////////////////////////////////////////////////
/////////////////INCLUDE_PART////////////////////
/////////////////////////////////////////////////
#include "module_app_core.h"
/////////////////////////////////////////////////
/////////////CUSTOM_DATA_PART////////////////////
/////////////////////////////////////////////////
// custom structs and global things here
#define ZOMBIES_COUNT 20
struct objs{
double x; double y;
double hitbox_w; double hitbox_h;
double velo_x; double velo_y;
// hitbox later
};
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
struct imgs zombie;
struct pos_2d ball_pos;
struct box_2d hammer;
double hammer_rotation;

struct objs zombies[ZOMBIES_COUNT];

struct colors test_color1;
struct colors test_color2;
};

int init_program(struct program_datas* data);
int update_control(struct program_datas* data);
void update_logic(struct program_datas* data);
void update_render(struct program_datas* data);
void clean_up_program(struct program_datas* data);


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
if( !init_window(800, 600, "zombie_bonk") ){ return 0; }
load_img_vram(&data->zombie, "./resource/foo.png");

set_pos_2d(&data->ball_pos, (double) get_window_w() / 2, (double) get_window_h() / 2);
set_box_2d(&data->hammer, (double) get_window_w() / 2, (double) get_window_h() / 2, 50, 70);
data->hammer_rotation = 0;

for(int a = 0; a < ZOMBIES_COUNT; ++a){
data->zombies[a].x = rand() % get_window_w();
data->zombies[a].y = rand() % get_window_h();
data->zombies[a].hitbox_w = data->zombie.w;
data->zombies[a].hitbox_h = data->zombie.h;
data->zombies[a].velo_x = (double)(rand() % 100) / 10;
data->zombies[a].velo_y = (double)(rand() % 100) / 10;
data->zombies[a].velo_x *= ((rand() & 1) == 0) ? 1 : -1;
data->zombies[a].velo_y *= ((rand() & 1) == 0) ? 1 : -1;
}

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


if( io->key_arrow_right ){ data->ball_pos.x += 2.0f; }
if( io->key_arrow_left ){ data->ball_pos.x -= 2.0f; }
if( io->key_arrow_up ){ data->hammer_rotation -= 0.2f; }
if( io->key_arrow_down ){ data->hammer_rotation += 0.2f; }

if(io->mouse_click_left){
data->hammer_rotation += 0.2f;
}

if(io->mouse_click_right){
data->hammer_rotation -= 0.2f;
}
data->hammer.x = io->mouse_x;
data->hammer.y = io->mouse_y;

////////////////////////
////////////////////////
return !data->quit_signal;
}


void update_logic(struct program_datas* data){
if(!data->update_logic_flag){ return; } 
////////////////////////
////////////////////////
// code goes here


for(int a = 0; a < ZOMBIES_COUNT; ++a){
if( check_two_box_2d_hit_centralized(
//&(struct box_2d){data->zombies[a].x, data->zombies[a].y, data->zombies[a].hitbox_w, data->zombies[a].hitbox_h},
(struct box_2d*)(&data->zombies[a]),
&data->hammer
) ){

continue;
}

data->zombies[a].x += data->zombies[a].velo_x;
data->zombies[a].y += data->zombies[a].velo_y;
if(data->zombies[a].x >= get_window_w()){
data->zombies[a].x = data->zombies[a].x - get_window_w();
}
else if(data->zombies[a].x < 0){
data->zombies[a].x = data->zombies[a].x + get_window_w();
}
if(data->zombies[a].y >= get_window_h()){
data->zombies[a].y = data->zombies[a].y - get_window_h();
}
else if(data->zombies[a].y < 0){
data->zombies[a].y = data->zombies[a].y + get_window_h();
}
}
////////////////////////
////////////////////////
return;
}


void update_render(struct program_datas* data){
if(!data->update_render_flag){ return; } 
////////////////////////
////////////////////////
// code goes here

draw_begin();
clear_window_screen();
//draw_box(&data->hammer, (struct colors){255, 193, 140, 0xFF});
//draw_box_rotate(&data->hammer, data->hammer_rotation, (struct colors){255, 193, 140, 0xFF});
//draw_img(&data->zombie, data->hammer.x, data->hammer.y);
//draw_box_centralized(&data->hammer, (struct colors){255, 193, 140, 0xFF});
//draw_img_centralized(&data->zombie, data->hammer.x, data->hammer.y);
draw_box_centralized(&data->hammer, (struct colors){255, 193, 140, 0xFF});
draw_img_centralized_rotate(&data->zombie, data->hammer.x, data->hammer.y, data->hammer_rotation);
for(int a = 0; a < ZOMBIES_COUNT; ++a){
//draw_box_basic(data->zombies[a].x, data->zombies[a].y, 10, 10, (struct colors){255, 193, 140, 0xFF});
draw_box_centralized(&(struct box_2d){data->zombies[a].x, data->zombies[a].y, data->zombies[a].hitbox_w + 10, data->zombies[a].hitbox_h + 10}, (struct colors){255, 193, 140, 0xFF});
draw_img_centralized(&data->zombie, data->zombies[a].x, data->zombies[a].y);
}


draw_end();

//DrawText("move the ball with Arrow keys", 10, 10, 20, DARKGRAY);
//DrawCircleV(data->ball_pos, 50, MAROON);
//printf("rot %f \n", data->hammer_rotation);
//printf("x %f y %f \n", data->hammer.x, data->hammer.y);
//printf("x %f y %f \n", data->zombies[0].x, data->zombies[0].y);


//DrawRectanglePro(data->hammer, (Vector2){0, 0}, data->hammer_rotation, (Color){255, 193, 140, 0xFF});

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






