/******************************************************************************
 *
 * Jaephys - Just An Extendable PHYsics Simulator
 *
 * Copyright (C) 2001  Federico G. Diez <fedegdiez@hotmail.com>
 *   
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/

#include "Display3D_gl.hpp"
#include "Geometry.hpp"
#include "debugging.h"

//#include "image.h"

#include <GL/glut.h>
#include <math.h>
#include <vector>

using namespace jaephys;

double Display3D_gl::visionAngle_ = PI_DIV_4;  // 45 degrees
double Display3D_gl::depth_ = 40;
double Display3D_gl::focalDistance_ = 300;

//////////////////////////////////////////////////////////////////////////////
Display3D_gl::~Display3D_gl()
{
}

//////////////////////////////////////////////////////////////////////////////
Display3D_gl::Display3D_gl( unsigned int width, unsigned int height )
{
  width_= width;
  height_=  height;

  TRACEC(JAEPHYS_DISP,1,"enter");

  glutInitWindowPosition(0, 0);
  glutInitWindowSize( width, height);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  
  int res = glutCreateWindow("JAEPHYS - Just An Extendable PHYsics Simulator");
	GUARANTEE( res != GL_FALSE, "Unable to create the GL window" );

  glutReshapeFunc(rescale);

  perspective(30,10,1000);
  
  glLineWidth(1);

	glEnable(GL_CULL_FACE);
  //glDisable(GL_CULL_FACE);

  //glShadeModel(GL_FLAT);
  //glShadeModel(GL_SMOOTH);
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);
  
  //glDisable(GL_POLYGON_STIPPLE);
  //glDisable(GL_POLYGON_SMOOTH);
  //glDisable(GL_LINE_STIPPLE);
  //glDisable(GL_LINE_SMOOTH);
  //glDisable(GL_BLEND);

  //glEnable(GL_TEXTURE_2D);
  //ini_textura("s128.rgb");
  //glDisable(GL_TEXTURE_2D);
  //glBindTexture(GL_TEXTURE_2D,t1id);
  //glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  //glEnable(GL_BLEND);


  glEnable(GL_LIGHTING);
  //glDisable(GL_LIGHTING);

  GLfloat ambient[4]={0,0,0,0};
  GLfloat luzAmbiente[4]={0.3, 0.3, 0.3};
  GLfloat luzDifusa[4]={0.5,0.5,0.5,0.5};
  //GLfloat luzEspec[4]={0.9,0.9,0.9,0.1};
  GLfloat posLuz0[4]={-100,0,0,1};
  GLfloat dirLuz0[4]={0,0,-1};

  GLfloat posLuz1[4]={100,0,0,1};
  GLfloat dirLuz1[4]={0,0,-1};

  GLfloat matDifusa[4]={1,1,1,0.1};
  GLfloat matEspec[4]={0.2,0.2,0.2,0.1};

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

  glLightfv(GL_LIGHT0,GL_AMBIENT,luzAmbiente);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,luzDifusa);
  //glLightfv(GL_LIGHT0,GL_SPECULAR, luzEspec);
  glLightfv(GL_LIGHT0,GL_POSITION, posLuz0);
  glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, dirLuz0);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT1,GL_AMBIENT,luzAmbiente);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,luzDifusa);
  //glLightfv(GL_LIGHT1,GL_SPECULAR, luzEspec);
  glLightfv(GL_LIGHT1,GL_POSITION, posLuz1);
  glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, dirLuz1);
  glEnable(GL_LIGHT1);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,matDifusa);
  glMaterialfv(GL_FRONT,GL_SPECULAR,matEspec);
  glMateriali(GL_FRONT,GL_SHININESS,128);
}

//////////////////////////////////////////////////////////////////////////////
void 
Display3D_gl::perspective( double visionAngle, double proximity, double depth)
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  visionAngle_ = visionAngle;
  depth_ = depth;
  focalDistance_ = proximity;
  centre_ = Coord3D(0, 0, -focalDistance_);
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::rescale( int width, int height )
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  glViewport(0, 0, (GLint)width, (GLint)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  double factor;
  if(width >= height)
  {
    factor= static_cast<double>(width)/height;
  }
  else
  {
    factor= static_cast<double>(height)/width;
  }

  gluPerspective(visionAngle_, factor, focalDistance_, depth_);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::clean() const
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//////////////////////////////////////////////////////////////////////////////
void
Display3D_gl::flush() const
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::useColor(unsigned int color)
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  glIndexi(color);
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::useColor(int r, int g, int b)
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  TRACEC(JAEPHYS_DISP,3,"r="<< r << " g=" << g << " b"<< b);
  glColor4ub(r, g, b,255);
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::drawLine( const Coord3D & p0, const Coord3D & p1 ) const
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  Coord3D p;
  glBegin(GL_LINES);
    applyView( p0, p );
    glVertex3f( p.x, p.y, p.z );
    applyView( p1, p );
    glVertex3f(p.x, p.y, p.z);
  glEnd();
}

void Display3D_gl::useTexture( unsigned int id )
{
  glEnable(GL_TEXTURE_2D);
  glCallList(id);
}

void Display3D_gl::disableTexture()
{
  glDisable(GL_TEXTURE_2D);
}

///////////////////////////////////////////////////////////////////////////////
void Display3D_gl::drawTexturedPolygon( Polygon & polyg) const
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  Coord3D p;

  glBegin(GL_TRIANGLE_FAN);

  Coord3D n(polyg.normal());
  n.normalize();
  multiply( mView_, n, p );
  glNormal3f(p.x, p.y, p.z);
  float s=0,t=1;
  int size= polyg.numberOfEdges();
  for( int i=0; i< size; ++i)
  {
    const Coord3D & v = (polyg.edge(i)).end1();
    applyView(v,p);
    glTexCoord2f(s,t);
    if(s==0){
      if(t==0)
        s=1;
      else
        t=0;
    }
    else{
      if(t==0)
        t=1;
      else
        s=0;
    }
    glVertex3f(p.x, p.y, p.z);
  }
  glEnd();
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::drawFlatColorPolygon( Polygon & polyg) const
{
  TRACEC(JAEPHYS_DISP,2,"enter");
  Coord3D p;

  glBegin(GL_TRIANGLE_FAN);

  Coord3D n(polyg.normal());
  n.normalize();
  multiply( mView_, n, p );
  glNormal3f(p.x,p.y,p.z);
	int size = polyg.numberOfEdges();
  for( int i=0; i< size; ++i)
  {
    const Coord3D & v = (polyg.edge(i)).end1();
    applyView(v,p);
    glVertex3f( p.x, p.y, p.z );
  }
  glEnd();
}

//////////////////////////////////////////////////////////////////////////////
void Display3D_gl::drawWiredPolygon( Polygon & polyg) const
{
  TRACEC(JAEPHYS_DISP,2,"enter");

  Coord3D p;

  glBegin(GL_LINE_LOOP);

  glNormal3f(0,0,1);
	int size = polyg.numberOfEdges();
  for( int i=0; i< size; ++i)
  {
		const Coord3D & v = (polyg.edge(i)).end1();
    applyView( v, p );
    glVertex3f( p.x, p.y, p.z);
  }
  glEnd();
}

///////////////////////////////////////////////////////////////////////////////
unsigned int 
Display3D_gl::iniTexture(char* /* filename */)
{
	/*
  GLuint idTex;
  GLuint textureId;
  IMAGE *img;
  
  cout << filename<< endl;
  if(!(img=ImageLoad(filename))) {
    cerr << "Error reading a texture." << endl;
    exit(-1);
  }

  textureId= glGenLists(1);
  cout << "textureId=" << textureId << endl;
  glNewList(textureId,GL_COMPILE);

  glGenTextures(1,&idTex);
  cout << "idTex_=" << idTex<< endl;
  glBindTexture(GL_TEXTURE_2D,idTex);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glPixelStorei(GL_UNPACK_ALIGNMENT,4);
  GLint gluerr
   = gluBuild2DMipmaps
     ( GL_TEXTURE_2D, 3, img->sizeX, img->sizeY
     , GL_RGB, GL_UNSIGNED_BYTE,(GLvoid *)(img->data)
     );
  if(gluerr)
  {
    cerr << "Error in GLULib " << gluErrorString((GLenum)gluerr) << endl;
    exit(-1);
  }

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  glEndList();
  return textureId;
	*/
	return 0;
}
