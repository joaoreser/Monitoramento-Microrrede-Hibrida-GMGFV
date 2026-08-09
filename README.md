# Sistema de Monitoramento de Microrrede Híbrida GMG-FV

> Sistema de comunicação e visualização de dados em tempo real para uma microrrede híbrida **Gerador a Diesel (GMG) + Fotovoltaica (FV)**, com suporte à **estabilização elétrica** por meio de um inversor do tipo **filtro ativo**. Os dados de todas as fontes são centralizados em um **DSP**, encaminhados a um **Raspberry Pi** (servidor local) e disponibilizados em **dashboards do Grafana**, com **Prometheus** como banco de dados de séries temporais.

<!-- Badges opcionais — descomente e ajuste conforme o repositório
![Status](https://img.shields.io/badge/status-concluído-green)
![License](https://img.shields.io/badge/license-MIT-blue)
![Platform](https://img.shields.io/badge/DSP-C2000-red)
-->

---

## Sumário

- [Sobre o projeto](#sobre-o-projeto)
- [Arquitetura do sistema](#arquitetura-do-sistema)
- [Fluxo de dados e protocolos](#fluxo-de-dados-e-protocolos)
- [Hardware utilizado](#hardware-utilizado)
- [Stack de software](#stack-de-software)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Como reproduzir](#como-reproduzir)
- [Configuração do Prometheus e Grafana](#configuração-do-prometheus-e-grafana)
- [Resultados](#resultados)
- [Trabalhos futuros](#trabalhos-futuros)
- [Autores e créditos](#autores-e-créditos)
- [Aviso sobre propriedade intelectual](#aviso-sobre-propriedade-intelectual)
- [Licença](#licença)
- [Referências](#referências)

---

## Sobre o projeto

Este repositório reúne o firmware, os scripts e a documentação necessários para reproduzir ou estudar um **sistema de aquisição, comunicação e visualização de dados de uma microrrede híbrida**. A microrrede é composta por:

- uma **fonte fotovoltaica** (inversor FV);
- um **gerador a diesel (GMG)** como fonte despachável;
- um **inversor do tipo filtro ativo** (baseado em SiC), responsável pela **estabilização elétrica** da microrrede.

O objetivo do sistema não é controlar a microrrede, e sim **monitorá-la de forma centralizada e em tempo real**: cada fonte publica suas grandezas elétricas por um protocolo de comunicação distinto, um **DSP** atua como concentrador central desses dados, e um **Raspberry Pi** os expõe para visualização em dashboards.

O projeto foi desenvolvido como um **projeto EMBRAPII**, em parceria com a empresa **Eccosoma**, ao longo de **18 meses**, na **Universidade Federal de Santa Maria (UFSM)**.

A arquitetura proposta mostrou-se **escalável e replicável**, podendo ser aplicada em outros cenários que envolvam **múltiplos protocolos de comunicação**, mantendo a **padronização no transporte** e a **unificação na visualização** dos dados.

---

## Fluxo de dados e protocolos

| Origem dos dados | Interface física | Protocolo | Papel do DSP | Destino |
|---|---|---|---|---|
| Inversor fotovoltaico (Growatt) → Arduino | Serial | **Modbus** | — | Arduino Nano |
| Arduino Nano → DSP | Serial | **SCI (UART)** | mestre da leitura | DSP |
| Gerador a diesel (Cummins) | Barramento CAN (MCP2551) | **CAN** | nó CAN | DSP |
| Inversor filtro ativo (SiC) | Cabo de rede (rj45) | **Leitura dos sensores** ¹ | **escravo** | DSP |
| DSP → Servidor | SPI | **SPI** | escravo do RPi ² | Raspberry Pi |
| Raspberry Pi | Rede local | HTTP (scrape) | — | Prometheus |
| Prometheus → Visualização | Rede local | Query (PromQL) | — | Grafana |

¹ A comunicação com o inversor filtro ativo era feita por cabo de rede, com o **DSP operando como escravo** através da leitura dos sensores alocados no protótipo via **ADC**.
² Na ligação SPI, o Raspberry Pi atua como *master* e o DSP como *slave* (configuração padrão para SPI com Linux) — confirme se corresponde à sua implementação.

---
## Variáveis monitoradas

**Inversor fotovoltaico (Growatt)**
- Status (ON / OFF)
- Potência
- Potência de referência

**Gerador a diesel (Cummins)**
- Correntes: Iₐ, I_b, I_c
- Tensões: Vₐ, V_b, V_c
- Frequência
- Potências: ativa, reativa e aparente

**Inversor filtro ativo (SiC)**
- Tensões trifásicas (3)
- Correntes trifásicas (3)
- Potências: ativa, reativa e aparente
- Tensão do banco de capacitores

---
## Hardware utilizado

| Componente | Modelo / Especificação | Função no sistema |
|---|---|---|
| DSP | **Texas Instruments TMS320F28379D** (C2000 Delfino) | Concentrador central de dados |
| Microcontrolador auxiliar | **Arduino Nano** | Interface de leitura do inversor FV (SCI) |
| Servidor local | **Raspberry Pi 3 B+** | Servidor Prometheus + Grafana |
| Transceptor CAN | **MCP2551** | Camada física do barramento CAN (GMG) |
| Inversor fotovoltaico | **Growatt**  | Fonte FV da microrrede |
| Gerador a diesel (GMG) | **Cummins** | Fonte despachável |
| Inversor filtro ativo | **Baseado em SiC** | Estabilização elétrica (patente) |

> Os esquemáticos de ligação e a pinagem detalhada (SPI/SCI/CAN/Ethernet) não estão incluídos neste repositório.

---

## Stack de software

- **Firmware do DSP** — linguagem **C**, ambiente **Code Composer Studio**.
- **Firmware do Arduino** — linguagem **C**, **Arduino IDE**; faz a leitura do inversor FV via **Modbus** e repassa os dados ao DSP via **SCI (UART)**.
- **Raspberry Pi**
  - Script de leitura **SPI** dos dados vindos do DSP — **Python**.
  - **Exporter** que expõe as métricas para o Prometheus — biblioteca **`prometheus_client`**.
  - **Prometheus** — coleta e armazenamento das séries temporais.
  - **Grafana** — dashboards de visualização.

---

## Estrutura do repositório

> Sugestão de organização — ajuste conforme os arquivos que você já tem.

```
.
├── firmware/
│   ├── dsp/                 # Código do DSP (concentrador central)
│   └── arduino/             # Sketch de leitura do inversor FV (SCI)
├── raspberry-pi/
│   ├── spi_reader/          # Leitura SPI dos dados vindos do DSP
│   ├── prometheus/          # prometheus.yml e configuração de scrape
│   └── grafana/             # Dashboards exportados (.json)
├── docs/
│   ├── arquitetura.png      # Diagrama do sistema
│   ├── protocolos.md        # Detalhamento de SCI, CAN, SPI (frames/mapas)
│   └── imagens/             # Fotos do setup, prints dos dashboards
├── LICENSE
└── README.md
```

---

## Como reproduzir

### 1. DSP (concentrador central)

1. Abrir o projeto em `firmware/dsp/` no **Code Composer Studio**.
2. Configurar os periféricos de comunicação: **SCI** (Arduino/FV), **CAN** (GMG), **ADC's** (inversor filtro ativo, DSP como escravo) e **SPI** (Raspberry Pi).
3. Compilar e gravar o firmware no **TMS320F28379D**.

### 2. Arduino Nano (interface do inversor FV)

1. Abrir o sketch em `firmware/arduino/` na **Arduino IDE**.
2. Ajustar os parâmetros de leitura **Modbus** do inversor FV (endereços/registradores das variáveis: status, potência, potência de referência) e os parâmetros do **SCI** (baud rate) para o envio ao DSP.
3. Gravar no Arduino Nano.

### 3. Raspberry Pi (servidor local)

```bash
# Habilitar a interface SPI
sudo raspi-config   # Interface Options → SPI → Enable

# Dependências do leitor SPI 
sudo apt update
sudo apt install -y python3-pip
pip3 install spidev prometheus_client
```

1. Rodar o script de leitura SPI em `home/embrapi/`.
2. Iniciar o **exporter** em `spic2000.py`, que publica as métricas em um endpoint HTTP (ex.: `http://<ip-do-rpi>:8000/metrics`).

---

## Configuração do Prometheus e Grafana

### Prometheus

Exemplo de `prometheus.yml` apontando para o exporter do Raspberry Pi:

```yaml
global:
  scrape_interval: 15s  # Intervalo de coleta das métricas

scrape_configs:
  - job_name: 'prometheus'  # Nome da tarefa para monitorar o próprio Prometheus
    static_configs:
      - targets: ['localhost:9090']  # IP do Prometheus

  - job_name: 'spi_prometheus'  # Nome da tarefa para monitorar o script Python
    static_configs:
      - targets: ['<ip-do-rpi>:8000']  # IP do Raspberry

```

```bash
# Execução via Docker (opção prática no Raspberry Pi)
docker run -d --name prometheus \
  -p 9090:9090 \
  -v $(pwd)/raspberry-pi/prometheus/prometheus.yml:/etc/prometheus/prometheus.yml \
  prom/prometheus
```

### Grafana

```bash
docker run -d --name grafana -p 3000:3000 grafana/grafana
```

1. Acessar `http://<ip-do-rpi>:3000` (login padrão `admin` / `admin`).
2. Adicionar o **Prometheus** como *data source* (`http://<ip-do-rpi>:9090`).
3. Importar os dashboards de `raspberry-pi/grafana/` (arquivos `.json`).


---

## Resultados

```markdown
![Dashboard Grafana](docs/imagens/dashboard.png)
![Setup físico](docs/imagens/setup.png)
```

---

## Autores e créditos

- João Vitor Reser da Silva, joaovitorreser@gmail.com.
- Orientação: Jonas Roberto Tibola.
- Instituição: (UFSM).

---

## Aviso sobre propriedade intelectual

O **inversor do tipo filtro ativo** utilizado para estabilização elétrica é objeto de **patente**. O conteúdo deste repositório destina-se a fins de **estudo e reprodução do sistema de monitoramento** (aquisição, comunicação e visualização de dados).

---

## Referências

- [Documentação do Prometheus](https://prometheus.io/docs/)
- [Documentação do Grafana](https://grafana.com/docs/)
- [TMS320F28379D — Texas Instruments](https://www.ti.com/product/TMS320F28379D)
