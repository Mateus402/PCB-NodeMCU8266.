## Protocolo MQTT e Mensageria

O **MQTT (Message Queuing Telemetry Transport)** é um protocolo de mensagens leve, eficiente e ideal para comunicações entre dispositivos em redes de IoT (Internet das Coisas). Ele opera sobre TCP/IP e usa um modelo de publicação/assinatura para distribuir mensagens entre dispositivos.

### Funcionamento Básico do MQTT

O MQTT utiliza três componentes principais:

- **Cliente MQTT**: Dispositivo ou aplicação que se conecta a um broker MQTT para publicar mensagens em tópicos ou para subscrever tópicos e receber mensagens.
  
- **Broker MQTT**: Servidor que recebe todas as mensagens dos clientes e as encaminha para os clientes que estão interessados nos tópicos específicos. É o intermediário central no protocolo MQTT.
  
- **Tópicos MQTT**: Categorias ou canais aos quais as mensagens são publicadas. Os clientes podem subscrever tópicos para receber todas as mensagens publicadas nesses tópicos.

### Estrutura da Mensagem MQTT

- **Tópico**: Define o destino ou categoria da mensagem. É uma string que os clientes usam para subscrever ou publicar mensagens. No código abaixo, o tópico "ControleRelay" é utilizado para controlar um relé.

- **Payload**: Contém os dados da mensagem. No exemplo abaixo, o payload pode ser "ON" ou "OFF", indicando os comandos para ligar ou desligar o relé.

### Exemplo de Implementação em Arduino (ESP8266)

Aqui está um exemplo de código em Arduino (para ESP8266) que utiliza MQTT para controlar um relé:

```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "NomeDaSuaRedeWiFi";
const char *password = "SenhaDaSuaRedeWiFi";
const char *mqtt_server = "EnderecoIPdoSeuBrokerMQTT";

WiFiClient espClient;
PubSubClient client(espClient);
const int Relay = D1;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP:");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.println(message);

  if (String(topic) == "ControleRelay")
  {
    if (message == "ON")
    {
      digitalWrite(Relay, LOW);
      Serial.println("Relé ligado");
    }
    else if (message == "OFF")
    {
      digitalWrite(Relay, HIGH);
      Serial.println("Relé desligado");
    }
  }
}

void setup()
{
  pinMode(Relay, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP8266Client"))
    {
      Serial.println("Conectado ao broker MQTT");
      client.subscribe("ControleRelay");
    }
    else
    {
      Serial.print("Falha na conexão, código de retorno=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())

  {
    reconnect();
  }
  client.loop();
}
```
## Prints
![fea8ddf2-f8a8-4360-9526-6bba1eb46fd7](https://github.com/Mateus402/PCB-NodeMCU8266./assets/112894988/8276482b-41f2-49f6-925f-6c5e4c2d76fe)

