#include<fstream.h>
#include<math.H>
#include<dos.h>
#include<ctype.h>
#include<conio.H>
#include<graphics.h>

 double midx = 319;
 double midy = 239;
 double maxx;
 double maxy;
void translate(double &x,double &y)
{
 x+=maxx/2;
 y = 0-y;
 y+=maxy/2;
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
 setviewport(midx+100,0,2*midx,2*midy,1);
 setfillstyle(1,DARKGRAY);
 bar3d(0,0,getmaxx(),getmaxy(),0,0);
 setviewport(0, midy+100,midx+100,2*midy,0);
 setfillstyle(1,LIGHTGRAY);
 bar3d(0,0,midx+100,getmaxy(),0,0);
 setviewport(1,1,midx+99,10,1);
 outtextxy((midx+99)/2-20,0,"GRAPH");
 setviewport(1,10,midx+99,midy+99,1);

 maxx = midx + 99;
 maxy = midy + 99;
 stack p,p2;
// fstream pts("in.dat",ios::binary|ios::out|ios::in|ios::trunc);
 if(graphresult() != 0)
  {
   cout<<"error in viewport";
   return ;

  }
 double i,j,k;
 double ti = i,
 tj = j,
 tk = k;
 int cl;
 double rt[3] = {0,20,30};
 double expval;
 setcolor(15);
 outtextxy(maxx/2-32,maxy/2,"LOADING...");
 setcolor(1);
  for(i = -50; i<50;  i+=1)
  for(j = -50; j<50; j+=1)
   for(k =-50; k<50; k+=1)
   {
	 //lhs of expression goes here
	  expval = 54*sin(i)*sin(j)*sin(k);
	  if(expval <55 && expval >53/*rhs of expression(put a range)*/
		  ||(i == 0 && k== 0 ) || (j == 0 && k == 0) || (i == 0 && j == 0))
		{
		 // pts.write((char*)&i,sizeof(double));
		 // pts.write((char*)&j,sizeof(double));
		 // pts.write((char*)&k,sizeof(double));
			 p.insert((int)i,(int)j,(int)k);
		}
   }
 clearviewport();
 outtext("done");
// long int size = pts.tellp();
// pts.seekg(0);
 while(1){
   p2 = p;
 /*		if(kbhit())
		 {
		  cleardevice();
		  char ch = tolower(getch());
		  switch(ch)
		   {
			case 'a':
			  rt[2] -=M_PI/30;
			  break;
			case 'd':
			  rt[2] +=M_PI/30;
			  break;
			case 'w':
			  rt[1] +=M_PI/30;
			  break;
			case 's':
			  rt[1] -= M_PI/30;
			  break;
			case 'q':
			  rt[0] +=M_PI/30;
			  break;
			case 'c':
			  rt[0] -= M_PI/30;
			  break;

			default:
			  break;
		   } }    */
		if(kbhit())
		   break;
		 while(p2.top != NULL)//pts.tellg()<size)
		 {
		   //	pts.read((char*)&ti,sizeof(double));
		  //	pts.read((char*)&tj,sizeof(double));
		  //	pts.read((char*)&tk,sizeof(double));
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
		//	setfillstyle(1,cl);
	  //		bar(ti-4,tj-4,ti+4,tj+4);
			putpixel(ti,tj,cl);
			p2.top = p2.top->next;
		 }
		 rt[2] += M_PI/30;
		 rt[0] += M_PI/40;
		 rt[1] += M_PI/50;
			 if(rt[2] >= 1000*M_PI)
				 rt[2] = 0;
			 else if( rt[1] >= 1000*M_PI)
			 rt[1] = 0;
			 else if( rt[0] >= 1000*M_PI)
			 rt[0] = 0;
	   //	 pts.seekg(ios::beg);
	   //	 pts.seekp(ios::beg);
		 delay(30);
	 //    getch();
		 clearviewport();
  }
  //pts.close();
 }
int main()
{
 int gd= 0;
 int gm;
 initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
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
 closegraph();
 return 1;
}