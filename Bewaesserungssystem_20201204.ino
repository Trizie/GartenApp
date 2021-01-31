
 
#include <WiFi101.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


  int sensor_pin = A0;
  int output_value;
  int vari;
  char buffer[150];


  //IP-Adresse und user-Daten für MariaDB
  IPAddress server_addr(***,***,***,**);  
  char user[] = "garten";              
  char password[] = "*****";        

  //WiFi-Netzwerk 
  char ssid[] = "####################;             
  char pass[] = "####################";      

  //WiFi-Client erstellen
  WiFiClient client;           
  MySQL_Connection conn((Client *)&client);

  int status = WL_IDLE_STATUS;      //Verbindungsstatus
  WiFiServer server(80);            //Server-Socket


  //Funktion zum Ausgeben des Status der WiFi-Verbindung am Serial Monitor
  void printWiFiStatus() {
  //Netzwerk-SSID ausgeben
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  //IP-Adresse ausgeben
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  //Signalstärke ausgeben
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  }


  //Funktion um Pumpenmodus umzusetzen, der über Qt eingestellt wurde
  void write_pumpe(int pumpeModus){
      
    if (pumpeModus == 1)
      digitalWrite(7, HIGH);
          

    if (pumpeModus == 2)
      digitalWrite(7, LOW);
     

    if (pumpeModus == 3){
    
      if (output_value < 20 )
        digitalWrite(7, HIGH);
    
    else digitalWrite(7, LOW);
    }
  }


  void setup() {
  
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    Serial.begin(9600); 
 
    //ist das Shield verbunden?
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      while (true);       
    }

    //versucht mit WiFi Netzwerk zu verbinden
    while ( status != WL_CONNECTED) {
      
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(ssid);                   // Netzwerk-Name (SSID) wird angezeigt;

      //Verbindung mit Netzwerk herstellen
      status = WiFi.begin(ssid, pass);
      // wait 10 seconds for connection:
      delay(10000);
    }
  
    server.begin();                           //der Server auf Port 80 wird gestartet
    printWiFiStatus();                        //WiFi-Status wird auf Serial Monitor ausgegeben

    //Verbindung zu MariaDB wird über Server hergestellt, Standardport 3306
    Serial.println("Connecting...");
      if (conn.connect(server_addr, 3306, user, password)) {
        delay(8000);
      }
      else
        Serial.println("Connection failed.");
  }

  void loop() {


    //WiFi-Verbindung prüfen
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(ssid);                  

      //Verbindung zum Netzwerk wieder herstellen
      status = WiFi.begin(ssid, pass);
      // wait 10 seconds for connection:
      delay(10000);
    }

    delay(2000);
    Serial.println("Connecting...");
    if (conn.connect(server_addr, 3306, user, password)) {
      delay(8000);
      

      //Bodenfeuchte aus Sensor-Wert berechnen und am Serial Monitor ausgeben
      output_value = analogRead(sensor_pin);
      vari = 100-((100 / 1023.0) * output_value);
      Serial.print("Bodenfeuchte: ");
      Serial.print(vari);
      Serial.print("%");
      Serial.print('\n');

      //Werte in die Datenbank schreiben (INSERT)
      Serial.println("Recording data.");
      snprintf(buffer, 150, "INSERT INTO gartentest.feuchtigkeit (Bodenfeuchtigkeit) VALUES ('%d')", vari); //sprintf() damit Wert in die DB aufgenommen werden kann (versch. Datentypen!)

      //Eine Instanz der Klasse MySQL_Cursor für die Query erstellen
      MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
      //Die Query ausführen
      cur_mem->execute(buffer);
      //Serial.println(buffer);

      //Abfrage Pumpenmodus aus Datenbank (SELECT), (Werte werden über QT-Software in DB geschrieben)
      row_values *row = NULL;
      int head_count = 0;
  
      char query[] = "SELECT Pumpenmodus FROM gartentest.steuerung";
      cur_mem->execute(query);

      column_names *columns = cur_mem->get_columns();

      //Reihe auslesen, nur der Wert der letzen wird verwendet (letzte Pumpeneinstellung)
      do {
        row = cur_mem->get_next_row();
        if (row != NULL) {
          head_count = atol(row->values[0]); //atol() - string zu int konvertieren
        }
      } while (row != NULL);

  
      //Cursor löschen (Speicherplatz)
      delete cur_mem;

      write_pumpe(head_count); 
      Serial.print("Pumpenmodus = ");
      Serial.println(head_count);
    }

    else
      Serial.println("Connection failed.");
    }
