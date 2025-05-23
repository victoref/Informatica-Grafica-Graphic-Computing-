#include "escena.h"
#include <GL/freeglut.h>
//-------------------------------------------------------------------------

void Escena::init(){

	rect.textura.init();
	rect.textura.load("../bmps/ray.bmp");
	rect.textura.desactivar();
	rect2.textura.init();
	rect2.textura.load("../bmps/Zelda.bmp", 100);
	rect2.textura.desactivar();
	rect3.textura.init();
	rect3.textura.load("../bmps/meatboy.bmp", PixMap24RGB::rgb_color{ 0,0,0 }, 100);
	rect3.textura.desactivar();
	

}

//-------------------------------------------------------------------------
Escena::Escena() : ejes(200), rect(600, 800), rect2(200, 266), rect3(100, 133), rect4 (600,800), tri(100, 200, 0, 0), t(100), cubo(100) {
	piramides.push_back(new PiramideTri(100, 100));
	piramides.push_back(new PiramideTri(100, 100));
	piramides.push_back(new PiramideTri(-100, -100));
	piramides.push_back(new PiramideTri(-100, -100));


}
Escena::~Escena(){
	// liberar memoria y recursos 
}

//-------------------------------------------------------------------------

void Escena::draw(){
	ejes.draw();

}
void Escena::drawDiabolo() {
	glRotated(rotacion, 1.0, 0.0, 0.0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glTranslated(0.0, 0.0, -100);
	piramides[0]->draw();

	glRotated(180, 1.0, 0.0, 0.0);
	piramides[3]->draw();


	glTranslated(1.0, 0.0, -200);
	piramides[1]->draw();

	glRotated(180.0, 90.0, 0.0, 1.0);
	piramides[2]->draw();

	glRotated(-90.0, -90.0, 0.0, -1.0);
	glTranslated(0.0, -100.0, 0);

	glRotated(-rotacion, 1.0, 0.0, 0.0);
}


void Escena::dibujarCubo(int lado){
	Rectangulo r1(lado, lado);
	
	ejes.draw();

	glTranslated(0.0, 0.0, lado / 2);
	r1.draw();
	glTranslated(0.0, 0.0, -lado / 2);
	glTranslated(0.0, 0.0, -lado / 2);
	r1.draw();
	glTranslated(0.0, 0.0, lado / 2);

	glTranslated(-lado / 2, 0, 0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	r1.draw();
	glRotated(-90.0, 0.0, 1.0, 0.0);
	glTranslated(lado / 2, 0, 0);

	glTranslated(lado / 2, 0, 0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	r1.draw();
	glRotated(-90.0, 0.0, 1.0, 0.0);
	glTranslated(-lado / 2, 0, 0);

	glTranslated(0, lado / 2, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	r1.draw();
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glTranslated(0, -lado / 2, 0);

	glTranslated(0, -lado / 2, 0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	r1.draw();
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glTranslated(0, lado / 2, 0);
}




void Escena::capturar(int ancho, int alto){

	PixMap24RGB pixmap;
	pixmap.create_pixmap(ancho, alto);

	glReadBuffer(GL_FRONT);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, ancho, alto, 0);
	glReadBuffer(GL_BACK);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixmap.data());

	pixmap.save_bmpBGR("../bmps/collage.bmp");


}

//-------------------------------------------------------------------------

Triangulo::Triangulo(GLdouble r){

	x = r * cos(var);
	y = r * sin(var);

	vertices[0].set(r, 0, 0);
	vertices[1].set(x, y, 0);
	vertices[2].set(x, -y, 0);

	normales[0].set(0, 0, 1);
	normales[1].set(0, 0, 1);
	normales[2].set(0, 0, 1);
	
	colores.a = 1;
	colores.b = 0;
	colores.g = 0;
	colores.r = 0;

	
	texttri[0].s = 0;
	texttri[0].t = 0;
	texttri[1].s = 0;
	texttri[1].t = 0;
	texttri[2].s = 0;
	texttri[2].t = 0;
	
	radio = r;

}


void Triangulo::set(int n, GLdouble h) {
	vertices[n] = { 0, 0, h };
}

void Triangulo::cambio(){
	slinea = false;
}

void Triangulo::draw(){

	if (slinea){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_TEXTURE_2D);

	glVertexPointer(3, GL_DOUBLE, 0, vertices);

	glTexCoordPointer(2, GL_DOUBLE, 0, texttri);

	glNormal3d(normales[0].x, normales[0].y, normales[0].z);

	glColor4d(colores.r, colores.g, colores.b, colores.a);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_TEXTURE_2D);

}

bool Triangulo::dentro(GLdouble x, GLdouble y){
	if (((vertices[0].x - x) * (vertices[1].y - y)) - ((vertices[0].y - y) * (vertices[1].x - x)) < 0){
		return false;
	}

	else if (((vertices[1].x - x) * (vertices[2].y - y)) - ((vertices[1].y - y) * (vertices[2].x - x)) < 0){
		return false;
	}

	else if (((vertices[2].x - x) * (vertices[0].y - y)) - ((vertices[2].y - y) * (vertices[0].x - x)) < 0){
		return false;
	}
	else return true;
}



void  Triangulo::posicionar(GLdouble x, GLdouble y){

	centro = { x, y, 0 }; 
	vertices[0] = { radio*cos(rotacion) + centro.x, radio * sin(rotacion) + centro.y, 0 };
	vertices[1] = { radio*cos(rotacion + 2.0 * 3.14 / 3.0) + centro.x, radio * sin(rotacion + 2.0 * 3.14 / 3.0) + centro.y, 0 };
	vertices[2] = { radio*cos(rotacion + 4.0 * 3.14 / 3.0) + centro.x, radio * sin(rotacion + 4.0 * 3.14 / 3.0) + centro.y, 0 };
	
}


void Triangulo::rotar(){
	rotacion += 0.1;
	vertices[0] = { radio*cos(rotacion) + centro.x, radio * sin(rotacion) + centro.y, 0 };
	vertices[1] = { radio*cos(rotacion + 2.0 * 3.14 / 3.0) + centro.x, radio * sin(rotacion + 2.0 * 3.14 / 3.0) + centro.y, 0 };
	vertices[2] = { radio*cos(rotacion + 4.0 * 3.14 / 3.0) + centro.x, radio * sin(rotacion + 4.0 * 3.14 / 3.0) + centro.y, 0 };
}


void  Triangulo::recortar(int ancho, int alto, PVec3 vt[3]){
	
	texttri[0].s = (vt[0].x + ancho / 2) / ancho;
	texttri[0].t = (vt[0].y + alto / 2) / alto;
	texttri[1].s = (vt[1].x + ancho / 2) / ancho;
	texttri[1].t = (vt[1].y + alto / 2) / alto;
	texttri[2].s = (vt[2].x + ancho / 2) / ancho;
	texttri[2].t = (vt[2].y + alto / 2) / alto;

}


void Triangulo::setAnimar(){
	centro = { 0, 0, 0 };
	rotacion = 0;
	vertices[0] = { radio*cos(rotacion) + centro.x, radio * sin(rotacion) + centro.y, 0 };
	vertices[1] = { radio*cos(rotacion + 2.0 * 3.14 / 3.0) + centro.x, radio * sin(rotacion + 2.0 * 3.14 / 3.0) + centro.y, 0 };
	vertices[2] = { radio*cos(rotacion + 4.0 * 3.14 / 3.0) + centro.x, radio * sin(rotacion + 4.0 * 3.14 / 3.0) + centro.y, 0 };
}
//-------------------------------------------------------------------------

TriAnimado::TriAnimado(GLdouble dim, GLdouble rd, GLdouble tr, GLdouble rot) : tri(dim){
	radio = rd;
	trans = tr;
	rotacion = rot;

}

void TriAnimado::draw(){

	posx = radio * cos(trans);
	posy = radio * sin(trans);

	glTranslated(posx, posy, 0);
	glRotated(rotacion, 0.0, 0.0, 1.0);

	tri.cambio();

	tri.draw();

	glRotated(-rotacion, 0.0, 0.0, 1.0);
	glTranslated(-posx, -posy, 0);

}

void TriAnimado::update(){

	rotacion += 15;
	trans += 0.2;

}

//-------------------------------------------------------------------------

PiramideTri::PiramideTri(GLdouble h, GLdouble r) {
	triangulos.push_back(new Triangulo(r));
	triangulos[0]->set(0, h);
	triangulos.push_back(new Triangulo(r));
	triangulos[1]->set(1, h);
	triangulos.push_back(new Triangulo(r));
	triangulos[2]->set(2, h);
}
void PiramideTri::draw(){
	for (auto i : triangulos){
		i->cambio();
		i->draw();
	}
}

void PiramideTri::añadirText(CTex2 coor[3]){
	triangulos[0]->texttri[0] = coor[0];
	triangulos[0]->texttri[1] = coor[1];
	triangulos[0]->texttri[2] = coor[2];

	triangulos[1]->texttri[0] = coor[1];
	triangulos[1]->texttri[1] = coor[2];
	triangulos[1]->texttri[2] = coor[0];

	triangulos[2]->texttri[0] = coor[2];
	triangulos[2]->texttri[1] = coor[0];
	triangulos[2]->texttri[2] = coor[1];
	
}


//-------------------------------------------------------------------------

Ejes::Ejes(GLdouble l) {
	vertices[0].set(0, 0, 0);
	vertices[1].set(l, 0, 0);
	vertices[2].set(0, 0, 0);
	vertices[3].set(0, l, 0);
	vertices[4].set(0, 0, 0);
	vertices[5].set(0, 0, l);

	colores[0].set(1, 0, 0);
	colores[1].set(1, 0, 0);
	colores[2].set(0, 1, 0);
	colores[3].set(0, 1, 0);
	colores[4].set(0, 0, 1);
	colores[5].set(0, 0, 1);
}

//-------------------------------------------------------------------------

Rectangulo::Rectangulo(GLdouble alto, GLdouble ancho){

	vertices[0].set(-ancho / 2, alto / 2, 0);  
	vertices[1].set(-ancho / 2, -alto / 2, 0);
	vertices[2].set(ancho / 2, alto / 2, 0);
	vertices[3].set(ancho / 2, -alto / 2, 0);

	vtext[0].s = 0;
	vtext[0].t = 1;
	vtext[1].s = 0;
	vtext[1].t = 0;
	vtext[2].s = 1;
	vtext[2].t = 1;
	vtext[3].s = 1;
	vtext[3].t = 0;
	
	normal.set(0, 0, 1);

	color.a = 1;
	color.b = 0;
	color.g = 0;
	color.r = 0;

}

void Rectangulo::draw(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor4d(color.r, color.g, color.b, color.a);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertexPointer(3, GL_DOUBLE, 0, vertices);
	glNormal3d(normal.x, normal.y, normal.z);

	glTexCoordPointer(2, GL_DOUBLE, 0, vtext);

	glLineWidth(2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glLineWidth(1);

	glDisableClientState(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Rectangulo::set(GLdouble anchop, GLdouble altop){
	vertices[0].set(-anchop / 2, altop / 2, 0);
	vertices[1].set(-anchop / 2, -altop / 2, 0);
	vertices[2].set(anchop / 2, altop / 2, 0);
	vertices[3].set(anchop / 2, -altop / 2, 0);
}

//-------------------------------------------------------------------------

Cubo::Cubo(int lado){
	vertices[0].set(-lado / 2, lado / 2, lado / 2);
	vertices[1].set(-lado / 2, -lado / 2, lado / 2);
	vertices[2].set(-lado / 2, lado / 2, -lado / 2);
	vertices[3].set(-lado / 2, -lado / 2, -lado / 2);
	vertices[4].set(lado / 2, lado / 2, -lado / 2);
	vertices[5].set(lado / 2, -lado / 2, -lado / 2);
	vertices[6].set(lado / 2, lado / 2, lado / 2);
	vertices[7].set(lado / 2, -lado / 2, lado / 2);

	normal[0].set(0, 0, 1);
}


void Cubo::draw(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4d(1, 0, 0, 1);
	
	glVertexPointer(3, GL_DOUBLE, 0, vertices);
	glNormal3d(normal[0].x, normal[0].y, normal[0].z);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
	glRotated(180.0, 1.0, 0.0, 0.0);
	glRotated(90.0, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	

	
	glDisableClientState(GL_VERTEX_ARRAY);


}



//-------------------------------------------------------------------------


void Ejes::draw(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, vertices);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_DOUBLE, 0, colores);

	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 6);
	glLineWidth(1);

	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	

	/* glLineWidth(2);
	glBegin(GL_LINES);
	glColor3d(1.0, 0.0, 0.0); // red
	glVertex3d(vertices[0].x, vertices[0].y, vertices[0].z);   // origin
	glVertex3d(vertices[1].x, vertices[1].y, vertices[1].z);   // x

	glColor3d(0.0, 1.0, 0.0); // green
	glVertex3d(vertices[2].x, vertices[2].y, vertices[2].z);   // origin
	glVertex3d(vertices[3].x, vertices[3].y, vertices[3].z);	  // y

	glColor3d(0.0, 0.0, 1.0); // blue
	glVertex3d(vertices[4].x, vertices[4].y, vertices[4].z);   // origin
	glVertex3d(vertices[5].x, vertices[5].y, vertices[5].z);	  // z
	glEnd();
	glLineWidth(1);
	*/
}




//-------------------------------------------------------------------------



