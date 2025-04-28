   // Smarter with a GUI

#include <stdbool.h>
#include <stdio.h>
#include "./putils/colors.h"
#define FILENAME "p2c_save.txt"

char** load_from_file(char** to_be_used){
   FILE* file = fopen(FILENAME, "r");
   if(file == NULL){
      printf("%sSave not found!%s", COLOR_RED, COLOR_RESET);
      return to_be_used;
   }
   char buffer[50] = {0};
   for (int i = 0; i < 8; i++) {
      fgets(buffer, 50, file);
      for (int n = 0; n < 8; n++) {
         to_be_used[i][n] = buffer[n];
      }
   }
   fclose(file);
   return to_be_used;
}

short menu(){
   short int option = -1;
   int temp = 0;
   while (option < 0) {
      printf("\tStart new Game\t(%s1%s)\n", COLOR_YELLOW, COLOR_RESET);
      printf("\tResume old Game\t(%s2%s)\n", COLOR_YELLOW, COLOR_RESET);
      printf("\tleave\t\t(%s0%s)\n", COLOR_YELLOW, COLOR_RESET);
      printf("What do you want to do?\t%s ", COLOR_CYAN);
      scanf("%d", &temp);
      printf("%s", COLOR_RESET);
      switch (temp) {
         case 0:
            option = 0;
            break;
         case 1:
            option = 1;
            break;
         case 2:
            option = 2;
            break;
         default:
            printf("invalid\n");
            break;
      }
      printf("\n");
   }
   return option;
}

int main(int argc, char** argv){
   bool game = true;

   printf(
      "%s       ______     __  __     ______     ______     ______        ______     ______     __    __     ______\n"
      "      /\\  ___\\   /\\ \\_\\ \\   /\\  ___\\   /\\  ___\\   /\\  ___\\      /\\  ___\\   /\\  __ \\   /\\ '-./  \\   /\\  ___\\   \n"
      "      \\ \\ \\____  \\ \\  __ \\  \\ \\  __\\   \\ \\___  \\  \\ \\___  \\     \\ \\ \\__ \\  \\ \\  __ \\  \\ \\ \\-./\\ \\  \\ \\  __\\   \n"
      "       \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\/\\_____\\  \\/\\_____\\     \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\ \\ \\_\\  \\ \\_____\\ \n"
      "        \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/   \\/_____/      \\/_____/   \\/_/\\/_/   \\/_/  \\/_/   \\/_____/ %s\n\n",
      COLOR_YELLOW, COLOR_RESET);

   while (game == true) {
      switch (menu()) {
         case 0:
            printf("Exiting...\n");

            printf("%sB O B O B O B O P O P O P%s\n\n", COLOR_BLUE, COLOR_RESET);
            printf("%sB O B O B B B O P O P P P%s\n\n", COLOR_CYAN, COLOR_RESET);
            printf("%sB O B O B O B O P O P O P%s\n\n", COLOR_BLUE, COLOR_RESET);
            printf("%sB O B O B B B O P O P P P%s\n\n", COLOR_CYAN, COLOR_RESET);
            printf("%sB O B O B O B O P O P O P%s\n\n", COLOR_BLUE, COLOR_RESET);
            printf("%sB O B O B B B O P O P P P%s\n", COLOR_CYAN, COLOR_RESET);
            game = false;
            break;
         case 1:

            break;
         case 2:

            break;
      }
   }

   return 0;
}
