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
/////////////RENDER_FUNC_AREA////////////////////
/////////////////////////////////////////////////
void clear_window_screen(){
BeginDrawing();
ClearBackground((Color){0x00, 0x00, 0x00, 0xFF});
}

void clear_window_screen_color(struct colors color){
BeginDrawing();
ClearBackground((Color){color.r, color.g, color.b, color.a});
}

void update_window_screen(){
EndDrawing();
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
/*
img->angle_rad = 0.0f;
img->draw_centralized_mode = 0;
img->flip_h_mode = 0;
img->flip_v_mode = 0;

img->rotation_origin_x = 0.5f;
img->rotation_origin_y = 0.5f;

img->clip_box_x = 0.0f;
img->clip_box_y = 0.0f;
img->clip_box_w = img->w;
img->clip_box_h = img->h;

img->offset_x = 0.0f;
img->offset_y = 0.0f;

img->scale_w = 1;
img->scale_h = 1;

img->tint_color.r = 0xFF;
img->tint_color.g = 0xFF;
img->tint_color.b = 0xFF;
img->tint_color.a = 0xFF;
*/
img->draw_mode = 1;
return 1;
}

int load_img_ram(struct imgs* img, const char* file_path){

}


static inline void draw_imgs_core(struct imgs* img, double x, double y, double scale_w, double scale_h, double angle_rad, int flip_h, int flip_v, int center_draw, struct box_2d* clip){
register double clip_x_final;
register double clip_y;
register double clip_w;
register double clip_h;
if(clip == NULL){
clip_x_final = 0;
clip_y = 0;
clip_w = img->w;
clip_h = img->h;
}
else{
clip_x_final = clip->x;
clip_y = clip->y;
clip_w = (clip->w > img->w) ? img->w : clip->w;
clip_h = (clip->h > img->h) ? img->h : clip->h;
}


if(clip_w <= 0 || clip_h <= 0 || img->w <= 0 || img->h <= 0){ return; }

register double scale_w_final = scale_w;
register double scale_h_final =  2 - scale_h; // inverted scale due to inverted y coor

if(scale_w_final < 0){ scale_w_final = 0; }
if(scale_h_final < 0){ scale_h_final = 0; }

if(clip_x_final < 0){ clip_x_final = 0; }
if(clip_x_final > img->w){ clip_x_final = img->w; }
if(clip_x_final + clip_w > img->w){ clip_w = img->w - clip_x_final; }

if(clip_y < 0){ clip_y = 0; }
if(clip_y > img->h){ clip_y = img->h; }
if(clip_y + clip_h > img->h){ clip_h = img->h - clip_y; }

register double clip_y_final = -clip_y + (img->h - clip_h);

register double rotation_origin_x = 0.5 * clip_w * scale_w_final;
register double rotation_origin_y = 0.5 * clip_h * scale_h_final;

register double w_final = clip_w * scale_w_final;
register double h_final = clip_h * scale_h_final;

register double x_final =
x
+ rotation_origin_x // fixing weird raylib orgin act as offset
+ clip_x_final * scale_w_final; // make sure cliping act properly when move x_clip, the actual x move to compensate pos shifting

register double y_final =
get_window_h() - y - img->h // inverted draw, to bottom left begin
+ rotation_origin_y // fixing weird raylib orgin act as offset
+ clip_y_final * scale_h_final // make sure cliping act properly when move x_clip, the actual x move to compensate pos shifting
+ (1 - scale_h_final) * img->h; // since it is inverted and have an init y offset, doing this make sure the offset scale


if(center_draw){
x_final += -w_final / 2 - clip_x_final;
y_final += h_final / 2 + clip_y;
}


DrawTexturePro(
*((Texture*)img->data_vram),
(Rectangle){clip_x_final, clip_y_final, (flip_h) ? -clip_w : clip_w, (flip_v) ? -clip_h : clip_h},
(Rectangle){x_final, y_final, w_final, h_final},
(Vector2){rotation_origin_x, rotation_origin_y},
angle_rad * 180 / PI,
(Color){0xFF, 0xFF, 0xFF, 0xFF}
);
}


void draw_imgs(struct imgs* img, double x, double y, double scale_w, double scale_h, double angle_rad, int flip_h, int flip_v, int center_draw){
draw_imgs_core(img, x, y, scale_w, scale_h, angle_rad, flip_h, flip_v, center_draw, NULL);
}


void draw_imgs_part(struct imgs* img, double x, double y, double scale_w, double scale_h, double angle_rad, int flip_h, int flip_v, int center_draw, struct box_2d* clip){
draw_imgs_core(img, x, y, scale_w, scale_h, angle_rad, flip_h, flip_v, center_draw, clip);
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


io->key_f = IsKeyDown(KEY_F);
io->key_g = IsKeyDown(KEY_G);


io->key_a = IsKeyDown(KEY_A);
io->key_d = IsKeyDown(KEY_D);
io->key_w = IsKeyDown(KEY_W);
io->key_s = IsKeyDown(KEY_S);


io->key_h = IsKeyDown(KEY_H);
io->key_j = IsKeyDown(KEY_J);
io->key_u = IsKeyDown(KEY_U);
io->key_k = IsKeyDown(KEY_K);

io->key_x = IsKeyDown(KEY_X);
io->key_z = IsKeyDown(KEY_Z);


io->key_space = IsKeyDown(KEY_SPACE);


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





