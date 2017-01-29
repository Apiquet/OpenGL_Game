/*****************************************************************************\
 * TP CPE, 3ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
\*****************************************************************************/

#define texte_Afficher true

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>

#include "glutils.hpp"

#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "triangle_index.hpp"
#include "vertex_opengl.hpp"
#include "image.hpp"
#include "mesh.hpp"


/*****************************************************************************\
 * Variables globales
 *
 *
\*****************************************************************************/
//int etat=0;
//double saut=0;

//variable de début et fin jeu
int start_game=0;
//variable mise à 1 si contact obstacle ou chute
int you_loose=0;

float angle_milieu=0;
//la vitesse du jeu s'incrément au fur et à mesure
float vitesse_jeu=0.08;

//Mise à 1 de prise_rampe si moto sur une rampe
float prise_rampe=0;
float numero_rampe=0;
float vitesse_montee=0.04f;
float chute=0;
//backflip passe à un lorsque le joueur souhaite en faire un (en appuyant sur A)
float backflip=0;
float vitesse_backflip=0.177;
//Variable permettant des translations aléatoires des pièces et obstacles
float position_piece1=0;
float position_piece2=0;
float position_legoman=0;
float position_legoman2=0;

//score qui s'incrémente
int score=0;
float position_rampe=0;

void Affiche_texte(int x, int y,int z, char *string, int valeur,void *police)
{//Fonction permettant d'afficher un texte (Concaténation de string + variable global Score) avec la police "police"
   /* Police : GLUT_BITMAP_HELVETICA_18
    * GLUT_BITMAP_HELVETICA_12
    * GLUT_BITMAP_HELVETICA_10
    * GLUT_BITMAP_TIMES_ROMAN_24
    * GLUT_BITMAP_TIMES_ROMAN_10
    * GLUT_BITMAP_9_BY_15
    * GLUT_BITMAP_9_BY_13
    * */

   char  Affichage_Score[]={};
    if(valeur!=-1)
   sprintf(Affichage_Score,"%s %d",string,valeur);//Concaténation
    else
        sprintf(Affichage_Score,"%s ",string);//Concaténation


   glRasterPos3f(x,y,z); // Positionnement du premier caractère
   for (int i = 0; i < (int) std::strlen(Affichage_Score); i++) glutBitmapCharacter(police,Affichage_Score[i]); // Affiche l'ensemble de la chaine
}
//identifiant du shader
GLuint shader_program_id;

//identifiants pour object 1
GLuint vbo_object_1=0;
GLuint vboi_object_1=0;
GLuint texture_id_object_1=0;
int nbr_triangle_object_1;

//identifiants pour object 2
GLuint vbo_object_2=0;
GLuint vboi_object_2=0;
GLuint texture_id_object_2=0;
int nbr_triangle_object_2;

//identifiants pour object 3
GLuint vbo_object_3=0;
GLuint vboi_object_3=0;
GLuint texture_id_object_3=0;
int nbr_triangle_object_3;

//identifiants pour object 4
GLuint vbo_object_4=0;
GLuint vboi_object_4=0;
GLuint texture_id_object_4=0;
int nbr_triangle_object_4;

//identifiants pour object 5
GLuint vbo_object_5=0;
GLuint vboi_object_5=0;
GLuint texture_id_object_5=0;
int nbr_triangle_object_5;

//identifiants pour object 6
GLuint vbo_object_6=0;
GLuint vboi_object_6=0;
GLuint texture_id_object_6=0;
int nbr_triangle_object_6;

//identifiants pour object 7
GLuint vbo_object_7=0;
GLuint vboi_object_7=0;
GLuint texture_id_object_7=0;
int nbr_triangle_object_7;

//identifiants pour object 8
GLuint vbo_object_8=0;
GLuint vboi_object_8=0;
GLuint texture_id_object_8=0;
int nbr_triangle_object_8;

//identifiants pour object 9
GLuint vbo_object_9=0;
GLuint vboi_object_9=0;
GLuint texture_id_object_9=0;
int nbr_triangle_object_9;

//identifiants pour object 10
GLuint vbo_object_10=0;
GLuint vboi_object_10=0;
GLuint texture_id_object_10=0;
int nbr_triangle_object_10;

//Matrice de transformation
struct transformation
{
    mat4 rotation;
    vec3 rotation_center;
    vec3 translation;

    transformation():rotation(),rotation_center(),translation(){}
};

//Transformation des modeles
transformation transformation_model_1;
transformation transformation_model_2;
transformation transformation_model_3;
transformation transformation_model_4;
transformation transformation_model_5;
transformation transformation_model_6;
transformation transformation_model_7;
transformation transformation_model_8;
transformation transformation_model_9;
transformation transformation_model_10;
//Transformation de la vue (camera)
transformation transformation_view;

//Matrice de projection
mat4 projection;

//angle de deplacement
float angle_x_model_1 = 0.0f;
float angle_y_model_1 = 0.0f;
float angle_view = 0.0f;
float angle_x_model_2 = 0.0f;
float angle_y_model_2 = 0.0f;

float angle_x_model_3 = 0.0f;
float angle_y_model_3 = 0.0f;

float angle_x_model_4 = 0.0f;
float angle_y_model_4 = 1.55f;
float angle_z_model_4 = 1.55f;

float angle_x_model_5 = 0.0f;
float angle_y_model_5 = 0.0f;

float angle_x_model_6 = 0.0f;
float angle_y_model_6 = 0.0f;

float angle_x_model_7 = 0.0f;
float angle_y_model_7 = -1.0f;
float angle_z_model_7=  0.0f;

float angle_x_model_8 = 1.55f;
float angle_y_model_8 = 0.0f;
float angle_z_model_8 = 1.55f;

float angle_x_model_9 = 1.55f;
float angle_y_model_9 = 0.0f;
float angle_z_model_9 =  1.55f;

float angle_x_model_10 = 0.0f;
float angle_y_model_10 = -1.0f;
float angle_z_model_10 =  0.0f;

void load_texture(const char* filename,GLuint *texture_id);

void init_model_1();
void init_model_2();
void init_model_3();
void init_model_4();
void init_model_5();
void init_model_7();
void init_model_8();
void init_model_9();
void init_model_10();


void draw_model_1();
void draw_model_2();
void draw_model_3();
void draw_model_4();
void draw_model_5();
void draw_model_7();
void draw_model_8();
void draw_model_9();
void draw_model_10();


void deplacement_decors();
void saut_rampe();
void collision();


static void init()
{

    // Chargement du shader
    shader_program_id = read_shader("shader.vert", "shader.frag");

    //matrice de projection
    projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
    glUniformMatrix4fv(get_uni_loc(shader_program_id,"projection"),1,false,pointeur(projection)); PRINT_OPENGL_ERROR();

    //centre de rotation de la 'camera' (les objets sont centres en z=-2)
    transformation_view.rotation_center = vec3(0.0f,0.0f,-2.0f);

    //activation de la gestion de la profondeur
    glEnable(GL_DEPTH_TEST); PRINT_OPENGL_ERROR();

    // Charge modele 1 sur la carte graphique
    init_model_1();
    // Charge modele 2 sur la carte graphique
    init_model_2();
    // Charge modele 3 sur la carte graphique
    init_model_3();
    init_model_4();
    init_model_5();
    init_model_7();
    init_model_8();
    init_model_9();
    init_model_10();
}


//Fonction d'affichage
static void display_callback()
{
    //effacement des couleurs du fond d'ecran
    glClearColor(0.1f, 1.0f, 1.0f, 1.0f);                 PRINT_OPENGL_ERROR();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   PRINT_OPENGL_ERROR();

    // Affecte les parametres uniformes de la vue (identique pour tous les modeles de la scene)
    {
        //envoie de la rotation
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_view"),1,false,pointeur(transformation_view.rotation)); PRINT_OPENGL_ERROR();

        //envoie du centre de rotation
        vec3 cv = transformation_view.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_view") , cv.x,cv.y,cv.z , 0.0f); PRINT_OPENGL_ERROR();

        //envoie de la translation
        vec3 tv = transformation_view.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_view") , tv.x,tv.y,tv.z , 0.0f); PRINT_OPENGL_ERROR();
    }


    // Affiche le modele numero 1 (dinosaure)
    draw_model_1();
    // Affiche le modele numero 2 (sol)
    draw_model_2();
    // Affiche le modele numero 2 (monstre)
    draw_model_3();
    draw_model_4();
    draw_model_5();
    draw_model_7();
    draw_model_8();
    draw_model_9();
    draw_model_10();
    //envoie des parametres uniformes
    if(texte_Afficher)
    {
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_view.rotation));    PRINT_OPENGL_ERROR();
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") ,0,0,0, 0.0f);                                 PRINT_OPENGL_ERROR();
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , 0,0,0 , 0.0f);                                     PRINT_OPENGL_ERROR();
    }
//    Affiche_texte(-1,1,-1,"Score",score,GLUT_BITMAP_HELVETICA_18);
  Affiche_texte(-0.8,1,-1,"Niveau:",numero_rampe,GLUT_BITMAP_HELVETICA_18);
    if(start_game==0 && you_loose==0)
    {
        Affiche_texte(-4,10,-20,"Press SPACE to START",-1,GLUT_BITMAP_TIMES_ROMAN_24);
    }
    if(prise_rampe==1 && backflip!=1)
        Affiche_texte(-1,1,-2,"Press A to BACKFLIP",-1,GLUT_BITMAP_HELVETICA_18);
    if(you_loose==1) Affiche_texte(-0.8,0.5,-1,"You lose",-1,GLUT_BITMAP_HELVETICA_18);
        //Changement de bufferd'affichage pour eviter un effet de scintillement
    }
    glutSwapBuffers();


}



/*****************************************************************************\
 * keyboard_callback                                                         *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
    float d_angle=0.1f;
    float dz=0.5f;

    //quitte le programme si on appuie sur les touches 'q', 'Q', ou 'echap'
    switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
        exit(0);
    break;
    //appuie sur a pour faire un backflip
    case 'a':
    case 'A':
        if(prise_rampe==1) backflip=1;
    break;
    //début du jeu en appuyant sur espace
    case 32:
        if(start_game==0 && you_loose==0){
        start_game=1;
        angle_y_model_1+=31*d_angle;
        angle_milieu=angle_y_model_1;
        transformation_view.translation.z -= 2*dz;

        }
        break;

    }

    transformation_model_1.rotation = matrice_rotation(angle_y_model_1 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_1 , 1.0f,0.0f,0.0f);
    transformation_view.rotation = matrice_rotation(angle_view , 0.0f,1.0f,0.0f);

    transformation_model_3.rotation = matrice_rotation(angle_y_model_3 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_3 , 1.0f,0.0f,0.0f);
}

/*****************************************************************************\
 * special_callback                                                          *
\*****************************************************************************/
static void special_callback(int key, int,int)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        if(angle_y_model_1<angle_milieu+0.60)
        angle_y_model_1 += 0.03;
        break;
    case GLUT_KEY_RIGHT:
        if(angle_y_model_1>angle_milieu-0.60)
         angle_y_model_1 -= 0.03;
        break;
    }

    transformation_model_1.rotation = matrice_rotation(angle_y_model_1 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_1 , 1.0f,0.0f,0.0f);
    transformation_model_2.rotation = matrice_rotation(angle_y_model_2 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_2 , 1.0f,0.0f,0.0f);
    transformation_model_3.rotation = matrice_rotation(angle_y_model_3 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_3 , 1.0f,0.0f,0.0f);
    transformation_model_4.rotation = matrice_rotation(angle_y_model_4 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_4 , 1.0f,0.0f,0.0f) * matrice_rotation(angle_z_model_4 , 0.0f,0.0f,1.0f);
    transformation_model_5.rotation = matrice_rotation(angle_y_model_5 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_5 , 1.0f,0.0f,0.0f);
    transformation_view.rotation = matrice_rotation(angle_view , 0.0f,1.0f,0.0f);

    //reactualisation de l'affichage
    glutPostRedisplay();
}

void deplacement_decors(){
    if(start_game==1) {
        vitesse_jeu+=0.0002;
        //Déplcament sol
        if(transformation_model_2.translation.z>100) transformation_model_2.translation.z-=100;
        //Déplacement mur et rampe
        score++;
        if(transformation_model_4.translation.z>30) {
            transformation_model_4.translation.z-=100;
            transformation_model_5.translation.z-=100;
            if(transformation_model_2.translation.x<0) transformation_model_5.translation.x-=3;
            else if(transformation_model_2.translation.x>0) transformation_model_5.translation.x+=3;
            numero_rampe+=1;
            vitesse_montee+=0.033;
            vitesse_backflip+=0.0014;


        }
        //Déplacement monstre
        if(transformation_model_3.translation.z>220) {
            transformation_model_3.translation.z-=450;
        }

        //Déplacement LEGO MAN
        if(transformation_model_7.translation.z>55) {
            transformation_model_7.translation.z-=100;
            if(position_legoman==0) transformation_model_7.translation.x-=2.5;
            if(position_legoman==1) transformation_model_7.translation.x+=1;
            if(position_legoman==0) position_legoman=1;
            else position_legoman=0;
            if(transformation_model_7.translation.x-transformation_model_2.translation.x>2.8) transformation_model_7.translation.x-=5;
            if(transformation_model_2.translation.x-transformation_model_7.translation.x>2.8) transformation_model_7.translation.x+=5;
        }
        //Déplacement LEGO MAN 2
        if(transformation_model_10.translation.z>95) {
            transformation_model_10.translation.z-=100;
            if(position_legoman2==0) transformation_model_10.translation.x-=1;
            if(position_legoman2==1) transformation_model_10.translation.x+=2.5;
            if(position_legoman2==0) position_legoman2=1;
            else position_legoman2=0;
            if(transformation_model_10.translation.x-transformation_model_2.translation.x>2.8) transformation_model_10.translation.x-=5;
            if(transformation_model_2.translation.x-transformation_model_10.translation.x>2.8) transformation_model_10.translation.x+=5;
        }
        //Déplacement pièce 1
        if(transformation_model_8.translation.z>75) {
            transformation_model_8.translation.z-=35;
            if(position_piece1==0) transformation_model_8.translation.x=2;
            if(position_piece1==1) transformation_model_8.translation.x=-2;
            if(position_piece1==0) position_piece1=1;
            else position_piece1=0;
            if(transformation_model_8.translation.x-transformation_model_2.translation.x>4) transformation_model_8.translation.x-=4.5;
            if(transformation_model_2.translation.x-transformation_model_8.translation.x>4) transformation_model_8.translation.x+=4.5;
        }
        //Déplacement pièce2
        if(transformation_model_9.translation.z>70) {
            transformation_model_9.translation.z-=35;
            if(position_piece2==0) transformation_model_9.translation.x=2;
            if(position_piece2==1) transformation_model_9.translation.x=-2;
            if(position_piece2==0) position_piece2=1;
            else position_piece2=0;
            if(transformation_model_9.translation.x-transformation_model_2.translation.x>4) transformation_model_9.translation.x-=4.5;
            if(transformation_model_2.translation.x-transformation_model_9.translation.x>4) transformation_model_9.translation.x+=4.5;
        }
        //Déplacement du décors
        transformation_model_2.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_2.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);

        transformation_model_3.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_3.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);

        transformation_model_4.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_4.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);

        transformation_model_5.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_5.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);

        transformation_model_6.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_6.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);

        transformation_model_7.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_7.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);
        //LEGO MAN tourne sur lui-même
        angle_y_model_7+=0.15f;

        transformation_model_8.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_8.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);
        //Piece 1 tourne sur elle-même
        angle_y_model_8+=0.12f;

        transformation_model_9.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_9.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);
        //Piece 2 tourne sur elle-même
        angle_y_model_9+=0.12f;

        transformation_model_10.translation.x+=vitesse_jeu*sin(angle_y_model_1+3.14);
        transformation_model_10.translation.z+=vitesse_jeu*cos(angle_y_model_1+3.14);
        //LEGO MAN 2 tourne sur lui-même
        angle_y_model_10+=0.15f;
    }

    //Actualisation
    transformation_model_1.rotation = matrice_rotation(angle_y_model_1 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_1 , 1.0f,0.0f,0.0f);
    transformation_model_2.rotation = matrice_rotation(angle_y_model_2 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_2 , 1.0f,0.0f,0.0f);
    transformation_model_3.rotation = matrice_rotation(angle_y_model_3 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_3 , 1.0f,0.0f,0.0f);
    transformation_model_4.rotation = matrice_rotation(angle_y_model_4 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_4 , 1.0f,0.0f,0.0f) * matrice_rotation(angle_z_model_4 , 0.0f,0.0f,1.0f);
    transformation_model_5.rotation = matrice_rotation(angle_y_model_5 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_5 , 1.0f,0.0f,0.0f);
    transformation_model_7.rotation = matrice_rotation(angle_y_model_7 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_7 , 1.0f,0.0f,0.0f) * matrice_rotation(angle_z_model_7 , 0.0f,0.0f,1.0f);
    transformation_model_8.rotation = matrice_rotation(angle_y_model_8 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_8 , 1.0f,0.0f,0.0f) * matrice_rotation(angle_z_model_8 , 0.0f,0.0f,1.0f);
    transformation_model_9.rotation = matrice_rotation(angle_y_model_9 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_9 , 1.0f,0.0f,0.0f) * matrice_rotation(angle_z_model_9 , 0.0f,0.0f,1.0f);
    transformation_model_10.rotation = matrice_rotation(angle_y_model_10 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_10 , 1.0f,0.0f,0.0f) * matrice_rotation(angle_z_model_10 , 0.0f,0.0f,1.0f);

    transformation_view.rotation = matrice_rotation(angle_view , 0.0f,1.0f,0.0f);


}
void saut_rampe(){
    float vitesse_descente=0.13;
    //Prise d'une rampe
    if(transformation_model_5.translation.x<-2.5 && transformation_model_5.translation.x>-5)
    {
        if(transformation_model_5.translation.z>17 && transformation_model_5.translation.z<18)
        {
            prise_rampe=1;

        }
    }
    if(transformation_model_5.translation.x>-2.5 || transformation_model_5.translation.x<-5)
    {
            prise_rampe=2;
    }
    if(prise_rampe==1) {
        transformation_model_2.translation.y-=vitesse_montee;
        transformation_model_4.translation.y-=vitesse_montee;
        transformation_model_5.translation.y-=vitesse_montee;
        transformation_model_6.translation.y-=vitesse_montee;
        transformation_model_7.translation.y-=vitesse_montee;
        transformation_model_8.translation.y-=vitesse_montee;
        transformation_model_9.translation.y-=vitesse_montee;
        transformation_model_10.translation.y-=vitesse_montee;

        if(angle_x_model_1>-0.25) angle_x_model_1-=0.01;

    }
    if(transformation_model_7.translation.y<=-4.3f && prise_rampe==1) prise_rampe=2;
    if(prise_rampe==2 && backflip==0){
        angle_x_model_1+=0.01;
        if(angle_x_model_1>0) angle_x_model_1=0;
        transformation_model_2.translation.y+=vitesse_descente;
        transformation_model_4.translation.y+=vitesse_descente;
        transformation_model_5.translation.y+=vitesse_descente;
        transformation_model_6.translation.y+=vitesse_descente;
        transformation_model_7.translation.y+=vitesse_descente;
        transformation_model_8.translation.y+=vitesse_descente;
        transformation_model_9.translation.y+=vitesse_descente;
        transformation_model_10.translation.y+=vitesse_descente;

    }
    if(prise_rampe==2 && backflip==1){
        angle_x_model_1-=vitesse_backflip;
        transformation_model_2.translation.y+=vitesse_descente;
        transformation_model_4.translation.y+=vitesse_descente;
        transformation_model_5.translation.y+=vitesse_descente;
        transformation_model_6.translation.y+=vitesse_descente;
        transformation_model_7.translation.y+=vitesse_descente;
        transformation_model_8.translation.y+=vitesse_descente;
        transformation_model_9.translation.y+=vitesse_descente;
        transformation_model_10.translation.y+=vitesse_descente;
        score=score+20;

    }

    if(transformation_model_7.translation.y>=0 && prise_rampe==2){
        prise_rampe=0;
        transformation_model_2.translation.y=0.02;
        transformation_model_4.translation.y=0.02;
        transformation_model_5.translation.y=0.02;
        transformation_model_6.translation.y=0.02;
        transformation_model_7.translation.y=0.02;
        transformation_model_8.translation.y=0.02;
        transformation_model_9.translation.y=0.02;
        transformation_model_10.translation.y=0.02;

        angle_x_model_1=0;
        backflip=0;
    }
    transformation_model_1.rotation = matrice_rotation(angle_y_model_1 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_1 , 1.0f,0.0f,0.0f);


}
void collision(){
    //Sortie de la route
    if(transformation_model_2.translation.x<-5)
    {
        chute=1;
    }

    if(transformation_model_2.translation.x>5)
    {
        chute=1;
    }
    if(chute==1)
    {
        if(angle_x_model_1<0.5) angle_x_model_1+=0.08;

        transformation_model_1.translation.y-=0.03;
    }
    if(transformation_model_1.translation.y<-2)
    {
        start_game=0;
        you_loose=1;
    }
    transformation_model_1.rotation = matrice_rotation(angle_y_model_1 , 0.0f,1.0f,0.0f) * matrice_rotation(angle_x_model_1 , 1.0f,0.0f,0.0f);







    //Collision Mur
    if(transformation_model_4.translation.z>26.9f && transformation_model_4.translation.z<27.5)
    {
        if(transformation_model_2.translation.y>-3)
        {
            start_game=0;
            you_loose=1;
        }
    }

    //Collision Bot
    if(transformation_model_7.translation.z>46.2f && transformation_model_7.translation.z<47.5f)
    {
        if(transformation_model_7.translation.x>-3.85f && transformation_model_7.translation.x<-2.05f)
        {
            start_game=0;
            you_loose=1;

        }
    }

    //Prise Pièce1

    if(transformation_model_8.translation.z>66.6f && transformation_model_8.translation.z<67.4f)
    {
        if(transformation_model_8.translation.x>-1.3f && transformation_model_8.translation.x<-0.55f)
        {
            transformation_model_8.translation.z-=17;
            if(position_piece1==0) transformation_model_8.translation.x=2;
            if(position_piece1==1) transformation_model_8.translation.x=-2;
            if(position_piece1==0) position_piece1=1;
            else position_piece1=0;
            if(transformation_model_8.translation.x-transformation_model_2.translation.x>4) transformation_model_8.translation.x-=4.5;
            if(transformation_model_2.translation.x-transformation_model_8.translation.x>4) transformation_model_8.translation.x+=4.5;
            score=score+1000;
        }
    }
    //Prise Pièce2
    if(transformation_model_9.translation.z>61.6f && transformation_model_9.translation.z<62.4f)
    {
        if(transformation_model_9.translation.x<1.9f && transformation_model_9.translation.x>1.05f)
        {
            transformation_model_9.translation.z-=17;
            if(position_piece1==0) transformation_model_9.translation.x=2;
            if(position_piece1==1) transformation_model_9.translation.x=-2;
            if(position_piece1==0) position_piece1=1;
            else position_piece1=0;
            if(transformation_model_9.translation.x-transformation_model_2.translation.x>4) transformation_model_9.translation.x-=4.5;
            if(transformation_model_2.translation.x-transformation_model_9.translation.x>4) transformation_model_9.translation.x+=4.5;
        }
    }
    //Collision Bot2
    if(transformation_model_10.translation.z>86.0f && transformation_model_10.translation.z<87.5f)
    {
        if(transformation_model_10.translation.x>0.78f && transformation_model_10.translation.x<2.6f)
        {
            start_game=0;
            you_loose=1;
        }
    }
}
/*****************************************************************************\
 * timer_callback                                                            *
\*****************************************************************************/
static void timer_callback(int)
{
    //demande de rappel de cette fonction dans 25ms
    glutTimerFunc(25, timer_callback, 0);
    deplacement_decors();
    saut_rampe();
    collision();

    /*//programmation d'un saut
    if(etat==1) {
        transformation_model_1.translation.y += 0.1;
        transformation_view.translation.y-=0.1;
    }
    if(transformation_model_1.translation.y>=1.5 && etat==1) etat=2;
    if(etat==3) {
        transformation_model_1.translation.y += 0.1;
    }
    if(transformation_model_1.translation.y>=saut+2 && etat==3) {
        etat=2;
        saut=0;
    }
    if(etat==2){
        transformation_model_1.translation.y -= 0.13;
        transformation_view.translation.y+=0.13;
    }
    if(transformation_model_1.translation.y<=0 && etat==2){
        etat=0;
        transformation_model_1.translation.y=0;
        transformation_view.translation.y=0;
    }*/

    //reactualisation de l'affichage
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    //**********************************************//
    //Lancement des fonctions principales de GLUT
    //**********************************************//

    //initialisation
    glutInit(&argc, argv);

    //Mode d'affichage (couleur, gestion de profondeur, ...)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    //Taille de la fenetre a l'ouverture
    glutInitWindowSize(600, 600);

    //Titre de la fenetre
    glutCreateWindow("OpenGL");

    //Fonction de la boucle d'affichage
    glutDisplayFunc(display_callback);

    //Fonction de gestion du clavier
    glutKeyboardFunc(keyboard_callback);

    //Fonction des touches speciales du clavier (fleches directionnelles)
    glutSpecialFunc(special_callback);

    //Fonction d'appel d'affichage en chaine
    glutTimerFunc(25, timer_callback, 0);

    //Initialisation des fonctions OpenGL
    glewInit();

    //Notre fonction d'initialisation des donnees et chargement des shaders
    init();


    //Lancement de la boucle (infinie) d'affichage de la fenetre
    glutMainLoop();

    //Plus rien n'est execute apres cela

    return 0;
}


void draw_model_1()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_1.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_1.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_1.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_1);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_1, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}

void draw_model_2()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_2.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_2.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_2.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_2);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_2);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_2);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_2, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }
}

void draw_model_3()
{
    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_3.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_3.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_3.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_3);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_3);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_3);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_3, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }
}

void draw_model_4()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_4.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_4.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_4.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_4);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_4);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_4);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_4, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}

void draw_model_5()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_5.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_5.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_5.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_5);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_5);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_5);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_5, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }
}

void draw_model_7()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_7.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_7.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_7.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_7);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_7);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_7);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_7, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}

void draw_model_8()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_8.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_8.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_8.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_8);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_8);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_8);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_8, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}
void draw_model_9()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_9.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_9.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_9.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_9);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_9);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_9);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_9, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}
void draw_model_10()
{

    //envoie des parametres uniformes
    {
        glUniformMatrix4fv(get_uni_loc(shader_program_id,"rotation_model"),1,false,pointeur(transformation_model_10.rotation));    PRINT_OPENGL_ERROR();

        vec3 c = transformation_model_10.rotation_center;
        glUniform4f(get_uni_loc(shader_program_id,"rotation_center_model") , c.x,c.y,c.z , 0.0f);                                 PRINT_OPENGL_ERROR();

        vec3 t = transformation_model_10.translation;
        glUniform4f(get_uni_loc(shader_program_id,"translation_model") , t.x,t.y,t.z , 0.0f);                                     PRINT_OPENGL_ERROR();
    }

    //placement des VBO
    {
        //selection du VBO courant
        glBindBuffer(GL_ARRAY_BUFFER,vbo_object_10);                                                    PRINT_OPENGL_ERROR();

        // mise en place des differents pointeurs
        glEnableClientState(GL_VERTEX_ARRAY);                                                          PRINT_OPENGL_ERROR();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_opengl), 0);                                        PRINT_OPENGL_ERROR();

        glEnableClientState(GL_NORMAL_ARRAY); PRINT_OPENGL_ERROR();                                    PRINT_OPENGL_ERROR();
        glNormalPointer(GL_FLOAT, sizeof(vertex_opengl), buffer_offset(sizeof(vec3)));                 PRINT_OPENGL_ERROR();

        glEnableClientState(GL_COLOR_ARRAY); PRINT_OPENGL_ERROR();                                     PRINT_OPENGL_ERROR();
        glColorPointer(3,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(2*sizeof(vec3)));              PRINT_OPENGL_ERROR();

        glEnableClientState(GL_TEXTURE_COORD_ARRAY); PRINT_OPENGL_ERROR();                             PRINT_OPENGL_ERROR();
        glTexCoordPointer(2,GL_FLOAT, sizeof(vertex_opengl), buffer_offset(3*sizeof(vec3)));           PRINT_OPENGL_ERROR();

    }

    //affichage
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_10);                           PRINT_OPENGL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture_id_object_10);                             PRINT_OPENGL_ERROR();
        glDrawElements(GL_TRIANGLES, 3*nbr_triangle_object_10, GL_UNSIGNED_INT, 0);     PRINT_OPENGL_ERROR();
    }

}
void init_model_1()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/Moto.obj");

    // Affecte une transformation sur case 'n':

    //les sommets du maillage
    float s = 0.005f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
                          0.0f,    s, 0.0f,-0.9f,
                          0.0f, 0.0f,   s ,-2.5f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_1.rotation_center = vec3(0.0f,-0.5f,-2.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_1); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_1); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_1); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_1); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_1 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/stegosaurus.tga",&texture_id_object_1);

}

void init_model_2()
{
    //Creation manuelle du model 2

    //coordonnees geometriques des sommets
    vec3 p1=vec3(-5.0f,-0.9f,-200.0f);
    vec3 p2=vec3( 5.0f,-0.9f,-200.0f);
    vec3 p3=vec3( 5.0f,-0.9f, 0.0f);
    vec3 p0=vec3(-5.0f,-0.9f, 0.0f);

    //normales pour chaque sommet
    vec3 n0=vec3(0.0f,1.0f,0.0f);
    vec3 n1=n0;
    vec3 n2=n0;
    vec3 n3=n0;

    //couleur pour chaque sommet
    vec3 c0=vec3(1.0f,1.0f,1.0f);
    vec3 c1=c0;
    vec3 c2=c0;
    vec3 c3=c0;

    //texture du sommet
    vec2 t0=vec2(0.0f,0.0f);
    vec2 t1=vec2(1.0f,0.0f);
    vec2 t2=vec2(1.0f,1.0f);
    vec2 t3=vec2(0.0f,1.0f);

    vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
    vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
    vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
    vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);


    //tableau entrelacant coordonnees-normales
    vertex_opengl geometrie[]={v0,v1,v2,v3};


    //indice des triangles
    triangle_index tri0=triangle_index(0,1,2);
    triangle_index tri1=triangle_index(0,2,3);
    triangle_index index[]={tri0,tri1};
    nbr_triangle_object_2 = 2;

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_2);                                             PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_2);                                PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_2);                                            PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_2);                       PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

    // Chargement de la texture
    load_texture("../data/Log_Defuse.tga",&texture_id_object_2);

}


void init_model_3()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_off_file("../data/armadillo_light.off");

    // Affecte une transformation sur les sommets du maillage
    float s = 0.5f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 20.0f,
                          0.0f,    s, 0.0f, -20.0f,
                          0.0f, 0.0f,   s , -200.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    apply_deformation(&m,matrice_rotation(M_PI/2.0f,1.0f,0.0f,0.0f));
    apply_deformation(&m,matrice_rotation(1.5*M_PI/2.0f,0.0f,1.0f,0.0f));
    apply_deformation(&m,transform);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_3);                                 PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_3); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_3); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_3); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 3
    nbr_triangle_object_3 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/white.tga",&texture_id_object_3);


}
void init_model_4()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/Wall.obj");

    // Affecte une transformation sur case 'n':

    //les sommets du maillage
    float s = 0.125f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, -5.0f,
                          0.0f,    s, 0.0f,-0.9f,
                          0.0f, 0.0f,   s ,-30.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_4.rotation_center = vec3(-5.0f,-0.9f,-30.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_4); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_4); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_4); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_4); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_4 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/Log_Defuse.tga",&texture_id_object_4);



}
void init_model_5()
{
    //Creation manuelle du model 2

    //coordonnees geometriques des sommets
    vec3 p1=vec3( 2.5f, 2.7f,-30.0f);
    vec3 p2=vec3( 5.0f, 2.7f, -30.0f);
    vec3 p3=vec3( 5.0f,-0.9f, -20.0f);
    vec3 p0=vec3( 2.5f, -0.9f, -20.0f);


    //normales pour chaque sommet
    vec3 n0=vec3(0.0f,1.0f,0.0f);
    vec3 n1=n0;
    vec3 n2=n0;
    vec3 n3=n0;

    //couleur pour chaque sommet
    vec3 c0=vec3(1.0f,1.0f,1.0f);
    vec3 c1=c0;
    vec3 c2=c0;
    vec3 c3=c0;

    //texture du sommet
    vec2 t0=vec2(0.0f,0.0f);
    vec2 t1=vec2(1.0f,0.0f);
    vec2 t2=vec2(1.0f,1.0f);
    vec2 t3=vec2(0.0f,1.0f);

    vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
    vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
    vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
    vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);


    //tableau entrelacant coordonnees-normales
    vertex_opengl geometrie[]={v0,v1,v2,v3};


    //indice des triangles
    triangle_index tri0=triangle_index(0,1,2);
    triangle_index tri1=triangle_index(0,2,3);
    triangle_index index[]={tri0,tri1};
    nbr_triangle_object_5 = 2;

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_5);                                             PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_5);                                PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_5);                                            PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_5);                       PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);  PRINT_OPENGL_ERROR();

    // Chargement de la texture
    load_texture("../data/white.tga",&texture_id_object_5);

}

void init_model_7()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/LEGO_Man.obj");

    // Affecte une transformation sur case 'n':

    //les sommets du maillage
    float s = 0.8f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 2.9f,
                          0.0f,    s, 0.0f, -0.9f,
                          0.0f, 0.0f,   s , -50.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_7.rotation_center = vec3(2.9f,-0.9f,-50.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_7); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_7); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_7); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_7); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_7 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/white.tga",&texture_id_object_7);

}

void init_model_8()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/coin.obj");

    // Affecte une transformation sur case 'n':

    //les sommets du maillage
    float s = 0.3f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 1.0f,
                          0.0f,    s, 0.0f, -0.4f,
                          0.0f, 0.0f,   s , -70.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_8.rotation_center = vec3(1.0f,-0.4f,-70.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_8); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_8); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_8); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_8); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_8 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/coin-texture.tga",&texture_id_object_8);

}
void init_model_9()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/coin.obj");

    // Affecte une transformation sur case 'n':

    //les sommets du maillage
    float s = 0.3f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, -1.5f,
                          0.0f,    s, 0.0f, -0.4f,
                          0.0f, 0.0f,   s , -65.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_9.rotation_center = vec3(-1.5f,-0.4f,-65.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_9); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_9); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_9); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_9); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_9 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/coin-texture.tga",&texture_id_object_9);

}
void init_model_10()
{
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("../data/LEGO_Man.obj");

    // Affecte une transformation sur case 'n':

    //les sommets du maillage
    float s = 0.8f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, -1.5f,
                          0.0f,    s, 0.0f, -0.9f,
                          0.0f, 0.0f,   s , -90.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    apply_deformation(&m,transform);

    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    transformation_model_10.rotation_center = vec3(-1.5f,-0.9f,-90.0f);

    // Calcul automatique des normales du maillage
    update_normals(&m);
    // Les sommets sont affectes a une couleur blanche
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    //attribution d'un buffer de donnees (1 indique la création d'un buffer)
    glGenBuffers(1,&vbo_object_10); PRINT_OPENGL_ERROR();
    //affectation du buffer courant
    glBindBuffer(GL_ARRAY_BUFFER,vbo_object_10); PRINT_OPENGL_ERROR();
    //copie des donnees des sommets sur la carte graphique
    glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();


    //attribution d'un autre buffer de donnees
    glGenBuffers(1,&vboi_object_10); PRINT_OPENGL_ERROR();
    //affectation du buffer courant (buffer d'indice)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi_object_10); PRINT_OPENGL_ERROR();
    //copie des indices sur la carte graphique
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); PRINT_OPENGL_ERROR();

    // Nombre de triangles de l'objet 1
    nbr_triangle_object_10 = m.connectivity.size();

    // Chargement de la texture
    load_texture("../data/white.tga",&texture_id_object_10);

}
void load_texture(const char* filename,GLuint *texture_id)
{
    // Chargement d'une texture (seul les textures tga sont supportes)
    Image  *image = image_load_tga(filename);
    if (image) //verification que l'image est bien chargee
    {

        //Creation d'un identifiant pour la texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); PRINT_OPENGL_ERROR();
        glGenTextures(1, texture_id); PRINT_OPENGL_ERROR();

        //Selection de la texture courante a partir de son identifiant
        glBindTexture(GL_TEXTURE_2D, *texture_id); PRINT_OPENGL_ERROR();

        //Parametres de la texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); PRINT_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); PRINT_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); PRINT_OPENGL_ERROR();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); PRINT_OPENGL_ERROR();

        //Envoie de l'image en memoire video
        if(image->type==IMAGE_TYPE_RGB){ //image RGB
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data); PRINT_OPENGL_ERROR();}
        else if(image->type==IMAGE_TYPE_RGBA){ //image RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data); PRINT_OPENGL_ERROR();}
        else{
            std::cout<<"Image type not handled"<<std::endl;}

        delete image;
    }
    else
    {
        std::cerr<<"Erreur chargement de l'image, etes-vous dans le bon repertoire?"<<std::endl;
        abort();
    }

    glUniform1i (get_uni_loc(shader_program_id, "texture"), 0); PRINT_OPENGL_ERROR();
}
