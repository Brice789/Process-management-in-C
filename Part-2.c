{\rtf1\ansi\ansicpg1252\cocoartf2706
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 
\
#include <sys/types.h>
\
#include <unistd.h>
\
#include <string.h>
\
#include <stdlib.h>
\
#include <stdio.h>
\
#include <sys/wait.h>
\
#include <signal.h>
\
#include <stdbool.h>
\

\

\
pid_t pid_attendu = 0;
\
void traitement_signal_fils(int sig) \{
\
  pid_t res = 0;
\
  //le pere attends n'importe quel fils, ici on a un seul fils
\
  res = wait(NULL);
\
  if (res == pid_attendu) \{
\
printf("je suis le pere, mon fils vient de se terminier son PID etait %d\\nje meurt aussi.. \\n", res);
\
    pid_attendu = 0;
\
  \} else \{
\
    perror("error d'attente du fils !!");
\
  \}
\
\}
\
void signal_papa(int s) \{ // le num\'e9ro du signal n'est pas utilis\'e9.
\
  //le fils fini son execution
\
  printf("je suis le fils, j'ai recu le signal de mon pere, je meurt..\\n");
\
exit(0); \}
\
int main(void) \{
\
  // Mise en place du gestionnaire de signal pour SIGCHLD. j'utilise les drapeaux :
\
// SA_RESTART pour reprendre l'appel syst\'e8me sur lequel le processus \'e9tait bloqu\'e9 lors de
\
// la r\'e9ception du signal (ici read, dans getline), et SA_NOCLDSTOP pour ne prend en compte
\
  // SIGCHLD que lorsqu'un fils se termine, pas simplement lorsqu'il est stopp\'e9...
\
  struct sigaction sa;
\
  sigaction(SIGCHLD, NULL, & sa);
\
  sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
\
  sa.sa_handler = & traitement_signal_fils;
\
  int res = sigaction(SIGCHLD, & sa, NULL);
\
  if (res == -1) \{
\
    perror("error de gesion de SIGCHLD");
\
return 1; \}
\
  pid_t pere = getpid();
\
  pid_t fils = fork();
\
  //tant que ce pid_attendu est est egale a celui du fils , le pere attend
\
  pid_attendu = fils;
\
  if (fils == -1) \{
\
    printf("erreur de creation");
\
return 1; \}
\
//le pere
\
if (fils > 0) \{
\
printf("je suis le p\'e8re, mon fils a pour PID %d , mon PID %d \\n ", fils,
\
getpid());
\
    while (pid_attendu == fils) \{
\
      char lettre = 0;
\
      scanf("%c", & lettre);
\
while (lettre != 'p') \{
\
        printf("veillez saisir la lettre p  \\n \\t");
\
        scanf("%c", & lettre);
\
\}
\
      //envoi le signal a ton fils
\
      kill(fils, SIGALRM);
\
      //le pere attends la reception du signal de terminaison de son fils
\
pause(); \}
\
  \} //le fils
\
  else \{
\
printf("Je suis le fils, mon p\'e8re a pour PID %d et mon PID %d \\n", pere, getpid());
\
    // Mise en place du gestionnaire de signal pour SIGALRM.
\
    //le pere envoi ce signal a son fils, pour le tuer
\
    
\
    struct sigaction sb;
\
    sigaction(SIGALRM, NULL, & sb);
\
    sb.sa_flags = SA_RESTART | SA_NOCLDSTOP;
\
    sb.sa_handler = & signal_papa;
\
    int resf = sigaction(SIGALRM, & sb, NULL);
\
    if (resf == -1) \{
\
      perror("error l'ors de gesion de SIGALRM pour le fils");
\
    \}
\
    //le fils attends le signal du pere:
\
    while (1) pause();
\
  \}
\
  return EXIT_SUCCESS;
\
\}}



