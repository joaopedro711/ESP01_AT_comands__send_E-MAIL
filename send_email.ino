//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*
 * Código foi desenvolvido para o ESP32, mas basta mudar os pinos do Serial 2, utilizando o Software Serial (ao inves de Serial 2) do Arduino
 * 
 * Não precisa colocar os caracteres "\n\r" nos finais dos comandos AT, pois já são inclusos altomaticamente, eles geram busy p...(pino ocupado)
 * Mas quando colocar o AT+CIPSEND precisa colocar a quantidade+2 (\n\r) de caracteres 
 * 
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
char wifi[100] = "";

//variaveis servidor smtp2go
char  type_conexao[] = "\"TCP\"";
char  site_servidor_smtp2go[] = "\"mail.smtp2go.com\"";
char  porta[] = "2525";
char  helo[] = "EHLO 192.168.1.123";                                  // HELO/EHLO: sinal de aceno ao site
char cipstart[100] = "";
char cipsend[100] = "";

//variaveis de LOGIN smtp2go
char  username_base64[] = "YXJhcHVrYUBteXlhaG9vLmNvbQ==";         // username: arapuka@myyahoo.com
char  password_base64[] = "dGNjX2pvYW9wZWRyb18yMDIz";             // senha : tcc_joaopedro_2023

// E-MAILs de remetente e destinatario
char  mail_FROM[] = "toquinhafc@gmail.com";                   // MAIL FROM tem que ser alterado junto com o site smtp2go
char  rcpt_TO[] = "jo88791@gmail.com";
char mail_remetente[100] = "";
char mail_destinatario[100] = "";  

// Assunto e Corpo do e-mail
char  subject[] = "Testando rotina";
char  body[] = "Esta funcionando\nShowww de bola\nestou sentindo uma treta\nirineu voce nao sabe nem eu";
char assunto[100] = "";


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//FUNCOES DO PROGRAMA
void send_email();
void send_email_refatorado();
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

     //chama a função de enviar e-mail, utilizando funções de contagem de caracteres e variaveis globais, mas é a mesma coisa da outra de cima
     send_email_refatorado();   
   }
  

  // Atualiza o estado anterior do botão interno
  previousButtonState = buttonState;
  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
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
      // Verifica se há dados disponíveis na Serial2 (ESP01)
      while(Serial2.available()) {
        // Lê a resposta do módulo ESP01
        String response = Serial2.readStringUntil('\n');
        Serial.println(response);
      }
    }
    Serial.println("\n----------------------------------------------\n");
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//FUNCAO QUE CONTA A QUANTIDADE DE CARACTERES DE UMA STRING
int contarCaracteres(const char* string) {
    int contador = 2;                       //devido aos caracteres \n\r
    
    while (*string != '\0') {
        contador++;
        string++;
    }
    
    return contador;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//FUNÇÂO QUE ENVIA E-MAIL
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
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Funcao envia email com funções complexas e variaveis
void send_email_refatorado(){
  sendATCommand("AT+CWMODE=3", 500);

 //------------ wifi ----------------------------------//   
 
  sprintf(wifi, "AT+CWJAP=%s,%s", SSID_wifi, Password_wifi);
  sendATCommand(wifi, 10000);  
  
 //--------------------------------------------------// 
   
  sendATCommand("AT+CIPMUX=1", 1000);

  sendATCommand("AT+CIPSERVER=1,80", 500);

 //------------ servidor smtp2go ----------------------// 

  sprintf(cipstart, "AT+CIPSTART=4,%s,%s,%s", type_conexao, site_servidor_smtp2go, porta);
  sendATCommand(cipstart, 3000); 
//--------------------------------------------------// 

 //------------ EHLO para o smtp2go ----------------// 

  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(helo));
  sendATCommand(cipsend, 500);  
  sendATCommand(helo, 1000);
//--------------------------------------------------// 

//------------ LOGIN -------------------------------------------------// 
  sendATCommand("AT+CIPSEND=4,12", 500);
  
  sendATCommand("AUTH LOGIN", 2000);
  
  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(username_base64));
  sendATCommand(cipsend, 500);  
  sendATCommand(username_base64, 1000);
  
  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(password_base64));
  sendATCommand(cipsend, 500);  
  sendATCommand(password_base64, 1000);
//-----------------------------------------------------------------------------------// 

//------------ E-MAILs de remetente e de destinatário ----------------//
// MAIL FROM tem que ser alterado junto com o site smtp2go

  sprintf(mail_remetente, "MAIL FROM:<%s>", mail_FROM);
 
  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(mail_remetente));
  sendATCommand(cipsend, 500);  
  sendATCommand(mail_remetente, 1000);

//Mail RCPT

  sprintf(mail_destinatario, "RCPT To:<%s>", rcpt_TO);
  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(mail_destinatario));
  sendATCommand(cipsend, 500);  
  sendATCommand(mail_destinatario, 1000);

//-----------------------------------------------------------------------------------// 

//----------------------------- ASSUNTO e CORPO do E-MAIL ------------------------------------------------------// 
  sendATCommand("AT+CIPSEND=4,6", 500);
  sendATCommand("DATA", 1000);

//Assunto
  sprintf(assunto, "Subject: %s", subject);
  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(assunto));
  sendATCommand(cipsend, 500);  
  sendATCommand(assunto, 5000);
  
//Corpo

  sprintf(cipsend, "AT+CIPSEND=4,%d", contarCaracteres(body));
  sendATCommand(cipsend, 500);
  //Antes desse comando dava busy p...
  //Aumentei o delay para 15 segundos e deu certo  
  sendATCommand(body, 15000);
 
//----------------------------- Finalizar E-mail e conexoes com servidor ------------------------------------------------------// 
  sendATCommand("AT+CIPSEND=4,3", 500);
  
  sendATCommand(".", 1000);
  
  sendATCommand("AT+CIPSEND=4,6", 500);
  
  sendATCommand("QUIT", 1000);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
