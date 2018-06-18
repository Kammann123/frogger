#ifndef joydrv_h
#define joydrv_h



typedef struct {
	int16_t x;
	int16_t y;
	
} jcoord_t;


typedef enum {J_NOPRESS ,J_PRESS} jswitch_t;
typedef enum {JOY_ROTATE,JOY_NORMAL} jaxis_t;

#define J_INV_FALSE  1			// Keep Default x and y axis Direction
#define J_INV_TRUE  -1			// Invert default x and y axis Direction

#define forever for(;;)

// Joystick Services

void joy_init(void);
int joystick_update(void);
jcoord_t joystick_get_coord(void);
jswitch_t joystick_get_switch_value(void);
void set_joy_axis(uint8_t orientation);
void set_joy_direction(int8_t dir_x,int8_t dir_y);



#endif // End of driver definitions
