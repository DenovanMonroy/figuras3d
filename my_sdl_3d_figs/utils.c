
#include <stdio.h>
#include <math.h>
#include "utils.h"


extern unsigned int pixel[WIDTH*HEIGHT];


void swap(int *a, int *b){
	int aux = *a;
	*a = *b;
	*b = aux;
}



/********* MARIO section *********/

// Mario colors
int piel[3] = {247, 164, 34};
int rojo[3] = {255, 23, 3};
int cafe[3] = {133, 75, 12};
int fondo[3] = {0, 0, 0};

// Mario sprite
extern int mario[16][16][3];
char* src_name  = "marioRCP.txt";

void set_pixel_sprite(int i, int j, int c[3]){
	mario[i][j][0] = c[0];
	mario[i][j][1] = c[1];
	mario[i][j][2] = c[2];
}

int load_mario_sprite(){
	FILE *src = fopen(src_name, "r");
	if (!src) return 0;	// error

	char c;
	for (int j=15; j>=0; j--){
		for (int i=0; i<16; i++){
			fscanf(src, "%c", &c);
			switch(c){
			case 'P':
				set_pixel_sprite(i, j, piel);
				break;
			case 'R':
				set_pixel_sprite(i, j, rojo);
				break;
			case 'C':
				set_pixel_sprite(i, j, cafe);
				break;
			default:
				set_pixel_sprite(i, j, fondo);
				break;
			}
		}
		fscanf(src, "%c", &c);
	}

	fclose(src);
	return 1;	//success
}

void draw_mario_sprite(int tx, int ty, int sx, int sy, float ang){
	ang *= PI/180.0;
	for (int j=0; j<16; j++){
		for (int i=0; i<16; i++){
			int x = sx*(i-8);
			int y = sy*(j-8);

			float x2 = x*cos(ang) - y*sin(ang);
			float y2 = x*sin(ang) + y*cos(ang);

			x = tx+(int)x2+8;
			y = ty+(int)y2+8;

			set_pixel(x, y, mario[i][j]);
			set_pixel(x-1, y, mario[i][j]);
			set_pixel(x+1, y, mario[i][j]);
			set_pixel(x, y-1, mario[i][j]);
			set_pixel(x, y+1, mario[i][j]);
		}
	}
	
}

/********* MARIO section *********/


//extern int cube_top   [16][16][3];
//extern int cube_side  [16][16][3];
//extern int cube_bottom[16][16][3];

int load_ppm(char* file_name, int sprite[16][16][3]){
	FILE *archi = fopen(file_name, "r");
	if (!archi) return 0;	// error

	int w, h, aux;
	fscanf(archi, "P3\n");
	fscanf(archi, "%d %d\n", &w, &h);
	fscanf(archi, "%d\n", &aux);

	// valores RGB
	for (int j=h-1; j>=0; j--)
		for (int i=0; i<w; i++)
			for (int k=0; k<3; k++)
				fscanf(archi, "%d ", &sprite[i][j][k]);

	fclose(archi);
	return 1;	// success
}



// Fill image with color
void init_image(unsigned int color) {
	for (int j=HEIGHT-1; j>=0; j--)
		for (int i=0; i<WIDTH; i++)
			pixel[i+j*WIDTH] = color; // color in RGBA
}

// Change image
void change_image_test() {
	for (int j=HEIGHT-1; j>=0; j--)
		for (int i=0; i<WIDTH; i++)
			pixel[i+j*WIDTH] += 0x100;
}

// Set pixel with RGBA values
void set_pixel(int i, int j, int c[3]){
	if (i<0 || j<0 || i>=WIDTH || j>= HEIGHT)
		return;
	j = HEIGHT-j-1;
	unsigned int p = c[0];
	pixel[i+j*WIDTH] = ((p<<8|c[1])<<8|c[2])<<8|255;
}



