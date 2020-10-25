#include<fstream.h>
#include<math.H>
#include<dos.h>
#include<ctype.h>
#include<conio.H>
int segment = 0xa000;
int global_plane = 0x4000;
int flag = 0;
//using write mode 2 produces choppy code, try write mode 0 later :)
//with mode 0, i can copy screen to screen, and rid myself of clearscreen!
void putpixel(int x, int y, int c)
{


 long offsets =((long)y << 6)+ ((long)y << 4)
				+ ((long)x >> 3);


 int mask = 0x8000 >> (x & 0x0007);
 if(mask != global_plane)
 {
 _asm{
		mov dx, 0x3ce
		mov ax,[mask]
		or ax, 8
		out dx, ax
		mov ax, 0x0205
		out dx, ax
	}
		 global_plane = mask;
 }

 _asm{

	push [segment]
	 pop ds
	 mov si, WORD [offsets]
	mov al,byte [si] //dummy read for values to latch onto vga registers
	mov al, byte [c]
	mov [si], al;
 }
 outport(0x3ce, 0x0005);     //reset settings to default
  outport(0x3ce, 0xff08);

}
void clearscreen(){
int segment = 0xa000;
_asm{
	//push ax
	//push cx
	pusha
	mov es, segment
	xor di,di
	mov dx, 0x3ce
	xor ah,ah
	mov al,0
	out dx,ax     //write 0
	mov ax,0x0f01 //for all bitplanes in mode 0
	out dx,ax
	mov cx,19200
	rep stosw     //for all words in video mem, latch them and write to them! = 80x640/2 = 19200
	mov ax,0x0005
	out dx,ax
	popa
	//pop cx
	//pop ax
	}
}
void fillscreen(unsigned char c)
{
  asm{
	mov ax,0x600   //scroll up and clear
	xor cx,cx      //upper row number and column number is 0
	mov dx,0x1d4f  //lower row number is 29, column is 79, since 640x480
	mov bh,[c]     //color to fill
	int 0x10       //call interrupt
  }
}
 double midx = 319;
 double midy = 239;
 double maxx;
 double maxy;
void translate(double &x,double &y,double offsetx = 0, double offsety = 0)
{
 x+=maxx/2;
 x+=offsetx;
 y = 0-y;
 y+=maxy/2;
 y+=offsety;
}
void scale(double &x, double &y)
{
 //scales to 320,240 from 100,100
 x*=3.2;
 y*=3.2;
}
void rot(double &a, double &b,double th)
{
 double a_t =a;
 double b_t = b;
 a = a_t*cos(th) - b_t*sin(th);
 b = a_t*sin(th) + b_t*cos(th);
}
double cns = atan(-300.0/5);
void z_s(double &x,double &y, double &z)
{
  double d = z-300;
  x -= 180*(atan(d/5) -cns);
  y -= 120*(atan(d/5) -cns);
//  scale(x,y);
}
//a struct of points that acts as a node
struct pt
{
 int x;
 int y;
 int z;
 pt *next;
 pt()
 {
  x = y = z = 0;
 }
};
//dynamic stack to store the points
struct stack
{
 pt *top;
 stack()
 {
  top = NULL;
 }
 void insert(int x,int y,int z)
 {
  pt *tm = new pt;
  tm->x = x;
  tm->y = y;
  tm->z = z;
  tm->next = top;
  top = tm;
 }
};
int max(int a,int b,int c)
{
  if(a>=b &&  a>=c)
   return a;
  else if(b>=a && b>=c)
   return b;
  else if(c>=a&&c>=b)
   return c;
  return -1;
}
void test2()
{

 maxx = midx + 99;
 maxy = midy + 99;
 stack p,p2;
 double i,j,k;
 double ti = i,
 tj = j,
 tk = k;
 int cl;
 double rt[3] = {0,20,30};
 double expval;

  for(i = -50; i<50;  i+=1)
  for(j = -50; j<50; j+=1)
   for(k =-50; k<50; k+=1)
   {
	  expval = 54*sin(i)*sin(j)*sin(k);
	  if(expval <55 && expval >53/*rhs of expression(put a range)*/
		  ||(i == 0 && k== 0 ) || (j == 0 && k == 0) || (i == 0 && j == 0))
		{
			 p.insert((int)i,(int)j,(int)k);
		}
   }
 while(1){
   p2 = p;
		if(kbhit())
		   break;
		 while(p2.top != NULL)//pts.tellg()<size)
		 {
			ti = p2.top->x;
			tj = p2.top->y;
			tk = p2.top->z;
			if     (tk<0&&ti<0&&tj<0)cl=5;
			else if(tk<0&&ti<0&&tj>0)cl=6;
			else if(tk<0&&ti>0&&tj<0)cl=7;
			else if(tk<0&&ti>0&&tj>0)cl=8;
			else if(tk>0&&ti<0&&tj<0)cl=9;
			else if(tk>0&&ti<0&&tj>0)cl=2;
			else if(tk>0&&ti>0&&tj<0)cl=3;
			else if(tj == 0 && ti == 0) cl = 11;
			else if(tj == 0 && tk == 0) cl = 13;
			else cl=4;
			rot(tj,tk,rt[2]);
			rot(ti,tk,rt[0]);
			rot(ti,tj,rt[1]);
			 z_s(ti,tj,tk);
			 scale(ti,tj);
			 ti*=(2.0/4);
			 tj*=(2.0/4);
			translate(ti,tj);
			putpixel(ti,tj,cl);
			p2.top = p2.top->next;
		 }
		 rt[2] += M_PI/150;
		 rt[0] += M_PI/200;
		 rt[1] += M_PI/250;
			 if(rt[2] >= 1000*M_PI)
				 rt[2] = 0;
			 else if( rt[1] >= 1000*M_PI)
			 rt[1] = 0;
			 else if( rt[0] >= 1000*M_PI)
			 rt[0] = 0;
		 clearscreen();
  }
 }
int main()
{
 asm mov ax,0x0012
 asm int 0x10
 test2();
 /*
 double xyrot = 0; //radians
 double yzrot = 0;
 double xzrot = M_PI_4;
 double x = 10;
 double z = 15;
 double y = 30;
 //standard 3d proc
 rot(x,z,xzrot);
 z_s(x,y,z,xzrot);
 scale(x,y);
 translate(x,y);
 //endproc
 */
 return 1;
}