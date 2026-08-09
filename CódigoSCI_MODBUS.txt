#include <ModbusMaster.h>

#define MAX485_DE 10
#define numChars 4

// Comunicação SCI
unsigned char receivedChars[numChars];
boolean newData = false;
unsigned char Tipo_OP_sci = 0, ID_sci = 0;
unsigned int DataR = 0;

unsigned long lastReceiveTime = 0;
const unsigned long timeout = 100;
bool activeCommunication = true;
unsigned long lastPacketTime = 0;
const unsigned long communicationTimeout = 1000;

// Variáveis Modbus
ModbusMaster node;
int reg_pot_out = 36;
int reg_status_inv = 0;
int reg_pot_in = 36;  // Confirmado
int reg_freq = 37;

// Variáveis sistema
int potencia_ref = 1000;
bool inversor_on = false;
int potencia_w = 0;
int tensao_paineis = 0;

// --- Funções RS485 ---
void preTransmission() {
  digitalWrite(MAX485_DE, 1);
}
void postTransmission() {
  digitalWrite(MAX485_DE, 0);
}

// --- Comunicação SCI com DSP ---
void enviarRetorno(byte id, int valor) {
  byte pacote[4];
  pacote[0] = (id << 4) | 3;
  pacote[1] = (valor >> 8) & 0xFF;
  pacote[2] = valor & 0xFF;
  pacote[3] = '\n';
  Serial2.write(pacote, 4);
  Serial.print("Enviado retorno -> ID: ");
  Serial.print(id);
  Serial.print(" | Valor: ");
  Serial.println(valor);
}

// --- Setup geral ---
void setup() {
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);
  Serial2.begin(9600); // SCI
  Serial3.begin(9600); // RS485 Modbus

  Serial2.setTimeout(10);
  Serial.println("<Arduino pronto>");
  lastPacketTime = millis();

  node.begin(1, Serial3); // Modbus ID = 1
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

// --- Loop principal ---
void loop() {
  if (millis() - lastPacketTime > communicationTimeout) {
    if (activeCommunication) {
      activeCommunication = false;
      Serial.println("Sem comunicação - DSP parou de transmitir");
    }
    return;
  }

  recvWithTimeout();
  if (newData) {
    processarPacote();
    newData = false;
  }
}

// --- Recepção SCI com timeout ---
void recvWithTimeout() {
  static byte ndx = 0;

  if (millis() - lastReceiveTime > timeout && ndx > 0) {
    ndx = 0;
    Serial.println("Timeout de pacote - resetado");
  }

  while (Serial2.available() > 0 && activeCommunication) {
    lastReceiveTime = millis();
    lastPacketTime = millis();
    receivedChars[ndx] = Serial2.read();

    if (ndx == 0 && receivedChars[0] == '\n') continue;
    ndx++;
    if (ndx >= numChars) {
      Tipo_OP_sci = receivedChars[0] & 0x0F;
      ID_sci = (receivedChars[0] & 0xF0) >> 4;
      DataR = (receivedChars[1] << 8) | receivedChars[2];
      ndx = 0;
      newData = true;
      break;
    }
  }
}

// --- Leitura Modbus ---
uint16_t readRegister(uint16_t reg) {
  uint8_t result = node.readInputRegisters(reg, 1);
  if (result == node.ku8MBSuccess) {
    uint16_t data = node.getResponseBuffer(0);
    Serial.print("Register ");
    Serial.print(reg);
    Serial.print(": ");
    Serial.println(data);
    return data;
  } else {
    Serial.print("Erro ao ler registrador ");
    Serial.println(reg);
    return 0xFFFF;
  }
}

// --- Escritas Modbus ---
void turnOnOff(bool state) {
  uint16_t value = state ? 1 : 0;
  node.writeSingleRegister(0, value);
  Serial.println(state ? "Inversor Ligado" : "Inversor Desligado");
}

void limitActivePower(uint16_t percentage) {
  if (percentage > 100 && percentage != 255) percentage = 100;
  node.writeSingleRegister(3, percentage);
  Serial.print("Limite de potência ativa: ");
  Serial.println(percentage);
}

// --- Processa pacote SCI ---
void processarPacote() {
  Serial.println("--- Pacote Recebido ---");
  Serial.print("ID: "); Serial.print(ID_sci);
  Serial.print(" | Tipo_OP: "); Serial.println(Tipo_OP_sci);
  Serial.print("Valor (recebido do DSP): "); Serial.println(DataR);

  if (Tipo_OP_sci == 1) { // LEITURA
    int valor = 0;
    if (ID_sci == 1) {
      valor = readRegister(reg_status_inv);
      inversor_on = (valor == 1);
    } else if (ID_sci == 2) {
      valor = readRegister(reg_pot_out);
      if (valor != 0xFFFF) valor = (int)(valor * 0.1);
      potencia_w = valor;
    } else if (ID_sci == 3) {
      valor = readRegister(reg_pot_in);
      if (valor != 0xFFFF) valor = (int)(valor * 0.1);
      tensao_paineis = valor;
    } else if (ID_sci == 4) {
      valor = potencia_ref;
    }

    enviarRetorno(ID_sci, valor);
  }

  else if (Tipo_OP_sci == 2) { // ESCRITA
    if (ID_sci == 1) {
      inversor_on = (DataR == 1);
      turnOnOff(inversor_on);
      enviarRetorno(ID_sci, inversor_on ? 1 : 0);
    } else if (ID_sci == 4) {
      potencia_ref = DataR;
      float porcentagem = (float)potencia_ref / 600.0;
      uint16_t valor_percentual = (uint16_t)(porcentagem * 100.0);
      limitActivePower(valor_percentual);
      enviarRetorno(ID_sci, DataR);
    } else {
      Serial.println("Comando de escrita ignorado.");
    }
  }

  Serial.println(">>> Status Atual <<<");
  Serial.print("Inversor: "); Serial.println(inversor_on ? "LIGADO" : "DESLIGADO");
  Serial.print("Potência (W): "); Serial.println(potencia_w);
  Serial.print("Tensão Painéis (V): "); Serial.println(tensao_paineis);
  Serial.print("Potência Ref (W): "); Serial.println(potencia_ref);
  Serial.println("-----------------------");
}
