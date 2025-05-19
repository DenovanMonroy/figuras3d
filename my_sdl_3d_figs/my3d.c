
#include <stdio.h>
#include <math.h>

#include "utils.h"	// Include the header for utils functions
#include "my2d.h"
#include "poly.h"
#include "my3d.h"

//extern camera cam;
//extern transform obj;

//extern int mario[16][16][3];

extern int cube_top   [16][16][3];
extern int cube_side  [16][16][3];
extern int cube_bottom[16][16][3];



void draw_vert(int i, int j, int c[3]){
	//draw_circle(i, j, 5, c);
	set_pixel(i, j, c);
	set_pixel(i-1, j, c);
	set_pixel(i+1, j, c);
	set_pixel(i, j-1, c);
	set_pixel(i, j+1, c);
}


// Basic transforms
void scale(float v[3], float esc){
	v[0] *= esc;
	v[1] *= esc;
	v[2] *= esc;
}

void scale2(float v[3], float escX, float escY, float escZ){
	
	v[0] *= escX;
	v[1] *= escY;
	v[2] *= escZ;
}

void translate(float v[3], float x, float y, float z){
	v[0] += x;
	v[1] += y;
	v[2] += z;
}

void rotateX(float v[3], float ang){
	// TO DO
	// x2 = x
	// y2 = y*cos(ang) - z*sin(ang)
	// z2 = y*sin(ang) + z*cos(ang)
	ang*= PI/180.0;
	float y2 = v[1]*cos(ang) - v[2]*sin(ang);
	float z2 = v[1]*sin(ang) + v[2]*cos(ang);
	v[1] = y2;
	v[2] = z2;
}

void rotateY(float v[3], float ang){

	// TO DO
	// x2 = x*cos(ang) + z*sin(ang)
	// y2 = y
	// z2 = -x*sin(ang) + z*cos(ang)
	ang*= PI/180.0;
	float x2 = v[0]*cos(ang) + v[2]*sin(ang);
	float z2 = -v[0]*sin(ang) + v[2]*cos(ang);
	v[0] = x2;
	v[2] = z2;
}

void rotateZ(float v[3], float ang){
	// TO DO
	// x2 = x*cos(ang) - y*sin(ang)
	// y2 = x*sin(ang) + y*cos(ang)
	ang*= PI/180.0;
	float x2 = v[0]*cos(ang) - v[1]*sin(ang);
	float y2 = v[0]*sin(ang) + v[1]*cos(ang);
	v[0] = x2;
	v[1] = y2;
}


// find hidden faces

/*
	| i	j	k  |
	| ax	ay	az | =
	| bx	by	bz |

	(ay*bz - az*by)i - (ax*bz - az*bx)j + (ax*by - ay*bx)k
*/
void cross_prod(float a[3], float b[3], float c[3]){
	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];

	// TO DO
}

float dot_prod(float a[3], float b[3]){
	
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

int points_towards_cam(mesh *m, int face_idx, camera cam, float n[3]){
	float ab[3], ad[3], aux[3];

	float *A, *B, *C, *D;
	int *idxs = (int*)&(m->faces[face_idx]);
	A = (float*)&(m->verts[idxs[0]]);
	B = (float*)&(m->verts[idxs[1]]);
	C = (float*)&(m->verts[idxs[2]]);
	D = (float*)&(m->verts[idxs[3]]);

	ab[0] = B[0]-A[0];
	ab[1] = B[1]-A[1];
	ab[2] = B[2]-A[2];

	ad[0] = D[0]-A[0];
	ad[1] = D[1]-A[1];
	ad[2] = D[2]-A[2];

	// calculate normal
	cross_prod(ab, ad, n);
	
	// TO DO

	aux[0] = n[0];
	aux[1] = n[1];
	aux[2] = n[2];

	rotateY(aux, cam.angY);
	rotateX(aux, cam.angX);
	rotateZ(aux, cam.angZ);
	
	// apply camera rotation
	// TO DO

	// using flat projection
	return (aux[2] > 0.0);
}

// map image to quad (asuming flat projection)
void map_img2quad(int quad[4][2], int sprite[16][16][3]){
	float ab[2], ad[2], delta = 1.0/16.0;

	ab[0] = quad[1][0] - quad[0][0];
	ab[1] = quad[1][1] - quad[0][1];

	ad[0] = quad[3][0] - quad[0][0];
	ad[1] = quad[3][1] - quad[0][1];

	// TO DO

	for (int i=0; i<16; i++){
		for (int j=0; j<16; j++){
			set_pixel(quad[0][0]+ab[0]*i*delta+ad[0]*j*delta,quad[0][1]+ab[1]*i*delta+ad[1]*j*delta , sprite[i][j]);
		}
	}

}

int calc_m_inv(int quad[4][2],int n ,float m_inv[2][2]){
	float ab[2], ad[2];
	ab[0] = quad[1][0] - quad[0][0];
	ab[1] = quad[1][1] - quad[0][1];
	ad[0] = quad[n-1][0] - quad[0][0];
	ad[1] = quad[n-1][1] - quad[0][1];
	// TO DO

	/*
	// This would be the matrix m
	m[0][0] = ab[0]; m[0][1] = ad[0];
	m[1][0] = ab[1]; m[1][1] = ad[1];
	*/

	float detM = ab[0]*ad[1] - ab[1]*ad[0];
	// check if detM is close to zero
	if (detM <= 0.000001 && detM >= -0.000001){
		//printf("WARNING: Determinant zero\n");
		return 0;
	}

	m_inv[0][0] = ad[1]/detM;
	m_inv[0][1] = -ad[0]/detM;
	m_inv[1][0] = -ab[1]/detM;
	m_inv[1][1] = ab[0]/detM;

	// So the inverse of m is:
	// TO DO
	// now we can transform from 3D coords to sprite (texture) coords
	return 1;
}



// vertices management

void project(float vv[3], int scr[2], camera cam){
	float v[3];
	v[0] = vv[0];
	v[1] = vv[1];
	v[2] = vv[2];

	// camera rotation
	rotateY(v, cam.angY);
	rotateX(v, cam.angX);
	rotateZ(v, cam.angZ);

	// projection
	float x_scr = v[0];
	float y_scr = v[1];
	if (!cam.is_flat_proj){
		x_scr = cam.d*x_scr/(v[2]+cam.D);
		y_scr = cam.d*y_scr/(v[2]+cam.D);
	}

	// viewport transform
	scr[0] = SEMI_WIDTH  + VIEW_SCALE*x_scr;
	scr[1] = SEMI_HEIGHT + VIEW_SCALE*y_scr;
}

void transform_verts(transform obj, mesh *m){

	for (int i=0;i<m->nv;i++){
		m-> verts[i][0] = m->base_verts[i][0];
		m-> verts[i][1] = m->base_verts[i][1];
		m-> verts[i][2] = m->base_verts[i][2];
		
		
		scale(m->verts[i], obj.esc);
		rotateY(m->verts[i], obj.angY);
		rotateX(m->verts[i], obj.angX);
		rotateZ(m->verts[i], obj.angZ);
		translate(m->verts[i], obj.tx, obj.ty, obj.tz);
	    }
}

void draw_verts(mesh *m, int c[3], camera cam){
	// TO DO
	// para cada vertice
	//	si se encuentra frente a la cámara
	//		proyecta sobre la pantalla
	//		dibuja el vertice sobre la pantalla
	for(int i=0; i<m->nv; i++){
	   if(m->verts[i][2]+cam.D<0.1){
		int scr[2];

		project(m->verts[i], scr, cam);
		draw_vert(scr[0], scr[1], c);
	   }


}
}

void draw_edges(mesh *m, camera cam){
	// TO DO
	// para cada arista
	//	obtener los indices de los dos vertices
	//	si ambos vertices estan frente a la camara
	//		proyectar los dos vertices
	//		dibujar la linea correspondiente
	for(int i=0; i<m->ne; i++){
		int idx1 = m->edges[i][0];
		int idx2 = m->edges[i][1];

		if (m->verts[idx1][2]+cam.D<-0.1 && m->verts[idx2][2]+cam.D<-0.1){
			int scr[2], scr2[2];

			project(m->verts[idx1], scr, cam);
			project(m->verts[idx2], scr2, cam);

			draw_line(scr[0], scr[1], scr2[0], scr2[1], m->color);
		}
	}
}

void draw_faces0(mesh *m, int c[3], camera cam, int sprite[16][16][3]){
	// TO DO
	// para cada cara poligonal (quad)
	//	obtener los indices de los 4 vertices
	//	proyectar los 4 vertices
	//	si la cara apunta hacia la cámara
	//		dibujar la cara
	//		mapear la imagen a la cara
	for(int i=0; i<m->nf; i++){
		int idx1 = m->faces[i][0];
		int idx2 = m->faces[i][1];
		int idx3 = m->faces[i][2];
		int idx4 = m->faces[i][3];
		float n[3];
		if (  points_towards_cam(m,i,cam,n)&&m->verts[idx1][2]+cam.D<-0.1 && m->verts[idx2][2]+cam.D<-0.1 &&
		    m->verts[idx3][2]+cam.D<-0.1 && m->verts[idx4][2]+cam.D<-0.1){

			int scr[4][2];

			project(m->verts[idx1], scr[0], cam);
			project(m->verts[idx2], scr[1], cam);
			project(m->verts[idx3], scr[2], cam);
			project(m->verts[idx4], scr[3], cam);

			//draw_fill_poly(scr, 4, c);
			//map_img2quad(scr, sprite);
			float m_inv[2][2];
			if (calc_m_inv(scr,4, m_inv)){
				//draw_fill_poly(scr, 4, c);
				draw_textured_poly(scr, 4, sprite, m_inv,0);	
			}

		}
	}
}
/////////////////////triangulos




void draw_faces(mesh *m, int c[3], camera cam, int sprite[16][16][3]){
	// TO DO
	// para cada cara poligonal (quad)
	//	obtener los indices de los 4 vertices
	//	proyectar los 4 vertices
	//	si la cara apunta hacia la cámara
	//		dibujar la cara
	//		mapear la imagen a la cara
	for(int i=0; i<m->nf; i++){
		int idx1 = m->faces[i][0];
		int idx2 = m->faces[i][1];
		int idx3 = m->faces[i][2];
		int idx4 = m->faces[i][3];
		float n[3];
		if (  points_towards_cam(m,i,cam,n)&&m->verts[idx1][2]+cam.D<-0.1 && m->verts[idx2][2]+cam.D<-0.1 &&
		    m->verts[idx3][2]+cam.D<-0.1 && m->verts[idx4][2]+cam.D<-0.1){

			int tri[3][2];

			project(m->verts[idx1], tri[0], cam);
			project(m->verts[idx2], tri[1], cam);
			project(m->verts[idx4], tri[2], cam);

			//draw_fill_poly(scr, 4, c);
			//map_img2quad(scr, sprite);
			float m_inv[2][2];
			if (calc_m_inv(tri,3, m_inv)){
				//draw_fill_poly(scr, 4, c);
				draw_textured_poly(tri, 3, sprite, m_inv,0);	
			}
			project(m->verts[idx3], tri[0], cam);
			project(m->verts[idx4], tri[1], cam);
			project(m->verts[idx2], tri[2], cam);
			//draw_fill_poly(scr, 4, c);
			//map_img2quad(scr, sprite);
			if (calc_m_inv(tri,3, m_inv)){
				//draw_fill_poly(scr, 4, c);
				draw_textured_poly(tri, 3, sprite, m_inv,1);	
			}



		}
	}
}













// shapes
float f(float x, float y){
	return 1-x*x-y*y;
}


void create_grid_verts(int n, int m, float v[][3]){
	for (int j=0; j<m; j++)
		for (int i=0; i<n; i++){
			int idx = j*n+i;
			v[idx][0] = i-n/2;
			v[idx][1] = j-m/2;
			v[idx][0]/=n/2; v[idx][1]/=m/2;
			v[idx][2] = 0.0;
		}
}

void create_grid_edges(int n, int m, int e[][2]){
	int k=0;
	int idx;
	for (int j=0; j<m; j++)
		for (int i=0; i<n-1; i++){
			idx = j*n+i;
			e[k][0] = idx;
			e[k++][1] = idx+1;
		}
	for (int i=0; i<n; i++)
		for (int j=0; j<m-1; j++){
			idx = j*n+i;
			e[k][0] = idx;
			e[k++][1] = idx+n;
		}
}

void create_grid_faces(int n, int m, int f[][4]){
	int k=0;
	for (int j=0; j<m-1; j++){
		for (int i=0; i<n-1; i++){
			int idx = j*n+i;
			f[k][0] = idx;
			f[k][1] = idx+1;
			f[k][2] = idx+n+1;
			f[k++][3] = idx+n;
		}
	}
}

void create_cylinder_verts(int n, int m, float v[][3]){
	// TO DO
	// rho = 1
	// z = rho cos(ang)
	// x = rho sin(ang)
	// y = t - 1/2
	// y = 2t-1
	float rho = 1.0;
	float delta = 2*PI/n; 
	for(int j=0; j<m; j++){
		float ang = 0.0;
		for (int i=0; i<n; i++){
			int idx = j*n+i;
			v[idx][2] =rho*cos(ang); 
			v[idx][0] = rho*sin(ang);
			v[idx][1] = (float)j*2/m - 0.5;
			ang += delta;
		}
	}	
}

void create_cone_verts(int n, int m, float v[][3]){
	// TO DO
	// rho = 1 - t
	// z = rho cos(ang)
	// x = rho sin(ang)
	// y = t - 1/2
	float delta = 2*PI/n;
	for(int j=0; j<m; j++){
		float rho = 1.0 - (float)j/m;
		float ang = -2*PI/n;
		for (int i=0; i<n; i++){
			int idx = j*n+i;
			v[idx][2] = rho*cos(ang);
			v[idx][0] = rho*sin(ang);
			v[idx][1] = (float)j*2/m - 0.5;
			ang += delta;
		}
	}
}

void create_sphere_verts(int n, int m, float v[][3]){
	// TO DO
	// rho = cos(phi)
	// z = rho cos(theta)
	// x = rho sin(theta)
	// y = sin(phi)
	float delta_theta = 2 * PI / n; // Incremento del ángulo theta (longitudinal)
    float delta_phi = PI / (m - 1); // Incremento del ángulo phi (latitudinal)

    for (int j = 0; j < m; j++) {
        float phi = -PI / 2 + j * delta_phi; // Ángulo phi (de -PI/2 a PI/2)
        float y = sin(phi);                 // Coordenada y (altura)
        float rho = cos(phi);               // Radio en el plano xz

        for (int i = 0; i < n; i++) {
            float theta = i * delta_theta;  // Ángulo theta (de 0 a 2PI)
            int idx = j * n + i;            // Índice del vértice

            v[idx][0] = rho * cos(theta);   // Coordenada x
            v[idx][1] = y;                  // Coordenada y
            v[idx][2] = rho * sin(theta);   // Coordenada z
        }
    }

}

void create_torus_verts(int n, int m, float ratio, float v[][3]){
    // n: divisiones alrededor del círculo mayor (longitud)
    // m: divisiones alrededor del círculo menor (latitud)
    // ratio: relación r2/r1 (radio menor / radio mayor)
    // v: arreglo de salida para los vértices

    float delta_theta = 2 * PI / n;
    float delta_phi   = 2 * PI / m;

    float r1 = 0.75;         // Radio mayor (distancia al centro del tubo)
    float r2 = r1 * ratio;   // Radio menor (radio del tubo)

    for (int j = 0; j < m; j++) {
        float phi = j * delta_phi; // Ángulo alrededor del círculo menor
        float cos_phi = cos(phi);
        float sin_phi = sin(phi);

        for (int i = 0; i < n; i++) {
            float theta = i * delta_theta; // Ángulo alrededor del círculo mayor
            float cos_theta = cos(theta);
            float sin_theta = sin(theta);

            int idx = j * n + i;

            float rho = r1 + r2 * cos_phi;
            v[idx][0] = rho * cos_theta; // x
            v[idx][1] = r2 * sin_phi;    // y
            v[idx][2] = rho * sin_theta; // z
        }
    }
}



/*
extern float curr[M*N][3];
extern float next[M*N][3];
extern float t;

void interpolate(mesh *m){
	float tt = 1.0-t;
	for (int i=0; i<m->nv; i++){
		// TO DO
	}
}
*/






