import serial

# Configurar puerto serial para LIN
lin = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

# Enviar trama LIN
lin_frame = bytes([0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF])
lin.write(lin_frame)

# Leer respuesta
response = lin.read(8)
print(f"Respuesta LIN: {response.hex(' ')}")

lin.close()

