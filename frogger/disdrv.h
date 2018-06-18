#ifndef disdrv_h
#define disdrv_h

typedef enum {NORMAL ,ROTATE} daxis_t;

typedef enum {D_OFF, D_ON} dlevel_t;

#define MAX_X		    	15
#define MAX_Y		    	15
#define MIN_X		    	0
#define MIN_Y		    	0


typedef struct {
	uint8_t x;
	uint8_t y;
} dcoord_t;


#define set_coord(x,y) (dcoord_t){x,y}


// Display Services

void display_init(void);
void display_clear(void);
void display_write(uint8_t x,uint8_t y, dlevel_t val);
void display_update(void);
void set_display_axis(uint8_t orientation);







#endif // End of driver definitions
