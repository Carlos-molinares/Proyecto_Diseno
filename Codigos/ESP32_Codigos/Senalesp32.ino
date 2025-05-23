#include <SPI.h>

const int chipSelectPin = 5;  // Pin de selección del chip (CS)
const int spiSpeed = 1000000;  // Velocidad de SPI (1 MHz)
const int adcDataLength = 2;  // Tamaño de los datos que esperamos recibir (en bytes)

// Definir el puerto SPI
SPIClass spiBus(VSPI);

// Definir el factor de conversión por burden voltage y la amplificación
const float burdenVoltage = 1.0 / 100.0;  // 1 mV / 100 mA (1mV por 100mA)
const float amplificadorFactor = 1000.0;  // Factor de amplificación

void setup() {
  // Inicializar comunicación serial
  Serial.begin(115200);

  // Inicializar SPI
  spiBus.begin();  // Usar los pines predeterminados: SCK, MISO, MOSI

  // Configurar el pin CS como salida
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, HIGH);  // Desactivar la selección del chip (CS)

  // Iniciar comunicación SPI
  Serial.println("Iniciando comunicación SPI...");
}

void loop() {
  // Iniciar la lectura del ADC
  digitalWrite(chipSelectPin, LOW);  // Activar la selección del chip (CS)

  // Enviar la solicitud de lectura de datos (esto depende del protocolo específico del ADC)
  byte requestData[2] = {0x01, 0x00};  // Solicitar datos al ADC, este comando depende del ADC utilizado
  spiBus.transfer(requestData, adcDataLength);  // Enviar la solicitud

  // Leer los datos recibidos desde el ADC
  byte adcData[2];  // Buffer para los datos del ADC (por ejemplo, 2 bytes para el valor de la lectura)
  spiBus.transfer(adcData, adcDataLength);  // Leer los datos del ADC

  // Procesar los datos leídos (esto depende del formato de salida del ADC)
  int adcValue = (adcData[0] << 8) | adcData[1];  // Unir los dos bytes en un valor de 16 bits
  float voltage = (adcValue / 32768.0) * 3.3;  // Convertir el valor ADC a voltaje (ajustar según la referencia)

  // Calcular la corriente: usar la ley de Ohm (I = V / R) y luego aplicar el burden voltage y la amplificación
  float corriente = (voltage * burdenVoltage) * amplificadorFactor;

  // Mostrar los resultados en el monitor serial
  Serial.print("Valor ADC: ");
  Serial.print(adcValue);
  Serial.print(" | Voltaje: ");
  Serial.print(voltage);
  Serial.print(" | Corriente: ");
  Serial.println(corriente);  // Mostrar la corriente calculada

  delay(1000);  // Esperar 1 segundo antes de la siguiente lectura
}