#include <WiFi.h>

/*------------------Servidor Web en puerto 80---------------------*/

#define PORT 80
WiFiServer server(PORT);

/*---------------------Credenciales de WiFi-----------------------*/

const char* ssid     = "CESJT0";
const char* password = "itisjtsmg";

/*---------------------VARIABLES GLOBALES-------------------------*/
int contconexion = 0;

String header; // Variable para guardar el HTTP request

const int leds[4]={16,5,4,0,2};
for(int i=0;i<5;i++){
  digitalWrite(leds[i],LOW);
}

/*------------------------CODIGO HTML------------------------------*/
/* - ATENCION -
se tiene que poner cada linea entre comillas y reemplazar las comillas dentro por
comillas simples */

String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Animaciones</title>"
"</head>"
"<body>"
"<center>"
"<h1>Animacion LEDs mediante servidor Web</h1>"
"<h2>Presione en uno de los botones para animar los LEDs:</h2>"
"<p><a href='/animacion1'><button style='height:50px;width:100px'>ANIMACION2</button></a></p>"
"<p><a href='/animacion2'><button style='height:50px;width:100px'>ANIMACION1</button></a></p>"
"</center>"
"</body>"
"</html>";
"<style>"
"button{
    cursor: pointer; /*efecto al pasar el mouse por encima */
    }"
"body{
    font-family: Arial, sans-serif;
    text-align: center;
    margin-top: 50px;
    }"
"</style>"


/*---------------------------SETUP--------------------------------*/
void setup() {
  Serial.begin(115200);
  Serial.println("");
  
  pinMode(salida, OUTPUT); 
  digitalWrite(salida, LOW);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

/*----------------------------LOOP----------------------------------*/

void loop(){
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // enciende y apaga el GPIO
            if (header.indexOf("GET /animacion1") >= 0) {
              Serial.println("ANIMACION1");
              for(int i=0;i<5;i++){
                digitalWrite(leds[i], HIGH);
                digitalWrite(leds[i-1],LOW);
                delay(3000);
              }
            } else if (header.indexOf("GET /animacion2") >= 0) {
              Serial.println("ANIMACION2");
              for(int i=0;i<5;i++){
                digitalWrite(leds[i], HIGH);
                delay(3000);
                digitalWrite(leds[i],LOW);
            }
            
            // Muestra la página web
            client.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
