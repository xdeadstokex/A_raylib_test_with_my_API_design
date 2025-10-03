#ifndef MODULE_COMMON_LOGIC_H
#define MODULE_COMMON_LOGIC_H

/////////////////////////////////////////////////
////////////////////DATA_SECTION/////////////////
/////////////////////////////////////////////////
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

/////////////////////////////////////////////////
//////////////QUICK_COPY_SECTION/////////////////
/////////////////////////////////////////////////
/*
check_point_in_area_2d_debug(register double x, register double y, register double x1_begin, register double y1_begin, register double x1_end, register double y1_end) // static inline int
check_area_overlap_2d_debug(register double x1_begin, register double y1_begin, register double x1_end, register double y1_end, register double x2_begin, register double y2_begin, register double x2_end, register double y2_end) // static inline int
check_point_in_area_2d(struct pos_2d pos, struct box_2d box) // static inline int
check_area_overlap_2d(struct pos_2d area1_start, struct pos_2d area1_end, struct pos_2d area2_start, struct pos_2d area2_end) // static inline int

check_two_box_2d_hit(const struct box_2d* box1, const struct box_2d* box2) // static inline int
check_two_box_2d_hit_centralized(const struct box_2d* box1, const struct box_2d* box2) // static inline int
check_point_in_box_2d_centralized(struct pos_2d* pos, struct box_2d* box) // static inline int

set_box_2d(struct box_2d* box, register double x, register double y, register double w, register double h) // static inline void 
static inline void set_pos_2d(struct pos_2d* pos, register double x, register double y) // static inline void 

crop_box_2d(struct box_2d* src_box, struct box_2d* crop_area, struct box_2d* return_box)  // static inline void 
crop_box_2d_centralized(struct box_2d* src_box, struct box_2d* crop_area, struct box_2d* return_box) // static inline void 
*/

/////////////////////////////////////////////////
////////////////////FUNC_SECTION/////////////////
/////////////////////////////////////////////////
static inline int check_point_in_area_2d_debug(register double x, register double y, register double x1_begin, register double y1_begin, register double x1_end, register double y1_end){
return (x >= x1_begin && x <= x1_end && y >= y1_begin && y <= y1_end);
}

static inline int check_area_overlap_2d_debug(register double x1_begin, register double y1_begin, register double x1_end, register double y1_end, register double x2_begin, register double y2_begin, register double x2_end, register double y2_end){
if(x2_begin > x1_end || x2_end < x1_begin || y2_begin > y1_end || y2_end < y1_begin) return 0;
return 1;
}

static inline int check_point_in_area_2d(struct pos_2d pos, struct box_2d box){
return (
pos.x >= box.x && pos.x <= box.x + box.w && 
pos.y >= box.y && pos.y <= box.y + box.h
);
}


static int check_point_in_box_2d_basic(
register double x0, register double y0,
register double x1, register double y1, register double w1, register double h1){
return (
x0 >= x1 && x0 <= x1 + w1 && 
y0 >= y1 && y0 <= y1 + h1
);
}


static inline int check_area_overlap_2d(struct pos_2d area1_start, struct pos_2d area1_end, struct pos_2d area2_start, struct pos_2d area2_end){
return check_area_overlap_2d_debug(area1_start.x, area1_start.y, area1_end.x, area1_end.y, area2_start.x, area2_start.y, area2_end.x, area2_end.y);
}


static inline int check_two_box_2d_hit(const struct box_2d* box1, const struct box_2d* box2){
if(box1->w <= 0 || box2->w <= 0 || box1->h <= 0 || box2->h <= 0){ return 0; }
// check boxes x range non-collision
if(box2->x > box1->x + box1->w){ return 0; }
if(box2->x + box2->w < box1->x){ return 0; }
// check boxes y range non-collision
if(box2->y > box1->y + box1->h){ return 0; }
if(box2->y + box2->h < box1->y){ return 0; }

return 1;// boxes collide (overlap on both x and y axes)
}

static inline int check_two_box_2d_hit_basic(
register double x0, register double y0, register double w0, register double h0,
register double x1, register double y1, register double w1, register double h1){
if(w0 <= 0 || w1 <= 0 || h0 <= 0 || h1 <= 0){ return 0; }
// check boxes x range non-collision
if(x1 > x0 + w0){ return 0; }
if(x1 + w1 < x0){ return 0; }
// check boxes y range non-collision
if(y1 > y0 + h0){ return 0; }
if(y1 + h1 < y0){ return 0; }

return 1;// boxes collide (overlap on both x and y axes)
}

static inline int check_two_box_2d_hit_centralized(const struct box_2d* box1, const struct box_2d* box2){
if(box1->w <= 0 || box2->w <= 0 || box1->h <= 0 || box2->h <= 0){ return 0; }
// check boxes x range non-collision
if(box2->x - box2->w / 2 > box1->x + box1->w / 2){ return 0; }
if(box2->x + box2->w / 2 < box1->x - box1->w / 2){ return 0; }
// check boxes y range non-collision
if(box2->y - box2->h / 2 > box1->y + box1->h / 2){ return 0; }
if(box2->y + box2->h / 2 < box1->y - box1->h / 2){ return 0; }

return 1;// boxes collide (overlap on both x and y axes)
}

static inline int check_point_in_box_2d_centralized(struct pos_2d* pos, struct box_2d* box){
return (
pos->x >= box->x - box->w / 2 &&
pos->x <= box->x + box->w / 2 &&
pos->y >= box->y - box->h / 2 &&
pos->y <= box->y + box->h / 2
);
}

static inline void set_box_2d(struct box_2d* box, register double x, register double y, register double w, register double h){
box->x = x; box->y = y;
box->w = w; box->h = h;
}

static inline void set_pos_2d(struct pos_2d* pos, register double x, register double y){
pos->x = x; pos->y = y;
}


static inline void crop_box_2d(struct box_2d* src_box, struct box_2d* crop_area, struct box_2d* return_box){
*return_box = *src_box;
if( !check_two_box_2d_hit(src_box, crop_area) ){ return; }

register double crop_x_end = crop_area->x + crop_area->w;
register double return_x_end = return_box->x + return_box->w;
register double crop_y_end = crop_area->y + crop_area->h;
register double return_y_end = return_box->y + return_box->h;

if(crop_area->x >= return_box->x && crop_x_end <= return_x_end){
return_box->x = crop_area->x;
return_box->w = crop_area->w;
}
else if(crop_area->x < return_box->x && crop_x_end < return_x_end){
return_box->w = crop_x_end - return_box->x;
}
else if(crop_area->x > return_box->x && crop_x_end > return_x_end){
return_box->x = crop_area->x;
return_box->w = return_x_end - crop_area->x;
}

if(crop_area->y >= return_box->y && crop_y_end <= return_y_end){
return_box->y = crop_area->y;
return_box->h = crop_area->h;
}
else if(crop_area->y < return_box->y && crop_y_end < return_y_end){
return_box->h = crop_y_end - return_box->y;
}
else if(crop_area->y > return_box->y && crop_y_end > return_y_end){
return_box->y = crop_area->y;
return_box->h = return_y_end - crop_area->y;
}
}

static inline void crop_box_2d_centralized(struct box_2d* src_box, struct box_2d* crop_area, struct box_2d* return_box){
*return_box = *src_box;
if( !check_two_box_2d_hit_centralized(src_box, crop_area) ){ return; }

register double crop_x_start = crop_area->x - crop_area->w / 2;
register double return_x_start = return_box->x - return_box->w / 2;
register double crop_y_start = crop_area->y - crop_area->h / 2;
register double return_y_start = return_box->y - return_box->h / 2;

register double crop_x_end = crop_area->x + crop_area->w / 2;
register double return_x_end = return_box->x + return_box->w / 2;
register double crop_y_end = crop_area->y + crop_area->h / 2;
register double return_y_end = return_box->y + return_box->h / 2;

if(crop_x_start >= return_x_start && crop_x_end <= return_x_end){
return_box->x = crop_x_start;
return_box->w = crop_area->w;
}
else if(crop_x_start < return_x_start && crop_x_end < return_x_end){
return_box->w = crop_x_end - return_x_start;
}
else if(crop_x_start > return_x_start && crop_x_end > return_x_end){
return_box->x = crop_x_start;
return_box->w = return_x_end - crop_x_start;
}

if(crop_y_start >= return_y_start && crop_y_end <= return_y_end){
return_box->x = crop_y_start;
return_box->w = crop_area->h;
}
else if(crop_y_start < return_y_start && crop_y_end < return_y_end){
return_box->h = crop_y_end - return_y_start;
}
else if(crop_y_start > return_y_start && crop_y_end > return_y_end){
return_box->y = crop_y_start;
return_box->h = return_y_end - crop_y_start;
}
}


#endif // MODULE_COMMON_LOGIC_H

