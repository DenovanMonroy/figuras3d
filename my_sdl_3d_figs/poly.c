
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "my2d.h"

#define MAX_INTERSECT 100

// intersections list
typedef struct {
    int y;
    int x[MAX_INTERSECT];
    int count;
}scanline;


void create_reg_poly(int x, int y, int r, int n, int v[][2]){
	float x2, y2, ang=90.0;
	for (int i=0; i<n; i++, ang+=360.0/n){
		x2 = r*cos(ang*PI/180);
		y2 = r*sin(ang*PI/180);
		v[i][0] = (int)x2 + x;
		v[i][1] = (int)y2 + y;
	}
}

void create_reg_star(int x, int y, int r, int n, int v[][2]){
	// TO DO
}


void draw_poly(int v[][2], int n, int c[3]){
	for (int i=0; i<n-1; i++)
		draw_line(v[i][0], v[i][1], v[i+1][0], v[i+1][1], c);
	draw_line(v[n-1][0], v[n-1][1], v[0][0], v[0][1], c);
}


int intersect(int p1[2], int p2[2], int y){
	int x1 = p1[0];
	int y1 = p1[1];
	int x2 = p2[0];
	int y2 = p2[1];

	if (y1 == y2) return -1;

	if (y2<y1){
		swap(&y1, &y2);
		swap(&x1, &x2);
	}

	if (y1<y && y<y2) {
		float x = x1 + (y-y1)*(x2-x1)/(y2-y1);
		return (int)x;
	}

	return -1;
}

void sort(int arr[], int n) {
	for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
void draw_fill_poly(int v[][2], int n, int color[3]) {
    if (n < 3) {
        printf("Error: Un polígono debe tener al menos 3 vértices.\n");
        return;
    }

    // Encontrar el rango de y (mínimo y máximo)
    int miny = v[0][1];
    int maxy = v[0][1];
    for (int i = 1; i < n; i++) {
        if (v[i][1] < miny) miny = v[i][1];
        if (v[i][1] > maxy) maxy = v[i][1];
    }

    // Escanear cada línea horizontal
    for (int y = miny; y <= maxy; y++) {
        int intersections[MAX_INTERSECT];
        int count = 0;

        // Encontrar intersecciones con cada borde
        for (int i = 0; i < n; i++) {
            int x1 = v[i][0];
            int y1 = v[i][1];
            int x2 = v[(i + 1) % n][0];
            int y2 = v[(i + 1) % n][1];

            // Ignorar bordes horizontales
            if (y1 == y2) continue;

            // Asegurar que y1 < y2
            if (y1 > y2) {
                swap(&x1, &x2);
                swap(&y1, &y2);
            }

            // Verificar si la línea horizontal cruza el borde
            if (y >= y1 && y < y2) {
                int x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                intersections[count++] = x;
            }
        }

        // Ordenar las intersecciones
        sort(intersections, count);

        // Dibujar líneas entre pares de intersecciones
        for (int i = 0; i < count; i += 2) {
            if (i + 1 < count) {
                draw_line(intersections[i], y, intersections[i + 1], y, color);
            }
        }
    }
}
void draw_textured_poly(int v[][2], int n, int sprite[16][16][3], float m_inv[2][2], int rotate_sprite) {
    if (n < 3) {
        printf("Error: Un polígono debe tener al menos 3 vértices.\n");
        return;
    }

    // Encontrar el rango de y (mínimo y máximo)
    int miny = v[0][1];
    int maxy = v[0][1];
    for (int i = 1; i < n; i++) {
        if (v[i][1] < miny) miny = v[i][1];
        if (v[i][1] > maxy) maxy = v[i][1];
    }

    // Escanear cada línea horizontal
    for (int y = miny; y <= maxy; y++) {
        int intersections[MAX_INTERSECT];
        int count = 0;

        // Encontrar intersecciones con cada borde
        for (int i = 0; i < n; i++) {
            int x1 = v[i][0];
            int y1 = v[i][1];
            int x2 = v[(i + 1) % n][0];
            int y2 = v[(i + 1) % n][1];

            // Ignorar bordes horizontales
            if (y1 == y2) continue;

            // Asegurar que y1 < y2
            if (y1 > y2) {
                swap(&x1, &x2);
                swap(&y1, &y2);
            }

            // Verificar si la línea horizontal cruza el borde
            if (y >= y1 && y < y2) {
                int x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                intersections[count++] = x;
            }
        }

        // Ordenar las intersecciones
        sort(intersections, count);

        // Verificar que el número de intersecciones sea par
        if (count % 2 != 0) {
            printf("Error: Número impar de intersecciones detectado en y = %d.\n", y);
            continue;
        }

        // Dibujar líneas entre pares de intersecciones
        for (int i = 0; i < count; i += 2) {
            if (i + 1 < count) {
                for (int j = intersections[i]; j < intersections[i + 1]; j++) {
                    // Calcular coordenadas de textura
                    float ii = m_inv[0][0] * (j - v[0][0]) + m_inv[0][1] * (y - v[0][1]);
                    float jj = m_inv[1][0] * (j - v[0][0]) + m_inv[1][1] * (y - v[0][1]);

                    // Verificar si las coordenadas de textura son válidas
                    if (ii >= 0 && ii < 1 && jj >= 0 && jj < 1) {
                        if (rotate_sprite) {
                            set_pixel(j, y, sprite[(int)(16 - ii * 16)][(int)(16 - jj * 16)]);
                        } else {
                            set_pixel(j, y, sprite[(int)(ii * 16)][(int)(jj * 16)]);
                        }
                    }
                }
            }
        }
    }
}