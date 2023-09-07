#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <string.h>
#include <WebServer.h>

#include "webpageCode.h"//importamos la pagina a mostrar
//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

const char* ssid     = "INFINITUM33A2_2.4";
const char* password = "DN9yAq8quM";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;
String header; // Variable para guardar el HTTP request
String estadoSalida = "off";


unsigned long previousMillisLlenado = 0;
//variables de pines del esp32
const int MotorIzquierda = 2;
const int MotorDerecha = 15;
const int Flotador = 0;
const int BombLLenado = 4;
const int BombBaciado = 16;
const int BombAgua = 17;

//variables controlan el tiempo de cada funcion de la lavadora 
int TimeLLenado = 110;
int TimeLavado = 550; 
int TimeVaciado = 250;
int TimeEnjuague = 350;
int TimeCentrifugado = 800;
int TimeCentrifugadoEnjuague =600;
int TimeRemojo=200;
int TimeExtraCen=900;

int contLLenado = 1;
int contLavado = 1;
int contVaciado = 1;
int contEnjuague =1;
int contCentrifugado = 1;
int contCentrifugadoEnjuague =1;
int contRemojo=1;
int contExtraCen=1;

//banderas de cada funcion para hacer dinamico el sistema
bool Flaglavar=false;
bool Flagenjuagar=false;
bool Flagcentrifugar=false;
bool Flagllenado=false;
bool Flagremojar=false;

//banderas para el modo manual
bool llenar = false;
bool remojar = false;
bool lavar = false;
bool enjuagar = false;
bool exprimir = false;
bool centrifugado = false;

bool bomba=true;

enum State {
  IDLE,
  PAUSED,
  AUTOMATIC,
  MANUAL
};

State currentState = IDLE;
State previousState = IDLE;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;
bool pausa=false;

//--------------------------SETUP--------------------------------


void setup() {
  Serial.begin(115200);
  Serial.println("");
  
  pinMode(MotorDerecha, OUTPUT);
  pinMode(MotorIzquierda,OUTPUT); 
  pinMode(Flotador,OUTPUT); 
  pinMode(BombLLenado,OUTPUT); 
  pinMode(BombBaciado,OUTPUT); 
  pinMode(BombAgua,OUTPUT); 
  digitalWrite(MotorDerecha , HIGH);
  digitalWrite(MotorIzquierda, HIGH);
  digitalWrite(BombLLenado, HIGH);
  digitalWrite(BombBaciado, HIGH);
  digitalWrite(Flotador, HIGH);
  digitalWrite(BombAgua, HIGH);
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

//----------------------------LOOP----------------------------------

const long llenadoInterval = 10000;// esquivale a 10 segundos
void llenado(){
  unsigned long currentMillis = millis();
    bomba=false;
    if (currentMillis - previousMillisLlenado >= llenadoInterval) {
      previousMillisLlenado = currentMillis;
      Serial.print("LLENANDO.... ");
      Serial.println(contLLenado);
      digitalWrite(BombLLenado, LOW);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(Flotador, LOW);
      delay(2000);
      contLLenado++;
   
    }
}
void vaciado(){
  
      Serial.print("VACIANDO.... ");
      Serial.println(contVaciado);
      digitalWrite(BombBaciado, LOW);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, HIGH);
      delay(1000);
      contVaciado++;
      if(contVaciado>TimeVaciado){
        previousMillisLlenado=0;
        contLLenado = 1; 
        bomba=true; 
            
      }
}      
void Lavado() {
   if(contLavado<=TimeLavado) {    
      Serial.print("LAVANDO....");     
      Serial.println(contLavado);
      digitalWrite(Flotador, LOW);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);                
      digitalWrite(MotorIzquierda, LOW);
      delay(400);
      digitalWrite(MotorIzquierda, HIGH);
      delay(400);
      digitalWrite(MotorDerecha, LOW);
      delay(400);
      digitalWrite(MotorDerecha, HIGH);
      delay(300);
      contLavado++;   
  }
}
void Enjuague(){
  if(contVaciado<=TimeVaciado){
    vaciado();
}else if(contCentrifugadoEnjuague<=TimeCentrifugadoEnjuague){
  Serial.print("EXPRIMIENDO ENJUAGUE.... ");
      Serial.println(contCentrifugadoEnjuague);
      digitalWrite(BombBaciado, LOW);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, HIGH);
      digitalWrite(MotorIzquierda,LOW);
      delay(1000);
      contCentrifugadoEnjuague++;

  }else if (contLLenado <= TimeLLenado) {
    llenado();
  } else if(contEnjuague<=TimeEnjuague) {
      Serial.print("ENJUAGANDO.... ");
      Serial.println(contEnjuague);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, LOW);
      
      digitalWrite(MotorIzquierda, LOW);
      delay(400);
      digitalWrite(MotorIzquierda, HIGH);
      delay(400);
      digitalWrite(MotorDerecha, LOW);
      delay(400);
      digitalWrite(MotorDerecha, HIGH);
      delay(400);
      contEnjuague++;
     
  }

}

void Centrifugado(){
  if(contVaciado<=TimeVaciado){
    vaciado();
    
   }else if(contCentrifugado<=TimeCentrifugado){
      Serial.print("EXPRIMIENDO.... ");
      Serial.println(contCentrifugado);
      digitalWrite(BombBaciado, LOW);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, HIGH);
      digitalWrite(MotorIzquierda,LOW);
      delay(1000);
      contCentrifugado++; 
          
}
}
void Remojado(){
  if(contRemojo<=TimeRemojo) {
      Serial.print("REMOJANDO....");     
      Serial.println(contRemojo);
      digitalWrite(Flotador, LOW);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);                
      digitalWrite(MotorIzquierda, LOW);
      delay(600);
      digitalWrite(MotorIzquierda, HIGH);
      delay(600);
      digitalWrite(MotorDerecha, LOW);
      delay(600);
      digitalWrite(MotorDerecha, HIGH);
      delay(600);
      contRemojo++;
        
  }
}
void CentrigradoExt(){
  if(contExtraCen<=TimeExtraCen){
      Serial.print("CENTRIFUGANDO.... ");
      Serial.println(contExtraCen);
      digitalWrite(BombBaciado, LOW);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, HIGH);
      digitalWrite(MotorIzquierda,LOW);
      delay(1000);
      contExtraCen++;     
}
}

void modoAutomatico(){
  
  if(Flagllenado){
    llenado();
    if(contLLenado>TimeLLenado){  
      previousMillisLlenado=0;   
      contLLenado =1;
      Flagllenado=false;
      Flagremojar=true;

    }
  }else if(Flagremojar){
    Remojado();
    if(contRemojo>TimeRemojo){      
      Flagremojar=false;
      Flaglavar=true;
    }
  }else if(Flaglavar){
    Lavado();
    if(contLavado>TimeLavado){      
      Flaglavar=false;
      Flagenjuagar=true;
    }
  }else if(Flagenjuagar){
    Enjuague();
    if(contEnjuague>TimeEnjuague){
      Flagenjuagar=false;
      Flagcentrifugar=true;
      previousMillisLlenado=0;
      contVaciado=1;
    }
  }else if(Flagcentrifugar){
    Centrifugado();
    if(contCentrifugado>TimeCentrifugado){
      Flagcentrifugar=false;
    }
  }else{
    Serial.println("lavado Terminado");
    ReinicioSistema();      
  }
}

void modoManual(){
  if (llenar) {
    llenado();
    if(contLLenado>TimeLLenado){  
      previousMillisLlenado=0;   
      contLLenado =1;
      llenar=false;
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(Flotador, HIGH);

    }
  } else if (remojar) {
    Remojado();
    if(contRemojo>TimeRemojo){
      contRemojo=1;      
      remojar=false;
       digitalWrite(Flotador, HIGH);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);                
      digitalWrite(MotorIzquierda, HIGH);
      digitalWrite(MotorDerecha, HIGH);
    }
  } else if(lavar){
    Lavado();
    if(contLavado>TimeLavado){ 
      contLavado=1;     
      lavar=false;
       digitalWrite(Flotador, HIGH);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);                
      digitalWrite(MotorIzquierda, HIGH);
      digitalWrite(MotorDerecha, HIGH);
    }
  }else if (enjuagar) {
    Enjuague();
    if(contEnjuague>TimeEnjuague){
      enjuagar=false;     
      previousMillisLlenado=0;
      contVaciado=1;
      contEnjuague=1;
      digitalWrite(Flotador, HIGH);
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);                
      digitalWrite(MotorIzquierda, HIGH);
      digitalWrite(MotorDerecha, HIGH);
    }
  } else if (exprimir) {
    Centrifugado();
    if(contCentrifugado>TimeCentrifugado){
      exprimir=false;
      contCentrifugado=1;
        digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, HIGH);
      digitalWrite(MotorIzquierda,HIGH);
    }
  }else if (centrifugado) {
    CentrigradoExt();
    if(contExtraCen>TimeExtraCen){
      centrifugado=false;
      contExtraCen=1;
      digitalWrite(BombBaciado, HIGH);
      digitalWrite(BombLLenado, HIGH);
      digitalWrite(Flotador, HIGH);
      digitalWrite(MotorIzquierda,HIGH);
    }
  }else{
    Serial.println("terminado");
    ReinicioSistema();
  }
}
void ReinicioSistema(){
    pausa= false;
    currentState = IDLE;
    digitalWrite(BombLLenado, HIGH);
    digitalWrite(BombAgua,HIGH);
    digitalWrite(Flotador, HIGH);
    digitalWrite(BombBaciado, HIGH);
    digitalWrite(MotorDerecha, HIGH);
    digitalWrite(MotorIzquierda, HIGH);
      
      if(bomba){
        previousMillisLlenado=0;
        contLLenado = 1;
        contVaciado = 1;      
      }
      llenar = false;
      contLavado = 1;     
      contEnjuague =1;
      contCentrifugado = 1;
      contCentrifugadoEnjuague =1;
      contRemojo=1;
      contExtraCen=1;
      Flagllenado=false;
      Flagremojar=false;
      Flaglavar=false;
      Flagenjuagar=false;
      Flagcentrifugar=false;      
      remojar = false;
      lavar = false;
      enjuagar = false;
      exprimir = false;
      centrifugado = false;
    
}

void loop() {
  handleClient();
   


  if(currentState == AUTOMATIC && pausa==false){
    modoAutomatico();    
  }else if(currentState== MANUAL && pausa==false){
    modoManual();
  }
}

void handleClient() {
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Verificar las rutas y realizar las acciones correspondientes
            String cadenaPrincipal = header;

            String subcadena = "%";
            //leemos el header va llegar algo como GET /"lo que queremos hacer"
            //este primer if es para el modo manual y va recibir las opciones que se activaron en el html
            //llegara algo como GET /_1%7C_2%7C_3%7C asi sucesivamente compara si encuentra un % o un _ es true y entra
              if (cadenaPrincipal.indexOf(subcadena) != -1 || cadenaPrincipal.indexOf("_") != -1) {
                currentState = previousState;
              pausa=false;
              Serial.println(cadenaPrincipal);
              Serial.println("La subcadena está presente en la cadena principal.");
               //aqui compara esa cadena y activa las funciones que el usuario selecciono
               //si el usuario deselecciono una opcion esta se volvera false 
                if(cadenaPrincipal.indexOf("_1")!=-1){
                  llenar=true;
                }else{
                  llenar=false;
                }
                if(cadenaPrincipal.indexOf("_2")!=-1){
                  remojar=true;
                }else{
                  remojar=false;
                }
                if(cadenaPrincipal.indexOf("_3")!=-1){
                  lavar=true;
                }else{
                  lavar=false;
                }
                if(cadenaPrincipal.indexOf("_4")!=-1){
                  enjuagar=true;
                }else{
                  enjuagar=false;
                }
                if(cadenaPrincipal.indexOf("_5")!=-1){
                  exprimir=true;
                }else{
                  exprimir=false;
                }
                if(cadenaPrincipal.indexOf("_6")!=-1){
                  centrifugado=true;
                }else{
                  centrifugado=false;
                }
                currentState = MANUAL;              
             } else {
              Serial.println("La subcadena no está presente en la cadena principal.");
              Serial.println(cadenaPrincipal);
              if (header.startsWith("GET /bajo")) {
              Serial.println("GPIO bajo");
              
              if(bomba){
                TimeLLenado=10;
              }else{
                TimeLLenado=10-contLLenado;
              }
              Serial.println(TimeLLenado);
             }else if (header.startsWith("GET /medio")) {
              Serial.println("GPIO medio");
              
              if(bomba){
                TimeLLenado=20;
              }else{
                TimeLLenado=20-contLLenado;
              }
              Serial.println(TimeLLenado);
             }else if (header.startsWith("GET /alto")) {
              Serial.println("GPIO alto");
             
              if(bomba){
                TimeLLenado=30;
              } else{
                TimeLLenado=30-contLLenado;
              }
               Serial.println(TimeLLenado);
             }else if (header.startsWith("GET /automatico")) {
              Serial.println("GPIO automatico");
              Flagllenado=true;
              currentState = AUTOMATIC;
              previousState = IDLE;
            }else if (header.startsWith("GET /pausar")) {
              Serial.println("GPIO pausar");
              previousState = currentState;
              currentState = PAUSED;
              pausa= true;
              digitalWrite(BombLLenado, HIGH);
    digitalWrite(BombAgua,HIGH);
    digitalWrite(Flotador, HIGH);
    digitalWrite(BombBaciado, HIGH);
    digitalWrite(MotorDerecha, HIGH);
    digitalWrite(MotorIzquierda, HIGH);
            } else if (header.startsWith("GET /reanudar")) {
              Serial.println("GPIO reanudar");
              currentState = previousState;
              pausa=false;

            } else if (header.startsWith("GET /reiniciar")) {
              Serial.println("GPIO reiniciar");   
              ReinicioSistema();
            }

}
            client.print(webpageCode);
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}