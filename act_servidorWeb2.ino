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

int selectDisplay1=1;
int selectDisplay2=3;

const int leds[7]={16,5,4,0,2,14,12};

int nums[10][7]{
  { 1, 1, 1, 1, 1, 1, 0 },  //0
  { 0, 1, 1, 0, 0, 0, 0 },  //1
  { 1, 1, 0, 1, 1, 0, 1 },  //2
  { 1, 1, 1, 1, 0, 0, 1 },  //3
  { 0, 1, 1, 0, 0, 1, 1 },  //4
  { 1, 0, 1, 1, 0, 1, 1 },  //5
  { 1, 0, 1, 1, 1, 1, 1 },  //6
  { 1, 1, 1, 0, 0, 0, 0 },  //7
  { 1, 1, 1, 1, 1, 1, 1 },  //8
  { 1, 1, 1, 1, 0, 1, 1 },  //9
}

/*------------------------CODIGO HTML------------------------------*/
/* - ATENCION -
se tiene que poner cada linea entre comillas y reemplazar las comillas dentro por
comillas simples */

String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Displays</title>"
"</head>"
"<body>"
"<center>"
"<h1>Displays 7 segmentos</h1>"
"<h2>Elija uno de los displays:</h2>"

"<p><a href='/display1'><button style='height:50px;width:100px'>Display 1</button></a></p>"
"<p><a href='/display2'><button style='height:50px;width:100px'>Display 2</button></a></p>"

"<h2>Ahora elija un número a mostrar:</h2>"

"<p><a href='/num1'><button style='height:40px;width:40px'>1 </button></a></p>"
"<p><a href='/num2'><button style='height:40px;width:40px'>2 </button></a></p>"
"<p><a href='/num3'><button style='height:40px;width:40px'>3 </button></a></p>"

"<h1></h1>"

"<p><a href='/num4'><button style='height:40px;width:40px'>4 </button></a></p>"
"<p><a href='/num5'><button style='height:40px;width:40px'>5 </button></a></p>"
"<p><a href='/num6'><button style='height:40px;width:40px'>6 </button></a></p>"

"<h1></h1>"

"<p><a href='/num7'><button style='height:40px;width:40px'>7 </button></a></p>"
"<p><a href='/num8'><button style='height:40px;width:40px'>8 </button></a></p>"
"<p><a href='/num9'><button style='height:40px;width:40px'>9 </button></a></p>"

"<h1></h1>"

"<p><a href='/num0'><button style='height:40px;width:40px'>0 </button></a></p>"
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
  for(int i=0;i<7;i++){
    pinMode(leds[i], OUTPUT);
  }
  for(int i=0;i<7;i++){
  digitalWrite(leds[i],LOW);
  }
  digitalWrite(selectDisplay1, LOW);
  digitalWrite(selectDisplay2, LOW);



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


            if (header.indexOf("GET /display1") >= 0) { //Tambien se iguala a 0 porque el valor que contiene sin haber sido pulsado es -1,
              digitalWrite(selectDisplay1, HIGH); //Prende el anodo de uno de los
              digitalWrite(selectDisplay2, LOW); // displays mientras que apaga el otro
              client.println(display1);
              }
            } else if (header.indexOf("GET /display2") >= 0) {
              digitalWrite(selectDisplay1, HIGH);
              digitalWrite(selectDisplay2, LOW);
              client.println(display2);
            }
            

            if (header.indexOf("GET /num1") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[1][i]);
                    if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num1);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num1)
                    }
                }
            }else if (header.indexOf("GET /num2") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[2][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num2);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num2)
                    }
            }else if (header.indexOf("GET /num3") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[3][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num3);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num3)
                    }
            }else if (header.indexOf("GET /num4") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[4][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num4);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num4)
                    }
            }else if (header.indexOf("GET /num5") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[5][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num5);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num5)
                    }
            }else if (header.indexOf("GET /num6") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[6][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num6);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num6)
                    }
            }else if (header.indexOf("GET /num7") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[7][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num7);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num7)
                    }
            }else if (header.indexOf("GET /num8") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[8][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num8);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num8)
                    }
            }else if (header.indexOf("GET /num9") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[9][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num9);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num9)
                    }
            }else if (header.indexOf("GET /num0") >= 0){
                for(int i=0;i<7;i++){
                    digitalWrite(leds[i], nums[0][i]);
                }
                if (header.indexOf("GET /display1") >= 0) {
                        client.println(display1_num0);
                    }else if (header.indexOf("GET /display2") >= 0) {
                        client.println(display2_num0)
                    }
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
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
        }
    