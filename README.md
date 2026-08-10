# ⚡ Sistema de Monitoramento de Microrrede Híbrida GMG-FV

> 📡 Sistema de comunicação e visualização de dados em tempo real para uma microrrede híbrida **Gerador a Diesel (GMG) + Fotovoltaica (FV)**, com suporte à **estabilização elétrica** por meio de um inversor do tipo **filtro ativo**. Os dados de todas as fontes são centralizados em um **DSP**, encaminhados a um **Raspberry Pi** (servidor local) e disponibilizados em **dashboards do Grafana**, com **Prometheus** como banco de dados de séries temporais.

!-- Badges opcionais — descomente e ajuste conforme o repositório
![Status](https://img.shields.io/badge/status-concluído-green)
![License](https://img.shields.io/badge/license-MIT-blue)
![Platform](https://img.shields.io/badge/DSP-C2000-red)
--

---

## 📑 Sumário

- [Sobre o projeto](#-sobre-o-projeto)
- [Fluxo de dados e protocolos](#-fluxo-de-dados-e-protocolos)
- [Variáveis monitoradas](#-variáveis-monitoradas)
- [Hardware utilizado](#-hardware-utilizado)
- [Stack de software](#-stack-de-software)
- [Estrutura do repositório](#-estrutura-do-repositório)
- [Como reproduzir a infraestrutura](#️-como-reproduzir-a-infraestrutura)
- [Autores e créditos](#-autores-e-créditos)
- [Aviso](#️-aviso)
- [Licença](#-licença)
- [Referências](#-referências)

---

## ℹ️ Sobre o projeto

Este repositório reúne o firmware, os scripts e a documentação necessários para reproduzir ou estudar um **sistema de aquisição, comunicação e visualização de dados de uma microrrede híbrida**. A microrrede é composta por:

- ☀️ uma **fonte fotovoltaica** (inversor FV);
- 🛢️ um **gerador a diesel (GMG)** como fonte despachável;
- ⚡ um **inversor do tipo filtro ativo** (baseado em SiC), responsável pela **estabilização elétrica** da microrrede.

O objetivo do sistema não é controlar a microrrede, e sim **monitorá-la de forma centralizada e em tempo real**: cada fonte publica suas grandezas elétricas por um protocolo de comunicação distinto, um **DSP** atua como concentrador central desses dados, e um **Raspberry Pi** os expõe para visualização em dashboards.

O projeto foi desenvolvido como um **projeto EMBRAPII**, em parceria com a empresa **Eccosoma**, ao longo de **18 meses**, na **Universidade Federal de Santa Maria (UFSM)**.

A arquitetura proposta mostrou-se **escalável e replicável**, podendo ser aplicada em outros cenários que envolvam **múltiplos protocolos de comunicação**, mantendo a **padronização no transporte** e a **unificação na visualização** dos dados.

---

## 🔄 Fluxo de dados e protocolos

| Origem dos dados | Interface física | Protocolo | Papel do DSP | Destino |
|---|---|---|---|---|
| Inversor fotovoltaico (Growatt) → Arduino | Serial | **Modbus** | — | Arduino Nano |
| Arduino Nano → DSP | Serial | **SCI (UART)** | mestre da leitura | DSP |
| Gerador a diesel (Cummins) → STM32 | Barramento CAN | **CAN** | nó CAN | DSP |
| Inversor filtro ativo (SiC) | RJ45 / Micro-Match | **ADC** ¹ | **escravo** | DSP |
| DSP → Servidor | SPI | **SPI** | escravo do RPi ² | Raspberry Pi |
| Raspberry Pi | Rede local | HTTP (scrape) | — | Prometheus |
| Prometheus → Visualização | Rede local | Query (PromQL) | — | Grafana |

> ¹ A comunicação com o inversor filtro ativo era feita por cabos diretos na placa, com o **DSP operando como escravo** através da leitura dos sinais analógicos diretos dos sensores de corrente e tensão utilizando conversores A/D.\
> ² Na ligação SPI, o Raspberry Pi atua como *master* e o DSP como *slave*.

---

## 📊 Variáveis monitoradas

☀️ **Inversor fotovoltaico (Growatt)**
- Status (ON / OFF)
- Potência Ativa
- Potência de referência

🛢️ **Gerador a diesel (Cummins)**
- Correntes: I_a, I_b, I_c
- Tensões: V_a, V_b, V_c
- Frequência
- Potências: ativa, reativa e aparente

⚡ **Inversor filtro ativo (SiC)**
- Tensões: V_a, V_b, V_c
- Correntes: I_a, I_b, I_c 
- Potências: ativa, reativa e aparente
- Tensão do banco de capacitores

---

## 🧰 Hardware utilizado

| Componente | Modelo / Especificação | Função no sistema |
|---|---|---|
| 🧠 DSP | **Texas Instruments TMS320F28379D** (C2000) | Receptor central de dados e controle |
| 🔌 Microcontrolador | **Arduino Nano** | Interface de leitura e escrita do inversor FV (SCI) |
| 📟 Microcontrolador | **STM32** | Aquisição dos dados do gerador a diesel e envio ao DSP |
| 🍓 Servidor local | **Raspberry Pi 3 B+** | Servidor central, host Prometheus e Grafana |
| 📡 Transceptor CAN | **MCP2551** | Camada física do barramento CAN (GMG) |
| ☀️ Inversor FV | **Growatt**  | Fonte FV da microrrede |
| 🛢️ Gerador a diesel | **Cummins** | Fonte despachável |
| ⚡ Inversor filtro ativo | **Baseado em SiC** | Estabilização elétrica (patente) |

> *Os esquemáticos de ligação e a pinagem detalhada (SPI/SCI/CAN/MODBUS) não estão incluídos neste repositório por questões de propriedade intelectual.*

---

## 💻 Stack de software

- 🧠 **Firmware do DSP:** Linguagem **C/C++**, ambiente **Code Composer Studio**.
- 🔌 **Firmware do Arduino:** Linguagem **C/C++**, **Arduino IDE**; faz a leitura do inversor FV via **Modbus** e repassa ao DSP via **SCI (UART)**.
- 📟 **Firmware do STM32:** Linguagem **C/C++**; realiza a leitura dos parâmetros do gerador e transmite ao DSP via **CAN**.
- 🍓 **Servidor Raspberry Pi:**
  - Código base em **Python** para requisição SPI dos dados do DSP.
  - **Exporter customizado** expondo métricas via biblioteca `prometheus_client`.
  - Serviços de automação nativos do Linux (**systemd** e **bash scripts**) para operação em Modo Kiosk.
  - Infraestrutura de dados conteinerizada via **Docker/Docker Compose** executando **Prometheus** e **Grafana**.

---

## 📂 Estrutura do repositório

```text
.
├── firmware/
│   ├── dsp/                 # Códigos em C do DSP (Config_can, Config_spi, Config_sci)
│   ├── stm32/               # Códigos em C para comunicação CAN do gerador a diesel
│   └── arduino/             # Sketch de leitura do inversor FV (CódigoSCI_MODBUS)
├── raspberry/
│   ├── spic2000.py          # Leitura SPI dos dados do DSP e exposição HTTP (Exporter)
│   ├── microrrede.service   # Configuração de execução automática do Python (systemd)
│   └── start-kiosk.sh       # Automação de inicialização do navegador em tela cheia
├── docker/
│   ├── docker-compose.yml   # Orquestração dos contêineres Grafana e Prometheus
│   └── prometheus.yml       # Configuração de rotas de scrape do banco de dados
├── docs/
│   ├── dashboards/          # Dashboards exportadas do Grafana (.json)
│   ├── estrutura.png        # Representação do sistema
│   └── dashboard_print.png  # Print da interface em funcionamento
├── LICENSE
└── README.md
```
---

## 🛠️ Como Reproduzir a Infraestrutura

### 1️⃣ DSP F28379D (Processamento Primário)

1. Abrir o projeto localizado em `/dsp/` utilizando o **Code Composer Studio (CCS)**.
2. Configurar os periféricos de hardware: 
   * **ADC / SDFM:** Aquisição direta dos sensores do filtro ativo (inversor SiC).
   * **SCI (Serial):** Recepção dos dados do inversor fotovoltaico via Arduino.
   * **CAN:** Recepção dos parâmetros do gerador a combustão via STM32.
   * **SPI (Modo Escravo):** Configurado para responder às requisições de dados do Raspberry Pi.
3. Compilar e gravar o firmware na placa da Texas Instruments.

### 2️⃣ Microcontroladores Auxiliares (Arduino e STM32)

1. **Interface do Inversor FV (Arduino):** Abrir o sketch em `/arduino/` na **Arduino IDE**. Ajustar os registradores **Modbus** (status, potência ativa e de referência) para leitura do inversor solar e definir o *baud rate* da porta serial (SCI) que se conecta ao DSP. Gravar no Arduino Nano.
2. **Interface do Gerador (STM32):** Abrir o projeto em `/stm32/`, configurar os *mailboxes* do protocolo **CAN** com os IDs corretos e gravar no microcontrolador.

### 3️⃣ Servidor Local (Raspberry Pi)

#### 3.1. Dependências e Interface SPI
No terminal do Raspberry Pi, habilite o hardware SPI e instale as bibliotecas base:
```bash
# Habilitar a interface SPI
sudo raspi-config   # Navegue até: Interface Options → SPI → Enable

# Instalar dependências do sistema e do leitor Python
sudo apt update
sudo apt install -y python3-pip curl
pip3 install spidev prometheus_client
```

#### 3.2. Execução do Código Principal (Manual e Automática)
O script `spic2000.py` é o coração do servidor. Ele requisita os dados ao DSP e os expõe na porta `8000`.

**Para testar o código manualmente (Play inicial):**
Para verificar se a comunicação está funcionando corretamente, execute o script no terminal:
```bash
python3 /home/embrapi/spic2000.py
```
*(Com o script rodando, acesse `http://<ip-do-raspberry>:8000` em um navegador para visualizar os dados brutos sendo exportados).*

**Para colocar em produção (Início Automático via systemd):**
Para que o código inicie sozinho ao ligar a placa e reinicie em caso de falhas, configure o serviço:
```bash
# 1. Copie o arquivo do serviço para o sistema
sudo cp raspberry/microrrede.service /etc/systemd/system/

# 2. Habilite e inicie o código em segundo plano
sudo systemctl daemon-reload
sudo systemctl enable microrrede.service
sudo systemctl start microrrede.service
```

### 4️⃣ Infraestrutura de Banco de Dados e Visualização (Docker)

Para garantir escalabilidade, o Prometheus e o Grafana rodam em contêineres unificados. As portas expostas são `9090` (Prometheus) e `3000` (Grafana).

1. Navegue até a pasta de configuração do Docker:
   ```bash
   cd docker
   ```
2. Inicie os contêineres em segundo plano (o arquivo `docker-compose.yml` já mapeia os volumes da versão `8.5.27` do Grafana com a identidade visual do projeto):
   ```bash
   sudo docker-compose up -d
   ```
*Acesse `http://<ip-do-raspberry>:3000`, adicione o Prometheus como Data Source e importe os arquivos `.json` localizados na pasta `/assets/` para montar a dashboard.*

### 5️⃣ Automação da Interface Visual (Modo Kiosk)

Para que a IHM abra a dashboard automaticamente em tela cheia na inicialização do Raspberry Pi, foi criado um script que desativa o bloqueio de tela e aguarda a porta `3000`.

Adicione a chamada para `/home/embrapi/start-kiosk.sh` aos aplicativos de inicialização da interface gráfica do Raspberry Pi (LXDE/Wayland). O script fará a checagem com `curl` e disparará o Chromium automaticamente.

## 👥 Autores e créditos

- João Vitor Reser da Silva, joaovitorreser@gmail.com.
- Orientação: Jonas Roberto Tibola.
- Instituição: (UFSM).

---

## ⚠️ Aviso 

O conteúdo deste repositório destina-se a fins de **estudo e reprodução do sistema de monitoramento** (aquisição, comunicação e visualização de dados) de arquiteturas híbridas.

---

## 📄 Licença

- **MIT** — permissiva e simples para o uso dos códigos de infraestrutura de software e integração.

---

## 🔗 Referências

- [Documentação do Prometheus](https://prometheus.io/docs/)
- [Documentação do Grafana](https://grafana.com/docs/)
- [TMS320F28379D — Texas Instruments](https://www.ti.com/product/TMS320F28379D)
