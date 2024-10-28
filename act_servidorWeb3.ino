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


const int semaforo[3]={16,5,4};
for(int i=0;i<3;i++){
    pinMode(semaforo[i], OUTPUT);
    digitalWrite(semaforo[i],LOW);
}

//LENTO
const int trojo1 = 70000;
const int trojo2 = 45000;
const int trojo3 = 20000;

//NORMAL
const int tamarillo1 = 3000;
const int tamarillo2 = 2000;
const int tamarillo3 = 1000;

//RAPIDO
const int tverde1 = 50000;
const int tverde2 = 65000;
const int tverde3 = 40000;

//EMERGENCIA
const int temergencia = 1500;


string modo;





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
            

            // cambia de modo
            if (header.indexOf("GET /lento") >= 0) {
                Serial.println("LENTO");
                modo = LENTO;
                digitalWrite(semaforo[0], HIGH);
                delay(trojo1);
                digitalWrite(semaforo[0], LOW);
                delay(50);
                digitalWrite(semaforo[1], HIGH);
                delay(tamarillo1);
                digitalWrite(semaforo[1], LOW);
                delay(50);
                digitalWrite(semaforo[2], HIGH);
                delay(tverde1);
                digitalWrite(semaforo[2],LOW);
                delay(50);
            }

            } else if (header.indexOf("GET /normal") >= 0) {
              Serial.println("NORMAL");
              modo = NORMAL;
                digitalWrite(semaforo[0], HIGH);
                delay(trojo2);
                digitalWrite(semaforo[0], LOW);
                delay(50);
                digitalWrite(semaforo[1], HIGH);
                delay(tamarillo2);
                digitalWrite(semaforo[1], LOW);
                delay(50);
                digitalWrite(semaforo[2], HIGH);
                delay(tverde3);
                digitalWrite(semaforo[2],LOW);
                delay(50);
            }

            } else if (header.indexOf("GET /rapido") >= 0) {
              Serial.println("RAPIDO");
              modo = RAPIDO;
                digitalWrite(semaforo[0], HIGH);
                delay(trojo3);
                digitalWrite(semaforo[0], LOW);
                delay(50);
                digitalWrite(semaforo[1], HIGH);
                delay(tamarillo3);
                digitalWrite(semaforo[1], LOW);
                delay(50);
                digitalWrite(semaforo[2], HIGH);
                delay(tverde4);
                digitalWrite(semaforo[2],LOW);
                delay(50);
            }

            } else if (header.indexOf("GET /emergencia") >= 0) {
              Serial.println("EMERGENCIA");
              modo = EMERGENCIA;
                digitalWrite(semaforo[0], HIGH);
                digitalWrite(semaforo[1], HIGH);
                delay(temergencia);
                digitalWrite(semaforo[0], LOW);
                digitalWrite(semaforo[1], LOW);
                delay(temergencia);         
            }
            

/*------------------------CODIGO HTML------------------------------*/
/* - ATENCION -
se tiene que poner cada linea entre comillas y reemplazar las comillas dentro por
comillas simples */

String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Semaforo</title>"
"</head>"
"<body>"
"<center>"

"<div class ='titulo'>"
"<h1>SEMAFORO</h1>"
"</div">
"<h2>Elija el modo que quiera simular:</h2>"



"<div class='lento'>"
    "<p><a href='/lento'><button style='height:200px;width:200px'>NORMAL</button></a></p>"
"</div>"
"<div class ='normal'>"
    "<p><a href='/normal'><button style='height:200px;width:200px'>NORMAL</button></a></p>"
"</div>"
    "<p><a href='/rapido'><button style='height:200px;width:200px'>RAPIDO</button></a></p>"
"<div class ='emergencia'>"
    "<p><a href='/emergencia'><button style='height:200px;width:200px'>EMERGENCIA</button></a></p>"
"</div>"

"<span> Modo:" +modo+ " </span>"




"</center>"
"</body>"
"</html>";


"<style>"
"button{
    cursor: pointer;
    }"
"body{
    font-family: Arial, sans-serif;
    text-align: center;
    margin-top: 50px;
    background-color: white;
    color: white;
    }"
".titulo{
    font-family: 'Impact';
    font-weight:bold;
    text-decoration: underline; /*subrayado*/
    text-decoration-thickness: 45px; /*grosor del subrayado*/

}"
".lento{
    width: 200px;
    height: 200px;
    background-color: red;
    font-size: 23px;
    border: 3px; /*borde negro*/
    border-radius: 100px; /*solido de radio*/
}"
".normal{
    width: 200px;
    height: 200px;
    background-color: yellow;
    border: 3px; /*borde negro*/
    border-radius: 100px; /*solido de radio*/
}"
".rapido{
    width: 200px;
    height: 200px;
    background-color: green;
    border: 3px; /*borde negro*/
    border-radius: 100px; /*solido de radio*/
}"
".emergencia{
    width: 200px;
    height: 200px;
    background-color: orange;
    border: 3px; /*borde negro*/
    border-radius: 100px; /*solido de radio*/
}"

"</style>"



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

