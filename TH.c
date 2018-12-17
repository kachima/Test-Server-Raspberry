#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <math.h>
#include <mysql.h>

//Gas
#define		DOpin	0
#define		Buzz	1

//DHT11
#define MAXTIMINGS	85
#define DHTPIN		5

int dht11_dat[5] = { 0, 0, 0, 0, 0 };
float nhietdo =0;
float doam=0;
 
void Print(int x)
{
	switch(x)
	{
		case 1:
			printf("\n*********\n"  );
			printf(  "* Safe~ *\n"  );
			printf(  "*********\n\n");
		break;
		case 0:
			printf("\n***************\n"  );
			printf(  "* Danger Gas! *\n"  );
			printf(  "***************\n\n");
		break;
		default:
			printf("\n**********************\n"  );
			printf(  "* Print value error. *\n"  );
			printf(  "**********************\n\n");
		break;
	}
} 

void read_dht11_dat()
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 18 );
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );
	pinMode( DHTPIN, INPUT );
 
	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( DHTPIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHTPIN );
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) )
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 50 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
		nhietdo = dht11_dat[2] + dht11_dat[3]/10.0;
		doam = dht11_dat[0] + dht11_dat[1]/10.0;
	}
	
}

 
int main( void )
{
	int handle = wiringPiI2CSetup(0x23) ;
	
		
	int tmp, status;
	
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}
	

	pinMode (DOpin,	INPUT);
	pinMode (Buzz,	OUTPUT);
	

	status = 0;
 
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *server = "localhost";
	char *user = "root";
	char *password = "123456"; //change password
	char *database = "homesystem";   //change database

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
	while ( 1 )
	{
		read_dht11_dat();
		sprintf(cmd, "insert into docnhietdo (temperature) values (%.2f);\n",nhietdo ); //change name column
		mysql_query(conn, cmd);
		printf(cmd);
		sprintf(cmd, "insert into docdoam (humidity) values (%.2f);\n",doam ); //change name column
		mysql_query(conn, cmd);
		printf(cmd);
	
		//sleep(10);
		//delay( 600 ); 
		
		wiringPiI2CWrite(handle,0x10);
		sleep(1);
		int word=wiringPiI2CReadReg16(handle,0x00);
		int lux=((word & 0xff00)>>8) | ((word & 0x00ff)<<8);
		printf("Current in Lux:%d \n",lux);
		//delay(400);
		sprintf(cmd, "insert into docmucsang (lux) values (%d);\n",lux );
		mysql_query(conn, cmd);
		printf(cmd);
		tmp = digitalRead(DOpin);
		
		//printf("%d\n", tmp);
		Print(tmp);
		
		if (tmp != status)
		{
			Print(tmp);
			status = tmp;
		}
		
		if (status == 0)
		{
			
			{digitalWrite(Buzz, HIGH);}
		}
		else
		{
			
			digitalWrite(Buzz, LOW);
		}
		
      
		//delay (1000);
	}
	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
	return(0);
}
