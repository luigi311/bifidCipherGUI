#ifndef __BIFIDGUI_H
#define __BIFIDGUI_H
#include <gtk/gtk.h>

/* Structure that holds the information for the GUI */
struct gui
{
  GtkWidget *window;
  GtkWidget *windowButton;
  GtkWidget *grid;
  GtkWidget *gridButton;
  GtkWidget *encrypted;
  GtkWidget *unecrypted;
  GtkWidget *textBox;
  gchar *strUnc;
  gchar *strEnc;
};

/* Structure to hold information on the encryption */
struct information
{
  int sizeOfArray;
  int *decrpytedCords;
  int *encrpytedCords;
  int *scani;
  int *scanj;
  int count;
  char *readArray;
  char *encrypted;
  char *decrypted;
};

struct information information;
struct gui gui;

int readfile(char *file2read);
void find_value(char value[information.sizeOfArray]);
void encrypt(int *encryptedNum, int size);
void decrypt(int *encryptedNum, int size);
void activate (GtkApplication *app, gpointer user_data);
void guiUpdate();
void work();
void text();
void file();
void clean();

#endif 
