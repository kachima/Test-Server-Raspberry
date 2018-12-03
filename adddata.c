#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;

   char *server = "localhost";
   char *user = "root";
   char *password = "123456"; /* set me first */
   char *database = "temp";
   
   

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }
   /* send SQL query */
   if (mysql_query(conn, "show tables")) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);

   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);

   /* Add new data */
   for(int i=1; i<100; i++)
   {
      char cmd[200];
      int randonnumber = rand() % 100;

      sprintf(cmd,"insert into log (idx,value) values (%d,%d)",i,randonnumber);
      //printf(cmd);
      mysql_query(conn, cmd);
   }  

   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
   return 0;
}

