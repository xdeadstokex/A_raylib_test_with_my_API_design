/////////////////////////////////////////////////
/////////////////INCLUDE_PART////////////////////
/////////////////////////////////////////////////
#include "module_app_core.h"
/////////////////////////////////////////////////
/////////////CUSTOM_DATA_PART////////////////////
/////////////////////////////////////////////////
// custom structs and global things here
#define ZOMBIES_COUNT 1
struct objs{
double x; double y;
double hitbox_w; double hitbox_h;
double velo_x; double velo_y;

int hp;

struct timers wandering_timer;
double wandering_delta_time; // ms
int stop_moving_signal;

int death_signal;
double death_timer_waiting_time;

//shaking
double rotation_angle_rad;
double min_rotation_angle_rad;
double max_rotation_angle_rad;
double rotation_angle_rad_additive;
int revert_animation;
int animation_amount;
int animation_max_amount;
int animation_signal;


int type; // 0 normal 1 conehead 2 buckethead

struct imgs* current_sprite;
};


struct hammers{
double x; double y;
double w; double h;

struct clicks click;

double rotation_angle_rad;
double min_rotation_angle_rad;
double max_rotation_angle_rad;
double rotation_angle_rad_additive;
int revert_animation;
int hammer_animation_signal;

struct imgs* current_sprite;
};

struct sprites_group{
struct imgs zombie;
struct imgs zombie_conehead;
struct imgs zombie_buckethead;

struct imgs tomb;

struct imgs hammer_sprite;
};

struct cameras{
double x; double y;
double w; double h;

double x_origin; double y_origin;
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

struct sprites_group app_sprites;

struct hammers hammer;
struct objs zombies[ZOMBIES_COUNT];

struct cameras camera;
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
void update_hammer_animation_logic(struct hammers* hammer);
void draw_hammer(struct hammers* hammer);

void assign_zombie_type(struct objs* zombie, int type);
void assign_zombie_sprite(struct objs* zombie, struct sprites_group* app_sprites);
void update_zombie_animation_logic(struct objs* zombie);
void draw_zombie(struct objs* zombie, struct cameras* camera);
void draw_border(struct cameras* camera);
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
void update_hammer_animation_logic(struct hammers* hammer){
if(hammer->hammer_animation_signal == 0){ return; }

if(hammer->revert_animation == 0){
hammer->rotation_angle_rad += hammer->rotation_angle_rad_additive;
if(hammer->rotation_angle_rad >= hammer->max_rotation_angle_rad){
hammer->revert_animation = 1;
hammer->rotation_angle_rad = hammer->max_rotation_angle_rad;
}
}
else{
hammer->rotation_angle_rad -= hammer->rotation_angle_rad_additive;
if(hammer->rotation_angle_rad <= hammer->min_rotation_angle_rad){
hammer->revert_animation = 0;
hammer->rotation_angle_rad = hammer->min_rotation_angle_rad;
hammer->hammer_animation_signal = 0;
}
}
}

void update_zombie_animation_logic(struct objs* zombie){
if(zombie->animation_signal == 0){ return; }

if(zombie->death_signal == 1){
zombie->rotation_angle_rad = 0;
zombie->animation_amount = zombie->animation_max_amount;
zombie->animation_signal = 0;
return;
}

if(zombie->revert_animation == 0){
zombie->rotation_angle_rad += zombie->rotation_angle_rad_additive;
if(zombie->rotation_angle_rad >= zombie->max_rotation_angle_rad){
zombie->revert_animation = 1;
zombie->rotation_angle_rad = zombie->max_rotation_angle_rad;
}
}
else{
zombie->rotation_angle_rad -= zombie->rotation_angle_rad_additive;
if(zombie->rotation_angle_rad <= zombie->min_rotation_angle_rad){
zombie->revert_animation = 0;
zombie->rotation_angle_rad = zombie->min_rotation_angle_rad;

zombie->animation_amount -= 1;
if(zombie->animation_amount <= 0){
zombie->animation_amount = zombie->animation_max_amount;
zombie->animation_signal = 0;
}

}
}

}

void draw_hammer(struct hammers* hammer){
//draw_img_centralized_rotate(hammer->current_sprite, hammer->x, hammer->y, hammer->rotation_angle_rad);

draw_imgs(hammer->current_sprite, hammer->x, hammer->y, 1, 1, hammer->rotation_angle_rad, 0, 0, 1);
return;
}


void assign_zombie_type(struct objs* zombie, int type){
zombie->type = type;
if(type == 0){ zombie->hp = 3; }
else if(type == 1){ zombie->hp = 6; }
else if(type == 2){ zombie->hp = 9; }
return;
}


void assign_zombie_sprite(struct objs* zombie, struct sprites_group* app_sprites){
if(zombie->death_signal == 1){ zombie->current_sprite = &app_sprites->tomb; return; }

if(zombie->type == 0){ zombie->current_sprite = &app_sprites->zombie; }
else if(zombie->type == 1){ zombie->current_sprite  = &app_sprites->zombie_conehead; }
else if(zombie->type == 2){ zombie->current_sprite  = &app_sprites->zombie_buckethead; }
}


void draw_zombie(struct objs* zombie, struct cameras* camera){
if( check_point_in_box_2d_centralized(&(struct pos_2d){zombie->x, zombie->y}, &(struct box_2d){camera->x, camera->y, camera->w, camera->h}) ){
register double x = zombie->x - (camera->x - camera->x_origin);
register double y = zombie->y - (camera->y - camera->y_origin);
draw_img_centralized_rotate(zombie->current_sprite, x, y, zombie->rotation_angle_rad);
}
}

void draw_border(struct cameras* camera){
static double box_x = 1000;
static double box_y = 1000;
static double box_w = 10;
static double box_h = 1000;

//if( check_point_in_box_2d_centralized(&(struct pos_2d){box_x, box_y}, &(struct box_2d){camera->x, camera->y, camera->w, camera->h})

if( check_two_box_2d_hit_centralized(&(struct box_2d){box_x, box_y, box_w, box_h}, &(struct box_2d){camera->x, camera->y, camera->w, camera->h}) ){
register double x = box_x - (camera->x - camera->x_origin);
register double y = box_y - (camera->y - camera->y_origin);

draw_box(&(struct box_2d){x, y, box_w, box_h}, (struct colors){0xFF, 0xFF, 0xFF, 0xFF});
}
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
load_img_vram(&data->app_sprites.zombie, "./resource/zombie.png");
load_img_vram(&data->app_sprites.zombie_conehead, "./resource/zombie_conehead.png");
load_img_vram(&data->app_sprites.zombie_buckethead, "./resource/zombie_buckethead.png");
load_img_vram(&data->app_sprites.tomb, "./resource/tomb.png");
load_img_vram(&data->app_sprites.hammer_sprite, "./resource/hammer.png");

set_box_2d((struct box_2d*) &data->hammer, (double) get_window_w() / 2, (double) get_window_h() / 2, 50, 70);
data->hammer.rotation_angle_rad = 0;
data->hammer.min_rotation_angle_rad = 0;
data->hammer.max_rotation_angle_rad = PI / 4;
data->hammer.rotation_angle_rad_additive = 0.2f;
data->hammer.revert_animation = 0;
data->hammer.hammer_animation_signal = 0;
reset_click(&data->hammer.click);
data->hammer.current_sprite = &data->app_sprites.hammer_sprite;

for(int a = 0; a < ZOMBIES_COUNT; ++a){
data->zombies[a].x = rand() % get_window_w();
data->zombies[a].y = rand() % get_window_h();
data->zombies[a].hitbox_w = data->app_sprites.zombie.w;
data->zombies[a].hitbox_h = data->app_sprites.zombie.h;
data->zombies[a].velo_x = (double)(rand() % 100) / 10;
data->zombies[a].velo_y = (double)(rand() % 100) / 10;
data->zombies[a].velo_x *= ((rand() & 1) == 0) ? 1 : -1;
data->zombies[a].velo_y *= ((rand() & 1) == 0) ? 1 : -1;

reset_timer(&data->zombies[a].wandering_timer);
data->zombies[a].wandering_delta_time = rand() % 2000 / 1000;
data->zombies[a].stop_moving_signal = 0;
data->zombies[a].death_signal = 0;
data->zombies[a].death_timer_waiting_time = 5;

data->zombies[a].rotation_angle_rad = 0;
data->zombies[a].min_rotation_angle_rad = 0;
data->zombies[a].max_rotation_angle_rad = PI / 8;
data->zombies[a].rotation_angle_rad_additive = 0.1f;
data->zombies[a].revert_animation = 0;
data->zombies[a].animation_max_amount = 2;
data->zombies[a].animation_amount = data->zombies[a].animation_max_amount;

data->zombies[a].animation_signal = 0;

assign_zombie_type(&data->zombies[a], rand() % 3);
assign_zombie_sprite(&data->zombies[a], &data->app_sprites);
}



data->camera.x_origin = (double)(get_window_w()) / 2;
data->camera.y_origin = (double)(get_window_h()) / 2;

data->camera.x = data->camera.x_origin;
data->camera.y = data->camera.y_origin;
data->camera.w = 800;
data->camera.h = 800;

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
//printf(" click left %d hammer click left %d \n", io->mouse_click_left, data->hammer.click.state);

data->hammer.x = io->mouse_x;
data->hammer.y = io->mouse_y;

if(io->key_arrow_up){
data->camera.y += 5;
}
if(io->key_arrow_down){
data->camera.y -= 5;
}
if(io->key_arrow_left){
data->camera.x -= 5;
}
if(io->key_arrow_right){
data->camera.x += 5;
}
//printf("c x %f c y %f\n", data->camera.x, data->camera.y);
////////////////////////
////////////////////////
return !data->quit_signal;
}


void update_logic(struct program_datas* data){
if(!data->update_logic_flag){ return; } 
////////////////////////
////////////////////////
// code goes here

update_hammer_animation_logic(&data->hammer);

// update zombies
for(int a = 0; a < ZOMBIES_COUNT; ++a){

if(data->zombies[a].death_signal == 1){
update_timer(&data->zombies[a].wandering_timer);

if( check_timer_delta_time_passed(&data->zombies[a].wandering_timer, data->zombies[a].death_timer_waiting_time) ){
data->zombies[a].death_signal = 0;
assign_zombie_sprite(&data->zombies[a], &data->app_sprites);
reset_timer(&data->zombies[a].wandering_timer);
}

}

if(data->zombies[a].death_signal == 1){ continue; }


if(data->hammer.click.state){
if( check_two_box_2d_hit_centralized((struct box_2d*)(&data->zombies[a]), (struct box_2d*) &data->hammer) ){
if(data->zombies[a].animation_signal == 0){ data->zombies[a].animation_signal = 1; }
data->zombies[a].hp -= 1;
if(data->zombies[a].hp <= 0){
data->zombies[a].stop_moving_signal = 1;
data->zombies[a].death_signal = 1;
reset_timer(&data->zombies[a].wandering_timer);
assign_zombie_type(&data->zombies[a], rand() % 3);
assign_zombie_sprite(&data->zombies[a], &data->app_sprites);
}

continue;
}
}

update_zombie_animation_logic(&data->zombies[a]);
/*
update_timer(&data->zombies[a].wandering_timer);
//printf(" delta time %f \n", data->zombies[a].wandering_timer.delta_time);
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
*/
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

clear_window_screen();


draw_box_centralized(&(struct box_2d){data->camera.x_origin, data->camera.y_origin, data->camera.w + 2, data->camera.h + 2}, (struct colors){0xFF, 0xFF, 0xFF, 0xFF});
draw_box_centralized(&(struct box_2d){data->camera.x_origin, data->camera.y_origin, data->camera.w, data->camera.h}, (struct colors){0x00, 0x00, 0x00, 0xFF});

for(int a = 0; a < ZOMBIES_COUNT; ++a){
draw_zombie(&data->zombies[a], &data->camera);
}
draw_hammer(&data->hammer);

draw_border(&data->camera);

static struct box_2d box1 = {100, 100, 100, 50};
static struct box_2d box2 = {200, 200, 100, 50};
static struct box_2d box3 = {100, 100, 100, 50};

struct io_data* io = &data->io;
box2.x = io->mouse_x;
box2.y = io->mouse_y;

draw_box_centralized(&(struct box_2d){box2.x, box2.y, box2.w + 2, box2.h + 2}, (struct colors){0xFF, 0x00, 0x00, 0xFF});
draw_box_centralized(&box2, (struct colors){0x00, 0x00, 0x00, 0xFF});
crop_box_2d_centralized(&box1, &box2, &box3);
printf("%f %f %f %f\n", box2.x, box2.y, box2.w, box2.h);
printf("%f %f %f %f\n", box3.x, box3.y, box3.w, box3.h);
draw_box_centralized(&box3, (struct colors){0xFF, 0xFF, 0xFF, 0xFF});

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






