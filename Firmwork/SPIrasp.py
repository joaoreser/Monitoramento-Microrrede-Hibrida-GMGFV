import spidev
import time
from prometheus_client import start_http_server, Gauge

# SPI config
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 500000
spi.mode = 0b00

# Prometheus server
start_http_server(8000)

# Pacote 1 (não mais usado ativamente)
#v_rms_gauge = Gauge('v_rms', 'Tensão RMS (V)')
#i_rms_gauge = Gauge('i_rms', 'Corrente RMS (A)')
#p_ativa_gauge = Gauge('p_ativa', 'Potência Ativa (W)')

# Pacote 1 - SIC
va_sic = Gauge('Va_SIC', 'Tensão fase A do inversor SIC (V)')
vb_sic = Gauge('Vb_SIC', 'Tensão fase B do inversor SIC (V)')
vc_sic = Gauge('Vc_SIC', 'Tensão fase C do inversor SIC (V)')
ia_sic = Gauge('Ia_SIC', 'Corrente fase A do inversor SIC (A)')
ib_sic = Gauge('Ib_SIC', 'Corrente fase B do inversor SIC (A)')
ic_sic = Gauge('Ic_SIC', 'Corrente fase C do inversor SIC (A)')
freq_sic = Gauge('Freq_SIC', 'Frequência do inversor SIC (Hz)')

#Pacote 2 - SIC
p_sic = Gauge('P_SIC', 'Potência ativa SIC (W)')
q_sic = Gauge('Q_SIC', 'Potência reativa SIC (var)')
s_sic = Gauge('S_SIC', 'Potência aparente SIC (VA)')
vae_sic = Gauge('Vae_SIC', 'Tensão dos supercapacitores (V)')
vcc_sic = Gauge('Vcc_SIC', 'Tensão do barramento DC (V)')
pboost = Gauge('Pboost', 'Potência do bosst (W)')
pcrowbar = Gauge('Pcrowbar', 'Potência do crowbar (W)')

# Pacote 3 - SCI/Modbus
status_inversor = Gauge('status_inversor', 'Inversor Ligado (1) ou Desligado (0)')
potencia_saida = Gauge('potencia_saida', 'Potência saída inversor (W)')
tensao_paineis = Gauge('tensao_paineis', 'Tensão dos painéis (V)')
potencia_ref = Gauge('potencia_ref', 'Potência de referência do inversor (W)')

# Pacote 4 - GMG
gmg_p_ativa = Gauge('gmg_p_ativa', 'Potência ativa GMG')
gmg_p_reativa = Gauge('gmg_p_reativa', 'Potência reativa GMG')
gmg_p_aparente = Gauge('gmg_p_aparente', 'Potência aparente GMG')
gmg_va = Gauge('gmg_va', 'Tensão VA GMG')
gmg_vb = Gauge('gmg_vb', 'Tensão VB GMG')
gmg_vc = Gauge('gmg_vc', 'Tensão VC GMG')
gmg_freq = Gauge('gmg_freq', 'Frequência GMG')

# Pacote 5 - GMG
gmg_ia = Gauge('gmg_ia', 'Corrente IA GMG')
gmg_ib = Gauge('gmg_ib', 'Corrente IB GMG')
gmg_ic = Gauge('gmg_ic', 'Corrente IC GMG')

# Função para enviar e receber dados de 16 bits
def spi_transfer_16bit(data):
    """
    Envia e recebe dados de 16 bits via SPI.
    :param data: Lista de inteiros de 16 bits (ex: [0x1234, 0x5678]).
    :return: Lista de inteiros de 16 bits recebidos.
    """
    tx_data = []
    for value in data:
        tx_data.append((value >> 8) & 0xFF)  # Byte mais significativo (MSB)
        tx_data.append(value & 0xFF)         # Byte menos significativo (LSB)

    # Envia os dados e recebe a resposta
    rx_data = spi.xfer2(tx_data)

    # Combina os bytes recebidos em valores de 16 bits
    response = []
    for i in range(0, len(rx_data), 2):
        msb = rx_data[i]
        lsb = rx_data[i + 1]
        response.append((msb << 8) | lsb)

    return response

# Dados de 16 bits a serem enviados (pode ser uma lista vazia se você só quer receber)
tx_data = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  # Dados de teste (opcional)
count = 0

try:
    while True:
        # Envia os dados e recebe a resposta
        count = count + 1
        if count > 5:
            count = 1
        tx_data = [count, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        rx_data = spi_transfer_16bit(tx_data)

        # Exibe os dados enviados e recebidos
        print("Dados enviados:", [(x) for x in tx_data])
        print("Dados recebidos:", [(x) for x in rx_data])

        # Verifica qual pacote foi recebido e processa os dados
        if rx_data[0] == 1:  # Pacote 1 - SIC Tensão, Corrente e Frequência
            va_sic.set(rx_data[1] / 128.0)
            vb_sic.set(rx_data[2] / 128.0)
            vc_sic.set(rx_data[3] / 128.0)
            ia_sic.set(rx_data[4] / 128.0)
            ib_sic.set(rx_data[5] / 128.0)
            ic_sic.set(rx_data[6] / 128.0)
            freq_sic.set(rx_data[7] / 128.0)

            print(f"Pacote 1 - SIC: VA={rx_data[1]/128:.2f} V, VB={rx_data[2]/128:.2f} V, VC={rx_data[3]/128:.2f} V, "
                  f"IA={rx_data[4]/128:.2f} A, IB={rx_data[5]/128:.2f} A, IC={rx_data[6]/128:.2f} A, "
                  f"FREQ={rx_data[7]/128:.2f} Hz")

        elif rx_data[0] == 2:  # Pacote 2 - SIC Potências e Tensões
            p_sic.set((rx_data[1] + 32768) * 0.5)
            q_sic.set((rx_data[2] + 32768) * 0.5)
            s_sic.set((rx_data[3] + 32768) * 0.5)
            vae_sic.set(rx_data[4] / 64.0)
            vcc_sic.set(rx_data[5] / 64.0)
            pboost.set(rx_data[6])
            pcrowbar.set(rx_data[7])

            print(f"Pacote 2 - SIC: P={p_sic._value.get():.2f} W, Q={q_sic._value.get():.2f} var, S={s_sic._value.get():.2f} VA, "
                  f"VAE={vae_sic._value.get():.2f} V, VCC={vcc_sic._value.get():.2f} V, BOOST={pboost._value.get():.2f} W, "
                  f"CROWBAR={pcrowbar._value.get():.2f} W")
   
        elif rx_data[0] == 3:
            status_inversor.set(rx_data[1])
            potencia_saida.set(rx_data[2])
            tensao_paineis.set(rx_data[3])
            potencia_ref.set(rx_data[4])
            print(f"Pacote 3 - Status: {rx_data[1]}, Pot: {rx_data[2]}W, Tensão: {rx_data[3]}V, Ref: {rx_data[4]}W")

        elif rx_data[0] == 4:  # Pacote 4 - GMG Potência e Tensão
            gmg_p_ativa.set(rx_data[1] / 1.6)
            gmg_p_reativa.set(rx_data[2] / 1.6)
            gmg_p_aparente.set(rx_data[3] / 1.6)
            gmg_va.set(rx_data[4] / 128.0)
            gmg_vb.set(rx_data[5] / 128.0)
            gmg_vc.set(rx_data[6] / 128.0)
            gmg_freq.set(rx_data[7] / 128.0)

            print(f"Pacote 4 (GMG): P={gmg_p_ativa._value.get()} W, Q={gmg_p_reativa._value.get()} var, S={gmg_p_aparente._value.get()} VA, "
                  f"VA={gmg_va._value.get()} V, VB={gmg_vb._value.get()} V, VC={gmg_vc._value.get()} V, Freq={gmg_freq._value.get()} Hz")

        elif rx_data[0] == 5:  # Pacote 5 - GMG Correntes
            gmg_ia.set(rx_data[1] / 128.0)
            gmg_ib.set(rx_data[2] / 128.0)
            gmg_ic.set(rx_data[3] / 128.0)

            print(f"Pacote 5 (GMG Correntes): IA={gmg_ia._value.get()}, IB={gmg_ib._value.get()}, IC={gmg_ic._value.get()}")


        # Aguarda um pouco antes de enviar novamente
        time.sleep(1)

except KeyboardInterrupt:
    print("Comunicação SPI interrompida pelo usuário.")

finally:
    spi.close()  # Fecha a conexão SPI
