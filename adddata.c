#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;

   char *server = "localhost";
   char *user = "root";
   char *password = "123456"; //change password
   char *database = "temp";   //change database

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
                           user, password, database, 0, NULL, 0))
   {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }
   /* send SQL query */
   if (mysql_query(conn, "show tables"))
   {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);

   /* output table name */
   printf("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);

   /* Add new data */
   char cmd[200];
   while (1)
   {
      int randonnumber = rand() % 100;
      sprintf(cmd, "insert into temp_log (temperature) values (%d)", randonnumber); //change name column
      mysql_query(conn, cmd);
      sleep(10);
   }
   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
   return 0;
}
