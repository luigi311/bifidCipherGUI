#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bifidGUI.h"

/* Global Cipher to use in all functions */
const char g_cipher[9][9] = {{'a','j','s','B','K','T','3','@','_'},{'b','k','t','C','L','U','4','#','+'},{'c','l','u','v','w','x','y','z','A'},{'d','m','D','E','F','G','H','I','J'},{'e','n','M','N','O','P','Q','R','S'},{'f','o','V','W','X','Y','Z','1','2'},{'g','p','5','6','7','8','9','0','!'},{'h','q','$','%','^','&','*','(',')'},{'i','r','|','-','=',',','.',' ','?'}};

/* Main that runs the application linking to activate in turn linking to all other functions
   when needed */
int main (int argc, char *argv[]) {
  GtkApplication *app;

  app = gtk_application_new("com.github.luigi311.bifidCipher", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_application_run(G_APPLICATION(app), 0, 0);
  g_object_unref(app);

  return 0; 
}

/* Creates the GUI holding all the information */
void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *textButton;
  GtkWidget *fileButton;
  GtkWidget *quit;
  GtkWidget *labelUnc;
  GtkWidget *labelEnc;

  /* Create Window to hold the grid and widgets */
  gui.window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(gui.window), "Bifid"); //Window Title
  gtk_window_set_default_size(GTK_WINDOW (gui.window), 300, 100); //Window Size

  /* Create Grid to hold all widgets */
  gui.grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(gui.window), gui.grid);
  gtk_grid_set_row_homogeneous(GTK_GRID(gui.grid),FALSE);
  gtk_grid_set_column_homogeneous(GTK_GRID(gui.grid),FALSE);

  /* Creates Text Widgets for unencrypted and encrypted */
  labelUnc = gtk_label_new("Unencrypted text: "); 
  gtk_grid_attach(GTK_GRID(gui.grid), GTK_WIDGET(labelUnc), 0, 0, 1, 1);
  gui.unecrypted = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(gui.grid), GTK_WIDGET(gui.unecrypted), 2, 0, 1, 1);
  labelEnc = gtk_label_new("Encrypted text: ");
  gtk_grid_attach(GTK_GRID(gui.grid), GTK_WIDGET(labelEnc), 0, 1, 1, 1);
  gui.encrypted = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(gui.grid), GTK_WIDGET(gui.encrypted), 2, 1, 1, 1);

  /* Text Box */
  gui.textBox = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(gui.grid), gui.textBox, 0, 2, 1, 1);

  /* Creates button titled text and launches text function to encrypt text from entry */
  textButton = gtk_button_new_with_label("Encrypt Text"); 
  g_signal_connect(textButton, "clicked", G_CALLBACK(text), NULL); 
  gtk_grid_attach(GTK_GRID(gui.grid), textButton, 0, 3, 1, 1); 

  /* Creates a button titled file that launches the file function to ecrypt text from file */
  fileButton = gtk_button_new_with_label("Encrypt File");
  g_signal_connect(fileButton, "clicked", G_CALLBACK(file), NULL);
  gtk_grid_attach_next_to(GTK_GRID(gui.grid), fileButton, textButton, GTK_POS_RIGHT, 1, 1);

  /* Creates buttone titled quit that exits the GUI and program */
  quit = gtk_button_new_with_label ("Quit"); 
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_widget_destroy), gui.window); 
  gtk_grid_attach(GTK_GRID(gui.grid), quit, 0, 4, 1, 1);

  /* Makes window visible causing it to show everything attached */
  gtk_widget_show_all(gui.window);
}

/* Encrypts text from text box */
void text() {
  /* Grabs text from the text box and puts it in the inforamtion struct under readarray along 
     with size in sizeofarray */
  information.sizeOfArray = gtk_entry_get_text_length(GTK_ENTRY(gui.textBox));
  information.readArray = malloc(sizeof(char)*information.sizeOfArray);
  information.readArray = (char*)gtk_entry_get_text(GTK_ENTRY(gui.textBox));
  
  /* Encrypts everything */
  work();
}

/* Encrypts the file entered in the text box */
void file() {
  int test = 0;
  char *filetoread = NULL;
  
  /* Grab file name from text box */
  filetoread = (char *)gtk_entry_get_text(GTK_ENTRY(gui.textBox));
  test = readfile(filetoread);
  
  /* Checks if file actually exists if not return file not found if it does encrypts the contents */
  if (test == -1) {
    gui.strUnc = g_strdup_printf("File \"%s\" does not exist",filetoread);
    gtk_label_set_text(GTK_LABEL(gui.unecrypted),gui.strUnc);
    gtk_label_set_text(GTK_LABEL(gui.encrypted),NULL);
  } else {
    information.readArray[information.sizeOfArray] = 0; 
    work();
  }
}

/* Updates the strings in the GUI to reflect the encrypted values */
void guiUpdate() {
  /* Converts the variable text to a predefined char to allow attaching to the gui */ 
  gui.strUnc = g_strdup_printf("%s",information.readArray);
  gtk_label_set_text(GTK_LABEL(gui.unecrypted),gui.strUnc); 

  gui.strEnc = g_strdup_printf("%s",information.encrypted);
  gtk_label_set_text(GTK_LABEL(gui.encrypted),gui.strEnc); 
  
  /* Frees all pointers */
  //clean();
}

/* Input the cordinates and it returns the character that matches it on the grid */
void encrypt(int *encryptedNum, int size) {
  int i = 0;
  int j = 0;
  int k = 0;
  int number = 0;
  
  /* Returns the encrypted letter for the given cordinates */
  for (i=0;i<size;i=i+2) {
    j = encryptedNum[i]; 
    k = encryptedNum[i+1];
    information.encrypted[number] = g_cipher[j][k]; 
    number++;
  }
  information.encrypted[information.count] = 0;
}

/* Decrypts the encrypted text output results to information struct under decrpytedCords and 
   decrypted */
void decrypt(int *encryptedNum, int size) {
  int i = 0;
  int j = 0;
  int k = 0;
  int number = 0;
  int value = 0;
  
  /* Decrypts the encrypted cordinates and returns the unencrypted text */
  for (i=0;i<information.count;i++) {
    j = encryptedNum[i]; 
    k = encryptedNum[i+information.count];
    information.decrpytedCords[value] = j;
    information.decrpytedCords[value+1] = k;
    information.decrypted[number] = g_cipher[j][k];  
    number++;
    value = value+2;
  }
  information.decrypted[information.count] = 0;
}
 
/* Scan cipher for matching values and returning the rows and columns of the given value */
void find_value(char value[information.sizeOfArray]) {
  int i = 0;
  int j = 0;
  int k = 0;
  information.count = 0;

  /* Checks for matching values and returns their respected cordinates */
  for (k=0;k<information.sizeOfArray;k++) {
    for (i=0; i<9; i++) {
      for (j=0;j<9; j++) {
        if (value[k] == g_cipher[i][j]) {
          information.scani[information.count] = i;  
          information.scanj[information.count] = j;  
          information.count++;
        }
      }
    }
  }
}

/* Read file and pass information to the struct for global usage */
int readfile(char *file2read) {
  FILE *fp;
  int r = 0;
  int count = 0;
  long size = 0;

  fp = fopen(file2read, "r");	
  if (fp != NULL) {
    fseek(fp,0,SEEK_END); 
    size = ftell(fp); //returns the number of characters in said file with a extra character
    rewind(fp); //resets the file to the beginning to begin reading
    information.sizeOfArray = size-1;  //passes size to the structure, the -1 is to remove the extra blank character that appears at the end
    information.readArray = malloc(sizeof(char)*(size));
    /* Aslog as file isnt empty */
    if (size != 0) {
      /* Reads till it reaches the end of said file */
      while ((r = getc(fp)) != EOF) {
        /* Passes the context of said file into a pointer array in struct */
        information.readArray[count] = r;
	count++;
      } 
    } else {
      printf("File \"%s\" is empty\n",file2read);
      return -1;
    }	
  } else {
    printf("File \"%s\" does not exist\n",file2read);
    return -1;
  }
  fclose(fp);
  return(0);
}

/* Goes through the encrypting and decrypting functions and allocating memory for all pointers */
void work() {
  int i = 0;
  int twiceCount = 0;
  int doubleArray = information.sizeOfArray*2;
  
  /* Memory allocation for all pointers */
  information.decrypted = malloc(sizeof(int)*information.sizeOfArray);
  information.encrypted = malloc(sizeof(char)*information.sizeOfArray);
  information.decrpytedCords = malloc(sizeof(int)*doubleArray);
  information.scani = malloc(sizeof(int)*information.sizeOfArray);
  information.scanj = malloc(sizeof(int)*information.sizeOfArray);

  /* Finds all the cordinates for the input and returns the cordinates to the information structure 
     under scani and scanj */
  find_value(information.readArray); 

  twiceCount = information.count * 2;
  information.encrpytedCords = malloc(sizeof(int)*twiceCount);

  /* Encrypts the cordinates by getting the cordinates from the information struct under the 
     variables scani and scanj */
  for (i=0;i<information.count;i++) {
    information.encrpytedCords[i] = information.scani[i]; 
    information.encrpytedCords[i+information.count] = information.scanj[i];  
  }

  /* Goes from the combined cordinates to encrypted letters and puts it into the information
     structure under encrypted */
  encrypt(information.encrpytedCords, doubleArray); 
  

  /* Sends encrypted cordinates to decyper the cordinates and calculing the decrypted text 
     putting it into the information structure under decrypted and the cordinates into
     decryptedCords */
  decrypt(information.encrpytedCords, twiceCount);
  
  #if DONT_NEED_PRINT_TO_CONSOLE
  /* Prints original text */
  printf("Original Text:%s",information.readArray);
  
  /* Prints the encrypted cordinates with the combined columns */
  printf("\nEncrpyted Cordinates:");
  for (i=0;i<twiceCount;i++)
    printf("%d",information.encrpytedCords[i]); 

  /* Prints the encrypted text */
  printf("\nEncrypted Text:%s\n",information.encrypted); 

  /* Prints the decrypted cordinates */
  printf("Decrpyted Cordinates:");
  for (i=0;i<twiceCount;i++)
    printf("%d",information.decrpytedCords[i]); 
  
  /* Prints the decrypted text */
  printf("\nDecrypted Text:%s\n",information.decrypted); 
  #endif

  /* Updates the information on the GUI */
  guiUpdate();
}

/* Frees all the pointers after updating the GUI */
void clean() {
  free(information.scanj);
  free(information.scani);
  free(information.encrypted);
  free(information.decrypted);
  free(information.decrpytedCords);
  free(information.encrpytedCords);
}
