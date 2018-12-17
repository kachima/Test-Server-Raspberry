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
   char *database = "nhietdo";   //change database

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
      sprintf(cmd, "insert into docnhietdo (temperature) values (%d)", randonnumber); //change name column
      mysql_query(conn, cmd);
      sprintf(cmd, "insert into docdoam (humidity) values (%d)", randonnumber); //change name column
      mysql_query(conn, cmd);
      sprintf(cmd, "insert into docmucsang (lux) values (%d)", randonnumber); //change name column
      mysql_query(conn, cmd);
      sleep(10);
   }
   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
   return 0;
}

//create table templog(stt int NOT NULL AUTO_INCREMENT PRIMARY KEY,temperature float(5,2) not null);
//create table docnhietdo(stt int NOT NULL AUTO_INCREMENT PRIMARY KEY,date_time datetime NOT NULL DEFAULT CURRENT_TIMESTAMP, temperature FLOAT(5,2) NOT NULL);
//SELECT time(date_time),temperature FROM (SELECT * FROM docnhietdo ORDER BY STT DESC LIMIT 50) sub ORDER BY STT ASC