// C Source File
// Created 24/07/2007; 10:34:54

#include <tigcclib.h>
#include "extgraph.h"
#include "sprites.h"
#include "map.h"

#define VIT_WALK 16/OPT_WALK
#define VIT_RUN VIT_WALK/2
#define VIT_WALK_T OPT_WALK
#define VIT_RUN_T VIT_WALK_T*2

// Variables globales
char *light = 0;
char *dark = 0;
int REG_DIR=0,MOVE=FALSE,RUN=FALSE,BIKE=FALSE,SURF=FALSE,ANTI_REPEAT=0,I=0,J=0,MAP_X=3,MAP_Y=2,MAP_x=4,MAP_y=2,M=0,N=0,EVT=0,SWITCH_ANIM=0,END_MOVE=0,TEXT_I=0,NO_REDRAW=0,KEY=FALSE,TEXT_X=5,TEXT_Y=83,NO_EVENT=0,PASS_EVENT=0,VIT_TEXT=0,LIGNE=0,TEMP_REG=-1,MENU_ESC=0,MENU_FCT=0,KEY_TIME=0;
unsigned char TEMP_LGN[2][100],TEMP_TXT=0;


// Options
const int OPT_WALK=2,OPT_CADRE=1,OPT_FONT=0;


// Lecture du clavier
int GetInput(void)
{
	if (_rowread(0x5F)&0x1) return 1;
	if (_rowread(0x7D)&0x2) {OSContrastUp(); ANTI_REPEAT=1;} // Plus
	if (_rowread(0x7D)&0x4) {OSContrastDn(); ANTI_REPEAT=1;} // Moins
	if ((_rowread(0x7E)&0x4) && !MOVE) {KEY=TRUE; KEY_TIME++; REG_DIR=0; MOVE=TRUE; if ((_rowread(0x7E)&0x40) && (!BIKE) && (!SURF)) RUN=TRUE;} // Bas
	if ((_rowread(0x7E)&0x2) && !MOVE) {KEY=TRUE; KEY_TIME++; REG_DIR=1; MOVE=TRUE; if ((_rowread(0x7E)&0x40) && (!BIKE) && (!SURF)) RUN=TRUE;} // Gauche
	if ((_rowread(0x7E)&0x1) && !MOVE) {KEY=TRUE; KEY_TIME++; REG_DIR=2; MOVE=TRUE; if ((_rowread(0x7E)&0x40) && (!BIKE) && (!SURF)) RUN=TRUE;} // Haut
	if ((_rowread(0x7E)&0x8) && !MOVE) {KEY=TRUE; KEY_TIME++; REG_DIR=3; MOVE=TRUE; if ((_rowread(0x7E)&0x40) && (!BIKE) && (!SURF)) RUN=TRUE;} // Droite
	if ((_rowread(0x7E)&0x80) && !MOVE && !ANTI_REPEAT && !RUN && !BIKE) {SURF=1-SURF; ANTI_REPEAT=1;} // Alpha
	if ((_rowread(0x7E)&0x20) && !MOVE && !RUN && !ANTI_REPEAT && !SURF) {BIKE=1-BIKE; ANTI_REPEAT=1;} // Shift -> Alternance entre TRUE et FALSE
	if ((_rowread(0x3F)&0x1) && !MOVE && !ANTI_REPEAT) {if (MENU_FCT==0) {MENU_ESC=1-MENU_ESC; ANTI_REPEAT=1;}} //return 1;	// ESC
	if ((_rowread(0x7E)&0x10) && !ANTI_REPEAT) {ANTI_REPEAT=1;}
	//if ((_rowread(0x7E)&0x10) && !ANTI_REPEAT) {KEY_2ND=1; ANTI_REPEAT=1;} else KEY_2ND=0;
	if (!(_rowread(0x7E)&0x4) && !(_rowread(0x7E)&0x2) && !(_rowread(0x7E)&0x1) && !(_rowread(0x7E)&0x8)) {KEY=FALSE; KEY_TIME=0;}
	if ((ANTI_REPEAT==1) && !(_rowread(0x7E)&0x80) && !(_rowread(0x7E)&0x20) && !(_rowread(0x3F)&0x1) && !(_rowread(0x7D)&0x2) && !(_rowread(0x7D)&0x4) && !(_rowread(0x7E)&0x10)) {ANTI_REPEAT=0;}
	return 0;
}


// Affichage caractères - GrayDrawChar2B(TEXT_X,TEXT_Y,Texte[TEXT_I],A_NORMAL,dark,light);
void StringDisplay(int x,int y, const char s[100])
{
  int i=0,d=strlen(s),e=0;
  
  if (OPT_FONT==0)
  {
  	FontSetSys(F_4x6); printf_xy(x,y,"%s",s);
  }
  else if (OPT_FONT==1)
  {
  	FontSetSys(F_6x8); printf_xy(x,y,"%s",s);
  }
  else if (OPT_FONT==2)
  {
  	x++;
  	for (i=0;i<d;i++)
		{
	  	if (s[i]==' ') {e++; GraySprite16_OR_R(x+i*6-e,y,16,letter[26],letter[26],dark,light);}
	  	else if (s[i]==',') {GraySprite16_OR_R(x+i*6-e,y,16,letter[27],letter[27],dark,light); e+=2;}
	  	else if (s[i]=='~') {GraySprite16_OR_R(x+i*6-e,y-9,16,letter[27],letter[27],dark,light); e+=4;}
	  	else if (s[i]=='.') {GraySprite16_OR_R(x+i*6-e,y,16,letter[28],letter[28],dark,light);}
	  	else if (s[i]=='!') {e-=1; GraySprite16_OR_R(x+i*6-e,y,16,letter[29],letter[29],dark,light); e+=3;}
	  	else if (s[i]=='@') {GraySprite16_OR_R(x+i*6-e,y,16,letter[30],letter[30],dark,light); e+=3;}
	  	else if (s[i]=='é') {GraySprite16_OR_R(x+i*6-e,y,16,letter[31],letter[31],dark,light);}
	  	else if (s[i]=='ê') {GraySprite16_OR_R(x+i*6-e,y,16,letter[32],letter[32],dark,light);}
	  	else if (s[i]=='è') {GraySprite16_OR_R(x+i*6-e,y,16,letter[33],letter[33],dark,light);}
	  	else if (s[i]=='?') {GraySprite16_OR_R(x+i*6-e,y,16,letter[34],letter[34],dark,light); e+=2;}
	  	else if (s[i]=='î') {GraySprite16_OR_R(x+i*6-e,y,16,letter[35],letter[35],dark,light); e+=2;}
	  	else if (s[i]=='à') {GraySprite16_OR_R(x+i*6-e,y,16,letter[36],letter[36],dark,light);}
	  	else if (toascii(s[i])-97>=0)
	  	{
	  		if ((s[i]=='i') && (e!=0)) {e+=1;}
				GraySprite16_OR_R(x+i*6-e,y,16,letter[toascii(s[i])-97],letter[toascii(s[i])-97],dark,light);
				if ((s[i]=='r') || (s[i]=='t') || (s[i]=='f') || (s[i]=='n')  || (s[i]=='s') || (s[i]=='k')) {e++;}
				if (s[i]=='l') {e+=2;}
				if (s[i]=='i') {e+=2;}
	  	}
	  	else {GraySprite16_OR_R(x+i*6-e,y,16,letter_M[toascii(s[i])-65],letter_M[toascii(s[i])-65],dark,light);}
	  }
	  memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE);
	}
}


// Décalage de l'écran
void Decalage()
{
	if ((!RUN) && (!BIKE) && (!SURF)) {if (REG_DIR==0) {N-=VIT_WALK_T;} if (REG_DIR==1) {M+=VIT_WALK_T;} if (REG_DIR==2) {N+=VIT_WALK_T;} if (REG_DIR==3) {M-=VIT_WALK_T;}}
	else {if (REG_DIR==0) {N-=VIT_RUN_T;} if (REG_DIR==1) {M+=VIT_RUN_T;} if (REG_DIR==2) {N+=VIT_RUN_T;} if (REG_DIR==3) {M-=VIT_RUN_T;}}
}


// Gestion position personnage
void Position()
{
	if (REG_DIR==0) {if (MAP_y==4) {MAP_y=0; MAP_Y++;} else MAP_y++;}
	if (REG_DIR==1) {if (MAP_x==0) {MAP_x=8; MAP_X--;} else MAP_x--;}
	if (REG_DIR==2) {if (MAP_y==0) {MAP_y=4; MAP_Y--;} else MAP_y--;}
	if (REG_DIR==3) {if (MAP_x==8) {MAP_x=0; MAP_X++;} else MAP_x++;}
}


// Gestion collision avec Plan 1
int Collision()
{
	if (REG_DIR==0) {if (MAP_y==4) {if (map_1[world[MAP_Y+1][MAP_X]][0*9+MAP_x]!=0) return 1;} else if (map_1[world[MAP_Y][MAP_X]][(MAP_y+1)*9+MAP_x]!=0) return 1;}
	if (REG_DIR==1) {if (MAP_x==0) {if (map_1[world[MAP_Y][MAP_X-1]][MAP_y*9+8]!=0) return 1;} else if (map_1[world[MAP_Y][MAP_X]][MAP_y*9+MAP_x-1]!=0) return 1;}
	if (REG_DIR==2) {if (MAP_y==0) {if (map_1[world[MAP_Y-1][MAP_X]][4*9+MAP_x]!=0) return 1;} else if (map_1[world[MAP_Y][MAP_X]][(MAP_y-1)*9+MAP_x]!=0) return 1;}
	if (REG_DIR==3) {if (MAP_x==8) {if (map_1[world[MAP_Y][MAP_X+1]][MAP_y*9+0]!=0) return 1;} else if (map_1[world[MAP_Y][MAP_X]][MAP_y*9+MAP_x+1]!=0) return 1;}
	return 0;
}


// Cadre
void Cadre(int x1,int y1, int x2, int y2)
{
	if (OPT_CADRE==0)
	{
		GrayFastOutlineRect_R(dark,light,x1,y1,x2,y2,3);
		GrayFastOutlineRect_R(dark,light,x1+1,y1+1,x2-1,y2-1,2); GrayFastOutlineRect_R(dark,light,x1+2,y1+2,x2-2,y2-2,3);
		GrayFastFillRect_R(dark,light,x1+3,y1+3,x2-3,y2-3,0);
	}
	else
	{
		GrayFastOutlineRect_R(dark,light,x1,y1,x2,y2,3);
		GrayFastOutlineRect_R(dark,light,x1+1,y1+1,x2-1,y2-1,2); GrayFastOutlineRect_R(dark,light,x1+2,y1+2,x2-2,y2-2,3);
		GrayFastFillRect_R(dark,light,x1+3,y1+3,x2-3,y2-3,0);
	}
}


// Texte
void CadreTexte(const char Texte[100])
{
	int d=strlen(Texte), e=1;
	if (NO_REDRAW==0) {Cadre(0,78,159,99); TEXT_X=5; TEXT_Y=83; TEXT_I=0; TEMP_TXT=0; NO_REDRAW=1; VIT_TEXT=0; LIGNE=0; TEMP_REG=-1;}
	
	VIT_TEXT++; // Temps d'attente : Le texte s'affiche dès que VIT_TEXT vaut 1 (incrémenté jusqu'a 9 puis retourne à 0).
	if ((TEXT_I<d) && (VIT_TEXT==1))
	{
		if (toascii(Texte[TEXT_I])==47/*"/"*/ && (LIGNE==0)) {TEXT_X=5; TEXT_Y=90; LIGNE++; TEMP_TXT=0;}
		else if (toascii(Texte[TEXT_I])==47/*"/"*/ && (LIGNE!=0)) // Défilement du texte vers le haut
		{
			GraySprite8_OR_R(TEXT_X,TEXT_Y,8,arrow,arrow,dark,light); memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE);
			while (!(_rowread(0x7E)&0x10) || ANTI_REPEAT)
			{
				if (!(_rowread(0x7E)&0x10)) ANTI_REPEAT=0;
				if ((_rowread(0x7E)&0x4) || (_rowread(0x7E)&0x2) || (_rowread(0x7E)&0x8)) {goto _ENDDLOG;}
				GraySprite8_AND_R(TEXT_X,TEXT_Y+1-e-2,8,vide,vide,dark,light);
				GraySprite8_OR_R(TEXT_X,TEXT_Y+e,8,arrow,arrow,dark,light); memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE); e=1-e;
			}
			GrayFastFillRect_R(dark,light,3,82,156,96,0); GrayDrawStr2B(5,83,TEMP_LGN[LIGNE],A_NORMAL,dark,light);
			memset(TEMP_LGN[0],0,100*sizeof(char)); strcpy(TEMP_LGN[0],TEMP_LGN[1]); TEXT_X=5; TEXT_Y=90; LIGNE=1; TEMP_TXT=0;
		}
		else if (toascii(Texte[TEXT_I])==35/*"#"*/) // Saut de page
		{
			GraySprite8_OR_R(TEXT_X,TEXT_Y,8,arrow,arrow,dark,light); memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE);
			//while (!(_rowread(0x7E)&0x10))
			while (!(_rowread(0x7E)&0x10) || ANTI_REPEAT)
			{
				if (!(_rowread(0x7E)&0x10)) ANTI_REPEAT=0;
				if ((_rowread(0x7E)&0x4) || (_rowread(0x7E)&0x2) || (_rowread(0x7E)&0x8)) {goto _ENDDLOG;}
				GraySprite8_AND_R(TEXT_X,TEXT_Y+1-e-2,8,vide,vide,dark,light);
				GraySprite8_OR_R(TEXT_X,TEXT_Y+e,8,arrow,arrow,dark,light); memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE); e=1-e;
			}
			GrayFastFillRect_R(dark,light,3,82,156,96,0); memset(TEMP_LGN[0],0,100*sizeof(char));
			TEXT_X=5; TEXT_Y=83; LIGNE=0; TEMP_TXT=0;
		}
		else {GrayDrawChar2B(TEXT_X,TEXT_Y,Texte[TEXT_I],A_NORMAL,dark,light); TEXT_X+=FontCharWidth(Texte[TEXT_I]); TEMP_LGN[LIGNE][TEMP_TXT]=Texte[TEXT_I]; TEMP_TXT++;}
	}
	
	else if (VIT_TEXT==1)
	{
		while (!(_rowread(0x7E)&0x10) || ANTI_REPEAT)
		{
			if (!(_rowread(0x7E)&0x10)) ANTI_REPEAT=0;
			if ((_rowread(0x7E)&0x4) || (_rowread(0x7E)&0x2) || (_rowread(0x7E)&0x8)) {goto _ENDDLOG;}
			GraySprite8_OR_R(TEXT_X,TEXT_Y,8,carre,carre,dark,light);
			if (e==2) {GraySprite8_AND_R(TEXT_X,TEXT_Y-1,8,vide,vide,dark,light); e=0;}
			e++;
			memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE);
		}
		/*2nd*/if (_rowread(0x7E)&0x10) {NO_REDRAW=0; PASS_EVENT=1; LIGNE=1; memset(TEMP_LGN[1],0,100*sizeof(char)); TEMP_REG=REG_DIR; ANTI_REPEAT=1;}
	}
	if (_rowread(0x7E)&0x40) {VIT_TEXT=9;}
	if (VIT_TEXT==9) {TEXT_I++; VIT_TEXT=0;}
	//printf_xy(0,0,"%i - %i",TEXT_I,VIT_TEXT);
	
	memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE);
	if ((_rowread(0x7E)&0x2) ^ (_rowread(0x7E)&0x8) ^ (_rowread(0x7E)&0x4)) {_ENDDLOG: NO_REDRAW=0; PASS_EVENT=1; LIGNE=1; memset(TEMP_LGN[0],0,100*sizeof(char));}
}


// Capture des évenements
void Event()
{
	EVT=0;
	if (REG_DIR==0) {if (MAP_y==4) EVT=event[world[MAP_Y+1][MAP_X]][0*9+MAP_x]; else EVT=event[world[MAP_Y][MAP_X]][(MAP_y+1)*9+MAP_x];}
	if (REG_DIR==1) {if (MAP_x==0) EVT=event[world[MAP_Y][MAP_X-1]][MAP_y*9+8]; else EVT=event[world[MAP_Y][MAP_X]][MAP_y*9+MAP_x-1];}
	if (REG_DIR==2) {if (MAP_y==0) EVT=event[world[MAP_Y-1][MAP_X]][4*9+MAP_x]; else EVT=event[world[MAP_Y][MAP_X]][(MAP_y-1)*9+MAP_x];}
	if (REG_DIR==3) {if (MAP_x==8) EVT=event[world[MAP_Y][MAP_X+1]][MAP_y*9+0]; else EVT=event[world[MAP_Y][MAP_X]][MAP_y*9+MAP_x+1];}
}


// Gestion des évenements
void G_Event()
{
	if (TEMP_REG==-1)
	{
		if ((EVT==1) && (REG_DIR==2)) {CadreTexte("A PROPOS DE...#POKeMON VERSION TITANIUM/(c) 2008 SENG STEPHANE");}
		if ((EVT==2) && (REG_DIR==2)) {CadreTexte("BOUTIQUE POKeMON#Achetez tout ce qui vous plait!");}
		if ((EVT==3) && (REG_DIR==2) && (KEY_TIME==5)) {printf_xy(0,0,"%s","OK!");}
	}
	if ((TEMP_REG!=REG_DIR) || ((_rowread(0x7E)&0x10) && !ANTI_REPEAT)) {TEMP_REG=-1;}
}


// Menu PAUSE
void Menu()
{
	if (NO_REDRAW==0) {Cadre(110,0,159,99); NO_REDRAW=1; PASS_EVENT=1; MENU_FCT=1;}
	GrayDrawStr2B(115,5,"POKeMON",A_NORMAL,dark,light);
	GrayDrawStr2B(115,12,"SAC",A_NORMAL,dark,light);
	GrayDrawStr2B(115,19,"SACHA",A_NORMAL,dark,light);
	GrayDrawStr2B(115,26,"SAUVER",A_NORMAL,dark,light);
	GrayDrawStr2B(115,33,"RETOUR",A_NORMAL,dark,light);
	GrayDrawStr2B(115,40,"QUITTER",A_NORMAL,dark,light);
	memcpy(GetPlane(0),light,LCD_SIZE); memcpy(GetPlane(1),dark,LCD_SIZE);
	if ((_rowread(0x3F)&0x1) && !(ANTI_REPEAT)) {MENU_FCT=0; MENU_ESC=0; NO_REDRAW=0; PASS_EVENT=1; ANTI_REPEAT++;}
}


// Affiche le terrain
void DrawTerrain()
{
	// Variables locales
	int X=MAP_X,x=MAP_x,Y=MAP_Y,y=MAP_y,TEMP_x,TEMP_y,Z=y*9+x;
	
	// Efface l'écran
	ClearGrayScreen2B(dark,light);
	
	// Evite l'affichage du personnage si matien de la direction
	if (J==1) {I=5; J=0;}
	
	// Background
	if (y-4<0) {Y--; y+=1;} else y-=4;
	for (TEMP_y=-22;TEMP_y<116;TEMP_y+=16)
	{
		X=MAP_X; x=MAP_x;
		if (x-6<0) {X--; x+=3;} else x-=6;
		for (TEMP_x=-24;TEMP_x<176;TEMP_x+=16)
		{
			Z=y*9+x;
			GrayClipSprite16_OR_R(TEMP_x+M,TEMP_y+N,16,sprite_0[map_0[world[Y][X]][Z]],&sprite_0[map_0[world[Y][X]][Z]][16],dark,light);
			if (map_1[world[Y][X]][Z]!=0) GrayClipSprite16_MASK_R(TEMP_x+M,TEMP_y+N,16,sprite_1[map_1[world[Y][X]][Z]],&sprite_1[map_1[world[Y][X]][Z]][16],sprite_1_M[map_1[world[Y][X]][y*9+x]],sprite_1_M[map_1[world[Y][X]][Z]],dark,light);
			x++; if (x>8) {X++; x=0;}
		}
		y++; if (y>4) {Y++; y=0;}
	}
	
	// Personnage immobile
	if ((!MOVE) && (!BIKE) && (!SURF)) {GraySprite32_MASK_R(64,32,32,sacha[REG_DIR],&sacha[REG_DIR][32],sacha_M[REG_DIR],sacha_M[REG_DIR],dark,light); I=0; Event();}
	// Personnage en vélo
	else if (((!MOVE) && (BIKE==TRUE)) || ((J==-1) && (BIKE==TRUE))) {GraySprite32_MASK_R(64,32,32,sacha_B[REG_DIR],&sacha_B[REG_DIR][32],sacha_B_M[REG_DIR],sacha_B_M[REG_DIR],dark,light); I=0; J=0; RUN=FALSE; Event();}
	// Personnage qui surfe
	else if (((!MOVE) && (SURF==TRUE)) || ((J==-1) && (SURF==TRUE))) {GraySprite32_MASK_R(64,32,32,sacha_S[REG_DIR],&sacha_S[REG_DIR][32],sacha_S_M[REG_DIR],sacha_S_M[REG_DIR],dark,light); I=0; J=0; RUN=FALSE; Event();}
	
	// Personnage
	if (((MOVE==TRUE) && (!RUN) && (!BIKE) && (!SURF)) || ((RUN==TRUE) && (I<5)))
	{
		I++; Event();
		if (Collision()!=0) {MOVE=FALSE; I=0;}
		if (I==0) 			GraySprite32_MASK_R(64,32,32,sacha[REG_DIR],&sacha[REG_DIR][32],sacha_M[REG_DIR],sacha_M[REG_DIR],dark,light);
		else if (I<5)		{GraySprite32_MASK_R(64,32,32,sacha[REG_DIR],&sacha[REG_DIR][32],sacha_M[REG_DIR],sacha_M[REG_DIR],dark,light);
										 if (!(_rowread(0x7E)&0x4)) MOVE=FALSE;
										 if (!(_rowread(0x7E)&0x2)) MOVE=FALSE;
										 if (!(_rowread(0x7E)&0x1)) MOVE=FALSE;
										 if (!(_rowread(0x7E)&0x8)) MOVE=FALSE;}
		else if (I<VIT_WALK+5)  {GraySprite32_MASK_R(64,32,32,sacha_A[REG_DIR*2+SWITCH_ANIM],&sacha_A[REG_DIR*2+SWITCH_ANIM][32],sacha_A_M[REG_DIR*2+SWITCH_ANIM],sacha_A_M[REG_DIR*2+SWITCH_ANIM],dark,light);}
		else if (I==VIT_WALK+5) {GraySprite32_MASK_R(64,32,32,sacha_A[REG_DIR*2+1],&sacha_A[REG_DIR*2+1][32],sacha_A_M[REG_DIR*2+1],sacha_A_M[REG_DIR*2+1],dark,light); MOVE=FALSE; J=1; SWITCH_ANIM=1-SWITCH_ANIM;}
	}
	else if ((MOVE==TRUE) && (RUN==TRUE) && (!BIKE) && (!SURF))
	{
		I++; Event();
		if (Collision()!=0) {MOVE=FALSE; I=0;}
		if (I==0) 			GraySprite32_MASK_R(64,32,32,sacha[REG_DIR],&sacha[REG_DIR][32],sacha_M[REG_DIR],sacha_M[REG_DIR],dark,light);
		else if (I<5) 	{GraySprite32_MASK_R(64,32,32,sacha_R[REG_DIR],&sacha_R[REG_DIR][32],sacha_R_M[REG_DIR],sacha_R_M[REG_DIR],dark,light);
										 if (!(_rowread(0x7E)&0x4)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x2)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x1)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x8)) {MOVE=FALSE; RUN=FALSE;}}
		else if (I<VIT_RUN+5)  {GraySprite32_MASK_R(64,32,32,sacha_R_A[REG_DIR*2+SWITCH_ANIM],&sacha_R_A[REG_DIR*2+SWITCH_ANIM][32],sacha_R_A_M[REG_DIR*2+SWITCH_ANIM],sacha_R_A_M[REG_DIR*2+SWITCH_ANIM],dark,light);}
		else if (I==VIT_RUN+5) {GraySprite32_MASK_R(64,32,32,sacha_R_A[REG_DIR*2+1],&sacha_R_A[REG_DIR*2+1][32],sacha_R_A_M[REG_DIR*2+1],sacha_R_A_M[REG_DIR*2+1],dark,light); MOVE=FALSE; RUN=FALSE; END_MOVE=1; /*I=0;*/ J=1; /*Position(); M=0; N=0;*/ SWITCH_ANIM=1-SWITCH_ANIM;}
	}
	else if ((MOVE==TRUE) && (!RUN) && (BIKE==TRUE) && (!SURF))
	{
		I++; Event();
		if (Collision()!=0) {MOVE=FALSE; I=0;}
		if (I==0) 			GraySprite32_MASK_R(64,32,32,sacha_B[REG_DIR],&sacha_B[REG_DIR][32],sacha_B_M[REG_DIR],sacha_B_M[REG_DIR],dark,light);
		else if (I<5) 	{GraySprite32_MASK_R(64,32,32,sacha_B[REG_DIR],&sacha_B[REG_DIR][32],sacha_B_M[REG_DIR],sacha_B_M[REG_DIR],dark,light);
										 if (!(_rowread(0x7E)&0x4)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x2)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x1)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x8)) {MOVE=FALSE; RUN=FALSE;}}
		else if (I<VIT_RUN+5)  {GraySprite32_MASK_R(64,32,32,sacha_B_A[REG_DIR*2+SWITCH_ANIM],&sacha_B_A[REG_DIR*2+SWITCH_ANIM][32],sacha_B_A_M[REG_DIR*2+SWITCH_ANIM],sacha_B_A_M[REG_DIR*2+SWITCH_ANIM],dark,light);}
		else if (I==VIT_RUN+5) {GraySprite32_MASK_R(64,32,32,sacha_B[REG_DIR],&sacha_B[REG_DIR][32],sacha_B_M[REG_DIR],sacha_B_M[REG_DIR],dark,light); MOVE=FALSE; RUN=FALSE; J=1; SWITCH_ANIM=1-SWITCH_ANIM;}
	}
	else if ((MOVE==TRUE) && (!RUN) && (!BIKE) && (SURF==TRUE))
	{
		I++; Event();
		if (Collision()!=0) {MOVE=FALSE; I=0;}
		if (I==0) 			GraySprite32_MASK_R(64,32,32,sacha_S[REG_DIR],&sacha_S[REG_DIR][32],sacha_S_M[REG_DIR],sacha_S_M[REG_DIR],dark,light);
		else if (I<5) 	{GraySprite32_MASK_R(64,32,32,sacha_S[REG_DIR],&sacha_S[REG_DIR][32],sacha_S_M[REG_DIR],sacha_S_M[REG_DIR],dark,light);
										 if (!(_rowread(0x7E)&0x4)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x2)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x1)) {MOVE=FALSE; RUN=FALSE;}
										 if (!(_rowread(0x7E)&0x8)) {MOVE=FALSE; RUN=FALSE;}}
		else if (I<VIT_RUN+5)  {GraySprite32_MASK_R(64,32,32,sacha_S_A[REG_DIR*2+SWITCH_ANIM],&sacha_S_A[REG_DIR*2+SWITCH_ANIM][32],sacha_S_A_M[REG_DIR*2+SWITCH_ANIM],sacha_S_A_M[REG_DIR*2+SWITCH_ANIM],dark,light);}
		else if (I==VIT_RUN+5) {GraySprite32_MASK_R(64,32,32,sacha_S[REG_DIR],&sacha_S[REG_DIR][32],sacha_S_M[REG_DIR],sacha_S_M[REG_DIR],dark,light); MOVE=FALSE; RUN=FALSE; J=1; SWITCH_ANIM=1-SWITCH_ANIM;}
	}
	
	// Plan 2
	X=MAP_X,x=MAP_x,Y=MAP_Y,y=MAP_y;
	if (y-4<0) {Y--; y+=1;} else y-=4;
	for (TEMP_y=-22;TEMP_y<116;TEMP_y+=16)
	{
		X=MAP_X; x=MAP_x;
		if (x-6<0) {X--; x+=3;} else x-=6;
		for (TEMP_x=-24;TEMP_x<176;TEMP_x+=16)
		{
			Z=y*9+x;
			if (map_2[world[Y][X]][Z]!=0) GrayClipSprite16_MASK_R(TEMP_x+M,TEMP_y+N,16,sprite_1[map_2[world[Y][X]][Z]],&sprite_1[map_2[world[Y][X]][Z]][16],sprite_1_M[map_2[world[Y][X]][Z]],sprite_1_M[map_2[world[Y][X]][Z]],dark,light);
			x++; if (x>8) {X++; x=0;}
		}
		y++; if (y>4) {Y++; y=0;}
	}
	
	if ((MOVE==TRUE) && (I>5) && (I<=VIT_WALK+5)) {Decalage();} if ((I==VIT_WALK+5) && (!SURF) && (!RUN) && (!BIKE)) {I=0; Position(); M=0; N=0;} else if ((I==VIT_RUN+5) && ((END_MOVE==TRUE) || (BIKE==TRUE) || (SURF==TRUE))) {I=0; Position(); M=0; N=0; END_MOVE=0;}
	
	// Affiche le contenu de l'écran virtuel
	memcpy(GetPlane(0),light,LCD_SIZE);
	memcpy(GetPlane(1),dark,LCD_SIZE);
}


// Fonction Principale
void _main(void)
{
	// Variables locales
	int k=0; //ESI ptr=top_estack;
	
	// Désactivation des associations de touches
	INT_HANDLER ai1,ai5;
	ai1 = GetIntVec(AUTO_INT_1);
	ai5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_5,DUMMY_HANDLER);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	FontSetSys(F_4x6);
	
	// Ecrans virtuels
	dark = malloc(LCD_SIZE); light = malloc(LCD_SIZE);
	
	// Erreur de l'allocation mémoire
	if (!dark | !light) {ST_helpMsg("Echec de l'allocation mémoire"); return;}
	
	// Efface l'écran et active les niveaux de gris
	if(!GrayOn()) return; 
	GrayAdjust(-27); OSContrastDn(); OSContrastDn();
	
	// Boucle principale
	while (k==0) {k=GetInput(); if (MENU_ESC==1) {Menu();} if (NO_REDRAW==0) DrawTerrain(); if (PASS_EVENT==0) {G_Event();} else {PASS_EVENT=0;}}
	/*while (k==0) {k=GetInput(); OPT_FONT=0; StringDisplay(0,0,"CENTRE POKéMON"); 
															OPT_FONT=1; StringDisplay(0,20,"CENTRE POKéMON");
															OPT_FONT=2; StringDisplay(0,40,"CENTRE POKéMON");}*/
	
	// Fin du programme
	OSContrastUp(); OSContrastUp();
	GrayOff();
	
	// Réactivation des associations de touches
	SetIntVec(AUTO_INT_1,ai1);
	SetIntVec(AUTO_INT_5,ai5);
	
	// Libération de la mémoire
	free(light);
	free(dark);
	
	// Efface l'appel de touche
	GKeyFlush();
}