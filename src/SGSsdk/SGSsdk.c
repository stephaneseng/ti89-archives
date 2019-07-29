// C Source File
// Created 22/12/2006; 19:27:39

#define OPTIMIZE_ROM_CALLS
#include <tigcclib.h>

// Variables globales
LCD_BUFFER(temp);
ICON i1[16] = {{{65024,33280,33280,33280,33280,33280,65024,0,0,0,0,0,0,0,0,0}}};
static int mat[9][9] = {{NULL,NULL}};
static int rec[9][9] = {{NULL,NULL}};
int gx=3, gy=11, box=0, sok=0; static int u=4, v=12;

// Tracé de la grille
void grille(int x, int y)
{
	int a=x; int b=y;
	while (y<b+73)
	{
		WIN_RECT rect = {x, y, x+8, y+8};
		DrawClipRect (&rect, ScrRect, A_NORMAL);
		if ((x==a+16) | (x==a+43)) {x=x+3;}
		x=x+8;
		if (x>a+70) {y=y+8; x=a;}
		if ((y==b+24) | (y==b+51)) {y=y+3;}
	}
	WIN_RECT rect = {a-3, b-3, a+81, b+81};
	DrawClipRect (&rect, ScrRect, A_NORMAL);

	x=106;y=35;
	while (x<136)
	{
		WIN_RECT rect2 = {x, y, x+10, y+10};
		DrawClipRect (&rect2, ScrRect, A_NORMAL);
		x=x+10;
		if ((x==136) & (y!=55)) {x=106; y=y+10;}
	}
	
	FontSetSys(F_6x8); DrawStr(118,47,"9",A_NORMAL);FontSetSys(F_4x6);
	
	DrawLine(0,0,159,0,A_NORMAL); DrawLine(0,8,159,8,A_NORMAL);
	DrawLine(0,0,0,99,A_NORMAL); DrawLine(159,0,159,99,A_NORMAL);
	DrawStr(0,2," NOUVEAU | RESOUDRE | SAVE | OPEN | ABOUT | OFF ",A_NORMAL);
	DrawLine(0,92,159,92,A_NORMAL); DrawLine(0,99,159,99,A_NORMAL);
}

// Vérfication avancement
void verif(int XX, int YY, int ZZ)
{
	if ((ZZ==1) & (mat[XX][YY]==0)) {box=box+1;}
	if ((ZZ==-1) & (mat[XX][YY]!=0)) {box=box-1;}
}

// Avancement
void avcmt()
{
	int pbox = (53*box)/27;
	DrawLine(0,95,158,95,A_REVERSE);
	DrawLine(0,96,158,96,A_REVERSE);
	DrawLine(0,95,pbox,95,A_NORMAL);
	DrawLine(0,96,pbox,96,A_NORMAL);
}

// Enregistrer
void save() 
{
	int x,y;
	for (y=0;y<9;y++) for (x=0;x<9;x++) {rec[x][y]=mat[x][y];}
	LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp);
}

// Ouvrir
void load() 
{
	int x,y;
	for (y=0;y<9;y++) for (x=0;x<9;x++) {mat[x][y]=rec[x][y];}
	LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp);
}

// A propos de
void appd()
{
	HANDLE dappd = DialogNewSimple(150,43);
	DialogAddTitle(dappd, "SGSsdk - A PROPOS", BT_NONE, BT_NONE);
	DialogAddText(dappd, 5, 13, "SGSsdk - v1.0");
	DialogAddText(dappd, 5, 20, "Résolution de grilles de SUDOKU");
	DialogAddText(dappd, 5, 27, "(c) 2006 The SGS Corporation");
	DialogAddText(dappd, 5, 34, "SENG Stéphane");
	DialogDo(dappd, CENTER, CENTER, NULL, NULL);
}

// Remplissage de la grille
void fill(int x, int y)
{
	int f=x+2; int g=y+2;
	char e[50]; int c, d;
	for (c=0; c<9; c++)
	{
		for (d=0; d<9; d++)
		{
			if (mat[d][c]!=0)
			{sprintf(e, "%d", mat[d][c]); DrawStr(f, g, e, A_REPLACE);}
			else {DrawStr(f, g, "  ", A_REPLACE);}
			if ((f==x+18) | (f==x+45)) {f=f+3;}
			f=f+8;
		}
		g=g+8; f=x+2;
		if ((g==y+26) | (g==y+53)) {g=g+3;}
	}
}

// Nouvelle grille
void new()
{
	int i,j;
	for(i=0; i<9; i++)
  for (j=0; j<9; j++) {mat[i][j] = 0;}
	LCD_restore(temp); fill(gx,gy); box=0; avcmt(); LCD_save(temp);
}

// Mode résolution horizontale
void resolh()
{
	int VH,i,IN,pos,ITF,pok,ii,posok,ok;
	int M,N,XX,YY,X,Y;
	for (VH=0;VH<9;VH++)
	{
		int ver_h[10]={0,1,1,1,1,1,1,1,1,1};
		for (i=0;i<9;i++) {ver_h[mat[i][VH]]=0;}
		for (IN=1;IN<10;IN++) if (ver_h[IN]==1)
		{
			pos=0;
			for (i=0;i<9;i++) {if (mat[i][VH]==IN) {pos=pos+1;ITF=i;}}
			posok=0; ok=0;
			if (pos==1) {verif(i,VH,1); mat[i][VH]=IN; LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp); sok=1;}
			else
			{
				for (i=0;i<9;i++) if (mat[i][VH]==0)
				{
					pok=1;
					for (ii=0;ii<9;ii++) if (mat[i][ii]==IN) {pok=0;} // Vérification horizontale
					// Vérification carrée
					M=1; N=1; XX=i-2; YY=VH-2;
					for (Y=YY; Y<YY+5; Y++)
					{
						for (X=XX; X<XX+5; X++)
						{
							if ((mat[X][Y]==IN) & (M>2-(i%3)) & (M<2-(i%3)+4) & (N>2-(VH%3)) & (N<2-(VH%3)+4))
							{pok=0;}
							M=M+1;
						}
						M=1; N=N+1;
					}
					// Fin vérification carrée
					if (pok==1) {posok=1;ITF=i;ok=ok+1;}
				}
			}
			if ((posok==1) & (ok==1) & (mat[ITF][VH]==0)) {verif(ITF,VH,1); mat[ITF][VH]=IN; LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp); sok=1;}
		}
	}
}

// Mode résolution verticale
void resolv()
{
	int VV,i,IN,pos,ITF,pok,ii,posok,ok;
	int M,N,XX,YY,X,Y;
	for (VV=0;VV<9;VV++)
	{
		int ver_v[10]={0,1,1,1,1,1,1,1,1,1};
		for (i=0;i<9;i++) {ver_v[mat[VV][i]]=0;}
		for (IN=1;IN<10;IN++) if (ver_v[IN]==1)
		{
			pos=0;
			for (i=0;i<9;i++) {if (mat[VV][i]==IN) {pos=pos+1;ITF=i;}}
			posok=0; ok=0;
			if (pos==1) {verif(VV,i,1); mat[VV][i]=IN; LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp); sok=1;}
			else
			{
				for (i=0;i<9;i++) if (mat[VV][i]==0)
				{
					pok=1;
					for (ii=0;ii<9;ii++) if (mat[ii][i]==IN) {pok=0;} // Vérification horizontale
					// Vérification carrée
					M=1; N=1; XX=VV-2; YY=i-2;
					for (Y=YY; Y<YY+5; Y++)
					{
						for (X=XX; X<XX+5; X++)
						{
							if ((mat[X][Y]==IN) & (M>2-(VV%3)) & (M<2-(VV%3)+4) & (N>2-(i%3)) & (N<2-(i%3)+4))
							{pok=0;}
							M=M+1;
						}
						M=1; N=N+1;
					}
					// Fin vérification carrée
					if (pok==1) {posok=1;ITF=i;ok=ok+1;}
				}
			}
			if ((posok==1) & (ok==1) & (mat[VV][ITF]==0)) {verif(VV,ITF,1); mat[VV][ITF]=IN; LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp); sok=1;}
		}
	}
}

// Mode résolution carré
void resolc()
{
	int X,Y,XX,YY,pos,ITFX,ITFY,IN,pok,posok,i;
	for (Y=0;Y<9;Y=Y+3) for (X=0;X<9;X=X+3)
	{	
		int ver_c[10]={0,1,1,1,1,1,1,1,1,1};
		for (YY=Y;YY<Y+3;YY++) for (XX=X;XX<X+3;XX++) {ver_c[mat[XX][YY]]=0;}
		for (IN=1;IN<10;IN++) if (ver_c[IN]==1)
		{
			pos=0;
			for (YY=Y;YY<Y+3;YY++) for (XX=X;XX<X+3;XX++)
			{if (mat[XX][YY]==0) {pos=pos+1;ITFX=XX;ITFY=YY;}}
			posok=0;
			if (pos==1) {verif(ITFX,ITFY,1); mat[ITFX][ITFY]=IN; LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp); sok=1;}
			else
			{
				for (YY=Y;YY<Y+3;YY++) for (XX=X;XX<X+3;XX++) if (mat[XX][YY]==0)
				{
					pok=1;
					for (i=0;i<9;i++) {if ((mat[i][YY]==IN) | (mat[XX][i]==IN)) {pok=0;}}
 				  if (pok==1) {posok=posok+1;ITFX=XX;ITFY=YY;}
 				}
			}
			if ((posok==1) & (mat[ITFX][ITFY]==0)) {verif(ITFX,ITFY,1); mat[ITFX][ITFY]=IN; LCD_restore(temp); avcmt(); fill(gx,gy); LCD_save(temp); sok=1;}
		}	
	}
}

// Interactivité
int inter(int x, int y)
{
	int k=0;
	while ((k!=264) | (k!=13))
	{
		LCD_restore(temp);
		avcmt();
		DrawIcon(u,v,&i1,A_NORMAL);
		k=ngetchx();
		if (k==344) {if ((u==x+17) | (u==x+44)) {u=u+11;} else {u=u+8;}}
		if (k==338) {if ((u==x+28) | (u==x+55)) {u=u-11;} else {u=u-8;}}
		if (k==340) {if ((v==y+17) | (v==y+44)) {v=v+11;} else {v=v+8;}}
		if (k==337) {if ((v==y+28) | (v==y+55)) {v=v-11;} else {v=v-8;}}
		if (u>x+71) {u=x+1;}
		if (u<x+1) {u=x+71;}
		if (v>y+71) {v=y+1;}
		if (v<y+1) {v=y+71;}
		
		if ((k>48) & (k<58))
		{
			LCD_restore(temp); verif((u-x-(u-x)%8)/8, (v-y-(v-y)%8)/8, 1); LCD_save(temp);
			if (k==49) {LCD_restore(temp); DrawStr(u+1, v+1, "1 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 1; LCD_save(temp);};
			if (k==50) {LCD_restore(temp); DrawStr(u+1, v+1, "2 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 2; LCD_save(temp);};
			if (k==51) {LCD_restore(temp); DrawStr(u+1, v+1, "3 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 3; LCD_save(temp);};
			if (k==52) {LCD_restore(temp); DrawStr(u+1, v+1, "4 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 4; LCD_save(temp);};
			if (k==53) {LCD_restore(temp); DrawStr(u+1, v+1, "5 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 5; LCD_save(temp);};
			if (k==54) {LCD_restore(temp); DrawStr(u+1, v+1, "6 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 6; LCD_save(temp);};
			if (k==55) {LCD_restore(temp); DrawStr(u+1, v+1, "7 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 7; LCD_save(temp);};
			if (k==56) {LCD_restore(temp); DrawStr(u+1, v+1, "8 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 8; LCD_save(temp);};
			if (k==57) {LCD_restore(temp); DrawStr(u+1, v+1, "9 ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 9; LCD_save(temp);};
		}
		if (k==257) {LCD_restore(temp); verif((u-x-(u-x)%8)/8, (v-y-(v-y)%8)/8, -1); avcmt(); DrawStr(u+1, v+1, "  ", A_REPLACE); mat[(u-x-(u-x)%8)/8][(v-y-(v-y)%8)/8] = 0; LCD_save(temp);};
		
		if (k==268) {new();}
		if (k==270) {save();}
		if (k==271) {load();}
//		if (k==271) {appd(); FontSetSys(F_4x6);}
		if (k==272) {off(); OSInitBetweenKeyDelay(10); OSInitKeyInitDelay(40);}
		if ((k==13) | (k==269)) 
		{
			LCD_restore(temp); sok=1;
			while (sok!=0) {sok=0; resolh(); resolv(); resolc();}
			LCD_save(temp);
		}
		if (k==264) {return 0;}
	}
}

// Main Function
void _main(void)
{
	ClrScr ();
	OSInitBetweenKeyDelay(10);
	OSInitKeyInitDelay(40);
	FontSetSys (F_4x6);
	
	grille(gx,gy); fill(gx,gy);
	LCD_save(temp);
	if (inter(gx,gy)==0) {return;}
	
}