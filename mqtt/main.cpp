#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Definição das credenciais WiFi e do servidor MQTT
const char *ssid = "Prof.Rafael";
const char *password = "senhasenhasenha";
const char *mqtt_server = "192.168.166.156";

// Criação de clientes: WiFiClient para conexão WiFi e PubSubClient para MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Pino onde o relé está conectado
const int Relay = D1;

// Função para configurar a conexão WiFi
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Conecta-se à rede WiFi especificada
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Mostra o endereço IP atribuído ao ESP8266
}

// Função de callback chamada quando uma mensagem MQTT é recebida
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i]; // Constrói a mensagem a partir do payload recebido
  }
  Serial.println(message); // Mostra a mensagem recebida no monitor serial

  // Verifica se a mensagem é para controlar o relé
  if (String(topic) == "ControleRelay")
  {
    if (message == "ON")
    {
      digitalWrite(Relay, LOW);  // Liga o relé (assumindo que LOW liga o relé)
      Serial1.print("OFFFFFFF"); // Exemplo de saída serial
    }
    else if (message == "OFF")
    {
      Serial.println("ONNNNNN"); // Exemplo de mensagem de debug
      digitalWrite(Relay, HIGH); // Desliga o relé
    }
  }
}

// Função de inicialização do programa Arduino
void setup()
{
  pinMode(Relay, OUTPUT);              // Configura o pino do relé como saída
  Serial.begin(9600);                  // Inicializa a comunicação serial com velocidade de 9600 baud
  setup_wifi();                        // Configura a conexão WiFi
  client.setServer(mqtt_server, 1883); // Configura o servidor MQTT e a porta
  client.setCallback(callback);        // Define a função de callback para mensagens recebidas
}

// Função para reconectar-se ao servidor MQTT
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");       // Indica conexão bem-sucedida no monitor serial
      client.subscribe("ControleRelay"); // Inscreve-se no tópico "ControleRelay"
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state()); // Mostra o estado de conexão do cliente MQTT
      Serial.println(" try again in 5 seconds");
      delay(5000); // Aguarda 5 segundos antes de tentar reconectar
    }
  }
}

// Função principal do loop
void loop()
{
  if (!client.connected())
  {
    reconnect(); // Se não estiver conectado ao servidor MQTT, tenta reconectar
  }
  client.loop(); // Mantém a conexão MQTT ativa, verifica mensagens recebidas, etc.
}
