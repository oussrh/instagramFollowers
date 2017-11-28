#include <LiquidCrystal.h> 
#include <SPI.h>
#include <Ethernet.h>

boolean startRead = false;
char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
String pageValue;
String connexion;

// Enter une MAC address pour l'Arduino.
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

char serverName[] = "www.htag.be";

//lien vers le fichier tu traitement PHP
String location = "/~htag/insta/folowers.php HTTP/1.0";

// Initialiser le client Ethernet
EthernetClient client;



LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
 
     lcd.begin(16, 2);
     lcd.setCursor(0,0);
     lcd.print("My Instagram");
     
     Ethernet.begin(mac);
    
     delay(1000);

     lcd.setCursor(0, 1);//mettre le curseur sur la deuxieme ligne

    
     Serial.begin(9600); 
     
     lcd.print("connecting...");
     

}

void loop()
{

 
  String connexion = cnx();
  String pageValue = readPage(); 
  
  Serial.println(pageValue);
  
  lcd.setCursor(0, 1);
  lcd.print("Folowers : ");
  lcd.print(pageValue);
  
  delay(5000); //5 seconde de delai avant une autre connexion
}


String cnx(){

 
     // if you get a connection, report back via serial:
     
     if (client.connect(serverName, 80)) {
       Serial.println("connected");
       // Make a HTTP request:
       client.println("GET http://htag.be/insta/folowers.php HTTP/1.0");
       client.println();
     } 
     else {
       // if you didn't get a connection to the server:
       Serial.println("connection failed");
     }
 
}


String readPage(){
  
      //read the page, and capture & return everything between '<' and '>'
      stringPos = 0;
      memset( &inString, 0, 32 ); //clear inString memory
    
      while(true){
    
        if (client.available()) {
          char c = client.read();
    
          if (c == '<' ) { //'<' is our begining character
            startRead = true; //Ready to start reading the part 
          }else if(startRead){
    
            if(c != '>'){ //'>' is our ending character
              inString[stringPos] = c;
              stringPos ++;
            }
          else{
              //got what we need here! We can disconnect now
              startRead = false;
              client.stop();
              client.flush();
              return inString; 
              
            }
          
          }
          
        }
    
      }

}

