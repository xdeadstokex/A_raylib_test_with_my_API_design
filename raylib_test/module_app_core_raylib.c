#include "module_app_core.h"

#include "raylib.h"

/////////////////////////////////////////////////
//////////////WINDOW_SECTION/////////////////////
/////////////////////////////////////////////////
int init_module(){
return 1;
}

int init_window(int w, int h, const char* window_name){
InitWindow(w, h, window_name);
return 1;
}


void cleanup_module(){
CloseWindow();
}

int get_window_w(){ return GetScreenWidth(); }
int get_window_h(){ return GetScreenHeight(); }

void set_window_fps(int fps){
SetTargetFPS(fps);
}


/////////////////////////////////////////////////
////////////COMMON_LOGIC_SECTION/////////////////
/////////////////////////////////////////////////
int check_point_in_area_2d_debug(register double x, register double y, register double x1_begin, register double y1_begin, register double x1_end, register double y1_end){
return (x >= x1_begin && x <= x1_end && y >= y1_begin && y <= y1_end);
}

int check_area_overlap_2d_debug(register double x1_begin, register double y1_begin, register double x1_end, register double y1_end, register double x2_begin, register double y2_begin, register double x2_end, register double y2_end){
if(x2_begin > x1_end || x2_end < x1_begin || y2_begin > y1_end || y2_end < y1_begin) return 0;
return 1;
}

int check_point_in_area_2d(struct pos_2d pos, struct box_2d box){
return (
pos.x >= box.x && pos.x <= box.x + box.w && 
pos.y >= box.y && pos.y <= box.y + box.h
);
}


int check_area_overlap_2d(struct pos_2d area1_start, struct pos_2d area1_end, struct pos_2d area2_start, struct pos_2d area2_end){
return check_area_overlap_2d_debug(area1_start.x, area1_start.y, area1_end.x, area1_end.y, area2_start.x, area2_start.y, area2_end.x, area2_end.y);
}


int check_two_box_2d_hit_centralized(const struct box_2d* box1, const struct box_2d* box2){
if(box1->w <= 0 || box2->w <= 0 || box1->h <= 0 || box2->h <= 0){ return 0; }
// check boxes x range non-collision
if(box2->x - box2->w / 2 > box1->x + box1->w / 2){ return 0; }
if(box2->x + box2->w / 2 < box1->x - box1->w / 2){ return 0; }
// check boxes y range non-collision
if(box2->y - box2->h / 2 > box1->y + box1->h / 2){ return 0; }
if(box2->y + box2->h / 2 < box1->y - box1->h / 2){ return 0; }

return 1;// boxes collide (overlap on both x and y axes)
}

void set_box_2d(struct box_2d* box, register double x, register double y, register double w, register double h){
box->x = x; box->y = y;
box->w = w; box->h = h;
}

void set_pos_2d(struct pos_2d* pos, register double x, register double y){
pos->x = x; pos->y = y;
}

/////////////////////////////////////////////////
/////////////RENDER_FUNC_AREA////////////////////
/////////////////////////////////////////////////
void draw_begin(){ BeginDrawing(); }
void draw_end(){ EndDrawing(); }

void clear_window_screen(){
ClearBackground((Color){0x00, 0x00, 0x00, 0xFF});
}

void clear_window_screen_color(struct colors color){
ClearBackground((Color){color.r, color.g, color.b, color.a});
}

void draw_box(const struct box_2d* box, struct colors color){
DrawRectangle(
(float) box->x, get_window_h() - (float) box->y - (float) box->h, (float) box->w, (float) box->h,
(Color){color.r, color.g, color.b, color.a}
);
}

void draw_box_rotate(const struct box_2d* box, double angle_rad, struct colors color){
DrawRectanglePro(
(Rectangle){(float) box->x + (float) box->w / 2, get_window_h() - (float) box->y - (float) box->h / 2, (float) box->w, (float) box->h},
(Vector2){(float) box->w / 2, (float) box->h / 2},
angle_rad * 180 / PI,
(Color){color.r, color.g, color.b, color.a}
);
}

void draw_box_centralized(struct box_2d* box, struct colors color){
DrawRectangle(
(float)(box->x - box->w / 2), get_window_h() - (float) box->y - (float) box->h / 2, (float) box->w, (float) box->h,
(Color){color.r, color.g, color.b, color.a}
);
}

void draw_box_basic(register double x, register double y, register double w, register double h, struct colors color){
DrawRectangle(
(float) x, get_window_h() - (float) y - (float) h, (float) w, (float) h,
(Color){color.r, color.g, color.b, color.a}
);
}



int load_img_vram(struct imgs* img, const char* file_path){
Texture2D texture = LoadTexture(file_path);
img->data_vram = malloc(sizeof(Texture2D));
if(img->data_vram == NULL){ return 0; }
memcpy(img->data_vram, &texture, sizeof(Texture2D));

img->w = texture.width; img->h = texture.height;
//    int mipmaps;            // Mipmap levels, 1 by default
//    int format;             // Data format (PixelFormat type)
// later

img->draw_mode = 1;
return 1;
}

int load_img_ram(struct imgs* img, const char* file_path){

}

void draw_img(struct imgs* img, double x, double y){
DrawTexture(*((Texture*)img->data_vram), x, get_window_h() - y - img->h, (Color){0xFF, 0xFF, 0xFF, 0xFF});
}

void draw_img_centralized(struct imgs* img, double x, double y){
DrawTexture(*((Texture*)img->data_vram), x - img->w / 2, get_window_h() - y - img->h / 2, (Color){0xFF, 0xFF, 0xFF, 0xFF});
}

void draw_img_centralized_rotate(struct imgs* img, double x, double y, double angle_rad){
DrawTexturePro(*((Texture*)img->data_vram), (Rectangle){0, 0, img->w, img->h}, (Rectangle){x, get_window_h() - y, img->w, img->h}, (Vector2){img->w / 2, img->h / 2}, angle_rad * 180 / PI, (Color){0xFF, 0xFF, 0xFF, 0xFF});
}


/////////////////////////////////////////////////
//////////////GUI_HELPER_SECTION/////////////////
/////////////////////////////////////////////////
void reset_click(struct clicks* click){
click->state = 0;
click->signal_last = 0;
return;
}

void update_click_hold(struct clicks* click, int signal){
click->state = signal;
return;
}

void update_click_switch(struct clicks* click, int signal){
if(signal && !click->signal_last){ click->state = !click->state; }
click->signal_last = signal;
return;
}

void update_click_once(struct clicks* click, int signal){
if(signal == 1){
if(click->signal_last == 1){ click->state = 0; }
else{ click->state = 1; }
}
click->signal_last = signal;
return;
}
/////////////////////////////////////////////////
//////////////////IO_SECTION/////////////////////
/////////////////////////////////////////////////
void init_io_data(struct io_data* io){
if(io != NULL) memset(io, 0x00, sizeof(struct io_data));
}

void get_io_signal(struct io_data* io, int mouse_signal, int kb_signal){
io->mouse_motion = 0;
io->mouse_scroll_horizontal = 0;
io->mouse_scroll_vertical = 0;
io->mouse_scroll = 0;

io->quit = WindowShouldClose();

if(mouse_signal){
io->mouse_x = GetMouseX();
io->mouse_y = get_window_h() - GetMouseY() - 1;

io->mouse_click_left = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
io->mouse_click_right = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
}
if(kb_signal){

io->key_arrow_up = IsKeyDown(KEY_UP);
io->key_arrow_down = IsKeyDown(KEY_DOWN);
io->key_arrow_left = IsKeyDown(KEY_LEFT);
io->key_arrow_right = IsKeyDown(KEY_RIGHT);




}
}



/////////////////////////////////////////////////
////////////////TIME_SECTION/////////////////////
/////////////////////////////////////////////////
void reset_timer(struct timers* timer){
timer->time_current = GetTime();
timer->time_last = GetTime();
timer->delta_time = 0;
return;
}

void update_timer(struct timers* timer){
timer->time_current = GetTime();
timer->delta_time = timer->time_current - timer->time_last;
return;
}

int check_timer_delta_time_passed(struct timers* timer, double tagret_delta_time_second){
//if(timer == NULL){ printf("time debug %f\n", GetTime()); }

if(timer == NULL){ return 0; }
return  (timer->delta_time >= tagret_delta_time_second);
}





