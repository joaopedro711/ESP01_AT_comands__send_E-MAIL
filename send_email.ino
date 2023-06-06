//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 * Código está para ESP32, mas basta mudar os pinos do Serial 2, utilizando o Software Serial (ao inves de Serial 2) do Arduino
 * 
 * Não precisa colocar os caracteres "\n\r" nos finais dos comandos AT, eles geram busy p...(pino ocupado)
 * No corpo do e-mail colocar um delay longo, 15 segundos resolveu, mas se gerar busy s..., aumenta o tempo 
 * se quiser pular linha no corpo do e-mail, basta colocar "\n" mas não pode colocar o "\r" (caractere que coloca o cursor no inicio da linha) pois gera busy s... (pino ocupado)
 */
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//VATIAVEIS PARA BOTAO DO ESP32
const int buttonPin = 0;  // Pino do botão interno (GPIO0)
boolean buttonState = false;  // Estado do botão interno
boolean previousButtonState = false;  // Estado anterior do botão interno
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//VARIAVEIS DO CODIGO DE E-MAIL
//variaveis wifi
char  SSID_wifi[] = "\"Netvirtua_VERA\"";
char Password_wifi[] = "\"40225295022\"";

//variaveis servidor smtp2go
char  type_conexao[] = "\"TCP\"";
char  site_servidor_smtp2go[] = "\"mail.smtp2go.com\"";
char  porta[] = "\"2525\"";

//variaveis de LOGIN smtp2go
char  username_base64[] = "\"YXJhcHVrYUBteXlhaG9vLmNvbQ==\"";         // username: arapuka@myyahoo.com
char  password_base64[] = "\"dGNjX2pvYW9wZWRyb18yMDIz\"";             // senha : tcc_joaopedro_2023

// E-MAILs de remetente e destinatario
char  mail_FROM[] = "\"toquinhafc@gmail.com\"";
char  rcpt_TO[] = "\"jo88791@gmail.com\"";

// Assunto e Corpo do e-mail
char  subject[] = "\"Teste\"";
char  body[] = "\"Esta funcionando\"";
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//FUNCOES DO PROGRAMA
void send_email();
void send_web();
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial padrão
  Serial2.begin(115200, SERIAL_8N1, 15, 4); // Inicializa a comunicação serial Serial2 para o módulo ESP01 com baud rate de 115200, paridade 8N1, e pino RX2 na GPIO15 e pino TX2 na GPIO4
  delay(1000);
  
  pinMode(buttonPin, INPUT); // Configura o pino do botão interno 

  sendATCommand("AT+RST", 10000);
}

void loop() {
  // Lê o estado atual do botão interno
  buttonState = digitalRead(buttonPin);

  // Verifica se o botão foi pressionado (mudança de HIGH para LOW)
  if (buttonState == LOW && previousButtonState == HIGH){

    //chama a função de enviar e-mail
    //send_email();

     //chama a função de postar na página web
     send_web();   
   }
  

  // Atualiza o estado anterior do botão interno
  previousButtonState = buttonState;
  
}

// Função para enviar comandos AT para o módulo ESP01
void sendATCommand(String command, int delayTime) {
  
    while(Serial2.available()) {
        // Lê a resposta do módulo ESP01
        String response = Serial2.readStringUntil('\n');
        Serial.println(response);
        //Serial.write(Serial2.read());
      }
      
    Serial2.println(command);

    long int time = millis();
    while((time+delayTime) > millis()){
      //delay(delayTime); // Aguarda o tempo de resposta
      // Verifica se há dados disponíveis na Serial2 (ESP01)
      while(Serial2.available()) {
        // Lê a resposta do módulo ESP01
        String response = Serial2.readStringUntil('\n');
        Serial.println(response);
        //Serial.write(Serial2.read());
      }
    }
    Serial.println("\n---------------------------------------\n");
}

//função que envia e-mail
void send_email(){
  sendATCommand("AT+CWMODE=3", 500);
    
  sendATCommand("AT+CWJAP=\"Netvirtua_VERA\",\"40225295022\"", 10000);
  
  sendATCommand("AT+CIPMUX=1", 1000);

  sendATCommand("AT+CIPSERVER=1,80", 500);
  
  sendATCommand("AT+CIPSTART=4,\"TCP\",\"mail.smtp2go.com\",2525", 3000);

  sendATCommand("AT+CIPSEND=4,20", 500);
  
  sendATCommand("EHLO 192.168.1.123", 1000);
  
  sendATCommand("AT+CIPSEND=4,12", 500);
  
  sendATCommand("AUTH LOGIN", 2000);
  
  // username: arapuka@myyahoo.com
  sendATCommand("AT+CIPSEND=4,30", 500);

  sendATCommand("YXJhcHVrYUBteXlhaG9vLmNvbQ==", 1000);
  
  // senha : tcc_joaopedro_2023
  sendATCommand("AT+CIPSEND=4,26", 500);
  
  sendATCommand("dGNjX2pvYW9wZWRyb18yMDIz", 1000);

  // MAIL FROM tem que ser alterado junto com o site smtp2go
  sendATCommand("AT+CIPSEND=4,34", 500);
  
  sendATCommand("MAIL FROM:<toquinhafc@gmail.com>", 1000);
  
  sendATCommand("AT+CIPSEND=4,29", 500);
  
  sendATCommand("RCPT To:<jo88791@gmail.com>", 1000);
  
  sendATCommand("AT+CIPSEND=4,6", 500);
  
  sendATCommand("DATA", 1000);
  
  sendATCommand("AT+CIPSEND=4,16", 500);
  
  sendATCommand("Subject: Teste", 1000);
  
  sendATCommand("AT+CIPSEND=4,61", 1000);

  //Antes desse comando que dá busy p...
  //Aumentei o delay para 15 segundos e deu certo
  sendATCommand("Parece que está dando certo\nTorcer para continuar assim_2!", 15000);
  
  sendATCommand("AT+CIPSEND=4,3", 500);
  
  sendATCommand(".", 1000);
  
  sendATCommand("AT+CIPSEND=4,6", 500);
  
  sendATCommand("QUIT", 1000);
}

//Funcao que posta na pagina web
void send_web(){
 // sendATCommand("AT+RST", 15000);
 
 // sendATCommand("AT", 500);

 // sendATCommand("AT+GMR", 1000);

 // sendATCommand("AT+CWMODE=3", 500);
 
 String wifi = "AT+CWJAP=";
 wifi += SSID_wifi;
 wifi += ",";
 wifi += Password_wifi;
 sendATCommand(wifi, 9000);   
 

 // sendATCommand("AT+CIPMUX=1", 500);
}
