/////////////////////////////////////////////////
/////////////////INCLUDE_PART////////////////////
/////////////////////////////////////////////////
#include "module_app_core.h"
/////////////////////////////////////////////////
/////////////CUSTOM_DATA_PART////////////////////
/////////////////////////////////////////////////
// custom structs and global things here
#define ZOMBIES_COUNT 40
struct objs{
double x; double y;
double hitbox_w; double hitbox_h;
double velo_x; double velo_y;

struct timers wandering_timer;
double wandering_delta_time; // ms
int stop_moving_signal;

int death_signal;
int death_timer_waiting_signal;
double death_timer_waiting_time;
};


struct hammers{
double x; double y;
double w; double h;

struct clicks click;

double rotation_angle_rad;
double min_rotation_angle_rad;
double max_rotation_angle_rad;
int revert_animation;
int hammer_animation_signal;
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
struct imgs tomb;
struct imgs hammer_sprite;

struct hammers hammer;

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
if( !init_window(1200, 900, "zombie_bonk") ){ return 0; }
load_img_vram(&data->zombie, "./resource/foo.png");
load_img_vram(&data->tomb, "./resource/tomb.png");
load_img_vram(&data->hammer_sprite, "./resource/hammer.png");

set_box_2d((struct box_2d*) &data->hammer, (double) get_window_w() / 2, (double) get_window_h() / 2, 50, 70);
data->hammer.rotation_angle_rad = 0;
data->hammer.min_rotation_angle_rad = 0;
data->hammer.max_rotation_angle_rad = PI / 4;
data->hammer.revert_animation = 0;
data->hammer.hammer_animation_signal = 0;
reset_click(&data->hammer.click);

for(int a = 0; a < ZOMBIES_COUNT; ++a){
data->zombies[a].x = rand() % get_window_w();
data->zombies[a].y = rand() % get_window_h();
data->zombies[a].hitbox_w = data->zombie.w;
data->zombies[a].hitbox_h = data->zombie.h;
data->zombies[a].velo_x = (double)(rand() % 100) / 10;
data->zombies[a].velo_y = (double)(rand() % 100) / 10;
data->zombies[a].velo_x *= ((rand() & 1) == 0) ? 1 : -1;
data->zombies[a].velo_y *= ((rand() & 1) == 0) ? 1 : -1;

reset_timer(&data->zombies[a].wandering_timer);
data->zombies[a].wandering_delta_time = rand() % 2000 / 1000;
data->zombies[a].stop_moving_signal = 0;
data->zombies[a].death_signal = 0;
data->zombies[a].death_timer_waiting_signal = 0;
data->zombies[a].death_timer_waiting_time = 5;
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

update_click_once(&data->hammer.click, io->mouse_click_left);
if(data->hammer.click.state == 1){
if(data->hammer.hammer_animation_signal == 0){ data->hammer.hammer_animation_signal = 1; }
}
printf(" click left %d hammer click left %d \n", io->mouse_click_left, data->hammer.click.state);

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

// update hammer
if(data->hammer.hammer_animation_signal == 1){

if(data->hammer.revert_animation == 0){
data->hammer.rotation_angle_rad += 0.2f;
if(data->hammer.rotation_angle_rad >= data->hammer.max_rotation_angle_rad){
data->hammer.revert_animation = 1;
data->hammer.rotation_angle_rad = data->hammer.max_rotation_angle_rad;
}
}
else{
data->hammer.rotation_angle_rad -= 0.2f;
if(data->hammer.rotation_angle_rad <= data->hammer.min_rotation_angle_rad){
data->hammer.revert_animation = 0;
data->hammer.rotation_angle_rad = data->hammer.min_rotation_angle_rad;
data->hammer.hammer_animation_signal = 0;
}
}

}

// update zombies
for(int a = 0; a < ZOMBIES_COUNT; ++a){

if(data->zombies[a].death_signal == 1){

if(data->zombies[a].death_timer_waiting_signal == 0){
reset_timer(&data->zombies[a].wandering_timer);
data->zombies[a].death_timer_waiting_signal = 1;
}

update_timer(&data->zombies[a].wandering_timer);

if( check_timer_delta_time_passed(&data->zombies[a].wandering_timer, data->zombies[a].death_timer_waiting_time) ){
data->zombies[a].death_signal = 0;
reset_timer(&data->zombies[a].wandering_timer);
}

}

if(data->zombies[a].death_signal == 1){ continue; }

if( check_two_box_2d_hit_centralized((struct box_2d*)(&data->zombies[a]), (struct box_2d*) &data->hammer) ){
if(data->hammer.click.state){
data->zombies[a].stop_moving_signal = 1;
data->zombies[a].death_signal = 1;
continue;
}
}

update_timer(&data->zombies[a].wandering_timer);
printf(" delta time %f \n", data->zombies[a].wandering_timer.delta_time);
if( check_timer_delta_time_passed(&data->zombies[a].wandering_timer, data->zombies[a].wandering_delta_time) ){
reset_timer(&data->zombies[a].wandering_timer);
data->zombies[a].wandering_delta_time = rand() % 2000 / 1000 + 1; // guarantee as least 1 sec move/stop

// change between standing still and move in different dir
if(data->zombies[a].stop_moving_signal == 0){
data->zombies[a].stop_moving_signal = 1;
data->zombies[a].velo_x = (double)(rand() % 100) / 10;
data->zombies[a].velo_y = (double)(rand() % 100) / 10;
data->zombies[a].velo_x *= ((rand() & 1) == 0) ? 1 : -1;
data->zombies[a].velo_y *= ((rand() & 1) == 0) ? 1 : -1;
}
else if(data->zombies[a].stop_moving_signal == 1){
data->zombies[a].stop_moving_signal = 0;
}
}

if(data->zombies[a].stop_moving_signal == 1){ continue; }

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

//draw_box_centralized((struct box_2d*) &data->hammer, (struct colors){255, 193, 140, 0xFF});
draw_img_centralized_rotate(&data->hammer_sprite, data->hammer.x, data->hammer.y, data->hammer.rotation_angle_rad);

for(int a = 0; a < ZOMBIES_COUNT; ++a){
//draw_box_basic(data->zombies[a].x, data->zombies[a].y, 10, 10, (struct colors){255, 193, 140, 0xFF});
//draw_box_centralized(&(struct box_2d){data->zombies[a].x, data->zombies[a].y, data->zombies[a].hitbox_w + 10, data->zombies[a].hitbox_h + 10}, (struct colors){255, 193, 140, 0xFF});

if(data->zombies[a].death_signal == 0){
draw_img_centralized(&data->zombie, data->zombies[a].x, data->zombies[a].y);
}
else{
draw_img_centralized(&data->tomb, data->zombies[a].x, data->zombies[a].y);
}
}


draw_end();

//DrawText("move the ball with Arrow keys", 10, 10, 20, DARKGRAY);
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






